// Copyright 2019 - 2020, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "odscellfloat.h"

const QString ODSCellFloat::CELLTYPE = "float";
const QString ODSCellFloat::CELLVALUETAG = "office:value";

ODSCellFloat::ODSCellFloat(double number, int repeat) : ODSCell(repeat)
{
    _valueNumber = number;
}

ODSCellFloat::ODSCellFloat(QXmlStreamReader& reader) : ODSCell(reader)
{
    _valueNumber = 0.0;

    Deserialize(reader);
}

double ODSCellFloat::getDouble()
{
    return _valueNumber;
}

// implements ODSCell
QString ODSCellFloat::InstanceCellType()
{
    return CELLTYPE;
}

// implements ODSSerializable
void ODSCellFloat::Deserialize(QXmlStreamReader& reader)
{
    int i = 0;
    ODSSerializable::LoopForProperties(reader, i);
}

void ODSCellFloat::DeserializeProperty(QStringRef attributename, QStringRef attributevalue)
{
    ODSRepeatable::DeserializeProperty(attributename, attributevalue);
    ODSStyleable::DeserializeProperty(attributename, attributevalue);

    if (attributename == CELLVALUETAG) {
        _valueNumber = attributevalue.toFloat();
    }
}

void ODSCellFloat::Serialize(QXmlStreamWriter* writer)
{
    ODSSerializable::SerializeStart(writer);
    ODSRepeatable::SerializeProperties(writer);
    ODSStyleable::SerializeProperties(writer);
    ODSCell::SerializeProperties(writer);
    SerializeProperties(writer);
    SerializeSubitems(writer);
    ODSSerializable::SerializeEnd(writer);
}

void ODSCellFloat::SerializeProperties(QXmlStreamWriter* writer)
{
    writer->writeAttribute(CELLVALUETAG, QString::number(_valueNumber, 'f', 2));
}

void ODSCellFloat::SerializeSubitems(QXmlStreamWriter* writer)
{
    writer->writeTextElement(ODSCell::TEXTVALUETAG, QString::number(_valueNumber, 'f', 2).replace(".", ","));
}