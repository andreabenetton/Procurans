// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include "../tag.h"
#include "../table/table.h"

using namespace qoasis::table;

namespace qoasis::office
{
	class Spreadsheet : public Tag
	{
	public:
		Spreadsheet();
		Spreadsheet(QXmlStreamReader& reader);
		Spreadsheet(const Spreadsheet& obj);

		static QSharedPointer<Tag> builder(QXmlStreamReader& reader);
		static const QLatin1String kTag;

		QSharedPointer<Table> getTable(QString& name);
		QSharedPointer<Table> getTable(int index);

		// implements Tag
		QLatin1String instanceTag() override;

	protected:
		// implements Tag
		void readSubtag(QXmlStreamReader& reader) override;
		void writeSubtags(QXmlStreamWriter* writer) override;

	private:
		QVector<QSharedPointer<Table>> tables_;
	};
}
#endif // SPREADSHEET_H
