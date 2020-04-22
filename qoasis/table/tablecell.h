// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECELL_H
#define TABLECELL_H

#include <QObject>
#include <QDate>
#include <QXmlStreamWriter>

#include "../tag.h"
#include "../istyleable.h"
#include "../irepeatable.h"

namespace qoasis::table {
    // Abstract class base for all cell types
    class Tablecell : public Tag, public IStyleable, public IRepeatable
    {
    public:
        Tablecell(int repeat = 1, QString style = "");
        Tablecell(const Tablecell &obj);

        static QSharedPointer<Tablecell> Clone(QSharedPointer<Tablecell> obj);
        static QSharedPointer<Tablecell> Builder(QXmlStreamReader& reader);

        static const QLatin1String kTag;
        static const QLatin1String kCellTypeAttribute;

        virtual QLatin1String InstanceCellType() = 0;

        QString getText();

        // implements IRepeatable
        virtual QLatin1String RepeatTag();

        // implements Tag
        virtual QLatin1String InstanceTag();

    protected:
        Tablecell(QXmlStreamReader& reader);

        static const QLatin1String kRepeatAttribute;
        static const QLatin1String kTextPTag;
        static const QLatin1String kCalcextValueType;

        // implements Tag
        virtual void WriteAttributes(QXmlStreamWriter* writer);
        virtual void WriteSubtags(QXmlStreamWriter* writer);
        virtual void ReadAttribute(QStringRef attributename, QStringRef attributevalue);
        virtual void ReadSubtag(QXmlStreamReader& reader);

        QString _valueText;
    };
}
#endif // TABLECELL_H
