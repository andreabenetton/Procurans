// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "currency.h"

namespace qoasis {

    bool Currency::_typeinitialized = false;
    QMap<int, QString> Currency::_symbols;
    QMap<int, QString> Currency::_iso;
    QMap<int, int> Currency::_decimals;

    Currency::Currency(int currencyconstant)
    {
        if(_typeinitialized == false) {
            AddCurrency(EUR, "€", "EUR", 2);
            _typeinitialized = true;
        }
        if (_symbols.contains(currencyconstant)) {
            _currency = currencyconstant;
            _isvalid = true;
        }
    }

    Currency::Currency(QString currencyISO)
    {
        if(_typeinitialized == false) {
            AddCurrency(EUR, "€", "EUR", 2);
            _typeinitialized = true;
        }
        _currency = _iso.key(currencyISO, 0);
        if (_currency != 0) {
            _isvalid = true;
        }
    }

    Currency::Currency(const Currency &obj)
    {
        _isvalid = obj._isvalid;
        _currency = obj._currency;
    }

    void Currency::AddCurrency(int currencyconstant, QString symbol, QString ISO, int decimals)
    {
        _symbols.insert(currencyconstant, symbol);
        _iso.insert(currencyconstant, ISO);
        _decimals.insert(currencyconstant, decimals);
    }

    bool Currency::GetIsValid()
    {
        return _isvalid;
    }

    QString Currency::GetSymbol()
    {
        return _symbols.value(_currency);
    }

    QString Currency::GetISO()
    {
        return _iso.value(_currency);
    }

    QString Currency::GetSymbol(int currencyconstant)
    {
        return _symbols.value(currencyconstant);
    }

    QString Currency::GetSymbolFromISO(QString ISO)
    {
        int currencyconstant = _iso.key(ISO);
        return _symbols.value(currencyconstant);
    }

    QString Currency::GetISO(int currencyconstant)
    {
        return _iso.value(currencyconstant);
    }

    QString Currency::FormatAmount(double amount)
    {
        QString pattern = "%'." + QString::number(_decimals.value(_currency)) + "f";
        QString out = "";
        if (amount < 0) out = "-";
        out += _symbols.value(_currency);
        if (amount < 0)
            out += QString::asprintf(qPrintable(pattern), -amount);
        else
            out += QString::asprintf(qPrintable(pattern), amount);

        out.replace(".", "!");
        out.replace(",", ".");
        out.replace("!", ",");
        return out;
    }
}

