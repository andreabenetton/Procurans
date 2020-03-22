// Copyright 2019 - 2020, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "CellFloat.h"

const QString CellFloat::CELLTYPE = "float";
const QString CellFloat::CELLVALUETAG = "office:value";

CellFloat::CellFloat(double number, int repeat) : CellAbstract(repeat)
{
    _valueNumber = number;
}

CellFloat::CellFloat(QXmlStreamReader& reader) : CellAbstract(reader)
{
    _valueNumber = 0.0;

    Deserialize(reader);
}

double CellFloat::getDouble()
{
    return _valueNumber;
}

// implements ODSCell
QString CellFloat::InstanceCellType()
{
    return CELLTYPE;
}

// implements ODSSerializable
void CellFloat::Deserialize(QXmlStreamReader& reader)
{
    int i = 0;
    SerializableAbstract::LoopForProperties(reader, i);
}

void CellFloat::DeserializeProperty(QStringRef attributename, QStringRef attributevalue)
{
    RepeatableAbstract::DeserializeProperty(attributename, attributevalue);
    StyleableAbstract::DeserializeProperty(attributename, attributevalue);

    if (attributename == CELLVALUETAG) {
        _valueNumber = attributevalue.toFloat();
    }
}

void CellFloat::Serialize(QXmlStreamWriter* writer)
{
    SerializableAbstract::SerializeStart(writer);
    RepeatableAbstract::SerializeProperties(writer);
    StyleableAbstract::SerializeProperties(writer);
    CellAbstract::SerializeProperties(writer);
    SerializeProperties(writer);
    SerializeSubitems(writer);
    SerializableAbstract::SerializeEnd(writer);
}

void CellFloat::SerializeProperties(QXmlStreamWriter* writer)
{
    writer->writeAttribute(CELLVALUETAG, QString::number(_valueNumber, 'f', 2));
}

void CellFloat::SerializeSubitems(QXmlStreamWriter* writer)
{
    writer->writeTextElement(CellAbstract::TEXTVALUETAG, QString::number(_valueNumber, 'f', 2).replace(".", ","));
}