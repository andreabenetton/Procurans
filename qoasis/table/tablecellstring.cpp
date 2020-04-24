// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecellstring.h"

namespace qoasis::table {

    const QLatin1String TablecellString::kCellTypeValue = QLatin1String("string");

    TablecellString::TablecellString(QString text, int repeat) : Tablecell(repeat)
    {
        _valueText = text;
    }

    TablecellString::TablecellString(QXmlStreamReader& reader) : Tablecell(reader)
    {
        read(reader);
    }

    TablecellString::TablecellString(const TablecellString &obj): Tablecell(obj)
    {
        _valueText = obj._valueText;
    }

    // implements Tablecell
    QLatin1String TablecellString::instanceCellType()
    {
        return kCellTypeValue;
    }

}
