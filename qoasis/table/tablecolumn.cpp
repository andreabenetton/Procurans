// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecolumn.h"

namespace qoasis::table {

    // Constants
    const QLatin1String Tablecolumn::kTag = QLatin1String("table:table-column");
    const QLatin1String Tablecolumn::kRepeatAttribute = QLatin1String("table:number-columns-repeated");
    const QLatin1String Tablecolumn::kDefaultCellStyleAttribute = QLatin1String("table:default-cell-style-name");

    // Constructors
    Tablecolumn::Tablecolumn(int repeat, QString style, QString default_cell_style) : Tag(), IStyleable(style),
                                                                                      IRepeatable(repeat)
    {
        last_defined_ = 0;
        default_cell_style_ = default_cell_style;
    }

    Tablecolumn::Tablecolumn(QXmlStreamReader& reader) : Tag(), IStyleable(""), IRepeatable(1)
    {
        Q_ASSERT(reader.qualifiedName() == Tablecolumn::kTag);
        Tag::read(reader);
    }

    Tablecolumn::Tablecolumn(const Tablecolumn &obj): Tag(), IStyleable(obj), IRepeatable(obj)
    {
        last_defined_ = obj.last_defined_;
        default_cell_style_ = obj.default_cell_style_;
    }

    // Static methods
    QSharedPointer<Tablecolumn> Tablecolumn::builder(QXmlStreamReader& reader)
    {
        Q_ASSERT(reader.qualifiedName() == Tablecolumn::kTag);
        return QSharedPointer<Tablecolumn>(new Tablecolumn(reader));
    }

    // Methods
    int Tablecolumn::getLastDefined() const
    {
        return last_defined_;
    }

    QString Tablecolumn::getDefaultCellStyle() const
    {
        return default_cell_style_;
    }

    void Tablecolumn::setDefaultCellStyle(QString style)
    {
        default_cell_style_ = style;
    }

    // implements IRepeatable
    QLatin1String Tablecolumn::repeatTag()
    {
        return kRepeatAttribute;
    }

    // implements Tag
    QLatin1String Tablecolumn::instanceTag()
    {
        return kTag;
    }

    void Tablecolumn::readAttribute(QStringRef name, QStringRef value)
    {
        if (name == repeatTag()) {
            IRepeatable::readRepeat(value);
            return;
        }
        if (name == styleTag()) {
            IStyleable::readStyle(value);
            return;
        }
        if (name == kDefaultCellStyleAttribute) {
            default_cell_style_ = value.toString();
            return;
        }
        // Deserialize present but unsupported attributes
        Tag::readAttribute(name, value);
    }

    void Tablecolumn::writeAttributes(QXmlStreamWriter* writer)
    {
        IRepeatable::writeRepeat(writer);
        IStyleable::writeStyle(writer);
        if (default_cell_style_ != "") {
            writer->writeAttribute(Tablecolumn::kDefaultCellStyleAttribute, default_cell_style_);
        }
        // Serialize present but unsupported attributes
        Tag::writeAttributes(writer);
    }

}
