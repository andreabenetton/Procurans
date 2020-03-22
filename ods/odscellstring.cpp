// Copyright 2019 - 2020, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "odscellstring.h"

const QString ODSCellString::CELLTYPE = "string";

ODSCellString::ODSCellString(QString text, int repeat) : ODSCell(repeat)
{
    _valueText = text;
}

ODSCellString::ODSCellString(QXmlStreamReader& reader) : ODSCell(reader)
{
    Deserialize(reader);
}

QString ODSCellString::getText()
{
    return _valueText;
}

// implements ODSCell
QString ODSCellString::InstanceCellType()
{
    return CELLTYPE;
}

// implements ODSSerializable
void ODSCellString::Deserialize(QXmlStreamReader& reader)
{
    int i = 0;
    ODSSerializable::LoopForProperties(reader, i);
    i = 0;
    ODSSerializable::LoopForSubitems(reader, i);
}

void ODSCellString::DeserializeProperty(QStringRef attributename, QStringRef attributevalue)
{
    ODSRepeatable::DeserializeProperty(attributename, attributevalue);
    ODSStyleable::DeserializeProperty(attributename, attributevalue);
}

QString ODSCellString::DeserializeSubitem(QXmlStreamReader& reader, int& numberofdeserializeitems)
{
    _valueText = ODSCell::DeserializeSubitem(reader, numberofdeserializeitems);
    return "";
}


void ODSCellString::Serialize(QXmlStreamWriter* writer)
{
    ODSSerializable::SerializeStart(writer);
    ODSRepeatable::SerializeProperties(writer);
    ODSStyleable::SerializeProperties(writer);
    if (!(_valueText.isNull() || _valueText.isEmpty())) {
        ODSCell::SerializeProperties(writer);
        SerializeSubitems(writer);
    }
    ODSSerializable::SerializeEnd(writer);
}

void ODSCellString::SerializeSubitems(QXmlStreamWriter* writer)
{
    if (!(_valueText.isNull() || _valueText.isEmpty())) {
        writer->writeTextElement(ODSCell::TEXTVALUETAG, _valueText);
    }
}