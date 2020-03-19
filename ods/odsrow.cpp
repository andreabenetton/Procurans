// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "odsrow.h"
#include "odscellempty.h"
#include "functions.h"

// Constants
const QString ODSRow::TAG = "table:table-row";
const QString ODSRow::REPEATTAG = "table:number-rows-repeated";

// Constructors
ODSRow::ODSRow(int repeat, QString style) : ODSStyleable(style), ODSRepeatable(repeat)
{
    InitializeContainers();
}

ODSRow::ODSRow(QXmlStreamReader& reader) : ODSStyleable(""), ODSRepeatable(1)
{
    Q_ASSERT(reader.qualifiedName() == ODSRow::TAG);

    InitializeContainers();
    Deserialize(reader);
}

// Static methods
ODSRow* ODSRow::Builder(QXmlStreamReader& reader)
{
    Q_ASSERT(reader.qualifiedName() == ODSRow::TAG);

    return new ODSRow(reader);
}

// Methods
void ODSRow::InitializeContainers()
{
    _lastdefined = 0;
    _lastnonempty = 0;
    _row = new QVector<QSharedPointer<ODSCell>>(256, QSharedPointer<ODSCell>(nullptr));
}

int ODSRow::GetLastDefined()
{
    return _lastdefined;
}

int ODSRow::GetLastNonEmpty()
{
    return _lastnonempty;
}

// implements ODSRepeatable
QString ODSRow::RepeatTag()
{
    return REPEATTAG;
}

// implements ODSSerializable
void ODSRow::Deserialize(QXmlStreamReader& reader)
{
    int i = 0;
    ODSSerializable::LoopForProperties(reader, i);
    ODSSerializable::LoopForSubitems(reader, _lastdefined);
}

void ODSRow::DeserializeProperty(QStringRef attributename, QStringRef attributevalue)
{
    ODSRepeatable::DeserializeProperty(attributename, attributevalue);
    ODSStyleable::DeserializeProperty(attributename, attributevalue);
}

QVariant ODSRow::DeserializeSubitem(QXmlStreamReader& reader, int & numberofdeserializeitems)
{
    if (IsStartElementNamed(reader, ODSCell::TAG)) {
        ODSCell* cell = ODSCell::Builder(reader);
        (*_row)[numberofdeserializeitems] = QSharedPointer<ODSCell>(cell);
        numberofdeserializeitems += cell->GetRepeat();

        if (cell->InstanceCellType() != ODSCellEmpty::CELLTYPE) {
            _lastnonempty = numberofdeserializeitems;
        }
    }
    return 0;
}

QString ODSRow::InstanceTag()
{
    return ODSRow::TAG;
}

void ODSRow::Serialize(QXmlStreamWriter* writer)
{
    ODSSerializable::SerializeStart(writer);
    ODSRepeatable::SerializeProperties(writer);
    ODSStyleable::SerializeProperties(writer);
    ODSRow::SerializeSubitems(writer);
    ODSSerializable::SerializeEnd(writer);
}

void ODSRow::SerializeProperties(QXmlStreamWriter* writer)
{
}

void ODSRow::SerializeSubitems(QXmlStreamWriter* writer)
{
    for (int i = 0; i < _row->length(); i++)
    {
        ODSCell* cell = (*_row)[i].get();
        if (cell != nullptr)
        {
            cell->Serialize(writer);
        }
    }
}
