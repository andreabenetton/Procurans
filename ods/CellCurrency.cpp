// Copyright 2019 - 2020, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "CellCurrency.h"

namespace Ods {

    const QString CellCurrency::kCellTypeValue = "currency";
    const QString CellCurrency::kCellTypeAttribute = "office:value";
    const QString CellCurrency::kCellCurrencyAttribute = "office:currency";

    CellCurrency::CellCurrency(ODSCurrency currency, double number, int repeat) : CellAbstract(repeat)
    {
        _valueAmount = number;
        _valueCurrency = currency;
        InitializeCurrencies();
    }

    CellCurrency::CellCurrency(QXmlStreamReader& reader) : CellAbstract(reader)
    {
        _valueAmount = 0.0;
        _valueCurrency = ODSCurrency::EUR;
        InitializeCurrencies();

        Deserialize(reader);
    }

    void CellCurrency::InitializeCurrencies()
    {
        _symbols.insert(ODSCurrency::EUR, "€");
        _iso.insert(ODSCurrency::EUR, "EUR");
    }

    double CellCurrency::getAmount()
    {
        return _valueAmount;
    }

    ODSCurrency CellCurrency::getCurrency()
    {
        return _valueCurrency;
    }

    QString CellCurrency::InstanceCellType()
    {
        return kCellTypeValue;
    }

    // implements ODSSerializable
    void CellCurrency::Deserialize(QXmlStreamReader& reader)
    {
        int i = 0;
        SerializableAbstract::LoopForProperties(reader, i);
    }

    void CellCurrency::DeserializeProperty(QStringRef attributename, QStringRef attributevalue)
    {
        RepeatableAbstract::DeserializeProperty(attributename, attributevalue);
        StyleableAbstract::DeserializeProperty(attributename, attributevalue);

        if (attributename == kCellTypeAttribute) {
            _valueAmount = attributevalue.toFloat();
        }
        if (attributename == kCellCurrencyAttribute) {
            _valueCurrency = _iso.key(attributevalue.toString());
        }
    }

    void CellCurrency::Serialize(QXmlStreamWriter* writer)
    {
        SerializableAbstract::SerializeStart(writer);
        RepeatableAbstract::SerializeProperties(writer);
        StyleableAbstract::SerializeProperties(writer);
        CellAbstract::SerializeProperties(writer);
        SerializeProperties(writer);
        SerializeSubitems(writer);
        SerializableAbstract::SerializeEnd(writer);
    }

    void CellCurrency::SerializeProperties(QXmlStreamWriter* writer)
    {
        writer->writeAttribute(kCellTypeAttribute, QString::number(_valueAmount, 'f', 2));
        writer->writeAttribute(kCellCurrencyAttribute, _iso.value(_valueCurrency));
    }

    void CellCurrency::SerializeSubitems(QXmlStreamWriter* writer)
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
        writer->writeTextElement(CellAbstract::kTextTag, out);
    }
}