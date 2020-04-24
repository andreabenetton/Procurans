// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECELLSTRING_H
#define TABLECELLSTRING_H

#include "tablecell.h"

namespace qoasis::table {

    class TablecellString : public Tablecell
    {
    public:
        TablecellString(QString text, int repeat = 1);
        TablecellString(QXmlStreamReader& reader);
        TablecellString(const TablecellString &obj);

        static const QLatin1String kCellTypeValue;

        // implements Tablecell
        QLatin1String instanceCellType() override;
    };
}
#endif // TABLECELLSTRING_H
