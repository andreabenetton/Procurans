// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECELL_H
#define TABLECELL_H

#include "../tag.h"

namespace qoasis::style {
    // Abstract class base for all cell types
    class Style : public Tag
    {
    public:
        Style();
        Style(const Style&obj);

        static const QLatin1String kTag;


        // implements Tag
        virtual QLatin1String InstanceTag();

    protected:
        Style(QXmlStreamReader& reader);

        // implements Tag
        void writeAttributes(QXmlStreamWriter* writer) override;
        void writeSubtags(QXmlStreamWriter* writer) override;
        void readAttribute(QStringRef name, QStringRef value) override;
        void readSubtag(QXmlStreamReader& reader) override;

    };
}
#endif // TABLECELL_H
