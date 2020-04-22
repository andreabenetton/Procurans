// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecellcurrency.h"

namespace qoasis::table {

    const QLatin1String TablecellCurrency::kCellTypeValue = QLatin1String("currency");
    const QLatin1String TablecellCurrency::kCellTypeAttribute = QLatin1String("office:value");
    const QLatin1String TablecellCurrency::kCellCurrencyAttribute = QLatin1String("office:currency");

    TablecellCurrency::TablecellCurrency(Currency currency, double number, int repeat) : Tablecell(repeat)
    {
        _valueAmount = number;
        _valueCurrency = &currency;
    }

    TablecellCurrency::TablecellCurrency(QXmlStreamReader& reader) : Tablecell(reader)
    {
        _valueAmount = 0.0;
        _valueCurrency = new Currency(Currency::EUR);

        Read(reader);
    }

    TablecellCurrency::TablecellCurrency(const TablecellCurrency &obj): Tablecell(obj)
    {
        _valueAmount = obj._valueAmount;
        _valueCurrency = new Currency(*obj._valueCurrency);
    }

    TablecellCurrency::~TablecellCurrency()
    {
        delete _valueCurrency;
    }

    double TablecellCurrency::getAmount()
    {
        return _valueAmount;
    }

    Currency TablecellCurrency::getCurrency()
    {
        return *_valueCurrency;
    }

    QLatin1String TablecellCurrency::InstanceCellType()
    {
        return kCellTypeValue;
    }

    // implements Tag
    void TablecellCurrency::ReadAttribute(QStringRef attributename, QStringRef attributevalue)
    {
        if (attributename == kCellTypeAttribute) {
            _valueAmount = attributevalue.toFloat();
            return;
        }
        if (attributename == kCellCurrencyAttribute) {
            _valueCurrency = new Currency(attributevalue.toString());
            return;
        }
        Tablecell::ReadAttribute(attributename, attributevalue);
    }

    void TablecellCurrency::WriteAttributes(QXmlStreamWriter* writer)
    {
        Tablecell::WriteAttributes(writer);
        writer->writeAttribute(kCellTypeAttribute, QString::number(_valueAmount, 'f'));
        writer->writeAttribute(kCellCurrencyAttribute, _valueCurrency->GetISO());
    }

    void TablecellCurrency::WriteSubtags(QXmlStreamWriter* writer)
    {
        QString out = "";
        if (_valueAmount < 0) out = "-";
        out += _valueCurrency->GetSymbol();
        if (_valueAmount < 0)
            out += QString::asprintf("%'.2f", -_valueAmount);
        else
            out += QString::asprintf("%'.2f", _valueAmount);

        out.replace(".", "!");
        out.replace(",", ".");
        out.replace("!", ",");
        writer->writeTextElement(Tablecell::kTextPTag, out);
    }
}
