// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECELLCURRENCY_H
#define TABLECELLCURRENCY_H

#include <QObject>
#include <QMap>
#include <QXmlStreamWriter>

#include "../currency.h"

#include "tablecell.h"

namespace qoasis::table {

    class TablecellCurrency : public Tablecell
    {
    public:
        TablecellCurrency(Currency currency, double amoount, int repeat = 1);
        TablecellCurrency(QXmlStreamReader& reader);
        TablecellCurrency(const TablecellCurrency &obj);

        ~TablecellCurrency();

        static const QLatin1String kCellTypeValue;

        double getAmount();
        Currency getCurrency();

        // implements Tablecell
        virtual QLatin1String InstanceCellType();

    protected:
        // implements Tag
        virtual void ReadAttribute(QStringRef attributename, QStringRef attributevalue);
        virtual void WriteAttributes(QXmlStreamWriter* writer);
        virtual void WriteSubtags(QXmlStreamWriter* writer);

    private:
        static const QLatin1String kCellTypeAttribute;
        static const QLatin1String kCellCurrencyAttribute;

        double _valueAmount;
        Currency* _valueCurrency;
    };
}
#endif // TABLECELLCURRENCY_H
