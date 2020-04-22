// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef CURRENCY_H
#define CURRENCY_H

#include <QMap>

namespace qoasis {

    class Currency
    {
    public:
        static const int EUR = 978;

        Currency(int currencyconstant);
        Currency(QString currencyISO);
        Currency(const Currency &obj);

        static QString GetSymbol(int currencyconstant);
        static QString GetISO(int currencyconstant);

        bool GetIsValid();
        QString GetSymbol();
        QString GetSymbolFromISO(QString ISO);
        QString GetISO();

        QString FormatAmount(double amount);

    private:
        static bool _typeinitialized;
        static QMap<int, QString> _symbols;
        static QMap<int, QString> _iso;
        static QMap<int, int> _decimals;
        static void AddCurrency(int currencyconstant, QString symbol, QString ISO, int decimals);

        bool _isvalid = false;
        int _currency;
    };
};
#endif // CURRENCY_H
