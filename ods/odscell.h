// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef ODSCELL_H
#define ODSCELL_H

#include <QObject>
#include <QDate>
#include <QXmlStreamWriter>

#include "odsserializable.h"
#include "odsstyleable.h"
#include "odsrepeatable.h"

// Abstract class base for all cell types
class ODSCell : public ODSSerializable, public ODSStyleable, public ODSRepeatable
{
public:
    ODSCell(int repeat = 1, QString style = "");
    
    static ODSCell* Builder(QXmlStreamReader& reader);

    static const QString TAG;
    static const QString CELLTYPETAG;

    virtual QString InstanceCellType() = 0;

    // implements ODSRepeatable
    QString RepeatTag();

    // implements ODSSerializable
    QString InstanceTag();
    virtual void Serialize(QXmlStreamWriter* writer) = 0;

protected:
    ODSCell(QXmlStreamReader& reader);

    static const QString REPEATTAG;
    static const QString TEXTVALUETAG;

    // implements ODSSerializable
    virtual void SerializeProperties(QXmlStreamWriter* writer);
    virtual void SerializeSubitems(QXmlStreamWriter* writer);
    QString DeserializeSubitem(QXmlStreamReader& reader, int& c);

};

#endif // ODSCELL_H
