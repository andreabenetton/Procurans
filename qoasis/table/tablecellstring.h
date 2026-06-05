// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECELLSTRING_H
#define TABLECELLSTRING_H

#include "tablecell.h"

namespace qoasis::table
{
	class TablecellString : public Tablecell
	{
	public:
		TablecellString(QString text, int repeat, QString style = "");
		TablecellString(QString text, QString style = "");
		TablecellString(QXmlStreamReader& reader);

		static const QString kCellTypeValue;
		static const QString kStringValueAttribute;

		// implements Tablecell
		QString instanceCellType() override;

	protected:
		// implements Tag
		void readAttribute(QStringView name, QStringView value) override;
		void writeAttributes(QXmlStreamWriter* writer) override;

	private:
		// office:string-value is OPTIONAL in ODF 1.2 §19.385 — <text:p>
		// alone is conformant. LibreOffice writes both for freshly-authored
		// cells, but auto-adding it on round-trip bloats files and can
		// disagree with rich-text <text:p> content (the attribute only
		// carries the flat projection). Default true so programmatic ctors
		// still emit a LO-style attribute; the XML-reading ctor clears it,
		// readAttribute flips it back on if the input carried one.
		bool _emit_string_value = true;
	};
}
#endif // TABLECELLSTRING_H
