// Copyright 2019 - 2020, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "Column.h"
#include "functions.h"

// Constants
const QString Column::TAG = "table:table-column";
const QString Column::REPEATTAG = "table:number-columns-repeated";
const QString Column::DEFAULTCELLSTYLETAG = "table:default-cell-style-name";

// Constructors
Column::Column(int repeat = 1, QString columnstyle = "", QString defaultcellstyle = "") : StyleableAbstract(columnstyle), RepeatableAbstract(repeat)
{
    _lastdefined = 0;
    _defaultcellstyle = defaultcellstyle;
}

Column::Column(QXmlStreamReader& reader) : StyleableAbstract(""), RepeatableAbstract(1)
{
    Q_ASSERT(reader.qualifiedName() == Column::TAG);

    Deserialize(reader);
}

// Static methods
Column* Column::Builder(QXmlStreamReader& reader)
{
    Q_ASSERT(reader.qualifiedName() == Column::TAG);

    return new Column(reader);
}

// Methods
int Column::GetLastDefined()
{
    return _lastdefined;
}

QString Column::GetDefaultCellStyle()
{
    return _defaultcellstyle;
}

void Column::SetDefaultCellStyle(QString style)
{
    _defaultcellstyle = style;
}

// implements ODSRepeatable
QString Column::RepeatTag()
{
    return Column::REPEATTAG;
}

// implements ODSSerializable
QString Column::InstanceTag()
{
    return Column::TAG;
}

void Column::Deserialize(QXmlStreamReader& reader)
{
    int i = 0;
    SerializableAbstract::LoopForProperties(reader, i);
}

void Column::DeserializeProperty(QStringRef attributename, QStringRef attributevalue)
{
    RepeatableAbstract::DeserializeProperty(attributename, attributevalue);
    StyleableAbstract::DeserializeProperty(attributename, attributevalue);
    if (attributename == DEFAULTCELLSTYLETAG) {
        _defaultcellstyle = attributevalue.toString();
    }
}

QVariant Column::DeserializeSubitem(QXmlStreamReader& reader, int& c)
{
    // LoopForSubitems never called; this is a placeholder function, should never be called
    Q_ASSERT(false);
}

void Column::Serialize(QXmlStreamWriter* writer)
{
    SerializableAbstract::SerializeStart(writer);
    RepeatableAbstract::SerializeProperties(writer);
    StyleableAbstract::SerializeProperties(writer);
    Column::SerializeProperties(writer);
    SerializableAbstract::SerializeEnd(writer);
}

void Column::SerializeProperties(QXmlStreamWriter* writer)
{
    if (_defaultcellstyle != "") {
        writer->writeAttribute(Column::DEFAULTCELLSTYLETAG, _defaultcellstyle);
    }
}

void Column::SerializeSubitems(QXmlStreamWriter* writer)
{
    // this is a placeholder function, should never be called
    Q_ASSERT(false);
}