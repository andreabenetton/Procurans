// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef IFILEABLE_H
#define IFILEABLE_H

#include <QObject>
#include <QFile>

namespace qoasis {

    class IFileable
    {
    public:
        IFileable(const QString& full_path);

        virtual bool Exist();
        virtual bool Load() = 0;
        virtual bool Save(const QString& full_path, bool overwriteprotected = true) = 0;
        virtual bool Save();

    protected:
        QString full_path_;

    };
}
#endif // IFILEABLE_H
