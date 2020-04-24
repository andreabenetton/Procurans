// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECELL_H
#define TABLECELL_H

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

        static QSharedPointer<Tablecell> clone(QSharedPointer<Tablecell> obj);
        static QSharedPointer<Tablecell> builder(QXmlStreamReader& reader);

        static const QLatin1String kTag;
        static const QLatin1String kCellTypeAttribute;

        virtual QLatin1String instanceCellType() = 0;

        QString getText() const;

        // implements IRepeatable
        QLatin1String repeatTag() override;

        // implements Tag
        QLatin1String instanceTag() override;

    protected:
        Tablecell(QXmlStreamReader& reader);

        static const QLatin1String kRepeatAttribute;
        static const QLatin1String kTextPTag;
        static const QLatin1String kCalcextValueType;

        // implements Tag
        void writeAttributes(QXmlStreamWriter* writer) override;
        void writeSubtags(QXmlStreamWriter* writer) override;
        void readAttribute(QStringRef name, QStringRef value) override;
        void readSubtag(QXmlStreamReader& reader) override;

        QString _valueText;
    };
}
#endif // TABLECELL_H
