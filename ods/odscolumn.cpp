// Copyright 2019 - 2020, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "odscolumn.h"
#include "functions.h"

// Constants
const QString ODSColumn::TAG = "table:table-column";
const QString ODSColumn::REPEATTAG = "table:number-columns-repeated";
const QString ODSColumn::DEFAULTCELLSTYLETAG = "table:default-cell-style-name";

// Constructors
ODSColumn::ODSColumn(int repeat = 1, QString columnstyle = "", QString defaultcellstyle = "") : ODSStyleable(columnstyle), ODSRepeatable(repeat)
{
    _lastdefined = 0;
    _defaultcellstyle = defaultcellstyle;
}

ODSColumn::ODSColumn(QXmlStreamReader& reader) : ODSStyleable(""), ODSRepeatable(1)
{
    Q_ASSERT(reader.qualifiedName() == ODSColumn::TAG);

    Deserialize(reader);
}

// Static methods
ODSColumn* ODSColumn::Builder(QXmlStreamReader& reader)
{
    Q_ASSERT(reader.qualifiedName() == ODSColumn::TAG);

    return new ODSColumn(reader);
}

// Methods
int ODSColumn::GetLastDefined()
{
    return _lastdefined;
}

QString ODSColumn::GetDefaultCellStyle()
{
    return _defaultcellstyle;
}

void ODSColumn::SetDefaultCellStyle(QString style)
{
    _defaultcellstyle = style;
}

// implements ODSRepeatable
QString ODSColumn::RepeatTag()
{
    return ODSColumn::REPEATTAG;
}

// implements ODSSerializable
QString ODSColumn::InstanceTag()
{
    return ODSColumn::TAG;
}

void ODSColumn::Deserialize(QXmlStreamReader& reader)
{
    int i = 0;
    ODSSerializable::LoopForProperties(reader, i);
}

void ODSColumn::DeserializeProperty(QStringRef attributename, QStringRef attributevalue)
{
    ODSRepeatable::DeserializeProperty(attributename, attributevalue);
    ODSStyleable::DeserializeProperty(attributename, attributevalue);
    if (attributename == DEFAULTCELLSTYLETAG) {
        _defaultcellstyle = attributevalue.toString();
    }
}

QVariant ODSColumn::DeserializeSubitem(QXmlStreamReader& reader, int& c)
{
    // LoopForSubitems never called; this is a placeholder function, should never be called
    Q_ASSERT(false);
}

void ODSColumn::Serialize(QXmlStreamWriter* writer)
{
    ODSSerializable::SerializeStart(writer);
    ODSRepeatable::SerializeProperties(writer);
    ODSStyleable::SerializeProperties(writer);
    ODSColumn::SerializeProperties(writer);
    ODSSerializable::SerializeEnd(writer);
}

void ODSColumn::SerializeProperties(QXmlStreamWriter* writer)
{
    if (_defaultcellstyle != "") {
        writer->writeAttribute(ODSColumn::DEFAULTCELLSTYLETAG, _defaultcellstyle);
    }
}

void ODSColumn::SerializeSubitems(QXmlStreamWriter* writer)
{
    // this is a placeholder function, should never be called
    Q_ASSERT(false);
}