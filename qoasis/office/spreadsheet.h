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

		static QSharedPointer<Tag> builder(QXmlStreamReader& reader);
		static const QString kTag;

		QSharedPointer<Table> getTable(QString& name);
		QSharedPointer<Table> getTable(int index);

		// implements Tag
		QString instanceTag() override;

	protected:
		// implements Tag
		void readAttribute(QStringRef name, QStringRef value) override;
		void readSubtag(QXmlStreamReader& reader) override;
		void writeAttributes(QXmlStreamWriter* writer) override;
		void writeSubtags(QXmlStreamWriter* writer) override;

	private:
		QVector<QSharedPointer<Table>> tables_;
	};
}
#endif // SPREADSHEET_H
