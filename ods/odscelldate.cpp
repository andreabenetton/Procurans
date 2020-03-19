// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "odscelldate.h"

const QString ODSCellDate::CELLTYPE = "date";
const QString ODSCellDate::CELLVALUETAG = "office:date-value";

ODSCellDate::ODSCellDate(QDate date, int repeat) : ODSCell(repeat)
{
    _valueDate = date;
}

ODSCellDate::ODSCellDate(QXmlStreamReader& reader) : ODSCell(reader)
{
    Deserialize(reader);
}

QDate ODSCellDate::getDate()
{
    return _valueDate;
}

// implements ODSCell
QString ODSCellDate::InstanceCellType()
{
    return CELLTYPE;
}

// implements ODSSerializable
void ODSCellDate::Deserialize(QXmlStreamReader& reader)
{
    int i = 0;
    ODSSerializable::LoopForProperties(reader, i);
}

void ODSCellDate::DeserializeProperty(QStringRef attributename, QStringRef attributevalue)
{
    ODSRepeatable::DeserializeProperty(attributename, attributevalue);
    ODSStyleable::DeserializeProperty(attributename, attributevalue);

    if (attributename == CELLVALUETAG) {
        _valueDate = QDate::fromString(attributevalue.toString(), "yyyy-MM-dd");
    }
}

void ODSCellDate::Serialize(QXmlStreamWriter* writer)
{
    ODSSerializable::SerializeStart(writer);
    ODSRepeatable::SerializeProperties(writer);
    ODSStyleable::SerializeProperties(writer);
    if (!_valueDate.isNull() && _valueDate.isValid()) {
        ODSCell::SerializeProperties(writer);
        SerializeProperties(writer);
        SerializeSubitems(writer);
    }
    ODSSerializable::SerializeEnd(writer);
}

void ODSCellDate::SerializeProperties(QXmlStreamWriter* writer)
{
    writer->writeAttribute(CELLVALUETAG, _valueDate.toString("yyyy-MM-dd"));
}

void ODSCellDate::SerializeSubitems(QXmlStreamWriter* writer)
{
    writer->writeTextElement(ODSCell::TEXTVALUETAG, _valueDate.toString("dd/MM/yyyy"));
}
