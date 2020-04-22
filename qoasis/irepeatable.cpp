// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "irepeatable.h"

namespace qoasis {

    // Constructors
    IRepeatable::IRepeatable(int repeat)
    {
        repeat_ = repeat;
    }

    IRepeatable::IRepeatable(const IRepeatable &obj)
    {
        repeat_ = obj.repeat_;
    }



    // Methods
    int IRepeatable::GetRepeat()
    {
        return repeat_;
    }

    void IRepeatable::DeserializeProperty(QStringRef attributevalue)
    {
        repeat_ = attributevalue.toInt();
    }

    void IRepeatable::SerializeProperties(QXmlStreamWriter* writer)
    {
        if (repeat_ > 1) {
            writer->writeAttribute(RepeatTag(), QString::number(repeat_));
        }
    }
}
