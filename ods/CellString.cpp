// Copyright 2019 - 2020, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "CellString.h"

namespace Ods {

    const QString CellString::kCellTypeValue = "string";

    CellString::CellString(QString text, int repeat) : CellAbstract(repeat)
    {
        _valueText = text;
    }

    CellString::CellString(QXmlStreamReader& reader) : CellAbstract(reader)
    {
        Deserialize(reader);
    }

    QString CellString::getText()
    {
        return _valueText;
    }

    // implements ODSCell
    QString CellString::InstanceCellType()
    {
        return kCellTypeValue;
    }

    // implements ODSSerializable
    void CellString::Deserialize(QXmlStreamReader& reader)
    {
        int i = 0;
        SerializableAbstract::LoopForProperties(reader, i);
        i = 0;
        SerializableAbstract::LoopForSubitems(reader, i);
    }

    void CellString::DeserializeProperty(QStringRef attributename, QStringRef attributevalue)
    {
        RepeatableAbstract::DeserializeProperty(attributename, attributevalue);
        StyleableAbstract::DeserializeProperty(attributename, attributevalue);
    }

    QString CellString::DeserializeSubitem(QXmlStreamReader& reader, int& numberofdeserializeitems)
    {
        _valueText = CellAbstract::DeserializeSubitem(reader, numberofdeserializeitems);
        return "";
    }


    void CellString::Serialize(QXmlStreamWriter* writer)
    {
        SerializableAbstract::SerializeStart(writer);
        RepeatableAbstract::SerializeProperties(writer);
        StyleableAbstract::SerializeProperties(writer);
        if (!(_valueText.isNull() || _valueText.isEmpty())) {
            CellAbstract::SerializeProperties(writer);
            SerializeSubitems(writer);
        }
        SerializableAbstract::SerializeEnd(writer);
    }

    void CellString::SerializeSubitems(QXmlStreamWriter* writer)
    {
        if (!(_valueText.isNull() || _valueText.isEmpty())) {
            writer->writeTextElement(CellAbstract::kTextTag, _valueText);
        }
    }
}