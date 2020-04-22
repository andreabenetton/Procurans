// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecolumn.h"

namespace qoasis::table {

    // Constants
    const QLatin1String Tablecolumn::kTag = QLatin1String("table:table-column");
    const QLatin1String Tablecolumn::kRepeatAttribute = QLatin1String("table:number-columns-repeated");
    const QLatin1String Tablecolumn::kDefaultCellStyleAttribute = QLatin1String("table:default-cell-style-name");

    // Constructors
    Tablecolumn::Tablecolumn(int repeat, QString columnstyle, QString defaultcellstyle) : IStyleable(columnstyle), IRepeatable(repeat)
    {
        lastdefined_ = 0;
        defaultcellstyle_ = defaultcellstyle;
    }

    Tablecolumn::Tablecolumn(QXmlStreamReader& reader) : IStyleable(""), IRepeatable(1)
    {
        Q_ASSERT(reader.qualifiedName() == Tablecolumn::kTag);
        Tag::Read(reader);
    }

    Tablecolumn::Tablecolumn(const Tablecolumn &obj): IStyleable(obj), IRepeatable(obj)
    {
        lastdefined_ = obj.lastdefined_;
        defaultcellstyle_ = obj.defaultcellstyle_;
    }

    // Static methods
    QSharedPointer<Tablecolumn> Tablecolumn::Builder(QXmlStreamReader& reader)
    {
        Q_ASSERT(reader.qualifiedName() == Tablecolumn::kTag);
        return QSharedPointer<Tablecolumn>(new Tablecolumn(reader));
    }

    // Methods
    int Tablecolumn::GetLastDefined()
    {
        return lastdefined_;
    }

    QString Tablecolumn::GetDefaultCellStyle()
    {
        return defaultcellstyle_;
    }

    void Tablecolumn::SetDefaultCellStyle(QString style)
    {
        defaultcellstyle_ = style;
    }

    // implements ODSRepeatable
    QLatin1String Tablecolumn::RepeatTag()
    {
        return Tablecolumn::kRepeatAttribute;
    }

    // implements ODSSerializable
    QLatin1String Tablecolumn::InstanceTag()
    {
        return Tablecolumn::kTag;
    }

    void Tablecolumn::ReadAttribute(QStringRef attributename, QStringRef attributevalue)
    {
        if (attributename == RepeatTag()) {
            IRepeatable::DeserializeProperty(attributevalue);
            return;
        }
        if (attributename == StyleTag()) {
            IStyleable::DeserializeProperty(attributevalue);
            return;
        }
        if (attributename == kDefaultCellStyleAttribute) {
            defaultcellstyle_ = attributevalue.toString();
            return;
        }
        // Deserialize present but unsupported attributes
        Tag::ReadAttribute(attributename, attributevalue);
    }

    void Tablecolumn::WriteAttributes(QXmlStreamWriter* writer)
    {
        IRepeatable::SerializeProperties(writer);
        IStyleable::SerializeProperties(writer);
        if (defaultcellstyle_ != "") {
            writer->writeAttribute(Tablecolumn::kDefaultCellStyleAttribute, defaultcellstyle_);
        }
    }

}
