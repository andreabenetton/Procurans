// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "Row.h"
#include "CellEmpty.h"
#include "functions.h"

namespace Ods {
    // Constants
    const QString ODSRow::kTag = "table:table-row";
    const QString ODSRow::kRepeatAttribute = "table:number-rows-repeated";

    // Constructors
    ODSRow::ODSRow(int repeat, QString style) : StyleableAbstract(style), RepeatableAbstract(repeat)
    {
        InitializeContainers();
    }

    ODSRow::ODSRow(QXmlStreamReader& reader) : StyleableAbstract(""), RepeatableAbstract(1)
    {
        Q_ASSERT(reader.qualifiedName() == ODSRow::kTag);

        InitializeContainers();
        Deserialize(reader);
    }

    // Static methods
    ODSRow* ODSRow::Builder(QXmlStreamReader& reader)
    {
        Q_ASSERT(reader.qualifiedName() == ODSRow::kTag);

        return new ODSRow(reader);
    }

    // Methods
    void ODSRow::InitializeContainers()
    {
        _lastdefined = 0;
        _lastnonempty = 0;
        _row = new QVector<QSharedPointer<CellAbstract>>(256, QSharedPointer<CellAbstract>(nullptr));
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
        return kRepeatAttribute;
    }

    // implements ODSSerializable
    void ODSRow::Deserialize(QXmlStreamReader& reader)
    {
        int i = 0;
        SerializableAbstract::LoopForProperties(reader, i);
        SerializableAbstract::LoopForSubitems(reader, _lastdefined);
    }

    void ODSRow::DeserializeProperty(QStringRef attributename, QStringRef attributevalue)
    {
        RepeatableAbstract::DeserializeProperty(attributename, attributevalue);
        StyleableAbstract::DeserializeProperty(attributename, attributevalue);
    }

    QString ODSRow::DeserializeSubitem(QXmlStreamReader& reader, int& numberofdeserializeitems)
    {
        if (IsStartElementNamed(reader, CellAbstract::kTag)) {
            CellAbstract* cell = CellAbstract::Builder(reader);
            (*_row)[numberofdeserializeitems] = QSharedPointer<CellAbstract>(cell);
            numberofdeserializeitems += cell->GetRepeat();

            if (cell->InstanceCellType() != CellEmpty::kCellTypeValue) {
                _lastnonempty = numberofdeserializeitems;
            }
        }
        return "";
    }

    QString ODSRow::InstanceTag()
    {
        return ODSRow::kTag;
    }

    void ODSRow::Serialize(QXmlStreamWriter* writer)
    {
        SerializableAbstract::SerializeStart(writer);
        RepeatableAbstract::SerializeProperties(writer);
        StyleableAbstract::SerializeProperties(writer);
        ODSRow::SerializeSubitems(writer);
        SerializableAbstract::SerializeEnd(writer);
    }

    void ODSRow::SerializeProperties(QXmlStreamWriter* writer)
    {
    }

    void ODSRow::SerializeSubitems(QXmlStreamWriter* writer)
    {
        for (int i = 0; i < _row->length(); i++)
        {
            CellAbstract* cell = (*_row)[i].get();
            if (cell != nullptr)
            {
                cell->Serialize(writer);
            }
        }
    }
}