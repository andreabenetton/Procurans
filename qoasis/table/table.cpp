// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "table.h"
#include "../irepeatable.h"

namespace qoasis::table {

    const QLatin1String Table::kTag = QLatin1String("table:table");
    const QLatin1String Table::kNameAttribute = QLatin1String("table:name");
    const QLatin1String Table::kPrintRangeAttribute = QLatin1String("table:print-ranges");

    // Constructors
    Table::Table(QString name, QString style) : Tag(), IStyleable(style), INameable(name)
    {
    }

    Table::Table(QXmlStreamReader& reader) 
    {
        Q_ASSERT(reader.qualifiedName() == Table::kTag);
        Table::read(reader);
    }

    Table::Table(const Table &obj): Tag(obj), IStyleable(obj), INameable(obj)
    {
        // deep copy on rows
        for (int i = 0; i < rows_.length(); i++) {
            QSharedPointer<Tablerow> row = obj.rows_.at(i);
            if (!row.isNull()) {
                rows_.replace(i, QSharedPointer<Tablerow>(new Tablerow(*row)));
            }
        }
        // deep copy on columns
        for (int i = 0; i < columns_.length(); i++) {
            QSharedPointer<Tablecolumn> column = obj.columns_.at(i);
            if (!column.isNull()) {
                columns_.replace(i, QSharedPointer<Tablecolumn>(new Tablecolumn(*column)));
            }
        }
        printranges_ = obj.printranges_;
    }

    // Static methods
    QSharedPointer<Tag> Table::Builder(QXmlStreamReader& reader)
    {
        Q_ASSERT(reader.qualifiedName() == Table::kTag);
        return QSharedPointer<Tag>(new Table(reader));
    }

    // Methods
    int Table::scanForwardForBaseOfRepeatedRows(int index) const
    {
        return IRepeatable::scanForwardForNotNull<Tablerow>(index, rows_);
    }

    int Table::scanBackwardForBaseOfRepeatedRows(int index) const
    {
        return IRepeatable::scanBackwardForNotNull<Tablerow>(index, rows_);
    }

    int Table::scanForwardForBaseOfRepeatedColumns(int index)
    {
        return IRepeatable::scanForwardForNotNull<Tablecolumn>(index, columns_);
    }

    int Table::scanBackwardForBaseOfRepeatedColumns(int index)
    {
        return IRepeatable::scanBackwardForNotNull<Tablecolumn>(index, columns_);
    }


    QSharedPointer<Tablecolumn> Table::GetColumn(int index)
    {
        Q_ASSERT(index >= 0);
        if((index>=columns_.length())) {
            return QSharedPointer<Tablecolumn>(nullptr);
        }
        return columns_.at(index);
    }

    QSharedPointer<Tablerow> Table::GetRow(int index)
    {
        Q_ASSERT(index >= 0);
        if((index>=rows_.length())) {
            return QSharedPointer<Tablerow>(nullptr);
        }
        return rows_.at(index);
    }


    void Table::AddRow(QSharedPointer<Tablerow> row)
    {
        rows_.append(row);
        processedrows_++;
    }

    // implements INameable
    QLatin1String Table::nameTag()
    {
        return kNameAttribute;
    }

    // implements Tag
    QLatin1String Table::instanceTag()
    {
        return Table::kTag;
    }

    void Table::read(QXmlStreamReader& reader)
    {
        processedcolumns_ = 0;
        processedrows_ = 0;
        Tag::read(reader);
    }

    void Table::readAttribute(QStringRef attributename, QStringRef attributevalue)
    {
        if (attributename == nameTag()) {
            INameable::readName(attributevalue);
            return;
        }
        if (attributename == styleTag()) {
            IStyleable::readStyle(attributevalue);
            return;
        }
        if (attributename == kPrintRangeAttribute) {
            printranges_ = attributevalue.toString();
            return;
        }
        // Deserialize present but unsupported attributes
        Tag::readAttribute(attributename, attributevalue);
    }

    void Table::readSubtag(QXmlStreamReader& reader)
    {
        if (isStartElementNamed(reader, Tablecolumn::kTag)) {
            Tablecolumn* column = new Tablecolumn(reader);
            columns_[processedcolumns_] = QSharedPointer<Tablecolumn>(column);
            processedcolumns_ += column->getRepeat();
        }
        if (isStartElementNamed(reader, Tablerow::kTag)) {
            Tablerow* row = new Tablerow(reader);
            rows_[processedrows_] = QSharedPointer<Tablerow>(row);
            processedrows_ += row->getRepeat();
        }
        // Deserialize present but unsupported subtags
        Tag::readSubtag(reader);
    }

    void Table::writeAttributes(QXmlStreamWriter* writer)
    {
        INameable::writeName(writer);
        IStyleable::writeStyle(writer);

        if (printranges_ != "") {
            writer->writeAttribute(Table::kPrintRangeAttribute, printranges_);
        }
        // Serialize present but unsupported attributes
        Tag::writeAttributes(writer);
    }

    void Table::writeSubtags(QXmlStreamWriter* writer)
    {
        for (auto& column : columns_) {
            if (!column.isNull()) {
                column->write(writer);
            }
        }
        for (auto& row : rows_) {
            if (!row.isNull()) {
                row->write(writer);
            }
        }
        // Serialize present but unsupported subtags
        Tag::writeSubtags(writer);
    }
}
