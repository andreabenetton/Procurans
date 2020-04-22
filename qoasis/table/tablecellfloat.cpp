// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecellfloat.h"

namespace qoasis::table {

    const QLatin1String TablecellFloat::kCellTypeValue = QLatin1String("float");
    const QLatin1String TablecellFloat::kCellTypeAttribute = QLatin1String("office:value");

    TablecellFloat::TablecellFloat(double number, int repeat) : Tablecell(repeat)
    {
        valueNumber_ = number;
    }

    TablecellFloat::TablecellFloat(QXmlStreamReader& reader) : Tablecell(reader)
    {
        valueNumber_ = 0.0;

        Read(reader);
    }

    TablecellFloat::TablecellFloat(const TablecellFloat &obj): Tablecell(obj)
    {
        valueNumber_ = obj.valueNumber_;
    }

    double TablecellFloat::getDouble()
    {
        return valueNumber_;
    }

    // implements Tablecell
    QLatin1String TablecellFloat::InstanceCellType()
    {
        return kCellTypeValue;
    }

    // implements Tag
    void TablecellFloat::ReadAttribute(QStringRef attributename, QStringRef attributevalue)
    {
        if (attributename == kCellTypeAttribute) {
            valueNumber_ = attributevalue.toFloat();
            return;
        }
        Tablecell::ReadAttribute(attributename, attributevalue);
    }

    void TablecellFloat::WriteAttributes(QXmlStreamWriter* writer)
    {
        Tablecell::WriteAttributes(writer);
        writer->writeAttribute(kCellTypeAttribute, QString::number(valueNumber_, 'f', 2));
    }

    void TablecellFloat::WriteSubtags(QXmlStreamWriter* writer)
    {
        writer->writeTextElement(Tablecell::kTextPTag, QString::number(valueNumber_, 'f', 2).replace(".", ","));
    }
}
