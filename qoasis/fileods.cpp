// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include <QDebug>

#include "fileods.h"
#include "quazip/JlCompress.h"

namespace qoasis {

    FileOds::FileOds(const QString& full_path) : IFileable(full_path)
    {
        temp_dir_.setAutoRemove(true);
        if (temp_dir_.isValid()) {
            temp_dir_path_ = temp_dir_.path();
            temp_dir_valid_ = true;
        }
        else{
            qCritical() << "The temporary directory is invalid";
        }
    }

    QSharedPointer<DocumentContent> FileOds::GetContent()
    {
        return contentfile->GetContent();
    }

    bool FileOds::Create()
    {
        if (Exist()||!temp_dir_valid_) {
            return false;
        }
        contentfile = new FileContent(QDir(temp_dir_path_).filePath(FileContent::FileName));
        return true;
    }

    bool FileOds::Load()
    {
        if (!Exist()||!temp_dir_valid_) {
            return false;
        }

        extracted_file_paths_ = JlCompress::extractDir(full_path_, temp_dir_path_);

        if (extracted_file_paths_.isEmpty()) {
            qCritical() << "Couldn't extract files";
            return false;
        }

        qInfo() << "Unzipped: " << full_path_;

        bool content_file_loaded = false;
        for (auto path : extracted_file_paths_) {
            if (path.endsWith(FileContent::FileName)) {
                contentfile = new FileContent(path);
                content_file_loaded = contentfile->Load();
            }
        }
        return content_file_loaded;
    }

    bool FileOds::Save(const QString& full_path, bool overwriteprotected)
    {
        if (overwriteprotected && Exist()) {
            qCritical() << "File already exists: " << qPrintable(full_path_);
            return false;
        }

        if (contentfile == nullptr) { // || document_styles_ == nullptr)
            qWarning() << "Nothing to save";
            return false;
        }

        QDir base_dir(temp_dir_path_);

        bool content_file_saved = contentfile->Save();
        if(!content_file_saved) {
            return false;
        }

        QString save_path = (full_path=="" ? full_path_ : full_path);
        if (!JlCompress::compressDir(save_path, temp_dir_path_)) {
            qCritical() << "Failed to compress dir: " << qPrintable(temp_dir_path_);
            return false;
        }

        qInfo() << "Zipped: " << qPrintable(save_path);
        return true;
    }

}

