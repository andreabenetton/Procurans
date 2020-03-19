// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "odsrepeatable.h"
#include "functions.h"

ODSRepeatable::ODSRepeatable(int repeat)
{
    _repeat = repeat;
}

int ODSRepeatable::GetRepeat()
{
    return _repeat;
}

void ODSRepeatable::DeserializeProperty(QStringRef attributename, QStringRef attributevalue)
{
    if (attributename == RepeatTag()) {
        _repeat = attributevalue.toInt();
    }
}

void ODSRepeatable::SerializeProperties(QXmlStreamWriter* writer)
{
    if (_repeat > 1) {
        writer->writeAttribute(RepeatTag(), QString::number(_repeat));
    }
}
