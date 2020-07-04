// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef FILECONTENT_H
#define FILECONTENT_H

#include "office/documentcontent.h"
#include "filexml.h"

using namespace qoasis::office;

namespace qoasis
{
	class FileContent : public FileXml
	{
	public:
		explicit FileContent(const QString& full_path);

		virtual bool create();
		QSharedPointer<DocumentContent> getContent();
		QString instanceFileName() override;

		static const QString kFileName;

	protected:
		QString getRootTag() override;
		void read(QXmlStreamReader& xml) override;
		void write(QXmlStreamWriter* xml) override;

		QSharedPointer<DocumentContent> content_;
	};
}
#endif // FILECONTENT_H
