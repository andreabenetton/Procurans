// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "odscell.h"

ODSCell::ODSCell(int repeat)
{
    repeated = repeat;
}

void ODSCell::WriteStart(QXmlStreamWriter* writer, QString style)
{
    writer->writeStartElement("table:table-cell");
    if(repeated>1)
        writer->writeAttribute("table:number-columns-repeated", QString::number(repeated));

    if (cellstyle!="")
        writer->writeAttribute("table:style-name", cellstyle);
    else if (style!="")
        writer->writeAttribute("table:style-name", style);
}

void ODSCell::WriteValue(QXmlStreamWriter* writer, QString value)
{
    if (!(value.isNull() || value.isEmpty())) {
        writer->writeTextElement("text:p", value);
    }
}

void ODSCell::WriteEnd(QXmlStreamWriter* writer)
{
    writer->writeEndElement();
}

int ODSCell::GetRepeated()
{
    return repeated;
}

QString ODSCell::TypeAttributeKey()
{
    return "office:value-type";
}


// ODSCellEmpty ***************************

ODSCellEmpty::ODSCellEmpty(int repeat) : ODSCell(repeat) {}

QString ODSCellEmpty::TypeAttributeValue()
{
    return "";
}

void ODSCellEmpty::Serialize(QXmlStreamWriter* writer, QString style)
{
    WriteStart(writer, style);
    WriteEnd(writer);
}

// ODSCellString ***************************

ODSCellString::ODSCellString(QString text, int repeat) : ODSCell(repeat)
{
    valueText = text;
}

QString ODSCellString::getText()
{
    return valueText;
}

QString ODSCellString::TypeAttributeValue()
{
    return "string";
}

void ODSCellString::Serialize(QXmlStreamWriter* writer, QString style)
{
    WriteStart(writer, style);
    if (!(valueText.isNull() || valueText.isEmpty())) {
        writer->writeAttribute(TypeAttributeKey(), TypeAttributeValue());
        writer->writeAttribute("calcext:value-type", TypeAttributeValue());
        WriteValue(writer, valueText);
    }
    WriteEnd(writer);
}

// ODSCellFloat ***************************

ODSCellFloat::ODSCellFloat(double number, int repeat) : ODSCell(repeat)
{
    valueNumber = number;
}

double ODSCellFloat::getDouble()
{
    return valueNumber;
}

QString ODSCellFloat::TypeAttributeValue()
{
    return "float";
}

void ODSCellFloat::Serialize(QXmlStreamWriter* writer, QString style)
{
    WriteStart(writer, style);
    writer->writeAttribute(TypeAttributeKey(), TypeAttributeValue());
    writer->writeAttribute("calcext:value-type", TypeAttributeValue());
    writer->writeAttribute("office:value", QString::number(valueNumber, 'f', 2));
    WriteValue(writer, QString::number(valueNumber, 'f', 2).replace(".",","));
    WriteEnd(writer);
}

// ODSCellCurrency ***************************

ODSCellCurrency::ODSCellCurrency(ODSCurrency currency, double number, int repeat) : ODSCell(repeat)
{
    valueAmount = number;
    valueCurrency = currency;
    symbols.insert(ODSCurrency::EUR, "€");
    iso.insert(ODSCurrency::EUR, "EUR");
}

double ODSCellCurrency::getAmount()
{
    return valueAmount;
}

ODSCurrency ODSCellCurrency::getCurrency()
{
    return valueCurrency;
}

QString ODSCellCurrency::TypeAttributeValue()
{
    return "currency";
}

void ODSCellCurrency::Serialize(QXmlStreamWriter* writer, QString style)
{
    WriteStart(writer, style);
    writer->writeAttribute(TypeAttributeKey(), TypeAttributeValue());
    writer->writeAttribute("calcext:value-type", TypeAttributeValue());
    writer->writeAttribute("office:value", QString::number(valueAmount, 'f', 2));
    writer->writeAttribute("office:currency", iso.value(valueCurrency));
    QString out = "";
    if(valueAmount<0) out = "-";
    out += symbols.value(valueCurrency);
    if(valueAmount<0)
        out += QString::asprintf("%'.2f", -valueAmount);
    else
        out += QString::asprintf("%'.2f", valueAmount);
    //out += QString::number(valueAmount, 'f', 2);
    out.replace(".","!");
    out.replace(",",".");
    out.replace("!",",");
    WriteValue(writer, out);
    WriteEnd(writer);
}

// ODSCellDate ***************************

ODSCellDate::ODSCellDate(QDate date, int repeat) : ODSCell(repeat)
{
    valueDate = date;
}

QDate ODSCellDate::getDate()
{
    return valueDate;
}

QString ODSCellDate::TypeAttributeValue()
{
    return "date";
}

void ODSCellDate::Serialize(QXmlStreamWriter* writer, QString style)
{
    WriteStart(writer, style);
    if (!valueDate.isNull() && valueDate.isValid()) {
        writer->writeAttribute(TypeAttributeKey(), TypeAttributeValue());
        writer->writeAttribute("calcext:value-type", TypeAttributeValue());
        writer->writeAttribute("office:date-value", valueDate.toString("yyyy-MM-dd"));
        WriteValue(writer, valueDate.toString("dd/MM/yyyy"));
    }
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
