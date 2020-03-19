// Copyright 2019 - 2020, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef ODSCELLSTRING_H
#define ODSCELLSTRING_H

#include <QObject>
#include <QXmlStreamWriter>
#include "odscell.h"

class ODSCellString: public ODSCell
{
public:
    ODSCellString(QString text, int repeat = 1); 
    ODSCellString(QXmlStreamReader& reader);

    static const QString CELLTYPE;

    QString getText();

    // implements ODSSerializable
    void Serialize(QXmlStreamWriter* writer);

    // implements ODSCell
    QString InstanceCellType();

private:
    QString _valueText;

    // implements ODSSerializable
    void Deserialize(QXmlStreamReader& reader);
    void DeserializeProperty(QStringRef attributename, QStringRef attributevalue);
    QVariant DeserializeSubitem(QXmlStreamReader& reader, int& c);
    void SerializeSubitems(QXmlStreamWriter* writer);
};

#endif // ODSCELLSTRING_H
