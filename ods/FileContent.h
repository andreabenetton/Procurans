// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef ODSCONTENTFILE_H
#define ODSCONTENTFILE_H

#include <QObject>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "CellAbstract.h"
#include "FileAbstract.h"

namespace Ods {

    class FileContent : public FileAbstract
    {
    public:
        void Parse();
        QString InstanceTag();

        static const QString Tag;
        static const QString filename;

        void Add(QList<QList<QSharedPointer<CellAbstract>>>* rowstoadd);

    private:
        void AddRows(QXmlStreamWriter* writer, QString at, int columns, QList<QString>* cellstyles);

        QList<QList<QSharedPointer<CellAbstract>>>* rows;
        void ParseTableColumn(const QXmlStreamReader& reader, int* cols);

        void writeCurrentToken(QXmlStreamWriter* writer, const QXmlStreamReader& reader);
    };
}
#endif // ODSCONTENTFILE_H
