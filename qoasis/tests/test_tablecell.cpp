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
#include "qoasis/table/tablecellboolean.h"
#include "qoasis/table/tablecellcovered.h"
#include "qoasis/table/tablecellcurrency.h"
#include "qoasis/table/tablecelldate.h"
#include "qoasis/table/tablecellfloat.h"
#include "qoasis/table/tablecellpercentage.h"
#include "qoasis/table/tablecellstring.h"
#include "qoasis/table/tablecelltime.h"

using qoasis::table::Tablecell;
using qoasis::table::TablecellBoolean;
using qoasis::table::TablecellCovered;
using qoasis::table::TablecellCurrency;
using qoasis::table::TablecellDate;
using qoasis::table::TablecellFloat;
using qoasis::table::TablecellPercentage;
using qoasis::table::TablecellString;
using qoasis::table::TablecellTime;

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
	void floatPrecisionBeyondTwoDecimals();
	void floatPreservesInputDisplayText();
	void dateAcceptsIsoDateTimeInput();
	void booleanValueTypeFallsBackAndRoundTrips();
	void timeValueTypeFallsBackAndRoundTrips();
	void percentageValueTypeFallsBackAndRoundTrips();
	void richTextSpanBetweenLiteralsRoundtrip();
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
	// 'g',15 emits the shortest representation that round-trips: "42.5".
	QVERIFY2(out.contains("office:value=\"42.5\""), out.constData());
	// Display text uses the , decimal separator. Programmatic synthesis
	// uses 'g' precision 15 (no forced trailing zeroes), so 42.5 renders
	// as "42,5" — the older 'f',2 form ("42,50") truncated higher-precision
	// values like 3.14159 to "3,14" and is no longer used.
	QVERIFY2(out.contains("<text:p>42,5</text:p>"), out.constData());
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

	// Write-preserves-input-shape: an input cell that carried
	// office:string-value writes it back...
	const QByteArray outWithAttr = writeCell(fromAttr);
	QVERIFY2(outWithAttr.contains("office:string-value=\"hello\""),
	         outWithAttr.constData());

	// ...but a <text:p>-only cell does NOT have office:string-value
	// auto-added on save. ODF 1.2 §19.385 allows either form; the previous
	// auto-add bloated round-tripped files and could disagree with rich
	// <text:p> content (the attribute carries only the flat projection).
	auto fromText = parseCell(
		"<table:table-cell"
		" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\""
		" xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
		" xmlns:text=\"urn:oasis:names:tc:opendocument:xmlns:text:1.0\""
		" office:value-type=\"string\">"
		"<text:p>world</text:p>"
		"</table:table-cell>");
	const QByteArray outNoAttr = writeCell(fromText);
	QVERIFY2(!outNoAttr.contains("office:string-value"), outNoAttr.constData());
	QVERIFY2(outNoAttr.contains("<text:p>world</text:p>"),
	         outNoAttr.constData());
}

void TestTablecell::floatPrecisionBeyondTwoDecimals()
{
	// Pre-fix, TablecellFloat hardcoded 'f' precision 2 on office:value AND
	// on its <text:p> display projection, so 3.14159 became "3.14" — silent
	// precision loss for any non-currency float. The attribute fix to 'g'
	// precision 15 went in earlier; this case also asserts the <text:p>
	// display now carries the full precision when synthesised programmatically.
	auto cell = parseCell(
		"<table:table-cell"
		" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\""
		" xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
		" office:value-type=\"float\""
		" office:value=\"3.14159\"/>");
	auto fcell = qSharedPointerDynamicCast<TablecellFloat>(cell);
	QVERIFY(fcell);
	QCOMPARE(fcell->getDouble(), 3.14159);

	const QByteArray out = writeCell(cell);
	QVERIFY2(out.contains("office:value=\"3.14159\""), out.constData());
	// Display synthesised in Italian locale (decimal comma) at full precision.
	QVERIFY2(out.contains("<text:p>3,14159</text:p>"), out.constData());
}

void TestTablecell::floatPreservesInputDisplayText()
{
	// Cells loaded from XML carry an explicit <text:p> with the host
	// application's number-format projection. Pre-fix, TablecellFloat's
	// writeSubtags ignored the loaded paragraphs and emitted its own
	// 2-decimal synthesis, so the formatted display was clobbered.
	auto cell = parseCell(
		"<table:table-cell"
		" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\""
		" xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
		" xmlns:text=\"urn:oasis:names:tc:opendocument:xmlns:text:1.0\""
		" office:value-type=\"float\""
		" office:value=\"3.14159\">"
		"<text:p>3.14159 pi</text:p>"
		"</table:table-cell>");
	const QByteArray out = writeCell(cell);
	QVERIFY2(out.contains("<text:p>3.14159 pi</text:p>"), out.constData());
}

void TestTablecell::dateAcceptsIsoDateTimeInput()
{
	// Pre-fix, QDate::fromString of the full ISO datetime failed → invalid
	// QDate. Now we take the first 10 chars (the date prefix).
	auto cell = parseCell(
		"<table:table-cell"
		" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\""
		" xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
		" office:value-type=\"date\""
		" office:date-value=\"2026-01-15T10:30:00\"/>");
	auto dcell = qSharedPointerDynamicCast<TablecellDate>(cell);
	QVERIFY(dcell);
	QCOMPARE(dcell->getDate(), QDate(2026, 1, 15));

	const QByteArray out = writeCell(cell);
	QVERIFY2(out.contains("office:date-value=\"2026-01-15\""), out.constData());
}

void TestTablecell::booleanValueTypeFallsBackAndRoundTrips()
{
	// Pre-A1, Tablecell::builder hit Q_ASSERT(false) and segfaulted on any
	// office:value-type outside {string, float, date, currency}. After C1,
	// boolean cells dispatch to TablecellBoolean with a typed accessor.
	auto cell = parseCell(
		"<table:table-cell"
		" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\""
		" xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
		" office:value-type=\"boolean\""
		" office:boolean-value=\"true\"/>");
	QVERIFY(cell);
	auto bcell = qSharedPointerDynamicCast<TablecellBoolean>(cell);
	QVERIFY(bcell);
	QCOMPARE(bcell->getBool(), true);
	QCOMPARE(cell->instanceCellType(), QStringLiteral("boolean"));

	const QByteArray out = writeCell(cell);
	QVERIFY2(out.contains("office:value-type=\"boolean\""), out.constData());
	QVERIFY2(out.contains("office:boolean-value=\"true\""), out.constData());
}

void TestTablecell::timeValueTypeFallsBackAndRoundTrips()
{
	auto cell = parseCell(
		"<table:table-cell"
		" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\""
		" xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
		" office:value-type=\"time\""
		" office:time-value=\"PT12H30M\"/>");
	QVERIFY(cell);
	auto tcell = qSharedPointerDynamicCast<TablecellTime>(cell);
	QVERIFY(tcell);
	QCOMPARE(tcell->getDuration(), QStringLiteral("PT12H30M"));
	QCOMPARE(cell->instanceCellType(), QStringLiteral("time"));

	const QByteArray out = writeCell(cell);
	QVERIFY2(out.contains("office:value-type=\"time\""), out.constData());
	QVERIFY2(out.contains("office:time-value=\"PT12H30M\""), out.constData());
}

void TestTablecell::percentageValueTypeFallsBackAndRoundTrips()
{
	auto cell = parseCell(
		"<table:table-cell"
		" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\""
		" xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
		" office:value-type=\"percentage\""
		" office:value=\"0.5\"/>");
	QVERIFY(cell);
	auto pcell = qSharedPointerDynamicCast<TablecellPercentage>(cell);
	QVERIFY(pcell);
	QCOMPARE(pcell->getPercentage(), 0.5);
	QCOMPARE(cell->instanceCellType(), QStringLiteral("percentage"));

	const QByteArray out = writeCell(cell);
	QVERIFY2(out.contains("office:value-type=\"percentage\""), out.constData());
	QVERIFY2(out.contains("office:value=\"0.5\""), out.constData());
}

void TestTablecell::richTextSpanBetweenLiteralsRoundtrip()
{
	// Regression: the previous Tag model concatenated all Characters tokens
	// into one inline_text_ and held child elements in a parallel subtags_
	// vector, so <text:p>a <text:span>bold</text:span> b</text:p> became
	// <text:p>a  b<text:span>bold</text:span></text:p> on save. Children are
	// now stored in source order.
	auto cell = parseCell(
		"<table:table-cell"
		" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\""
		" xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
		" xmlns:text=\"urn:oasis:names:tc:opendocument:xmlns:text:1.0\""
		" office:value-type=\"string\">"
		"<text:p>a <text:span text:style-name=\"T1\">bold</text:span> b</text:p>"
		"</table:table-cell>");
	const QByteArray out = writeCell(cell);
	// Span sits between the two literal fragments, in the original order.
	const int posA = out.indexOf("a ");
	const int posSpan = out.indexOf("<text:span");
	const int posB = out.indexOf(" b</text:p>");
	QVERIFY2(posA >= 0 && posSpan > posA && posB > posSpan, out.constData());
	QVERIFY2(!out.contains("a  b"), out.constData());
}

QTEST_MAIN(TestTablecell)
#include "test_tablecell.moc"
