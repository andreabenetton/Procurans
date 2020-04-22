// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "istyleable.h"

namespace qoasis {

    IStyleable::IStyleable(QString style)
    {
        style_ = style;
    }

    IStyleable::IStyleable(const IStyleable &obj)
    {
        style_ = obj.style_;
    }

    QString IStyleable::GetStyle()
    {
        return style_;
    }

    void IStyleable::SetStyle(QString style)
    {
        style_ = style;
    }

    QLatin1String IStyleable::StyleTag()
    {
        return QLatin1String("table:style-name");
    }

    void IStyleable::DeserializeProperty(QStringRef attributevalue)
    {
        style_ = attributevalue.toString();
    }

    void IStyleable::SerializeProperties(QXmlStreamWriter* writer)
    {
        if (style_ != "") {
            writer->writeAttribute(StyleTag(), style_);
        }
    }
}
