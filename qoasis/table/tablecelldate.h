// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECELLDATE_H
#define TABLECELLDATE_H

#include <QObject>
#include <QXmlStreamWriter>

#include "tablecell.h"

namespace qoasis::table {

    class TablecellDate : public Tablecell
    {
    public:
        TablecellDate(QDate date, int repeat = 1);
        TablecellDate(QXmlStreamReader& reader);
        TablecellDate(const TablecellDate &obj);

        static const QLatin1String kCellTypeValue;

        QDate getDate();

        // implements Tablecell
        virtual QLatin1String InstanceCellType();

    private:
        static const QLatin1String kCellTypeAttribute;

        QDate _valueDate;

        // implements Tags
        virtual void ReadAttribute(QStringRef attributename, QStringRef attributevalue);
        virtual void WriteAttributes(QXmlStreamWriter* writer);
        virtual void WriteSubtags(QXmlStreamWriter* writer);
    };
}
#endif // TABLECELLDATE_H
