// Copyright 2019 - 2020, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "Table.h"
#include "functions.h"

namespace Ods {

    const QString Table::kTag = "table:table";
    const QString Table::NAMETAG = "table:name";
    const QString Table::PRINTRANGETAG = "table:print-ranges";

    int Table::_sheetcount = 1;

    Table::Table(QString name)
    {
        Initialize();

        if (name != "") {
            _name = name;
        }
        else {
            _name = "Sheet" + QString::number(_sheetcount++);
        }
    }

    Table::Table(QXmlStreamReader& reader)
    {
        Q_ASSERT(reader.qualifiedName() == Table::kTag);
        Initialize();
        Deserialize(reader);
    }

    // Static methods
    Table* Table::Builder(QXmlStreamReader& reader)
    {
        Q_ASSERT(reader.qualifiedName() == Table::kTag);

        return new Table(reader);
    }

    // Methods
    void Table::Initialize()
    {
        _rows = new QVector<QSharedPointer<ODSRow>>(256);
        _columns = new QVector<QSharedPointer<Column>>(256);
        _printranges = "";
    }

    // implements ODSSerializable
    void Table::Deserialize(QXmlStreamReader& reader)
    {
        int i = 0;
        SerializableAbstract::LoopForProperties(reader, i);
        int j = 0;
        SerializableAbstract::LoopForSubitems(reader, j);
    }

    void Table::DeserializeProperty(QStringRef attributename, QStringRef attributevalue)
    {
        StyleableAbstract::DeserializeProperty(attributename, attributevalue);
        if (attributename == NAMETAG) {
            _name = attributevalue.toString();
        }
        if (attributename == PRINTRANGETAG) {
            _printranges = attributevalue.toString();
        }
    }

    QString Table::DeserializeSubitem(QXmlStreamReader& reader, int& numberofdeserializeitems)
    {
        if (IsStartElementNamed(reader, Column::kTag)) {
            Column* column = Column::Builder(reader);
            (*_columns)[numberofdeserializeitems] = QSharedPointer<Column>(column);
            numberofdeserializeitems += column->GetRepeat();
        }
        if (IsStartElementNamed(reader, ODSRow::kTag)) {
            ODSRow* row = ODSRow::Builder(reader);
            (*_rows)[numberofdeserializeitems] = QSharedPointer<ODSRow>(row);
            numberofdeserializeitems += row->GetRepeat();
        }
        return "";
    }

    QString Table::InstanceTag()
    {
        return kTag;
    }

    void Table::Serialize(QXmlStreamWriter* writer)
    {
        SerializableAbstract::SerializeStart(writer);
        StyleableAbstract::SerializeProperties(writer);
        Table::SerializeSubitems(writer);
        SerializableAbstract::SerializeEnd(writer);
    }

    void Table::SerializeProperties(QXmlStreamWriter* writer)
    {
        if (_name != "") {
            writer->writeAttribute(Table::NAMETAG, _name);
        }
        if (_printranges != "") {
            writer->writeAttribute(Table::PRINTRANGETAG, _printranges);
        }
    }

    void Table::SerializeSubitems(QXmlStreamWriter* writer)
    {
        for (int i = 0; i < _columns->length(); i++)
        {
            Column* column = (*_columns)[i].get();
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
}