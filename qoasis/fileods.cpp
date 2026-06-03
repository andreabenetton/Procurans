// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include <QDebug>
#include <QDirIterator>
#include <QFile>

#include "fileods.h"
#include "quazip/JlCompress.h"
#include "quazip/quazip.h"
#include "quazip/quazipfile.h"
#include "quazip/quazipnewinfo.h"

namespace
{
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
