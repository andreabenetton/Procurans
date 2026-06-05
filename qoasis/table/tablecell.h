// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECELL_H
#define TABLECELL_H

#include "../tag.h"
#include "../istyleable.h"
#include "../irepeatable.h"

namespace qoasis::table
{
	/**
	 * \brief The <table:table-cell> element represents a table cell. It is contained in a table row element.
	 *  A table cell can contain paragraphs and other text content as well as sub tables.
	 *  Table cells may span multiple columns and rows. Table cells may be empty.
	 *  https://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#__RefHeading__1415590_253892949
	 */
	class Tablecell : public Tag, public IStyleable, public IRepeatable
	{
	public:
		Tablecell(QString style = "");
		Tablecell(int repeat, QString style = "");

		static QSharedPointer<Tablecell> clone(QSharedPointer<Tablecell> obj);
		static QSharedPointer<Tablecell> builder(QXmlStreamReader& reader);
		static QSharedPointer<Tablecell> placeholder(int repeat = 1);

		static const QString kTag;
		static const QString kCellTypeAttribute;
		static const QString kCellTypeValue;

		virtual QString instanceCellType();

		QString getText() const;

		// Typed accessors for table:number-columns-spanned /
		// table:number-rows-spanned (ODF Part 1 §19.677 / §19.683). 1 means
		// the cell does not span; values >1 cover that many columns or rows
		// of trailing <table:covered-table-cell> placeholders. The previous
		// API only preserved these as opaque attributes via the generic Tag
		// path.
		int getColumnSpan() const;
		void setColumnSpan(int span);
		int getRowSpan() const;
		void setRowSpan(int span);

		// implements IRepeatable
		QString repeatTag() override;


		// implements Tag
		QString instanceTag() override;
		bool isEmpty() override;

	protected:
		Tablecell(QXmlStreamReader& reader);

		static const QString kRepeatAttribute;
		static const QString kTextPTag;

		// implements Tag
		void writeAttributes(QXmlStreamWriter* writer) override;
		void writeSubtags(QXmlStreamWriter* writer) override;
		void readAttribute(QStringView name, QStringView value) override;
		void readSubtag(QXmlStreamReader& reader) override;

		QString _valueText;
		QVector<QSharedPointer<Tag>> _paragraphs;
		// Captures office:value-type for cells that don't have a typed
		// subclass (boolean / time / percentage / future ODF or LO
		// extensions). Typed subclasses override instanceCellType() and
		// leave this empty; on write, the explicit type wins.
		QString _valueType;
		// table:number-columns-spanned / table:number-rows-spanned default
		// to 1 (no span). Captured from input on read, re-emitted on write
		// only when non-default to keep round-tripped XML byte-similar to
		// what LibreOffice writes.
		int _columnSpan = 1;
		int _rowSpan = 1;
	};
}
#endif // TABLECELL_H
