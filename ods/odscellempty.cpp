// Copyright 2019 - 2020, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "odscellempty.h"

// Constants
const QString ODSCellEmpty::CELLTYPE = "";

// Constructors
ODSCellEmpty::ODSCellEmpty(int repeat, QString style) : ODSCell(repeat, style) {}

ODSCellEmpty::ODSCellEmpty(QXmlStreamReader& reader) : ODSCell(reader) {}

// Methods
QString ODSCellEmpty::InstanceCellType()
{
    return CELLTYPE;
}

// implements ODSSerializable
void ODSCellEmpty::Deserialize(QXmlStreamReader& reader)
{
    int i = 0;
    ODSSerializable::LoopForProperties(reader, i);
}

void ODSCellEmpty::DeserializeProperty(QStringRef attributename, QStringRef attributevalue)
{
    ODSRepeatable::DeserializeProperty(attributename, attributevalue);
    ODSStyleable::DeserializeProperty(attributename, attributevalue);
}

QString ODSCellEmpty::DeserializeSubitem(QXmlStreamReader& reader, int& numberofdeserializeitems)
{
    return "";
}

void ODSCellEmpty::Serialize(QXmlStreamWriter* writer)
{
    ODSSerializable::SerializeStart(writer);
    ODSRepeatable::SerializeProperties(writer);
    ODSStyleable::SerializeProperties(writer);
    ODSSerializable::SerializeEnd(writer);
}

void ODSCellEmpty::SerializeProperties(QXmlStreamWriter* writer)
{
}

void ODSCellEmpty::SerializeSubitems(QXmlStreamWriter* writer)
{
}
