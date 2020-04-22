// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef BODY_H
#define BODY_H

#include "../tag.h"

#include "spreadsheet.h"

namespace qoasis::office {

    class Body : public Tag
    {
    public:
        Body();
        Body(QXmlStreamReader& reader);
        Body(const Body &obj);

        static QSharedPointer<Tag> Builder(QXmlStreamReader& reader);
        static const QLatin1String kTag;

        QSharedPointer<Spreadsheet> GetSpreadsheet();

        // implements Tag
        virtual QLatin1String InstanceTag();

    private:
        QSharedPointer<Spreadsheet> spreadsheet_;

        // implements Tag
        virtual void ReadSubtag(QXmlStreamReader& reader);
        virtual void WriteSubtags(QXmlStreamWriter* writer);

    };

};
#endif // BODY_H
