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

		// implements IRepeatable
		QString repeatTag() override;
		

		// implements Tag
		QString instanceTag() override;
		bool isEmpty() override;

	protected:
		Tablecell(QXmlStreamReader& reader);

		static const QString kRepeatAttribute;
		static const QString kTextPTag;
		static const QString kCalcextValueType;

		// implements Tag
		void writeAttributes(QXmlStreamWriter* writer) override;
		void writeSubtags(QXmlStreamWriter* writer) override;
		void readAttribute(QStringRef name, QStringRef value) override;
		void readSubtag(QXmlStreamReader& reader) override;

		QString _valueText;
	};
}
#endif // TABLECELL_H
