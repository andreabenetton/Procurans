// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "inameable.h"

namespace qoasis {

    QMap<QLatin1String, int> INameable::itemstypecounter_;

    INameable::INameable(QString name)
    {
        QLatin1String defaultname = DefaultName();

        int count = 1;
        if(!itemstypecounter_.contains(defaultname)) {
            itemstypecounter_.insert(defaultname, count);
        }
        else {
            count = itemstypecounter_.value(defaultname);
        }

        if (name != "") {
            name_ = name;
        }
        else {
            name_ = defaultname + QString::number(count);
            itemstypecounter_.insert(defaultname, ++count);
        }

    }

    INameable::INameable(const INameable &obj)
    {
        name_ = "Copy of " + obj.name_;
    }

    QLatin1String INameable::DefaultName()
    {
        return NameTag();
    }

    QString INameable::GetName()
    {
        return name_;
    }

    void INameable::SetName(QString name)
    {
        name_ = name;
    }

    void INameable::DeserializeProperty(QStringRef attributevalue)
    {
        name_ = attributevalue.toString();
    }

    void INameable::SerializeProperties(QXmlStreamWriter* writer)
    {
        if (name_ != "") {
            writer->writeAttribute(NameTag(), name_);
        }
    }
}
