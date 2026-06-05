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

		static const QString kVersionAttribute;

		QString getVersion() const;
		QSharedPointer<Body> getBody() const;

		// implements Tag
		QString instanceTag() override;

	protected:
		// implements Tag
		void readAttribute(QStringView name, QStringView value) override;
		void readSubtag(QXmlStreamReader& reader) override;
		void writeAttributes(QXmlStreamWriter* writer) override;
		void writeSubtags(QXmlStreamWriter* writer) override;

	private:
		QSharedPointer<Body> body_;
		QString version_;
	};
}
#endif // DOCUMENTCONTENT_H
