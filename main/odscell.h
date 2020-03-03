// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef ODSCELL_H
#define ODSCELL_H

#include <QObject>
#include <QDate>
#include <QMap>
#include <QXmlStreamWriter>

class ODSCell
{
public:
    ODSCell(int repeat = 1);
    int GetRepeated();

    virtual void Serialize(QXmlStreamWriter* writer, QString style = "")= 0;

protected:
    void WriteStart(QXmlStreamWriter* writer, QString style = "");
    void WriteValue(QXmlStreamWriter* writer, QString value);
    void WriteEnd(QXmlStreamWriter* writer);

    virtual QString TypeAttributeValue()= 0;
    QString TypeAttributeKey();

    int repeated;
    QString cellstyle;
};


class ODSCellEmpty: public ODSCell
{
public:
    ODSCellEmpty(int repeat = 1);
    void Serialize(QXmlStreamWriter* writer, QString style = "");

protected:
    QString TypeAttributeValue();
};

class ODSCellString: public ODSCell
{
public:
    ODSCellString(QString text, int repeat = 1);

    void Serialize(QXmlStreamWriter* writer, QString style = "");

    QString getText();

protected:
    QString TypeAttributeValue();

private:
    QString valueText;
};

class ODSCellFloat: public ODSCell
{
public:
    ODSCellFloat(double number, int repeat = 1);

    void Serialize(QXmlStreamWriter* writer, QString style = "");

    double getDouble();

protected:
    QString TypeAttributeValue();

private:
    double valueNumber;
};

class ODSCellDate: public ODSCell
{
public:
    ODSCellDate(QDate date, int repeat = 1) ;

    void Serialize(QXmlStreamWriter* writer, QString style = "");

    QDate getDate();

protected:
    QString TypeAttributeValue();

private:
    QDate valueDate;
};

enum ODSCurrency {
  EUR
};

class ODSCellCurrency: public ODSCell
{
public:
    ODSCellCurrency(ODSCurrency currency, double amoount, int repeat = 1);

    void Serialize(QXmlStreamWriter* writer, QString style = "");

    double getAmount();
    ODSCurrency getCurrency();

protected:
    QString TypeAttributeValue();

private:
    double valueAmount;
    ODSCurrency valueCurrency;
    QMap<ODSCurrency,QString> symbols;
    QMap<ODSCurrency,QString> iso;

};


#endif // ODSCELL_H
