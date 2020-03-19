// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include <QDebug>
#include "logger.h"

#include "odsdocument.h"
#include "odsfile.h"
#include "quazip/JlCompress.h"

ODSDocument::ODSDocument()
{
    InitTempDir();
}

void ODSDocument::InitTempDir()
{
    const bool do_remove = true;
    temp_dir_.setAutoRemove(do_remove);

    if (dev_mode_) {
        temp_dir_path_ = QDir::home().filePath("ods_dev_mode");
        QDir dir(temp_dir_path_);

        if (dir.exists() && !dir.removeRecursively())
            qWarning(logWarning()) << "Failed to delete recursively: " << dir.Name;


        if (!dir.mkpath("."))
            qWarning(logWarning()) << "Can't create temp dir";
    } else {
        temp_dir_path_ = temp_dir_.path();
    }
}

void ODSDocument::Load(const QString &full_path)
{
    InitTempDir();

    if (!dev_mode_ && !temp_dir_.isValid()) {
        qWarning(logWarning()) << "temp dir invalid";
        return;
    }

    extracted_file_paths_ = JlCompress::extractDir(full_path, temp_dir_path_);

    if (extracted_file_paths_.isEmpty()) {
        qWarning(logWarning()) <<"Couldn't extract files";
        return;
    }

    for (auto path : extracted_file_paths_) {
        if (path.endsWith(ODSContentFile::filename)) {
            contentfile = new ODSContentFile();
            contentfile->Load(path);
            break;
        }
    }

    qInfo(logInfo()) << "Unzipped: " << full_path;
}

void ODSDocument::AddRowToContent(QList<QList<QSharedPointer<ODSCell>>>* rowstoadd)
{
    contentfile->Add(rowstoadd);
    contentfile->Parse();
}

bool ODSDocument::Save(const QString &ods_path)
{
    if (contentfile == nullptr) { // || document_styles_ == nullptr)
        qWarning(logWarning()) <<"Nothing to save";
        return false;
    }

    QDir base_dir(temp_dir_path_);

    QString contentfilepath = base_dir.filePath(ODSContentFile::filename);
    contentfile->Save(contentfilepath);

    if (!JlCompress::compressDir(ods_path, temp_dir_path_)) {
        qWarning(logWarning()) << "Failed to compress dir";
        return false;
    }

    qInfo(logInfo()) << "Zipped: " << ods_path;

    return true;
}


