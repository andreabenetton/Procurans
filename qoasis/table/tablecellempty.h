// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECELLEMPTY_H
#define TABLECELLEMPTY_H

#include <QObject>
#include <QXmlStreamWriter>

#include "tablecell.h"

namespace qoasis::table {

    class TablecellEmpty : public Tablecell
    {
    public:
        TablecellEmpty(int repeat = 1, QString style = "");
        TablecellEmpty(QXmlStreamReader& reader);
        TablecellEmpty(const TablecellEmpty &obj);

        static const QLatin1String kCellTypeValue;

        // implements Tablecell
        virtual QLatin1String InstanceCellType();

    protected:
        // implements Tags
        virtual void ReadSubtag(QXmlStreamReader& reader);
        virtual void WriteAttributes(QXmlStreamWriter* writer);
        virtual void WriteSubtags(QXmlStreamWriter* writer);
        virtual void ReadAttribute(QStringRef attributename, QStringRef attributevalue);
    };
}
#endif // TABLECELLEMPTY_H
