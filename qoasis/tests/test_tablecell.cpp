// Copyright 2026 - 2026, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include <QtTest>
#include <QObject>
#include <QByteArray>
#include <QDate>
#include <QSharedPointer>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "qoasis/table/tablecell.h"
#include "qoasis/table/tablecellstring.h"
#include "qoasis/table/tablecellfloat.h"
#include "qoasis/table/tablecelldate.h"
#include "qoasis/table/tablecellcurrency.h"
#include "qoasis/table/tablecellcovered.h"

using qoasis::table::Tablecell;
using qoasis::table::TablecellString;
using qoasis::table::TablecellFloat;
using qoasis::table::TablecellDate;
using qoasis::table::TablecellCurrency;
using qoasis::table::TablecellCovered;

namespace
{
	QSharedPointer<Tablecell> parseCell(const QByteArray& xml)
	{
		QXmlStreamReader reader(xml);
		while (!reader.atEnd()
		       && reader.tokenType() != QXmlStreamReader::StartElement)
		{
			reader.readNext();
		}
		Q_ASSERT(reader.tokenType() == QXmlStreamReader::StartElement);
		return Tablecell::builder(reader);
	}

	QSharedPointer<Tablecell> parseCovered(const QByteArray& xml)
	{
		QXmlStreamReader reader(xml);
		while (!reader.atEnd()
		       && reader.tokenType() != QXmlStreamReader::StartElement)
		{
			reader.readNext();
		}
		Q_ASSERT(reader.tokenType() == QXmlStreamReader::StartElement);
		return TablecellCovered::builder(reader);
	}

	QByteArray writeCell(QSharedPointer<Tablecell> cell)
	{
		QByteArray out;
		QXmlStreamWriter writer(&out);
		writer.setAutoFormatting(false);
		cell->write(&writer);
		return out;
	}
}

class TestTablecell : public QObject
{
	Q_OBJECT

private slots:
	void noValueTypeYieldsBaseTablecell();
	void stringValueTypeYieldsTablecellString();
	void floatValueTypeYieldsTablecellFloatRoundTrip();
	void dateValueTypeYieldsTablecellDateRoundTrip();
	void currencyValueTypeYieldsTablecellCurrencyRoundTrip();
	void coveredCellTagAndRoundTrip();
	void noSyntheticCalcextOnTypedCells();
	void stringCellEmitsAndReadsOfficeStringValue();
};

void TestTablecell::noValueTypeYieldsBaseTablecell()
{
	auto cell = parseCell("<table:table-cell xmlns:table=\""
	                     "urn:oasis:names:tc:opendocument:xmlns:table:1.0\"/>");
	QCOMPARE(cell->instanceCellType(), QString());
	// Base Tablecell::isEmpty looks at _valueText, which was never set.
	QVERIFY(cell->isEmpty());
}

void TestTablecell::stringValueTypeYieldsTablecellString()
{
	auto cell = parseCell(
		"<table:table-cell"
		" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\""
		" xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
		" xmlns:text=\"urn:oasis:names:tc:opendocument:xmlns:text:1.0\""
		" office:value-type=\"string\">"
		"<text:p>hello</text:p>"
		"</table:table-cell>");
	QCOMPARE(cell->instanceCellType(), QStringLiteral("string"));
	QCOMPARE(cell->getText(), QStringLiteral("hello"));
	QVERIFY(qSharedPointerDynamicCast<TablecellString>(cell));
}

void TestTablecell::floatValueTypeYieldsTablecellFloatRoundTrip()
{
	auto cell = parseCell(
		"<table:table-cell"
		" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\""
		" xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
		" office:value-type=\"float\""
		" office:value=\"42.5\"/>");
	auto fcell = qSharedPointerDynamicCast<TablecellFloat>(cell);
	QVERIFY(fcell);
	QCOMPARE(fcell->getDouble(), 42.5);

	const QByteArray out = writeCell(cell);
	QVERIFY2(out.contains("office:value=\"42.50\""), out.constData());
	// Display text uses the , decimal separator (locale swap inside the
	// typed cell's writeSubtags).
	QVERIFY2(out.contains("42,50"), out.constData());
}

void TestTablecell::dateValueTypeYieldsTablecellDateRoundTrip()
{
	auto cell = parseCell(
		"<table:table-cell"
		" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\""
		" xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
		" office:value-type=\"date\""
		" office:date-value=\"2026-01-15\"/>");
	auto dcell = qSharedPointerDynamicCast<TablecellDate>(cell);
	QVERIFY(dcell);
	QCOMPARE(dcell->getDate(), QDate(2026, 1, 15));

	const QByteArray out = writeCell(cell);
	QVERIFY2(out.contains("office:date-value=\"2026-01-15\""), out.constData());
	QVERIFY2(out.contains("15/01/2026"), out.constData());
}

void TestTablecell::currencyValueTypeYieldsTablecellCurrencyRoundTrip()
{
	auto cell = parseCell(
		"<table:table-cell"
		" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\""
		" xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
		" office:value-type=\"currency\""
		" office:value=\"100.25\""
		" office:currency=\"EUR\"/>");
	auto ccell = qSharedPointerDynamicCast<TablecellCurrency>(cell);
	QVERIFY(ccell);
	QCOMPARE(ccell->getAmount(), 100.25);
	QCOMPARE(ccell->getCurrency().getIso(), QStringLiteral("EUR"));

	const QByteArray out = writeCell(cell);
	QVERIFY2(out.contains("office:value=\"100.25\""), out.constData());
	QVERIFY2(out.contains("office:currency=\"EUR\""), out.constData());
	// Locale-formatted display (€ + ",25" trailer).
	QVERIFY2(out.contains(",25"), out.constData());
}

void TestTablecell::coveredCellTagAndRoundTrip()
{
	auto cell = parseCovered(
		"<table:covered-table-cell"
		" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\"/>");
	QCOMPARE(cell->instanceTag(), QStringLiteral("table:covered-table-cell"));

	const QByteArray out = writeCell(cell);
	QVERIFY2(out.contains("table:covered-table-cell"), out.constData());
	QVERIFY2(!out.contains("<table:table-cell"), out.constData());
}

void TestTablecell::noSyntheticCalcextOnTypedCells()
{
	// Pre-fix, Tablecell synthesized calcext:value-type alongside
	// office:value-type. calcext is a LibreOffice extension, not in ODF
	// 1.2, and is emitted even when the input had no calcext declaration.
	auto cell = parseCell(
		"<table:table-cell"
		" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\""
		" xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
		" office:value-type=\"float\""
		" office:value=\"1.0\"/>");
	const QByteArray out = writeCell(cell);
	QVERIFY2(!out.contains("calcext:value-type"), out.constData());
}

void TestTablecell::stringCellEmitsAndReadsOfficeStringValue()
{
	// Read: office:string-value attribute also seeds the cell text (in
	// addition to <text:p>, which already worked).
	auto fromAttr = parseCell(
		"<table:table-cell"
		" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\""
		" xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
		" office:value-type=\"string\""
		" office:string-value=\"hello\"/>");
	QCOMPARE(fromAttr->getText(), QStringLiteral("hello"));

	// Write: a TablecellString round-trips with both office:string-value
	// AND the <text:p> child (matches LibreOffice's output style).
	auto fromText = parseCell(
		"<table:table-cell"
		" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\""
		" xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
		" xmlns:text=\"urn:oasis:names:tc:opendocument:xmlns:text:1.0\""
		" office:value-type=\"string\">"
		"<text:p>world</text:p>"
		"</table:table-cell>");
	const QByteArray out = writeCell(fromText);
	QVERIFY2(out.contains("office:string-value=\"world\""), out.constData());
	QVERIFY2(out.contains("<text:p>world</text:p>"), out.constData());
}

QTEST_MAIN(TestTablecell)
#include "test_tablecell.moc"
