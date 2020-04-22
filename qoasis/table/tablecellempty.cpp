// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecellempty.h"

namespace qoasis::table {
    // Constants
    const QLatin1String TablecellEmpty::kCellTypeValue = QLatin1String("");

    // Constructors
    TablecellEmpty::TablecellEmpty(int repeat, QString style) : Tablecell(repeat, style) {}

    TablecellEmpty::TablecellEmpty(QXmlStreamReader& reader) : Tablecell(reader) {}

    TablecellEmpty::TablecellEmpty(const TablecellEmpty &obj): Tablecell(obj) {}

    // Methods
    QLatin1String TablecellEmpty::InstanceCellType()
    {
        return kCellTypeValue;
    }

    // implements Tag
    void TablecellEmpty::ReadAttribute(QStringRef attributename, QStringRef attributevalue)
    {
        Tablecell::ReadAttribute(attributename, attributevalue);
    }

    void TablecellEmpty::ReadSubtag(QXmlStreamReader& reader)
    {
    }

    void TablecellEmpty::WriteAttributes(QXmlStreamWriter* writer)
    {
    }

    void TablecellEmpty::WriteSubtags(QXmlStreamWriter* writer)
    {
    }
}
