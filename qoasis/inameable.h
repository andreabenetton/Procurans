// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef INAMEABLE_H
#define INAMEABLE_H

#include <QObject>
#include <QXmlStreamWriter>
#include <QMap>

namespace qoasis {

    class INameable
    {
    public:
        INameable(QString name = "");
        INameable(const INameable &obj);

        virtual QLatin1String NameTag() = 0;
        virtual QLatin1String DefaultName();

        QString GetName();
        void SetName(QString name);

    protected:
        QString name_;

        void DeserializeProperty(QStringRef attributevalue);
        void SerializeProperties(QXmlStreamWriter* writer);

    private:
        static QMap<QLatin1String, int> itemstypecounter_;
    };

};
#endif // INAMEABLE_H
