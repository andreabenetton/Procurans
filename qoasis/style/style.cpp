// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecell.h"
#include "tablecellempty.h"
#include "tablecellstring.h"
#include "tablecelldate.h"
#include "tablecellfloat.h"
#include "tablecellcurrency.h"

namespace qoasis::table {

    // Constants
    const QLatin1String Tablecell::kTag = QLatin1String("table:table-cell");
    const QLatin1String Tablecell::kCellTypeAttribute = QLatin1String("office:value-type");
    const QLatin1String Tablecell::kRepeatAttribute = QLatin1String("table:number-columns-repeated");
    const QLatin1String Tablecell::kTextPTag = QLatin1String("text:p");
    const QLatin1String Tablecell::kCalcextValueType = QLatin1String("calcext:value-type");

    // Constructors
    Tablecell::Tablecell(int repeat, QString style) : IStyleable(style), IRepeatable(repeat) {}

    Tablecell::Tablecell(QXmlStreamReader& reader) : IStyleable(""), IRepeatable(1)
    {
        Q_ASSERT(reader.qualifiedName() == Tablecell::kTag);
    }

    Tablecell::Tablecell(const Tablecell &obj): IStyleable(obj), IRepeatable(obj)
    {
        _valueText = obj._valueText;
    }

    // Static methods
    QSharedPointer<Tablecell> Tablecell::Clone(QSharedPointer<Tablecell> obj)
    {
        Q_ASSERT(obj->InstanceTag() == Tablecell::kTag);

        if (obj->InstanceCellType() == TablecellString::kCellTypeValue)
        {
            QSharedPointer<TablecellString> cell= qSharedPointerCast<TablecellString>(obj);
            return QSharedPointer<Tablecell>(new TablecellString(*cell));
        }
        if (obj->InstanceCellType() == TablecellDate::kCellTypeValue)
        {
            QSharedPointer<TablecellDate> cell= qSharedPointerCast<TablecellDate>(obj);
            return QSharedPointer<Tablecell>(new TablecellDate(*cell));
        }
        if (obj->InstanceCellType() == TablecellFloat::kCellTypeValue)
        {
            QSharedPointer<TablecellFloat> cell= qSharedPointerCast<TablecellFloat>(obj);
            return QSharedPointer<Tablecell>(new TablecellFloat(*cell));
        }
        if (obj->InstanceCellType() == TablecellCurrency::kCellTypeValue)
        {
            QSharedPointer<TablecellCurrency> cell= qSharedPointerCast<TablecellCurrency>(obj);
            return QSharedPointer<Tablecell>(new TablecellCurrency(*cell));
        }
        QSharedPointer<TablecellEmpty> cell= qSharedPointerCast<TablecellEmpty>(obj);
        return QSharedPointer<Tablecell>(new TablecellEmpty(*cell));
    }

    QSharedPointer<Tablecell> Tablecell::Builder(QXmlStreamReader& reader)
    {
        Q_ASSERT(reader.qualifiedName() == Tablecell::kTag);

        if (!reader.attributes().hasAttribute(Tablecell::kCellTypeAttribute))
        {
            return QSharedPointer<Tablecell>(new TablecellEmpty(reader));
        }

        QString type = reader.attributes().value(Tablecell::kCellTypeAttribute).toString();
        if (type.isEmpty())
        {
            return QSharedPointer<Tablecell>(new TablecellEmpty(reader));
        }
        if (type == TablecellString::kCellTypeValue)
        {
            return QSharedPointer<Tablecell>(new TablecellString(reader));
        }
        if (type == TablecellDate::kCellTypeValue)
        {
            return QSharedPointer<Tablecell>(new TablecellDate(reader));
        }
        if (type == TablecellFloat::kCellTypeValue)
        {
            return QSharedPointer<Tablecell>(new TablecellFloat(reader));
        }
        if (type == TablecellCurrency::kCellTypeValue)
        {
            return QSharedPointer<Tablecell>(new TablecellCurrency(reader));
        }
        Q_ASSERT(false);
        //return nullptr;
    }

    // Methods
    QString Tablecell::getText()
    {
        return _valueText;
    }

    // implements IRepeatable
    QLatin1String Tablecell::RepeatTag()
    {
        return Tablecell::kRepeatAttribute;
    }

    // implements Tag
    QLatin1String Tablecell::InstanceTag()
    {
        return Tablecell::kTag;
    }

    void Tablecell::ReadAttribute(QStringRef attributename, QStringRef attributevalue)
    {
        if (attributename == StyleTag()) {
            IStyleable::DeserializeProperty(attributevalue);
            return;
        }
        if (attributename == RepeatTag()) {
            IRepeatable::DeserializeProperty(attributevalue);
            return;
        }
        // Deserialize present but unsupported attributes
        Tag::ReadAttribute(attributename, attributevalue);
    }

    void Tablecell::ReadSubtag(QXmlStreamReader& reader)
    {
        if (reader.qualifiedName() == kTextPTag) {
            _valueText = reader.readElementText(QXmlStreamReader::IncludeChildElements);
            return;
        }
        // Deserialize present but unsupported subtags
        Tag::ReadSubtag(reader);
    }

    void Tablecell::WriteAttributes(QXmlStreamWriter* writer)
    {
        writer->writeAttribute(Tablecell::kCellTypeAttribute, InstanceCellType());
        writer->writeAttribute(Tablecell::kCalcextValueType, InstanceCellType());
        // Serialize present but unsupported attributes
        Tag::WriteAttributes(writer);
    }

    void Tablecell::WriteSubtags(QXmlStreamWriter* writer)
    {
        if (!(_valueText.isNull() || _valueText.isEmpty())) {
            writer->writeTextElement(Tablecell::kTextPTag, _valueText);
        }
        // Serialize present but unsupported subtags
        Tag::WriteSubtags(writer);
    }
}
