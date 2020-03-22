// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef ODSDOCUMENT_H
#define ODSDOCUMENT_H

#include <QObject>
#include <QTemporaryDir>
#include "FileAbstract.h"
#include "FileContent.h"

class Document
{
public:

    //static ODSDocument* FromFile(const QString &full_path);

    Document();

    bool Save(const QString &full_path);
    bool Save();

    void Load(const QString &full_path);

    void AddRowToContent(QList<QList<QSharedPointer<CellAbstract>>>* rowstoadd);

private:
    void InitTempDir();

    const bool dev_mode_ = true;
    QStringList extracted_file_paths_;

    QTemporaryDir temp_dir_;
    QString temp_dir_path_;

    FileContent* contentfile;
};

#endif // ODSDOCUMENT_H
