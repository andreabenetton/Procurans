// Copyright 2019 - 2020, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef ODSCELLEMPTY_H
#define ODSCELLEMPTY_H

#include <QObject>
#include <QXmlStreamWriter>
#include "CellAbstract.h"

class CellEmpty : public CellAbstract
{
public:
    CellEmpty(int repeat = 1, QString style = "");
    CellEmpty(QXmlStreamReader& reader);

    static const QString CELLTYPE;

    // implements ODSSerializable
    void Serialize(QXmlStreamWriter* writer);
    void Deserialize(QXmlStreamReader& reader);
    QString DeserializeSubitem(QXmlStreamReader& reader, int& c);
    void SerializeProperties(QXmlStreamWriter* writer);
    void SerializeSubitems(QXmlStreamWriter* writer);
    void DeserializeProperty(QStringRef attributename, QStringRef attributevalue);

    // implements ODSCell
    QString InstanceCellType();
};

#endif // ODSCELLEMPTY_H