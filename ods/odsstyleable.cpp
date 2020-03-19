// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "odsstyleable.h"
#include "functions.h"

ODSStyleable::ODSStyleable(QString style)
{
    _style = style;
}

QString ODSStyleable::GetStyle()
{
    return _style;
}

void ODSStyleable::SetStyle(QString style)
{
    _style = style;
}

QString ODSStyleable::StyleTag()
{
    return "table:style-name";
}

void ODSStyleable::DeserializeProperty(QStringRef attributename, QStringRef attributevalue)
{
    if (attributename == StyleTag()) {
        _style = attributevalue.toString();
    }
}

void ODSStyleable::SerializeProperties(QXmlStreamWriter* writer)
{
    if (_style != "") {
        writer->writeAttribute(StyleTag(), _style);
    }
}