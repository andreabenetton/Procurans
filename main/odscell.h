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

class ODSCellCurrency: public ODSCell
{
public:
    enum Currency {
      EUR
    };

    ODSCellCurrency(Currency currency, double amoount, int repeat = 1);

    void Serialize(QXmlStreamWriter* writer, QString style = "");

    double getAmount();
    Currency getCurrency();

protected:
    QString TypeAttributeValue();

private:
    double valueAmount;
    Currency valueCurrency;
    QMap<Currency,QString> symbols;
    QMap<Currency,QString> iso;

};


#endif // ODSCELL_H
