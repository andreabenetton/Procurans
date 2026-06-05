// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include <QDebug>
#include <QDirIterator>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <algorithm>

#include "fileods.h"
#include "quazip/JlCompress.h"
#include "quazip/quazip.h"
#include "quazip/quazipfile.h"
#include "quazip/quazipnewinfo.h"

namespace
{
	const QString kManifestRel = QStringLiteral("META-INF/manifest.xml");
	const QString kMimetypeRel = QStringLiteral("mimetype");
	const QString kManifestNs =
		QStringLiteral("urn:oasis:names:tc:opendocument:xmlns:manifest:1.0");

	QString mediaTypeFor(const QString& rel)
	{
		if (rel.endsWith(QStringLiteral(".xml"), Qt::CaseInsensitive))
			return QStringLiteral("text/xml");
		// ODF Part 3 §6.1: manifest.rdf carries the package metadata graph
		// and LibreOffice tags it application/rdf+xml. The earlier octet-stream
		// fallback caused LO 24+ to warn about the type mismatch.
		if (rel.endsWith(QStringLiteral(".rdf"), Qt::CaseInsensitive))
			return QStringLiteral("application/rdf+xml");
		if (rel.endsWith(QStringLiteral(".png"), Qt::CaseInsensitive))
			return QStringLiteral("image/png");
		if (rel.endsWith(QStringLiteral(".jpg"), Qt::CaseInsensitive)
		    || rel.endsWith(QStringLiteral(".jpeg"), Qt::CaseInsensitive))
			return QStringLiteral("image/jpeg");
		if (rel.endsWith(QStringLiteral(".gif"), Qt::CaseInsensitive))
			return QStringLiteral("image/gif");
		if (rel.endsWith(QStringLiteral(".svg"), Qt::CaseInsensitive))
			return QStringLiteral("image/svg+xml");
		return QStringLiteral("application/octet-stream");
	}

	// Extracted from the input manifest before we rewrite it, so the regenerated
	// file echoes the original version + any foreign-namespace decorations the
	// host suite (LibreOffice >=24 declares xmlns:loext) put on the manifest
	// root. Defaults are used when the input had no manifest, which only
	// happens for create()-from-scratch flows.
	struct ManifestRootMeta
	{
		QString version = QStringLiteral("1.2");
		QList<QPair<QString, QString>> extra_namespaces;
	};

	ManifestRootMeta readManifestRootMeta(const QString& manifest_path)
	{
		ManifestRootMeta meta;
		QFile f(manifest_path);
		if (!f.open(QIODevice::ReadOnly)) return meta;
		QXmlStreamReader r(&f);
		while (!r.atEnd())
		{
			r.readNext();
			if (!r.isStartElement()) continue;
			if (r.qualifiedName() != QStringLiteral("manifest:manifest")) continue;
			for (const QXmlStreamAttribute& a : r.attributes())
			{
				if (a.qualifiedName() == QStringLiteral("manifest:version"))
				{
					meta.version = a.value().toString();
				}
			}
			for (const QXmlStreamNamespaceDeclaration& ns
			     : r.namespaceDeclarations())
			{
				const QString prefix = ns.prefix().toString();
				const QString uri = ns.namespaceUri().toString();
				if (prefix.isEmpty()) continue;
				if (prefix == QStringLiteral("manifest")) continue;
				meta.extra_namespaces.append({prefix, uri});
			}
			break;
		}
		return meta;
	}

	// Regenerate META-INF/manifest.xml from the actual temp-dir inventory so
	// the manifest can't drift away from the files we're about to repack.
	// Reads the mimetype file for the document root media-type, and inherits
	// the input manifest's version and foreign namespaces so a v1.4 archive
	// stays a v1.4 archive on round-trip (downgrading to 1.2 was what LO 24+
	// flagged as a damaged-file mismatch against content's office:version).
	bool writeFreshManifest(const QString& src_dir)
	{
		QDir src(src_dir);

		QFile mt(src.filePath(kMimetypeRel));
		if (!mt.open(QIODevice::ReadOnly))
		{
			qCritical() << "mimetype missing in" << qPrintable(src_dir);
			return false;
		}
		const QByteArray root_media_type = mt.readAll().trimmed();
		mt.close();

		const ManifestRootMeta input_meta =
			readManifestRootMeta(src.filePath(kManifestRel));

		QStringList entries;
		QDirIterator it(src_dir,
		                QDir::Files | QDir::Hidden | QDir::NoSymLinks,
		                QDirIterator::Subdirectories);
		while (it.hasNext())
		{
			const QString path = it.next();
			const QString rel = src.relativeFilePath(path);
			if (rel == kMimetypeRel || rel == kManifestRel) continue;
			entries.append(rel);
		}
		std::sort(entries.begin(), entries.end());

		src.mkpath(QStringLiteral("META-INF"));
		QFile mf(src.filePath(kManifestRel));
		if (!mf.open(QIODevice::WriteOnly | QIODevice::Truncate))
		{
			qCritical() << "Cannot write" << qPrintable(kManifestRel);
			return false;
		}

		QXmlStreamWriter w(&mf);
		w.setAutoFormatting(true);
		w.setAutoFormattingIndent(1);
		w.writeStartDocument();
		w.writeStartElement(QStringLiteral("manifest:manifest"));
		w.writeNamespace(kManifestNs, QStringLiteral("manifest"));
		for (const auto& ns : input_meta.extra_namespaces)
		{
			w.writeNamespace(ns.second, ns.first);
		}
		w.writeAttribute(QStringLiteral("manifest:version"), input_meta.version);

		w.writeStartElement(QStringLiteral("manifest:file-entry"));
		w.writeAttribute(QStringLiteral("manifest:full-path"), QStringLiteral("/"));
		w.writeAttribute(QStringLiteral("manifest:version"), input_meta.version);
		w.writeAttribute(QStringLiteral("manifest:media-type"),
		                 QString::fromUtf8(root_media_type));
		w.writeEndElement();

		// Per-file manifest:version is optional in ODF Part 3. LibreOffice >=24
		// omits it on individual file entries and only stamps it on '/' (the
		// root entry above). Matching that convention keeps round-tripped
		// archives byte-similar to fresh LO output and avoids the cross-entry
		// version mismatch the previous hardcoded "1.2" introduced.
		for (const QString& rel : entries)
		{
			w.writeStartElement(QStringLiteral("manifest:file-entry"));
			w.writeAttribute(QStringLiteral("manifest:full-path"), rel);
			w.writeAttribute(QStringLiteral("manifest:media-type"), mediaTypeFor(rel));
			w.writeEndElement();
		}

		w.writeEndElement();
		w.writeEndDocument();
		mf.close();
		return mf.error() == QFile::NoError;
	}

	// Re-pack the extracted ODS tree into an OASIS-compliant archive: the
	// "mimetype" file is written first, STORED (no deflate, no data
	// descriptor flag, no extra field). LibreOffice >=24 rejects archives
	// that violate this with "damaged file". Every other entry is added
	// with QuaZip's default deflate compression.
	bool packOdfArchive(const QString& archive_path, const QString& src_dir)
	{
		QuaZip zip(archive_path);
		if (!zip.open(QuaZip::mdCreate))
		{
			qCritical() << "Failed to create archive:" << qPrintable(archive_path);
			return false;
		}

		zip.setDataDescriptorWritingEnabled(false);

		const QString mimetype_path = QDir(src_dir).filePath("mimetype");
		QFile mimetype_in(mimetype_path);
		if (!mimetype_in.open(QIODevice::ReadOnly))
		{
			qCritical() << "mimetype missing in" << qPrintable(src_dir);
			zip.close();
			return false;
		}
		const QByteArray mimetype_bytes = mimetype_in.readAll();
		mimetype_in.close();

		QuaZipNewInfo mimetype_info("mimetype", mimetype_path);
		mimetype_info.uncompressedSize = mimetype_bytes.size();
		QuaZipFile mimetype_out(&zip);
		if (!mimetype_out.open(QIODevice::WriteOnly, mimetype_info,
		                       NULL, 0, 0, 0))
		{
			qCritical() << "Failed to open mimetype zip entry";
			zip.close();
			return false;
		}
		if (mimetype_out.write(mimetype_bytes) != mimetype_bytes.size())
		{
			qCritical() << "Short write on mimetype entry";
			mimetype_out.close();
			zip.close();
			return false;
		}
		mimetype_out.close();

		zip.setDataDescriptorWritingEnabled(true);

		QDir src(src_dir);
		QDirIterator it(src_dir,
		                QDir::Files | QDir::Hidden | QDir::NoSymLinks,
		                QDirIterator::Subdirectories);
		while (it.hasNext())
		{
			const QString file_path = it.next();
			const QString in_archive = src.relativeFilePath(file_path);
			if (in_archive == "mimetype") continue;

			QFile in(file_path);
			if (!in.open(QIODevice::ReadOnly))
			{
				qCritical() << "Failed to read" << qPrintable(file_path);
				zip.close();
				return false;
			}
			QuaZipFile out(&zip);
			if (!out.open(QIODevice::WriteOnly,
			              QuaZipNewInfo(in_archive, file_path)))
			{
				qCritical() << "Failed to add" << qPrintable(in_archive);
				zip.close();
				return false;
			}

			char buf[64 * 1024];
			while (!in.atEnd())
			{
				const qint64 n = in.read(buf, sizeof(buf));
				if (n < 0 || out.write(buf, n) != n)
				{
					qCritical() << "Failed copying" << qPrintable(in_archive);
					out.close();
					zip.close();
					return false;
				}
			}
			out.close();
			in.close();
			if (out.getZipError() != 0)
			{
				qCritical() << "Zip entry close error on"
				            << qPrintable(in_archive)
				            << "code" << out.getZipError();
				zip.close();
				return false;
			}
		}

		zip.close();
		if (zip.getZipError() != 0)
		{
			qCritical() << "Zip close error" << zip.getZipError();
			return false;
		}
		return true;
	}
}

namespace qoasis
{
	FileOds::FileOds(const QString& full_path) : IFileable(full_path)
	{
		temp_dir_.setAutoRemove(true);
		if (temp_dir_.isValid())
		{
			temp_dir_path_ = temp_dir_.path();
			temp_dir_valid_ = true;
		}
		else
		{
			qCritical() << "The temporary directory is invalid";
		}
	}

	QSharedPointer<DocumentContent> FileOds::getContent() const
	{
		return content_file_->getContent();
	}

	bool FileOds::create()
	{
		if (exist() || !temp_dir_valid_)
		{
			return false;
		}
		content_file_ = new FileContent(QDir(temp_dir_path_).filePath(FileContent::kFileName));
		return true;
	}

	bool FileOds::load()
	{
		if (!exist() || !temp_dir_valid_)
		{
			return false;
		}

		extracted_file_paths_ = JlCompress::extractDir(full_path_, temp_dir_path_);

		if (extracted_file_paths_.isEmpty())
		{
			qCritical() << "Couldn't extract files";
			return false;
		}

		qInfo() << "Unzipped: " << full_path_;

		bool content_file_loaded = false;
		for (auto path : extracted_file_paths_)
		{
			if (path.endsWith(FileContent::kFileName))
			{
				content_file_ = new FileContent(path);
				content_file_loaded = content_file_->load();
			}
		}
		return content_file_loaded;
	}

	bool FileOds::save(const QString& full_path, bool overwrite_protected)
	{
		if (overwrite_protected && exist())
		{
			qCritical() << "File already exists: " << qPrintable(full_path_);
			return false;
		}

		if (content_file_ == nullptr)
		{
			// || document_styles_ == nullptr)
			qWarning() << "Nothing to save";
			return false;
		}

		QDir base_dir(temp_dir_path_);

		const bool content_file_saved = content_file_->save();
		if (!content_file_saved)
		{
			return false;
		}

		if (!writeFreshManifest(temp_dir_path_))
		{
			qCritical() << "Failed to regenerate manifest in: " << qPrintable(temp_dir_path_);
			return false;
		}

		QString save_path = (full_path == "" ? full_path_ : full_path);
		if (!packOdfArchive(save_path, temp_dir_path_))
		{
			qCritical() << "Failed to compress dir: " << qPrintable(temp_dir_path_);
			return false;
		}

		qInfo() << "Zipped: " << qPrintable(save_path);
		return true;
	}

	bool FileOds::save()
	{
		return save(full_path_, false);
	}
}
