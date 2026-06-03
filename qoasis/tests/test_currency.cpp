// Copyright 2026 - 2026, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include <QtTest>
#include <QObject>
#include <QChar>
#include <QString>

#include "qoasis/currency.h"

class TestCurrency : public QObject
{
	Q_OBJECT

private slots:
	void defaultCtorIsEurAndValid();
	void isoCtorEurIsValid();
	void isoCtorBogusIsInvalid();
	void copyCtorPreservesState();
	void formatAmountIsItalianStyle();
	void formatAmountNegativePrefixesMinus();
	void staticLookupsAfterAnyInstance();
};

void TestCurrency::defaultCtorIsEurAndValid()
{
	qoasis::Currency c;
	QVERIFY(c.getIsValid());
	QCOMPARE(c.getIso(), QStringLiteral("EUR"));
	// EUR symbol is U+20AC (€). The Currency ctor adds it as QChar(0x20AC).
	QCOMPARE(c.getSymbol(), QString(QChar(0x20AC)));
}

void TestCurrency::isoCtorEurIsValid()
{
	qoasis::Currency c(QStringLiteral("EUR"));
	QVERIFY(c.getIsValid());
	QCOMPARE(c.getIso(), QStringLiteral("EUR"));
	QCOMPARE(c.getSymbol(), QString(QChar(0x20AC)));
}

void TestCurrency::isoCtorBogusIsInvalid()
{
	qoasis::Currency c(QStringLiteral("ZZZ"));
	QVERIFY(!c.getIsValid());
	// Lookups via key=0 fall through to empty/default.
	QCOMPARE(c.getIso(), QString());
	QCOMPARE(c.getSymbol(), QString());
}

void TestCurrency::copyCtorPreservesState()
{
	qoasis::Currency src;
	qoasis::Currency dup(src);
	QVERIFY(dup.getIsValid());
	QCOMPARE(dup.getIso(), src.getIso());
	QCOMPARE(dup.getSymbol(), src.getSymbol());

	qoasis::Currency invalid(QStringLiteral("ZZZ"));
	qoasis::Currency dup_invalid(invalid);
	QVERIFY(!dup_invalid.getIsValid());
}

void TestCurrency::formatAmountIsItalianStyle()
{
	qoasis::Currency eur;
	const QString s = eur.formatAmount(1234.56);
	// Decimal separator is comma (the . -> ! -> , -> . swap dance in
	// currency.cpp guarantees this regardless of locale).
	QVERIFY2(s.contains(QChar(',')), qPrintable(s));
	QVERIFY2(!s.contains(QStringLiteral(".56")), qPrintable(s));
	QVERIFY2(s.startsWith(QChar(0x20AC)), qPrintable(s));
	QVERIFY2(s.endsWith(QStringLiteral(",56")), qPrintable(s));
}

void TestCurrency::formatAmountNegativePrefixesMinus()
{
	qoasis::Currency eur;
	const QString s = eur.formatAmount(-12.34);
	QVERIFY2(s.startsWith(QChar('-')), qPrintable(s));
	QVERIFY2(s.contains(QChar(0x20AC)), qPrintable(s));
	QVERIFY2(s.endsWith(QStringLiteral(",34")), qPrintable(s));
}

void TestCurrency::staticLookupsAfterAnyInstance()
{
	// Static maps are populated lazily on first ctor; bootstrap one.
	qoasis::Currency boot;
	Q_UNUSED(boot);
	QCOMPARE(qoasis::Currency::getIso(qoasis::Currency::EUR), QStringLiteral("EUR"));
	QCOMPARE(qoasis::Currency::getSymbol(qoasis::Currency::EUR),
	         QString(QChar(0x20AC)));
	QCOMPARE(qoasis::Currency::getSymbolFromIso(QStringLiteral("EUR")),
	         QString(QChar(0x20AC)));
}

QTEST_MAIN(TestCurrency)
#include "test_currency.moc"
