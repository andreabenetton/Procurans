// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "StyleableAbstract.h"
#include "functions.h"

namespace Ods {
    StyleableAbstract::StyleableAbstract(QString style)
    {
        _style = style;
    }

    QString StyleableAbstract::GetStyle()
    {
        return _style;
    }

    void StyleableAbstract::SetStyle(QString style)
    {
        _style = style;
    }

    QString StyleableAbstract::StyleTag()
    {
        return "table:style-name";
    }

    void StyleableAbstract::DeserializeProperty(QStringRef attributename, QStringRef attributevalue)
    {
        if (attributename == StyleTag()) {
            _style = attributevalue.toString();
        }
    }

    void StyleableAbstract::SerializeProperties(QXmlStreamWriter* writer)
    {
        if (_style != "") {
            writer->writeAttribute(StyleTag(), _style);
        }
    }
}