// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include "../tag.h"
#include "../table/table.h"

using namespace qoasis::table;

namespace qoasis::office {

    class Spreadsheet : public Tag
    {
    public:
        Spreadsheet();
        Spreadsheet(QXmlStreamReader& reader);
        Spreadsheet(const Spreadsheet &obj);
        ~Spreadsheet();

        static QSharedPointer<Tag> Builder(QXmlStreamReader& reader);
        static const QLatin1String kTag;

        QSharedPointer<Table> GetTable(QString& name);
        QSharedPointer<Table> GetTable(int index);

        // implements Tag
        virtual QLatin1String InstanceTag();

    protected:
        // implements ODSSerializable
        virtual void ReadSubtag(QXmlStreamReader& reader);
        virtual void WriteSubtags(QXmlStreamWriter* writer);

    private:
        QVector<QSharedPointer<Table>>* tables_;

        void Initialize();
    };
}
#endif // SPREADSHEET_H
