// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "spreadsheet.h"
#include "../table/table.h"

namespace qoasis::office {

    // Constants
    const QLatin1String Spreadsheet::kTag = QLatin1String("office:spreadsheet");

    // Constructors
    Spreadsheet::Spreadsheet() : Tag()
    {
        Initialize();
        tables_->append(QSharedPointer<Table>(new Table));
    }

    Spreadsheet::Spreadsheet(QXmlStreamReader& reader): Tag()
    {
        Q_ASSERT(reader.qualifiedName() == Spreadsheet::kTag);
        Initialize();
        Tag::Read(reader);
    }

    Spreadsheet::Spreadsheet(const Spreadsheet &obj)
    {
        Initialize();
        // deep copy on tables
        for (int i = 0; i < tables_->length(); i++) {
            QSharedPointer<Table> table = obj.tables_->at(i);
            if (!table.isNull()) {
                tables_->replace(i, QSharedPointer<Table>(new Table(*table)));
            }
        }
    }

    Spreadsheet::~Spreadsheet()
    {
        delete tables_;
    }

    // Static methods
    QSharedPointer<Tag> Spreadsheet::Builder(QXmlStreamReader& reader)
    {
        Q_ASSERT(reader.qualifiedName() == Spreadsheet::kTag);
        return QSharedPointer<Tag>(new Spreadsheet(reader));
    }

    // Methods
    void Spreadsheet::Initialize()
    {
        tables_ = new QVector<QSharedPointer<Table>>();
    }

    QSharedPointer<Table> Spreadsheet::GetTable(QString& name)
    {
        for (auto& table : *tables_) {
            if (!table.isNull()) {
                if (name==(table->GetName())) {
                    return table;
                }
            }
        }
        return QSharedPointer<Table>(nullptr);
    }

    QSharedPointer<Table> Spreadsheet::GetTable(int index)
    {
        Q_ASSERT(index >= 0);
        if((index>= (*tables_).length())) {
            return QSharedPointer<Table>(nullptr);
        }
        return (*tables_)[index];
    }

    // implements Tag
    QLatin1String Spreadsheet::InstanceTag()
    {
        return Spreadsheet::kTag;
    }

    void Spreadsheet::ReadSubtag(QXmlStreamReader& reader)
    {
        if (IsStartElementNamed(reader, Table::kTag)) {
            tables_->append(QSharedPointer<Table>(new Table(reader)));
            return;
        }
        // Deserialize present but unsupported subtags
        Tag::ReadSubtag(reader);
    }

    void Spreadsheet::WriteSubtags(QXmlStreamWriter* writer)
    {
        for (auto& table : *tables_) {
            if (!table.isNull()) {
                table->Write(writer);
            }
        }
        // Serialize present but unsupported subtags
        Tag::WriteSubtags(writer);
    }
}
