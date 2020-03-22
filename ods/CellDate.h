// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef ODSCELLDATE_H
#define ODSCELLDATE_H

#include <QObject>
#include <QXmlStreamWriter>
#include "CellAbstract.h"

class CellDate: public CellAbstract
{
public:
    CellDate(QDate date, int repeat = 1) ;
    CellDate(QXmlStreamReader& reader);

    static const QString CELLTYPE;

    QDate getDate();

    // implements ODSSerializable
    void Serialize(QXmlStreamWriter* writer);

    // implements ODSCell
    QString InstanceCellType();

private:
    static const QString CELLVALUETAG;

    QDate _valueDate;

    // implements ODSSerializable
    void Deserialize(QXmlStreamReader& reader);
    void DeserializeProperty(QStringRef attributename, QStringRef attributevalue);
    void SerializeProperties(QXmlStreamWriter* writer);
    void SerializeSubitems(QXmlStreamWriter* writer);
};

#endif // ODSCELLDATE_H
