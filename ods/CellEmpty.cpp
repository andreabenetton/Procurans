// Copyright 2019 - 2020, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "CellEmpty.h"

// Constants
const QString CellEmpty::CELLTYPE = "";

// Constructors
CellEmpty::CellEmpty(int repeat, QString style) : CellAbstract(repeat, style) {}

CellEmpty::CellEmpty(QXmlStreamReader& reader) : CellAbstract(reader) {}

// Methods
QString CellEmpty::InstanceCellType()
{
    return CELLTYPE;
}

// implements ODSSerializable
void CellEmpty::Deserialize(QXmlStreamReader& reader)
{
    int i = 0;
    SerializableAbstract::LoopForProperties(reader, i);
}

void CellEmpty::DeserializeProperty(QStringRef attributename, QStringRef attributevalue)
{
    RepeatableAbstract::DeserializeProperty(attributename, attributevalue);
    StyleableAbstract::DeserializeProperty(attributename, attributevalue);
}

QString CellEmpty::DeserializeSubitem(QXmlStreamReader& reader, int& numberofdeserializeitems)
{
    return "";
}

void CellEmpty::Serialize(QXmlStreamWriter* writer)
{
    SerializableAbstract::SerializeStart(writer);
    RepeatableAbstract::SerializeProperties(writer);
    StyleableAbstract::SerializeProperties(writer);
    SerializableAbstract::SerializeEnd(writer);
}

void CellEmpty::SerializeProperties(QXmlStreamWriter* writer)
{
}

void CellEmpty::SerializeSubitems(QXmlStreamWriter* writer)
{
}
