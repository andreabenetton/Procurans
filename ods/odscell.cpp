// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "functions.h"
#include "odscell.h"
#include "odscellempty.h"
#include "odscellstring.h"
#include "odscellfloat.h"
#include "odscelldate.h"
#include "odscellcurrency.h"

// Constants
const QString ODSCell::TAG = "table:table-cell";
const QString ODSCell::CELLTYPETAG = "office:value-type";
const QString ODSCell::REPEATTAG = "table:number-columns-repeated";
const QString ODSCell::TEXTVALUETAG = "text:p";

// Constructors
ODSCell::ODSCell(int repeat, QString style) : ODSStyleable(style), ODSRepeatable(repeat) {}

ODSCell::ODSCell(QXmlStreamReader& reader) : ODSStyleable(""), ODSRepeatable(1)
{
    Q_ASSERT(reader.qualifiedName() == ODSCell::TAG);
}

// Static methods
ODSCell* ODSCell::Builder(QXmlStreamReader& reader)
{
    Q_ASSERT(reader.qualifiedName() == ODSCell::TAG);

    if (!reader.attributes().hasAttribute(ODSCell::CELLTYPETAG))
    {
        return new ODSCellEmpty(reader);
    }

    QString type = reader.attributes().value(ODSCell::CELLTYPETAG).toString();
    if (type.isEmpty())
    {
        return new ODSCellEmpty(reader);
    }
    if (type == ODSCellString::CELLTYPE)
    {
        return new ODSCellString(reader);
    }
    if (type == ODSCellDate::CELLTYPE)
    {
        return new ODSCellDate(reader);
    }
    if (type == ODSCellFloat::CELLTYPE)
    {
        return new ODSCellFloat(reader);
    }
    if (type == ODSCellCurrency::CELLTYPE)
    {
        return new ODSCellCurrency(reader);
    }
    Q_ASSERT(false);
    //return nullptr;
}

// Methods


// implements ODSRepeatable
QString ODSCell::RepeatTag()
{
    return REPEATTAG;
}

// implements ODSSerializable
QString ODSCell::InstanceTag()
{
    return TAG;
}

QVariant ODSCell::DeserializeSubitem(QXmlStreamReader& reader, int& numberofdeserializeitems)
{
    QString toret = "";
    if (reader.qualifiedName() == TEXTVALUETAG) {
        toret = reader.readElementText();
        numberofdeserializeitems++;
    }
    return toret;
}

void ODSCell::SerializeProperties(QXmlStreamWriter* writer)
{
    writer->writeAttribute(ODSCell::CELLTYPETAG, InstanceCellType());
    writer->writeAttribute("calcext:value-type", InstanceCellType());
}

void ODSCell::SerializeSubitems(QXmlStreamWriter* writer)
{
}
