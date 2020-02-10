#include "odsfile.h"

#include <QDebug>
#include <QSaveFile>
#include "logger.h"

void ODSFile::Load(const QString &full_path)
{
    QFile file(full_path);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qWarning(logWarning()) << "Failed to open file: " << file.fileName() << "error :" <<file.errorString();
        inbuffer = "";
        return;
    }

    QTextStream in(&file);
    inbuffer = in.readAll().toUtf8();

    qInfo(logInfo()) << Tag() << " file loaded.";
}

bool ODSFile::Save(const QString &full_path)
{
    QSaveFile file(full_path);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);


    file.write(outbuffer);
    const bool ok = file.commit();

    if (!ok)
        qWarning(logWarning()) << "Failed to save file: " << full_path;

    qInfo(logInfo()) << Tag() << " file saved.";

    return ok;
}

const QString ODSContentFile::tag = "document-content";
const QString ODSContentFile::filename = "content.xml";

QString ODSContentFile::Tag()
{
        return ODSContentFile::tag;
}

void ODSContentFile::Parse()
{
    reader = new QXmlStreamReader(inbuffer);
    writer = new QXmlStreamWriter(&outbuffer);

    writer->setAutoFormatting(true);
    writer->setAutoFormattingIndent(2);

    QXmlStreamAttributes at;

    int cols = 0;

    do {
        reader->readNext();

        if(reader->isStartElement()){
            if(reader->qualifiedName() == "table:table-column"){
                QXmlStreamAttributes ca = reader->attributes();
                if(ca.hasAttribute("table:number-columns-repeated")){
                    cols += ca.value("table:number-columns-repeated").toInt();
                }
                else
                    cols++;
            }
        }

        if(reader->isStartElement()){
            if(reader->qualifiedName() == "table:table-row"){
                QXmlStreamAttributes ca = reader->attributes();
                if(ca.hasAttribute("table:number-rows-repeated")){
                    if(ca.value("table:number-rows-repeated").toInt()>20)
                        continue;
                }
                at = reader->attributes();
            }
        }

        if(reader->isEndElement()){
            if(reader->qualifiedName() == "table:table"){
                AddRows(writer, at, cols);
                cols = 0;
            }
        }

        writeCurrentToken(writer, *reader);

    }
    while(!reader->atEnd() && !reader->hasError());

    delete reader;
    delete writer;
}

void ODSContentFile::AddRows(QXmlStreamWriter* writer, QXmlStreamAttributes at, int columns)
{  
    for (int i = 0; i < rows->size(); ++i) {
        int cols = 0;
        writer->writeStartElement("table:table-row");
        writer->writeAttributes(at);
        QList<QSharedPointer<ODSCell>> t = rows->at(i);
        for (auto &it : t) {
               it->Serialize(writer);
               cols++;
        }
        if(columns>cols){
            ODSCellEmpty e(columns-cols);
            e.Serialize(writer);
        }
        writer->writeEndElement();
    }
}

void ODSContentFile::Add(QList<QList<QSharedPointer<ODSCell>>>* rowstoadd)
{
    rows = rowstoadd;
}

void ODSContentFile::writeCurrentToken(QXmlStreamWriter* writer, const QXmlStreamReader &reader)
{
    switch (reader.tokenType()) {
    case QXmlStreamReader::NoToken:
        break;
    case QXmlStreamReader::StartDocument:
        writer->writeStartDocument();
        break;
    case QXmlStreamReader::EndDocument:
        writer->writeEndDocument();
        break;
    case QXmlStreamReader::StartElement: {
        writer->writeStartElement(reader.qualifiedName().toString());
        QXmlStreamNamespaceDeclarations namespaceDeclarations = reader.namespaceDeclarations();
        for (int i = 0; i < namespaceDeclarations.size(); ++i) {
            const QXmlStreamNamespaceDeclaration &namespaceDeclaration = namespaceDeclarations.at(i);
            writer->writeNamespace(namespaceDeclaration.namespaceUri().toString(),
                           namespaceDeclaration.prefix().toString());
        }
        writer->writeAttributes(reader.attributes());
             } break;
    case QXmlStreamReader::EndElement:
        writer->writeEndElement();
        break;
    case QXmlStreamReader::Characters:
        if (reader.isCDATA())
            writer->writeCDATA(reader.text().toString());
        else
            writer->writeCharacters(reader.text().toString());
        break;
    case QXmlStreamReader::Comment:
        writer->writeComment(reader.text().toString());
        break;
    case QXmlStreamReader::DTD:
        writer->writeDTD(reader.text().toString());
        break;
    case QXmlStreamReader::EntityReference:
        writer->writeEntityReference(reader.name().toString());
        break;
    case QXmlStreamReader::ProcessingInstruction:
        writer->writeProcessingInstruction(reader.processingInstructionTarget().toString(),
                                   reader.processingInstructionData().toString());
        break;
    default:
        Q_ASSERT(reader.tokenType() != QXmlStreamReader::Invalid);
        qWarning("QXmlStreamWriter: writeCurrentToken() with invalid state.");
        break;
    }
}
