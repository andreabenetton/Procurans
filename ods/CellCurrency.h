// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef ODSCELLCURRENCY_H
#define ODSCELLCURRENCY_H

#include <QObject>
#include <QXmlStreamWriter>
#include "CellAbstract.h"

namespace Ods {

    enum ODSCurrency {
        EUR
    };

    class CellCurrency : public CellAbstract
    {
    public:
        CellCurrency(ODSCurrency currency, double amoount, int repeat = 1);
        CellCurrency(QXmlStreamReader& reader);

        static const QString kCellTypeValue;

        double getAmount();
        ODSCurrency getCurrency();

        // implements ODSSerializable
        void Serialize(QXmlStreamWriter* writer);

        // implements ODSCell
        QString InstanceCellType();

    private:
        static const QString kCellTypeAttribute;
        static const QString kCellCurrencyAttribute;

        void InitializeCurrencies();

        double _valueAmount;
        ODSCurrency _valueCurrency;
        QMap<ODSCurrency, QString> _symbols;
        QMap<ODSCurrency, QString> _iso;

        // implements ODSSerializable
        void Deserialize(QXmlStreamReader& reader);
        void DeserializeProperty(QStringRef attributename, QStringRef attributevalue);
        void SerializeProperties(QXmlStreamWriter* writer);
        void SerializeSubitems(QXmlStreamWriter* writer);
    };
}
#endif // ODSCELLCURRENCY_H