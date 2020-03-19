// Copyright 2019 - 2020, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "odssheet.h"
#include "functions.h"

const QString ODSSheet::TAG = "table:table";

int ODSSheet::_sheetcount = 1;

ODSSheet::ODSSheet(QString name)
{
    _rows = new QVector<ODSRow>(256);
    _columns = new QVector<ODSColumn>(256);

    if (name != "") {
        _name = name;
    }
    else {
        _name = "Sheet" + QString::number(_sheetcount++);
    }
    
}


// implements ODSSerializable
QString ODSSheet::InstanceTag()
{
    return TAG;
}

void ODSSheet::DeserializeProperty(QXmlStreamReader& reader)
{
    for (auto& it : reader.attributes()) {
//        DeserializeStyleAttribute(it.qualifiedName(), it.value());
    }
}