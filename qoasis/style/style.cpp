// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "style.h"

namespace qoasis::style {

    // Constants
    const QLatin1String Style::kTag = QLatin1String("table:table-cell");


    Style::Style(const Style &obj): Tag(obj)
    {

    }




    // implements Tag
    QLatin1String Style::InstanceTag()
    {
        return kTag;
    }

    void Style::readAttribute(QStringRef attributename, QStringRef attributevalue)
    {


        // Deserialize present but unsupported attributes
        Tag::readAttribute(attributename, attributevalue);
    }

    void Style::readSubtag(QXmlStreamReader& reader)
    {

        // Deserialize present but unsupported subtags
        Tag::readSubtag(reader);
    }

    void Style::writeAttributes(QXmlStreamWriter* writer)
    {

        // Serialize present but unsupported attributes
        Tag::writeAttributes(writer);
    }

    void Style::writeSubtags(QXmlStreamWriter* writer)
    {

        // Serialize present but unsupported subtags
        Tag::writeSubtags(writer);
    }
}
