// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef DOCUMENTCONTENT_H
#define DOCUMENTCONTENT_H

#include "../tag.h"

#include "body.h"

namespace qoasis::office
{
	class DocumentContent : public Tag
	{
	public:
		DocumentContent();
		DocumentContent(QXmlStreamReader& reader);

		static QSharedPointer<Tag> builder(QXmlStreamReader& reader);

		static const QString kTag;

		static const QString kNsOfficeAttribute;
		static const QString kNsDcAttribute;
		static const QString kNsDrawAttribute;
		static const QString kNsFoAttribute;
		static const QString kNsNumberAttribute;
		static const QString kNsOfAttribute;
		static const QString kNsStyleAttribute;
		static const QString kNsSvgAttribute;
		static const QString kNsTableAttribute;
		static const QString kNsTextAttribute;
		static const QString kNsXlinkAttribute;

		static const QString kVersionAttribute;

		QString getVersion() const;
		QSharedPointer<Body> getBody() const;

		// implements Tag
		QString instanceTag() override;

	protected:
		// implements Tag
		void readAttribute(QStringRef name, QStringRef value) override;
		void readSubtag(QXmlStreamReader& reader) override;
		void writeAttributes(QXmlStreamWriter* writer) override;
		void writeSubtags(QXmlStreamWriter* writer) override;

	private:
		QSharedPointer<Body> body_;
		QMap<QString, QString> namespaces_;
		QString version_;
	};
}
#endif // DOCUMENTCONTENT_H
