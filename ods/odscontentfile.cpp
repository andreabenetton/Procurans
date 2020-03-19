// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "odscontentfile.h"

#include <QDebug>
#include <QList>
#include <QSaveFile>

#include "odsrow.h"
#include "odscellempty.h"

const QString ODSContentFile::Tag = "document-content";
const QString ODSContentFile::filename = "content.xml";

QString ODSContentFile::InstanceTag()
{
    return ODSContentFile::Tag;
}

void ODSContentFile::Parse()
{
    reader = new QXmlStreamReader(inbuffer);
    writer = new QXmlStreamWriter(&outbuffer);

    writer->setAutoFormatting(true);
    writer->setAutoFormattingIndent(2);

    QString row_fromprevious;
    QList<QString>* cellstyles = nullptr;

    int cols = 0;

    do {
        reader->readNext();

        if (IsStartElementNamed(*reader, "table:table-column"))
            ParseTableColumn(*reader, &cols);

        //if (IsStartElementNamed(*reader, ODSRow::Tag))
        //    ParseTableRow(*reader, &cols);

        //if (reader->isStartElement()) {

        //        if (breakcicle) continue;

        //    if (reader->qualifiedName() == "table:table-cell") {
        //        int repeated = 1;
        //        QString style = "";
        //        for (auto& it : reader->attributes()) {
        //            if (it.name() == "number-columns-repeated") {
        //                repeated = it.value().toInt();
        //            }
        //            if (it.name() == "style-name") {
        //                style = it.value().toString();
        //            }
        //        }
        //        for (int o = 0; o < repeated; ++o) {
        //            cellstyles->append(style);
        //        }
        //    }
        

        if (reader->isEndElement()) {
            if (reader->qualifiedName() == "table:table") {
                AddRows(writer, row_fromprevious, cols, cellstyles);
                cols = 0;
            }
        }

        writeCurrentToken(writer, *reader);
    } while (!reader->atEnd() && !reader->hasError());

    delete reader;
    delete writer;
}

void ODSContentFile::ParseTableColumn(const QXmlStreamReader& reader, int* cols)
{
    bool adddone = false;
    for (auto& it : reader.attributes()) {
        if (it.name() == "number-columns-repeated") {
            (*cols) += it.value().toInt();
            adddone = true;
        }
    }
    if (!adddone) (*cols)++;
}

//void ODSContentFile::ParseTableRow(const QXmlStreamReader& reader)
//{
//    bool breakcicle = false;
//
//    if (cellstyles != nullptr) delete cellstyles;
//    cellstyles = new QList<QString>();
//
//    for (auto& it : reader->attributes()) {
//        if (it.name() == "number-rows-repeated") {
//            if (it.value().toInt() > 10) { // skip rows
//                do {
//                    reader->readNext();
//                } while (IsNotEndElementNamed(*reader, "table:table-row"));
//                breakcicle = true;
//            }
//        }
//
//        if (it.name() == "style-name") {
//            row_fromprevious = it.value().toString();
//        }
//    }
//
//    do {
//        reader->readNext();
//        if (IsStartElementNamed(*reader, "table:table-cell"))
//            ODSCell c = ParseTableCell(*reader);
//
//    } while (IsNotEndElementNamed(*reader, "table:table-row"));
//}

void ODSContentFile::AddRows(QXmlStreamWriter* writer, QString previousrowstyle, int columns, QList<QString>* cellstyles)
{
    for (int i = 0; i < rows->size(); ++i) {
        int cols = 0;
        writer->writeStartElement("table:table-row");

        QXmlStreamAttributes at;
        at.append("table:style-name", previousrowstyle);
        writer->writeAttributes(at);

        QList<QSharedPointer<ODSCell>> t = rows->at(i);

        for (auto& it : t) {
            //it->Serialize(writer, cellstyles->at(cols));
            cols += it->GetRepeat();
        }

        if (columns > cols) {
            ODSCellEmpty e(columns - cols);
            e.Serialize(writer);
        }
        writer->writeEndElement();

    }
}

void ODSContentFile::Add(QList<QList<QSharedPointer<ODSCell>>>* rowstoadd)
{
    rows = rowstoadd;
}

void ODSContentFile::writeCurrentToken(QXmlStreamWriter* writer, const QXmlStreamReader& reader)
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
            const QXmlStreamNamespaceDeclaration& namespaceDeclaration = namespaceDeclarations.at(i);
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