// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef ODSCELLFLOAT_H
#define ODSCELLFLOAT_H

#include <QObject>
#include <QMap>
#include <QXmlStreamWriter>
#include "CellAbstract.h"

class CellFloat: public CellAbstract
{
public:
    CellFloat(double number, int repeat = 1);
    CellFloat(QXmlStreamReader& reader);

    static const QString CELLTYPE;

    double getDouble();

    // implements ODSSerializable
    void Serialize(QXmlStreamWriter* writer);

    // implements ODSCell
    QString InstanceCellType();

private:
    static const QString CELLVALUETAG;

    double _valueNumber;

    // implements ODSSerializable
    void Deserialize(QXmlStreamReader& reader);
    void DeserializeProperty(QStringRef attributename, QStringRef attributevalue);
    void SerializeProperties(QXmlStreamWriter* writer);
    void SerializeSubitems(QXmlStreamWriter* writer);
};

#endif // ODSCELLFLOAT_H