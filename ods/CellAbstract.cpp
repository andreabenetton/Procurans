// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "functions.h"
#include "CellAbstract.h"
#include "CellEmpty.h"
#include "CellString.h"
#include "CellDate.h"
#include "CellFloat.h"
#include "CellCurrency.h"

namespace Ods {

    // Constants
    const QString CellAbstract::kTag = "table:table-cell";
    const QString CellAbstract::kCellTypeAttribute = "office:value-type";
    const QString CellAbstract::kRepeatAttribute = "table:number-columns-repeated";
    const QString CellAbstract::kTextTag = "text:p";

    // Constructors
    CellAbstract::CellAbstract(int repeat, QString style) : StyleableAbstract(style), RepeatableAbstract(repeat) {}

    CellAbstract::CellAbstract(QXmlStreamReader& reader) : StyleableAbstract(""), RepeatableAbstract(1)
    {
        Q_ASSERT(reader.qualifiedName() == CellAbstract::kTag);
    }

    // Static methods
    CellAbstract* CellAbstract::Builder(QXmlStreamReader& reader)
    {
        Q_ASSERT(reader.qualifiedName() == CellAbstract::kTag);

        if (!reader.attributes().hasAttribute(CellAbstract::kCellTypeAttribute))
        {
            return new CellEmpty(reader);
        }

        QString type = reader.attributes().value(CellAbstract::kCellTypeAttribute).toString();
        if (type.isEmpty())
        {
            return new CellEmpty(reader);
        }
        if (type == CellString::kCellTypeValue)
        {
            return new CellString(reader);
        }
        if (type == CellDate::kCellTypeValue)
        {
            return new CellDate(reader);
        }
        if (type == CellFloat::kCellTypeValue)
        {
            return new CellFloat(reader);
        }
        if (type == CellCurrency::kCellTypeValue)
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
        return kRepeatAttribute;
    }

    // implements ODSSerializable
    QString CellAbstract::InstanceTag()
    {
        return kTag;
    }

    QString CellAbstract::DeserializeSubitem(QXmlStreamReader& reader, int& numberofdeserializeitems)
    {
        QString toret = "";
        if (reader.qualifiedName() == kTextTag) {
            toret = reader.readElementText();
            numberofdeserializeitems++;
        }
        return toret;
    }

    void CellAbstract::SerializeProperties(QXmlStreamWriter* writer)
    {
        writer->writeAttribute(CellAbstract::kCellTypeAttribute, InstanceCellType());
        writer->writeAttribute("calcext:value-type", InstanceCellType());
    }

    void CellAbstract::SerializeSubitems(QXmlStreamWriter* writer)
    {
    }
}