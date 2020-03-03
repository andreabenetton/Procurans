// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef ODSFILE_H
#define ODSFILE_H

#include <QObject>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include <odscell.h>

class ODSFile
{
public:
    void Load(const QString &full_path);
    bool Save(const QString &full_path);
    virtual void Parse() = 0;
    virtual QString Tag() = 0;

protected:
    QString inbuffer;
    QString outbuffer;

    QXmlStreamReader* reader;
    QXmlStreamWriter* writer;
};

class ODSContentFile: public ODSFile
{
public:
    void Parse();
    QString Tag();

    static const QString tag;
    static const QString filename;

    void Add(QList<QList<QSharedPointer<ODSCell>>>* rowstoadd);

private:
    void AddRows(QXmlStreamWriter* writer, QString at, int columns, QList<QString>* cellstyles);

    QList<QList<QSharedPointer<ODSCell>>>* rows;


    void writeCurrentToken(QXmlStreamWriter* writer, const QXmlStreamReader &reader);
};

#endif // ODSFILE_H
