// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef BODY_H
#define BODY_H

#include "../tag.h"

#include "spreadsheet.h"

namespace qoasis::office
{
	class Body : public Tag
	{
	public:
		Body();
		Body(QXmlStreamReader& reader);
		Body(const Body& obj);

		static QSharedPointer<Tag> builder(QXmlStreamReader& reader);
		static const QLatin1String kTag;

		QSharedPointer<Spreadsheet> getSpreadsheet();

		// implements Tag
		QLatin1String instanceTag() override;

	private:
		QSharedPointer<Spreadsheet> spreadsheet_;

		// implements Tag
		void readSubtag(QXmlStreamReader& reader) override;
		void writeSubtags(QXmlStreamWriter* writer) override;
	};
};
#endif // BODY_H
