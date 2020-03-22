// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "CellDate.h"

const QString CellDate::CELLTYPE = "date";
const QString CellDate::CELLVALUETAG = "office:date-value";

CellDate::CellDate(QDate date, int repeat) : CellAbstract(repeat)
{
    _valueDate = date;
}

CellDate::CellDate(QXmlStreamReader& reader) : CellAbstract(reader)
{
    Deserialize(reader);
}

QDate CellDate::getDate()
{
    return _valueDate;
}

// implements ODSCell
QString CellDate::InstanceCellType()
{
    return CELLTYPE;
}

// implements ODSSerializable
void CellDate::Deserialize(QXmlStreamReader& reader)
{
    int i = 0;
    SerializableAbstract::LoopForProperties(reader, i);
}

void CellDate::DeserializeProperty(QStringRef attributename, QStringRef attributevalue)
{
    RepeatableAbstract::DeserializeProperty(attributename, attributevalue);
    StyleableAbstract::DeserializeProperty(attributename, attributevalue);

    if (attributename == CELLVALUETAG) {
        _valueDate = QDate::fromString(attributevalue.toString(), "yyyy-MM-dd");
    }
}

void CellDate::Serialize(QXmlStreamWriter* writer)
{
    SerializableAbstract::SerializeStart(writer);
    RepeatableAbstract::SerializeProperties(writer);
    StyleableAbstract::SerializeProperties(writer);
    if (!_valueDate.isNull() && _valueDate.isValid()) {
        CellAbstract::SerializeProperties(writer);
        SerializeProperties(writer);
        SerializeSubitems(writer);
    }
    SerializableAbstract::SerializeEnd(writer);
}

void CellDate::SerializeProperties(QXmlStreamWriter* writer)
{
    writer->writeAttribute(CELLVALUETAG, _valueDate.toString("yyyy-MM-dd"));
}

void CellDate::SerializeSubitems(QXmlStreamWriter* writer)
{
    writer->writeTextElement(CellAbstract::TEXTVALUETAG, _valueDate.toString("dd/MM/yyyy"));
}
