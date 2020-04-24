// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include <QDebug>
#include <QList>
#include <QSaveFile>

#include "filecontent.h"
#include "office/documentcontent.h"

namespace qoasis
{
	const QLatin1String FileContent::kFileName = QLatin1String("content.xml");

	FileContent::FileContent(const QString& full_path) : FileXml(full_path)
	{
	}

	QLatin1String FileContent::instanceFileName()
	{
		return FileContent::kFileName;
	}

	QLatin1String FileContent::getRootTag()
	{
		return DocumentContent::kTag;
	}

	bool FileContent::create()
	{
		content_ = QSharedPointer<DocumentContent>(new DocumentContent());
		return true;
	}

	void FileContent::read(QXmlStreamReader& reader)
	{
		content_ = QSharedPointer<DocumentContent>(new DocumentContent(reader));
	}

	void FileContent::write(QXmlStreamWriter* writer)
	{
		content_->write(writer);
	}

	QSharedPointer<DocumentContent> FileContent::getContent()
	{
		return content_;
	}
}
