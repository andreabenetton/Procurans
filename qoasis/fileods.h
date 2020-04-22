// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef FILEODS_H
#define FILEODS_H

#include <QObject>
#include <QTemporaryDir>

#include "filecontent.h"
#include "office/documentcontent.h"

namespace qoasis {

    class FileOds : public IFileable
    {
    public:
        FileOds(const QString& full_path);

        virtual bool Create();
        virtual bool Load();
        virtual bool Save(const QString& full_path, bool overwriteprotected = true);

        QSharedPointer<DocumentContent> GetContent();

    private:
        bool temp_dir_valid_ = false;
        QStringList extracted_file_paths_;

        QTemporaryDir temp_dir_;
        QString temp_dir_path_;

        FileContent* contentfile;
    };

}
#endif // FILEODS_H
