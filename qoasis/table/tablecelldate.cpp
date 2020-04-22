// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecelldate.h"

namespace qoasis::table {

    const QLatin1String TablecellDate::kCellTypeValue = QLatin1String("date");
    const QLatin1String TablecellDate::kCellTypeAttribute = QLatin1String("office:date-value");

    TablecellDate::TablecellDate(QDate date, int repeat) : Tablecell(repeat)
    {
        _valueDate = date;
    }

    TablecellDate::TablecellDate(QXmlStreamReader& reader) : Tablecell(reader)
    {
        Read(reader);
    }

    TablecellDate::TablecellDate(const TablecellDate &obj): Tablecell(obj)
    {
        _valueDate = obj._valueDate;
    }

    QDate TablecellDate::getDate()
    {
        return _valueDate;
    }

    // implements Tablecell
    QLatin1String TablecellDate::InstanceCellType()
    {
        return kCellTypeValue;
    }

    // implements Tag
    void TablecellDate::ReadAttribute(QStringRef attributename, QStringRef attributevalue)
    {
        if (attributename == kCellTypeAttribute) {
            _valueDate = QDate::fromString(attributevalue.toString(), "yyyy-MM-dd");
            return;
        }
        Tablecell::ReadAttribute(attributename, attributevalue);
    }

    void TablecellDate::WriteAttributes(QXmlStreamWriter* writer)
    {     
        if (!_valueDate.isNull() && _valueDate.isValid()) {
            writer->writeAttribute(kCellTypeAttribute, _valueDate.toString("yyyy-MM-dd"));
        }
        Tag::WriteAttributes(writer);
    }

    void TablecellDate::WriteSubtags(QXmlStreamWriter* writer)
    {
        if (!_valueDate.isNull() && _valueDate.isValid()) {
            writer->writeTextElement(Tablecell::kTextPTag, _valueDate.toString("dd/MM/yyyy"));
        }
    }
}
