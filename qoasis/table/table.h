// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLE_H
#define TABLE_H

#include <QStringList>

#include "../tag.h"
#include "../istyleable.h"
#include "../inameable.h"

#include "tablecolumn.h"
#include "tablerow.h"

namespace qoasis::table
{
	class Table : public Tag, public IStyleable, public INameable
	{
	public:
		Table(QString name = "", QString style = "");
		Table(QXmlStreamReader& reader);

		static QSharedPointer<Table> Builder(QXmlStreamReader& reader);
		static const QString kTag;

		// table:print-ranges is an ODF cellRangeAddressList — a
		// whitespace-separated list of cell-range addresses. The
		// previous QString getter returned the whole attribute opaque;
		// the typed list lets callers inspect or rewrite individual
		// ranges. The single-string getter/setter are kept for source
		// compatibility and convert by joining/splitting on whitespace.
		QStringList getPrintRanges() const;
		void setPrintRanges(const QStringList& ranges);
		QString getPrintRange();
		void setPrintRange(QString name);

		QSharedPointer<Tablecolumn> getColumn(int index) const;
		
		QSharedPointer<Tablerow> getRow(int index) const;
		int getLastNonEmptyRow() const;
		void removeRow(int index);
		void removeEndingEmptyRows();
		void insertRow(int index, QSharedPointer<Tablerow> row);
		void appendRow(QSharedPointer<Tablerow> row);

		// implements INameable
		QString nameTag() override;

		// implements Tag
		QString instanceTag() override;

	protected:
		// implements Tag
		void readSubtag(QXmlStreamReader& reader) override;
		void writeAttributes(QXmlStreamWriter* writer) override;
		void writeSubtags(QXmlStreamWriter* writer) override;
		void readAttribute(QStringView name, QStringView value) override;

	private:
		static const QString kNameAttribute;
		static const QString kPrintRangeAttribute;

		QStringList print_ranges_;
		RepeatVector<Tablecolumn> columns_ = RepeatVector<Tablecolumn>();
		int processed_columns_ = 0;
		RepeatVector<Tablerow> rows_ = RepeatVector<Tablerow>();
		int processed_rows_ = 0;
	};
}
#endif // TABLE_H
