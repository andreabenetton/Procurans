// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECELLFLOAT_H
#define TABLECELLFLOAT_H

#include <QObject>
#include <QMap>
#include <QXmlStreamWriter>

#include "tablecell.h"

namespace qoasis::table {

    class TablecellFloat : public Tablecell
    {
    public:
        TablecellFloat(double number, int repeat = 1);
        TablecellFloat(QXmlStreamReader& reader);
        TablecellFloat(const TablecellFloat &obj);

        static const QLatin1String kCellTypeValue;

        double getDouble();

        // implements Tablecell
        virtual QLatin1String InstanceCellType();

    protected:
        // implements Tag
        virtual void ReadAttribute(QStringRef attributename, QStringRef attributevalue);
        virtual void WriteAttributes(QXmlStreamWriter* writer);
        virtual void WriteSubtags(QXmlStreamWriter* writer);

    private:
        static const QLatin1String kCellTypeAttribute;

        double valueNumber_;
    };
}
#endif // TABLECELLFLOAT_H
