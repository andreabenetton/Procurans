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
    QByteArray inbuffer;
    QByteArray outbuffer;

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
    void AddRows(QXmlStreamWriter* writer, QXmlStreamAttributes at, int columns);

    QList<QList<QSharedPointer<ODSCell>>>* rows;


    void writeCurrentToken(QXmlStreamWriter* writer, const QXmlStreamReader &reader);
};

#endif // ODSFILE_H