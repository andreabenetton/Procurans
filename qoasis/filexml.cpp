// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include <QDebug>
#include <QList>
#include <QSaveFile>

#include "filexml.h"

namespace qoasis
{
	FileXml::FileXml(const QString& full_path) : IFileable(full_path)
	{
	}

	bool FileXml::load()
	{
		if (!exist())
		{
			qCritical() << "File not exists: " << qPrintable(full_path_);
			return false;
		}

		if (!full_path_.endsWith(instanceFileName()))
		{
			qCritical() << "File name not match specification: " << qPrintable(full_path_);
			return false;
		}

		QFile file(full_path_);

		if (!file.open(QFile::ReadOnly | QFile::Text))
		{
			qCritical() << "Failed to open file: " << qPrintable(file.fileName())
				<< "error :" << qPrintable(file.errorString());
			return false;
		}

		QXmlStreamReader reader(&file);

		bool rootTokenFound = false;

		while (!reader.atEnd()) {
			reader.readNext();
			if (reader.tokenType() == QXmlStreamReader::StartElement) {
				if (reader.qualifiedName() == getRootTag()) {
					rootTokenFound = true;
					read(reader);
				}
			}
		}

		file.close();

		if (!rootTokenFound)
		{
			qCritical() << "Error: the XML file "
				<< qPrintable(instanceFileName()) << " doesn't comply XML expected schema - root missed";
			return false;
		}

		if (reader.hasError())
		{
			qCritical() << "Error: Failed to parse XML in file "
				<< qPrintable(instanceFileName()) << ": "
				<< qPrintable(reader.errorString());
			return false;
		}
		if (file.error() != QFile::NoError)
		{
			qCritical() << "Cannot read file " << qPrintable(instanceFileName())
				<< ": " << qPrintable(file.errorString());
			return false;
		}

		qInfo() << qPrintable(instanceFileName()) << " file loaded.";
		return true;
	}

	bool FileXml::save(const QString& full_path, const bool overwrite_protected)
	{
		if (overwrite_protected && exist())
		{
			qCritical() << "File already exists: " << qPrintable(full_path_);
			return false;
		}

		const QString save_path = (full_path == "" ? full_path_ : full_path);

		QFile file(save_path);
		file.open(QIODevice::WriteOnly);

		QXmlStreamWriter writer(&file);
		writer.setAutoFormatting(true);
		writer.setAutoFormattingIndent(2);
		writer.writeStartDocument();

		write(&writer);

		writer.writeEndDocument();
		file.close();
		if (file.error() != QFile::NoError)
		{
			qCritical() << "Cannot write file " << qPrintable(instanceFileName())
				<< ": " << qPrintable(file.errorString());
			return false;
		}

		qInfo() << qPrintable(instanceFileName()) << " file saved.";
		return true;
	}

	bool FileXml::save()
	{
		return FileXml::save(full_path_, false);
	}

	bool FileXml::isStartElementNamed(QXmlStreamReader& xml, const QString& token_name)
	{
		return ((xml.tokenType() == QXmlStreamReader::StartElement) && (xml.qualifiedName() == token_name));
	}

	bool FileXml::isNotEndElementNamed(QXmlStreamReader& xml, const QString& token_name)
	{
		return !((xml.tokenType() == QXmlStreamReader::EndElement) && (xml.qualifiedName() == token_name));
	}
}
