// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include <QDebug>

#include "Document.h"
#include "FileAbstract.h"
#include "quazip/JlCompress.h"

namespace Ods {

    Document::Document()
    {
        InitTempDir();
    }

    void Document::InitTempDir()
    {
        const bool do_remove = true;
        temp_dir_.setAutoRemove(do_remove);

        if (dev_mode_) {
            temp_dir_path_ = QDir::home().filePath("ods_dev_mode");
            QDir dir(temp_dir_path_);

            if (dir.exists() && !dir.removeRecursively())
                qWarning() << "Failed to delete recursively: " << dir.Name;


            if (!dir.mkpath("."))
                qWarning() << "Can't create temp dir";
        }
        else {
            temp_dir_path_ = temp_dir_.path();
        }
    }

    void Document::Load(const QString& full_path)
    {
        InitTempDir();

        if (!dev_mode_ && !temp_dir_.isValid()) {
            qWarning() << "temp dir invalid";
            return;
        }

        extracted_file_paths_ = JlCompress::extractDir(full_path, temp_dir_path_);

        if (extracted_file_paths_.isEmpty()) {
            qWarning() << "Couldn't extract files";
            return;
        }

        for (auto path : extracted_file_paths_) {
            if (path.endsWith(FileContent::filename)) {
                contentfile = new FileContent();
                contentfile->Load(path);
                break;
            }
        }

        qInfo() << "Unzipped: " << full_path;
    }

    void Document::AddRowToContent(QList<QList<QSharedPointer<CellAbstract>>>* rowstoadd)
    {
        contentfile->Add(rowstoadd);
        contentfile->Parse();
    }

    bool Document::Save(const QString& ods_path)
    {
        if (contentfile == nullptr) { // || document_styles_ == nullptr)
            qWarning() << "Nothing to save";
            return false;
        }

        QDir base_dir(temp_dir_path_);

        QString contentfilepath = base_dir.filePath(FileContent::filename);
        contentfile->Save(contentfilepath);

        if (!JlCompress::compressDir(ods_path, temp_dir_path_)) {
            qWarning() << "Failed to compress dir";
            return false;
        }

        qInfo() << "Zipped: " << ods_path;

        return true;
    }
}

