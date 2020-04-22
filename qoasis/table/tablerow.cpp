// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablerow.h"
#include "tablecell.h"
#include "tablecellempty.h"

namespace qoasis::table {
    // Constants
    const QLatin1String Tablerow::kTag = QLatin1String("table:table-row");
    const QLatin1String Tablerow::kRepeatAttribute = QLatin1String("table:number-rows-repeated");

    // Constructors
    Tablerow::Tablerow(int repeat, QString style) : IStyleable(style), IRepeatable(repeat)
    {
        Initialize();
    }

    Tablerow::Tablerow(QXmlStreamReader& reader) : Tablerow(1, "")
    {
        Q_ASSERT(reader.qualifiedName() == Tablerow::kTag);
        Tag::Read(reader);
    }

    Tablerow::Tablerow(const Tablerow &obj) : IStyleable(obj), IRepeatable(obj)
    {
        Initialize();
        for (int i = 0; i < cells_->length(); i++) {
            QSharedPointer<Tablecell> cell = obj.cells_->at(i);
            if (!cell.isNull()) {
                cells_->replace(i, Tablecell::Clone(cell));
            }
        }
        lastdefined_ = obj.lastdefined_;
        lastnonempty_ = obj.lastnonempty_;
    }

    Tablerow::~Tablerow()
    {
        delete cells_;
    }

    // Static methods
    QSharedPointer<Tag>Tablerow::Builder(QXmlStreamReader& reader)
    {
        Q_ASSERT(reader.qualifiedName() == Tablerow::kTag);
        return QSharedPointer<Tag>(new Tablerow(reader));
    }

    // Methods
    void Tablerow::Initialize()
    {
        lastdefined_ = 0;
        lastnonempty_ = 0;
        cells_ = new QVector<QSharedPointer<Tablecell>>(256, QSharedPointer<Tablecell>(nullptr));
    }

    QSharedPointer<Tablecell> Tablerow::GetCell(int index)
    {
        Q_ASSERT(index >= 0);
        if(index > (cells_->length()-1)) {
            return QSharedPointer<Tablecell>(nullptr);
        }
        return (*cells_)[index];
    }

    int Tablerow::ScanBackwardForBaseOfRepeatedCells(int index)
    {
        return IRepeatable::ScanBackwardForNotNull<Tablecell>(index, *cells_);
    }

    int Tablerow::ScanForwardForBaseOfRepeatedCells(int index)
    {
        return IRepeatable::ScanForwardForNotNull<Tablecell>(index, *cells_);
    }

    int Tablerow::GetLastDefined()
    {
        return lastdefined_;
    }

    int Tablerow::GetLastNonEmpty()
    {
        return lastnonempty_;
    }

    // implements IRepeatable
    QLatin1String Tablerow::RepeatTag()
    {
        return kRepeatAttribute;
    }

    // implements Tag
    QLatin1String Tablerow::InstanceTag()
    {
        return Tablerow::kTag;
    }

    void Tablerow::ReadAttribute(QStringRef attributename, QStringRef attributevalue)
    {
        if (attributename == RepeatTag()) {
            IRepeatable::DeserializeProperty(attributevalue);
            return;
        }
        if (attributename == StyleTag()) {
            IStyleable::DeserializeProperty(attributevalue);
            return;
        }
        // Deserialize present but unsupported attributes
        Tag::ReadAttribute(attributename, attributevalue);
    }

    void Tablerow::ReadSubtag(QXmlStreamReader& reader)
    {
        if (IsStartElementNamed(reader, Tablecell::kTag)) {
            QSharedPointer<Tablecell> cell = Tablecell::Builder(reader);
            (*cells_)[lastdefined_] = cell;
            lastdefined_ += cell->GetRepeat();

            if (cell->InstanceCellType() != TablecellEmpty::kCellTypeValue) {
                lastnonempty_ = lastdefined_;
            }
            return;
        }
        // Deserialize present but unsupported subtags
        Tag::ReadSubtag(reader);
    }

    void Tablerow::WriteAttributes(QXmlStreamWriter* writer)
    {
        IRepeatable::SerializeProperties(writer);
        IStyleable::SerializeProperties(writer);

        // Serialize present but unsupported attributes
        Tag::WriteAttributes(writer);
    }

    void Tablerow::WriteSubtags(QXmlStreamWriter* writer)
    {
        for (auto& cell : *cells_) {
            if (!cell.isNull()) {
                cell->Write(writer);
            }
        }
        // Serialize present but unsupported subtags
        Tag::WriteSubtags(writer);
    }
}
