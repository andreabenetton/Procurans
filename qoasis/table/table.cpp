// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "table.h"
#include "../irepeatable.h"

namespace qoasis::table {

    const QLatin1String Table::kTag = QLatin1String("table:table");
    const QLatin1String Table::kNameAttribute = QLatin1String("table:name");
    const QLatin1String Table::kPrintRangeAttribute = QLatin1String("table:print-ranges");

    // Constructors
    Table::Table(QString name) : INameable(name)
    {
        Initialize();
        rows_->append(QSharedPointer<Tablerow>(new Tablerow));
        columns_->append(QSharedPointer<Tablecolumn>(new Tablecolumn));
    }

    Table::Table(QXmlStreamReader& reader) : Table("")
    {
        Q_ASSERT(reader.qualifiedName() == Table::kTag);
        Read(reader);
    }

    Table::Table(const Table &obj): IStyleable(obj), INameable(obj)
    {
        Initialize();
        // deep copy on rows
        for (int i = 0; i < rows_->length(); i++) {
            QSharedPointer<Tablerow> row = obj.rows_->at(i);
            if (!row.isNull()) {
                rows_->replace(i, QSharedPointer<Tablerow>(new Tablerow(*row)));
            }
        }
        // deep copy on columns
        for (int i = 0; i < columns_->length(); i++) {
            QSharedPointer<Tablecolumn> column = obj.columns_->at(i);
            if (!column.isNull()) {
                columns_->replace(i, QSharedPointer<Tablecolumn>(new Tablecolumn(*column)));
            }
        }
        printranges_ = obj.printranges_;
    }

    Table::~Table()
    {
        delete rows_;
        delete columns_;
    }

    // Static methods
    QSharedPointer<Tag> Table::Builder(QXmlStreamReader& reader)
    {
        Q_ASSERT(reader.qualifiedName() == Table::kTag);
        return QSharedPointer<Tag>(new Table(reader));
    }

    // Methods
    void Table::Initialize()
    {
        rows_ = new QVector<QSharedPointer<Tablerow>>(256, QSharedPointer<Tablerow>(nullptr));
        columns_ = new QVector<QSharedPointer<Tablecolumn>>(256, QSharedPointer<Tablecolumn>(nullptr));
        printranges_ = "";
    }

    int Table::ScanForwardForBaseOfRepeatedRows(int index)
    {
        return IRepeatable::ScanForwardForNotNull<Tablerow>(index, *rows_);
    }

    int Table::ScanBackwardForBaseOfRepeatedRows(int index)
    {
        return IRepeatable::ScanBackwardForNotNull<Tablerow>(index, *rows_);
    }

    int Table::ScanForwardForBaseOfRepeatedColumns(int index)
    {
        return IRepeatable::ScanForwardForNotNull<Tablecolumn>(index, *columns_);
    }

    int Table::ScanBackwardForBaseOfRepeatedColumns(int index)
    {
        return IRepeatable::ScanBackwardForNotNull<Tablecolumn>(index, *columns_);
    }


    QSharedPointer<Tablecolumn> Table::GetColumn(int index)
    {
        Q_ASSERT(index >= 0);
        if((index>=(columns_)->length())) {
            return QSharedPointer<Tablecolumn>(nullptr);
        }
        return (*columns_)[index];
    }

    QSharedPointer<Tablerow> Table::GetRow(int index)
    {
        Q_ASSERT(index >= 0);
        if((index>=(rows_)->length())) {
            return QSharedPointer<Tablerow>(nullptr);
        }
        return (*rows_)[index];
    }


    void Table::AddRow(QSharedPointer<Tablerow> row)
    {
        (*rows_).append(row);
        processedrows_++;
    }

    // implements NameableAbstract
    QLatin1String Table::NameTag()
    {
        return kNameAttribute;
    }

    // implements ODSSerializable
    QLatin1String Table::InstanceTag()
    {
        return Table::kTag;
    }

    void Table::Read(QXmlStreamReader& reader)
    {
        processedcolumns_ = 0;
        processedrows_ = 0;
        Tag::Read(reader);
    }

    void Table::ReadAttribute(QStringRef attributename, QStringRef attributevalue)
    {
        if (attributename == NameTag()) {
            INameable::DeserializeProperty(attributevalue);
            return;
        }
        if (attributename == StyleTag()) {
            IStyleable::DeserializeProperty(attributevalue);
            return;
        }
        if (attributename == kPrintRangeAttribute) {
            printranges_ = attributevalue.toString();
            return;
        }
        // Deserialize present but unsupported attributes
        Tag::ReadAttribute(attributename, attributevalue);
    }

    void Table::ReadSubtag(QXmlStreamReader& reader)
    {
        if (IsStartElementNamed(reader, Tablecolumn::kTag)) {
            Tablecolumn* column = new Tablecolumn(reader);
            (*columns_)[processedcolumns_] = QSharedPointer<Tablecolumn>(column);
            processedcolumns_ += column->GetRepeat();
        }
        if (IsStartElementNamed(reader, Tablerow::kTag)) {
            Tablerow* row = new Tablerow(reader);
            (*rows_)[processedrows_] = QSharedPointer<Tablerow>(row);
            processedrows_ += row->GetRepeat();
        }
        // Deserialize present but unsupported subtags
        Tag::ReadSubtag(reader);
    }

    void Table::WriteAttributes(QXmlStreamWriter* writer)
    {
        INameable::SerializeProperties(writer);
        IStyleable::SerializeProperties(writer);

        if (printranges_ != "") {
            writer->writeAttribute(Table::kPrintRangeAttribute, printranges_);
        }
        // Serialize present but unsupported attributes
        Tag::WriteAttributes(writer);
    }

    void Table::WriteSubtags(QXmlStreamWriter* writer)
    {
        for (auto& column : *columns_) {
            if (!column.isNull()) {
                column->Write(writer);
            }
        }
        for (auto& row : *rows_) {
            if (!row.isNull()) {
                row->Write(writer);
            }
        }
        // Serialize present but unsupported subtags
        Tag::WriteSubtags(writer);
    }
}
