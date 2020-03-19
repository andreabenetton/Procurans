// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef ODSFILE_H
#define ODSFILE_H

#include <QObject>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "odscell.h"

class ODSFile
{
public:
    void Load(const QString &full_path);
    bool Save(const QString &full_path);

    virtual void Parse() = 0;
    //virtual QString Tag() = 0;

protected:
    QString inbuffer;
    QString outbuffer;

    bool IsStartElementNamed(QXmlStreamReader& xml, const QString& tokenName);
    bool IsNotEndElementNamed(QXmlStreamReader& xml, const QString& tokenName);

    QXmlStreamReader* reader;
    QXmlStreamWriter* writer;
};
#endif // ODSFILE_H
