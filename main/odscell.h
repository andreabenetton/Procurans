#ifndef ODSSCHEMAFIELD_H
#define ODSSCHEMAFIELD_H

#include <QObject>
#include <QDate>
#include <QXmlStreamWriter>

class ODSCell
{
public:
    ODSCell(int repeat = 1);

    static const QString tag;
    static const QString attribute;

    virtual void Serialize(QXmlStreamWriter* writer)= 0;

protected:
    void WriteStart(QXmlStreamWriter* writer);
    void WriteEnd(QXmlStreamWriter* writer);

    int repeated;
};


class ODSCellEmpty: public ODSCell
{
public:
    ODSCellEmpty(int repeated = 1);
    void Serialize(QXmlStreamWriter* writer);
};

class ODSCellString: public ODSCell
{
public:
    ODSCellString(QString text, int repeated = 1);
    static const QString attributevalue;
    static const QString valuesubtag;

    void Serialize(QXmlStreamWriter* writer);

    QString getText();

private:
    QString valueText;
};

class ODSCellFloat: public ODSCell
{
public:
    ODSCellFloat(double number, int repeated = 1);
    static const QString attributevalue;
    static const QString valuesubtag;

    void Serialize(QXmlStreamWriter* writer);

    double getDouble();

private:
    double valueNumber;
};

class ODSCellDate: public ODSCell
{
public:
    ODSCellDate(QDate date, int repeated = 1) ;
    static const QString attributevalue;
    static const QString valuesubtag;

    void Serialize(QXmlStreamWriter* writer);

    QDate getDate();

private:
    QDate valueDate;
};


#endif // ODSSCHEMAFIELD_H
