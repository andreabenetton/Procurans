// Copyright 2019 - 2020, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "odscellcurrency.h"

const QString ODSCellCurrency::CELLTYPE = "currency";
const QString ODSCellCurrency::CELLVALUETAG = "office:value";
const QString ODSCellCurrency::CELLCURRENCYTAG = "office:currency";

ODSCellCurrency::ODSCellCurrency(ODSCurrency currency, double number, int repeat) : ODSCell(repeat)
{
    _valueAmount = number;
    _valueCurrency = currency;
    InitializeCurrencies();
}

ODSCellCurrency::ODSCellCurrency(QXmlStreamReader& reader) : ODSCell(reader)
{
    _valueAmount = 0.0;
    _valueCurrency = ODSCurrency::EUR;
    InitializeCurrencies();

    Deserialize(reader);
}

void ODSCellCurrency::InitializeCurrencies()
{
    _symbols.insert(ODSCurrency::EUR, "€");
    _iso.insert(ODSCurrency::EUR, "EUR");
}

double ODSCellCurrency::getAmount()
{
    return _valueAmount;
}

ODSCurrency ODSCellCurrency::getCurrency()
{
    return _valueCurrency;
}

QString ODSCellCurrency::InstanceCellType()
{
    return CELLTYPE;
}

// implements ODSSerializable
void ODSCellCurrency::Deserialize(QXmlStreamReader& reader)
{
    int i = 0;
    ODSSerializable::LoopForProperties(reader, i);
}

void ODSCellCurrency::DeserializeProperty(QStringRef attributename, QStringRef attributevalue)
{
    ODSRepeatable::DeserializeProperty(attributename, attributevalue);
    ODSStyleable::DeserializeProperty(attributename, attributevalue);

    if (attributename == CELLVALUETAG) {
        _valueAmount = attributevalue.toFloat();
    }
    if (attributename == CELLCURRENCYTAG) {
        _valueCurrency = _iso.key(attributevalue.toString());
    }
}

void ODSCellCurrency::Serialize(QXmlStreamWriter* writer)
{
    ODSSerializable::SerializeStart(writer);
    ODSRepeatable::SerializeProperties(writer);
    ODSStyleable::SerializeProperties(writer);
    ODSCell::SerializeProperties(writer);
    SerializeProperties(writer);
    SerializeSubitems(writer);
    ODSSerializable::SerializeEnd(writer);
}

void ODSCellCurrency::SerializeProperties(QXmlStreamWriter* writer)
{
    writer->writeAttribute(CELLVALUETAG, QString::number(_valueAmount, 'f', 2));
    writer->writeAttribute(CELLCURRENCYTAG, _iso.value(_valueCurrency));
}

void ODSCellCurrency::SerializeSubitems(QXmlStreamWriter* writer)
{
    QString out = "";
    if (_valueAmount < 0) out = "-";
    out += _symbols.value(_valueCurrency);
    if (_valueAmount < 0)
        out += QString::asprintf("%'.2f", -_valueAmount);
    else
        out += QString::asprintf("%'.2f", _valueAmount);

    out.replace(".", "!");
    out.replace(",", ".");
    out.replace("!", ",");
    writer->writeTextElement(ODSCell::TEXTVALUETAG, out);
}