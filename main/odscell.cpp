#include "odscell.h"

ODSCell::ODSCell(int repeat)
{
    repeated = repeat;
}

void ODSCell::WriteStart(QXmlStreamWriter* writer)
{
    writer->writeStartElement(ODSCell::tag);
    if(repeated>1)
        writer->writeAttribute("table:number-columns-repeated", QString::number(repeated));
}

void ODSCell::WriteEnd(QXmlStreamWriter* writer)
{
    writer->writeEndElement();
}

const QString ODSCell::tag = "table:table-cell";
const QString ODSCell::attribute = "office:value-type";

ODSCellEmpty::ODSCellEmpty(int repeat) : ODSCell(repeat) {}

void ODSCellEmpty::Serialize(QXmlStreamWriter* writer)
{
    WriteStart(writer);
    WriteEnd(writer);
}

//ODSCell *ODSCell::Builder(QXmlStreamReader& reader)
//{
//    Q_ASSERT(reader.qualifiedName() == ODSCell::tag &&
//             reader.attributes().hasAttribute(ODSCell::attribute));

//    QString type = reader.attributes().value(ODSCell::attribute).toString();
//    if (type==ODSCellString::attributevalue)
//    {
//        return new ODSCellString();
//    }
//    return nullptr;
//}

ODSCellString::ODSCellString(QString text, int repeat) : ODSCell(repeat)
{
    valueText = text;
}

const QString ODSCellString::attributevalue = "string";
const QString ODSCellString::valuesubtag = "text:p";

QString ODSCellString::getText()
{
    return valueText;
}


//void ODSCellString::Parse()
//{
//    do {
//        reader->readNext();

//        /* Let's read the name... */
//        QString elementName = reader->name().toString();
//        if(elementName==ODSCellString::valuesubtag) {
//            valueText = reader->readElementText();
//        }

//    /* ...go to the next. */
//    reader->readNext();
//    }

//    while(isNotEndElementNamed(ODSCell::tag));
//}



void ODSCellString::Serialize(QXmlStreamWriter* writer)
{
    WriteStart(writer);
    if (!(valueText.isNull() || valueText.isEmpty())) {
        writer->writeAttribute(ODSCell::attribute, ODSCellString::attributevalue);
        writer->writeAttribute("calcext:value-type", ODSCellString::attributevalue);
        writer->writeTextElement(ODSCellString::valuesubtag, valueText);
    }
    WriteEnd(writer);
}

ODSCellFloat::ODSCellFloat(double number, int repeat) : ODSCell(repeat)
{
    valueNumber = number;
}

const QString ODSCellFloat::attributevalue = "float";
const QString ODSCellFloat::valuesubtag = "text:p";

double ODSCellFloat::getDouble()
{
    return valueNumber;
}

void ODSCellFloat::Serialize(QXmlStreamWriter* writer)
{
    WriteStart(writer);
    writer->writeAttribute(ODSCell::attribute, ODSCellFloat::attributevalue);
    writer->writeAttribute("office:value", QString::number(valueNumber, 'f', 2));
    writer->writeAttribute("calcext:value-type", ODSCellFloat::attributevalue);
    writer->writeTextElement(ODSCellFloat::valuesubtag, QString::number(valueNumber, 'f', 2).replace(".",","));
    WriteEnd(writer);
}

ODSCellDate::ODSCellDate(QDate date, int repeat) : ODSCell(repeat)
{
    valueDate = date;
}

const QString ODSCellDate::attributevalue = "date";
const QString ODSCellDate::valuesubtag = "text:p";

QDate ODSCellDate::getDate()
{
    return valueDate;
}


void ODSCellDate::Serialize(QXmlStreamWriter* writer)
{
    WriteStart(writer);
    if (!valueDate.isNull() && valueDate.isValid()) {
        writer->writeAttribute(ODSCell::attribute, ODSCellDate::attributevalue);
        writer->writeAttribute("office:date-value", valueDate.toString("yyyy-MM-dd"));
        writer->writeAttribute("calcext:value-type", ODSCellDate::attributevalue);
        writer->writeTextElement(ODSCellDate::valuesubtag, valueDate.toString("dd/MM/yyyy"));
    }
    WriteEnd(writer);
}

