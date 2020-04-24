// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECELLDATE_H
#define TABLECELLDATE_H

#include <QDateTime>

#include "tablecell.h"

namespace qoasis::table {

    class TablecellDate : public Tablecell
    {
    public:
        TablecellDate(QDate date, int repeat = 1);
        TablecellDate(QXmlStreamReader& reader);
        TablecellDate(const TablecellDate &obj);

        static const QLatin1String kCellTypeValue;

        QDate getDate() const;

        // implements Tablecell
        QLatin1String instanceCellType() override;

    private:
        static const QLatin1String kCellTypeAttribute;

        QDate value_date_;

        // implements Tags
        void readAttribute(QStringRef name, QStringRef value) override;
        void writeAttributes(QXmlStreamWriter* writer) override;
        void writeSubtags(QXmlStreamWriter* writer) override;
    };
}
#endif // TABLECELLDATE_H
