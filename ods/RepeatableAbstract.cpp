// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "RepeatableAbstract.h"
#include "SerializableAbstract.h"
#include "functions.h"

namespace Ods {

    RepeatableAbstract::RepeatableAbstract(int repeat)
    {
        _repeat = repeat;
    }

    int RepeatableAbstract::GetRepeat()
    {
        return _repeat;
    }

    void RepeatableAbstract::DeserializeProperty(QStringRef attributename, QStringRef attributevalue)
    {
        if (attributename == RepeatTag()) {
            _repeat = attributevalue.toInt();
        }
    }

    void RepeatableAbstract::SerializeProperties(QXmlStreamWriter* writer)
    {
        if (_repeat > 1) {
            writer->writeAttribute(RepeatTag(), QString::number(_repeat));
        }
    }
}