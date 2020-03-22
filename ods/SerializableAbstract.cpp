// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "SerializableAbstract.h"
#include "functions.h"

namespace Ods {

    void SerializableAbstract::SerializeStart(QXmlStreamWriter* writer)
    {
        writer->writeStartElement(InstanceTag());
    }

    void SerializableAbstract::SerializeEnd(QXmlStreamWriter* writer)
    {
        writer->writeEndElement();
    }

    void SerializableAbstract::LoopForProperties(QXmlStreamReader& reader, int& numberofloopeditems)
    {
        for (auto& it : reader.attributes()) {
            DeserializeProperty(it.qualifiedName(), it.value());
        }
    }

    void SerializableAbstract::LoopForSubitems(QXmlStreamReader& reader, int& numberofloopeditems)
    {
        do {
            reader.readNext();
            QVariant skip = DeserializeSubitem(reader, numberofloopeditems);
        } while (IsNotEndElementNamed(reader, InstanceTag()));
    }
}