// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "functions.h"
#include "CellAbstract.h"
#include "CellEmpty.h"
#include "CellString.h"
#include "CellDate.h"
#include "CellFloat.h"
#include "CellCurrency.h"

// Constants
const QString CellAbstract::TAG = "table:table-cell";
const QString CellAbstract::CELLTYPETAG = "office:value-type";
const QString CellAbstract::REPEATTAG = "table:number-columns-repeated";
const QString CellAbstract::TEXTVALUETAG = "text:p";

// Constructors
CellAbstract::CellAbstract(int repeat, QString style) : StyleableAbstract(style), RepeatableAbstract(repeat) {}

CellAbstract::CellAbstract(QXmlStreamReader& reader) : StyleableAbstract(""), RepeatableAbstract(1)
{
    Q_ASSERT(reader.qualifiedName() == CellAbstract::TAG);
}

// Static methods
CellAbstract* CellAbstract::Builder(QXmlStreamReader& reader)
{
    Q_ASSERT(reader.qualifiedName() == CellAbstract::TAG);

    if (!reader.attributes().hasAttribute(CellAbstract::CELLTYPETAG))
    {
        return new CellEmpty(reader);
    }

    QString type = reader.attributes().value(CellAbstract::CELLTYPETAG).toString();
    if (type.isEmpty())
    {
        return new CellEmpty(reader);
    }
    if (type == CellString::CELLTYPE)
    {
        return new CellString(reader);
    }
    if (type == CellDate::CELLTYPE)
    {
        return new CellDate(reader);
    }
    if (type == CellFloat::CELLTYPE)
    {
        return new CellFloat(reader);
    }
    if (type == CellCurrency::CELLTYPE)
    {
        return new CellCurrency(reader);
    }
    Q_ASSERT(false);
    //return nullptr;
}

// Methods


// implements ODSRepeatable
QString CellAbstract::RepeatTag()
{
    return REPEATTAG;
}

// implements ODSSerializable
QString CellAbstract::InstanceTag()
{
    return TAG;
}

QString CellAbstract::DeserializeSubitem(QXmlStreamReader& reader, int& numberofdeserializeitems)
{
    QString toret = "";
    if (reader.qualifiedName() == TEXTVALUETAG) {
        toret = reader.readElementText();
        numberofdeserializeitems++;
    }
    return toret;
}

void CellAbstract::SerializeProperties(QXmlStreamWriter* writer)
{
    writer->writeAttribute(CellAbstract::CELLTYPETAG, InstanceCellType());
    writer->writeAttribute("calcext:value-type", InstanceCellType());
}

void CellAbstract::SerializeSubitems(QXmlStreamWriter* writer)
{
}
