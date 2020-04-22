// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include <QDebug>
#include <QFileInfo>

#include "ifileable.h"

namespace qoasis {

    IFileable::IFileable(const QString& full_path)
    {
       full_path_ = full_path;
       if (Exist()) {
            qInfo() << "File exist: " << full_path;
       } else {
            qInfo() << "File not exist: " << full_path;
       }
    }

    bool IFileable::Exist()
    {
        QFileInfo check_file(full_path_);
        // check if file exists and if yes: Is it really a file and no directory?
        return (check_file.exists() && check_file.isFile());
    }

    bool IFileable::Save()
    {
        return Save(full_path_);
    }

}
