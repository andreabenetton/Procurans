// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "odsserializable.h"
#include "functions.h"

ODSSerializable::ODSSerializable()
{

}

void ODSSerializable::SerializeStart(QXmlStreamWriter* writer)
{
    writer->writeStartElement(InstanceTag());
}

void ODSSerializable::SerializeEnd(QXmlStreamWriter* writer)
{
    writer->writeEndElement();
}

void ODSSerializable::LoopForProperties(QXmlStreamReader& reader, int& numberofloopeditems)
{
    for (auto& it : reader.attributes()) {
        DeserializeProperty(it.qualifiedName(), it.value());
    }
}

void ODSSerializable::LoopForSubitems(QXmlStreamReader& reader, int& numberofloopeditems)
{
    do {
        reader.readNext();
        QVariant skip = DeserializeSubitem(reader, numberofloopeditems);
    } while (IsNotEndElementNamed(reader, InstanceTag()));
}
