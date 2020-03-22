// Copyright 2019 - 2020, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "odssheet.h"
#include "functions.h"

const QString ODSSheet::TAG = "table:table";
const QString ODSSheet::NAMETAG = "table:name";
const QString ODSSheet::PRINTRANGETAG = "table:print-ranges";

int ODSSheet::_sheetcount = 1;

ODSSheet::ODSSheet(QString name)
{
    Initialize();

    if (name != "") {
        _name = name;
    }
    else {
        _name = "Sheet" + QString::number(_sheetcount++);
    }
}

ODSSheet::ODSSheet(QXmlStreamReader& reader)
{
    Q_ASSERT(reader.qualifiedName() == ODSSheet::TAG);
    Initialize();
    Deserialize(reader);
}

// Static methods
ODSSheet* ODSSheet::Builder(QXmlStreamReader& reader)
{
    Q_ASSERT(reader.qualifiedName() == ODSSheet::TAG);

    return new ODSSheet(reader);
}

// Methods
void ODSSheet::Initialize()
{
    _rows = new QVector<QSharedPointer<ODSRow>>(256);
    _columns = new QVector<QSharedPointer<ODSColumn>>(256);
    _printranges = "";
}

// implements ODSSerializable
void ODSSheet::Deserialize(QXmlStreamReader& reader)
{
    int i = 0;
    ODSSerializable::LoopForProperties(reader, i);
    int j = 0;
    ODSSerializable::LoopForSubitems(reader, j);
}

void ODSSheet::DeserializeProperty(QStringRef attributename, QStringRef attributevalue)
{
    ODSStyleable::DeserializeProperty(attributename, attributevalue);
    if (attributename == NAMETAG) {
        _name = attributevalue.toString();
    }
    if (attributename == PRINTRANGETAG) {
        _printranges = attributevalue.toString();
    }
}

QString ODSSheet::DeserializeSubitem(QXmlStreamReader& reader, int& numberofdeserializeitems)
{
    if (IsStartElementNamed(reader, ODSColumn::TAG)) {
        ODSColumn* column = ODSColumn::Builder(reader);
        (*_columns)[numberofdeserializeitems] = QSharedPointer<ODSColumn>(column);
        numberofdeserializeitems += column->GetRepeat();
    }
    if (IsStartElementNamed(reader, ODSRow::TAG)) {
        ODSRow* row = ODSRow::Builder(reader);
        (*_rows)[numberofdeserializeitems] = QSharedPointer<ODSRow>(row);
        numberofdeserializeitems += row->GetRepeat();
    }
    return "";
}

QString ODSSheet::InstanceTag()
{
    return TAG;
}

void ODSSheet::Serialize(QXmlStreamWriter* writer)
{
    ODSSerializable::SerializeStart(writer);
    ODSStyleable::SerializeProperties(writer);
    ODSSheet::SerializeSubitems(writer);
    ODSSerializable::SerializeEnd(writer);
}

void ODSSheet::SerializeProperties(QXmlStreamWriter* writer)
{
    if (_name != "") {
        writer->writeAttribute(ODSSheet::NAMETAG, _name);
    }
    if (_printranges != "") {
        writer->writeAttribute(ODSSheet::PRINTRANGETAG, _printranges);
    }
}

void ODSSheet::SerializeSubitems(QXmlStreamWriter* writer)
{
    for (int i = 0; i < _columns->length(); i++)
    {
        ODSColumn* column = (*_columns)[i].get();
        if (column != nullptr)
        {
            column->Serialize(writer);
        }
    }

    for (int i = 0; i < _rows->length(); i++)
    {
        ODSRow* row = (*_rows)[i].get();
        if (row != nullptr)
        {
            row->Serialize(writer);
        }
    }
}