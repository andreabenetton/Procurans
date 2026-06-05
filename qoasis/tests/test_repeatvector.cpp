// Copyright 2026 - 2026, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include <QtTest>
#include <QObject>
#include <QSharedPointer>

#include "qoasis/repeatvector.h"
#include "qoasis/table/tablecell.h"
#include "qoasis/table/tablecellstring.h"

using qoasis::RepeatVector;
using qoasis::table::Tablecell;
using qoasis::table::TablecellString;

namespace
{
	QSharedPointer<Tablecell> mkEmptyBase(int repeat = 1)
	{
		// Plain Tablecell with no _valueText — its isEmpty() reports true.
		return QSharedPointer<Tablecell>(new Tablecell(repeat));
	}

	QSharedPointer<Tablecell> mkString(const QString& text)
	{
		return QSharedPointer<Tablecell>(new TablecellString(text));
	}
}

class TestRepeatVector : public QObject
{
	Q_OBJECT

private slots:
	void emptyVectorReportsZero();
	void appendNonRepeatGrowsByOne();
	void appendRepeatThreeExpandsPlaceholders();
	void lastNotEmptyTracksContent();
	void insertBeyondEndPadsWithPlaceholder();
	void replaceAtConcreteIndex();
	void removeOnPlaceholderDecrementsBaseRepeat();
	void removeEndingEmptyStripsTrailing();
	void appendHighRepeatPreserved();
};

void TestRepeatVector::emptyVectorReportsZero()
{
	RepeatVector<Tablecell> rv;
	QCOMPARE(rv.size(), 0);
	QCOMPARE(rv.lastNotEmpty(), -1);
	QVERIFY(rv.at(0).isNull());
}

void TestRepeatVector::appendNonRepeatGrowsByOne()
{
	RepeatVector<Tablecell> rv;
	auto c = mkString(QStringLiteral("hello"));
	rv.append(c);
	QCOMPARE(rv.size(), 1);
	QVERIFY(rv.at(0) == c);
}

void TestRepeatVector::appendRepeatThreeExpandsPlaceholders()
{
	RepeatVector<Tablecell> rv;
	auto base = mkEmptyBase(3);
	rv.append(base);

	// size() previously returned last-valid-index; verifies the eb7aa8f fix.
	QCOMPARE(rv.size(), 3);
	QVERIFY(rv.at(0) == base);
	QVERIFY(rv.at(1).isNull());
	QVERIFY(rv.at(2).isNull());
}

void TestRepeatVector::lastNotEmptyTracksContent()
{
	RepeatVector<Tablecell> rv;
	QCOMPARE(rv.lastNotEmpty(), -1);

	// An empty Tablecell (no _valueText) is isEmpty()==true.
	rv.append(mkEmptyBase());
	QCOMPARE(rv.lastNotEmpty(), -1);

	// A TablecellString with text is isEmpty()==false (inherits base).
	rv.append(mkString(QStringLiteral("X")));
	QCOMPARE(rv.lastNotEmpty(), 1);
}

void TestRepeatVector::insertBeyondEndPadsWithPlaceholder()
{
	RepeatVector<Tablecell> rv;
	auto a = mkString(QStringLiteral("A"));
	rv.append(a);
	auto z = mkString(QStringLiteral("Z"));
	rv.insert(3, z);

	// Padded with a placeholder(repeat=2) for the 2 vacated slots, then z.
	QCOMPARE(rv.size(), 4);
	QVERIFY(rv.at(0) == a);
	QVERIFY(rv.at(3) == z);
}

void TestRepeatVector::replaceAtConcreteIndex()
{
	// Exercises RepeatVector::replace on a non-null slot — the path that
	// before eb7aa8f hit the 'vector_.insertWithRepeats' typo. Use distinct
	// concrete cells so the swap target is unambiguous.
	RepeatVector<Tablecell> rv;
	auto a = mkString(QStringLiteral("A"));
	auto b = mkString(QStringLiteral("B"));
	auto c = mkString(QStringLiteral("C"));
	rv.append(a);
	rv.append(b);
	rv.append(c);

	auto z = mkString(QStringLiteral("Z"));
	rv.replace(1, z);

	QVERIFY(rv.at(0) == a);
	QVERIFY(rv.at(1) == z);
	QVERIFY(rv.at(2) == c);
}

void TestRepeatVector::removeOnPlaceholderDecrementsBaseRepeat()
{
	RepeatVector<Tablecell> rv;
	auto base = mkEmptyBase(3);
	rv.append(base);
	QCOMPARE(rv.size(), 3);
	QCOMPARE(base->getRepeat(), 3);

	rv.remove(1);   // remove a placeholder
	QCOMPARE(base->getRepeat(), 2);
	QCOMPARE(rv.size(), 2);
}

void TestRepeatVector::removeEndingEmptyStripsTrailing()
{
	RepeatVector<Tablecell> rv;
	rv.append(mkString(QStringLiteral("A")));
	rv.append(mkEmptyBase(2));    // empty base + one placeholder
	QCOMPARE(rv.size(), 3);

	rv.removeEndingEmpty();
	QCOMPARE(rv.lastNotEmpty(), 0);
	QCOMPARE(rv.size(), 1);
}

void TestRepeatVector::appendHighRepeatPreserved()
{
	// Regression: LibreOffice emits <table:table-column
	// number-columns-repeated="1010"/> (or 1024) as the trailing default-column
	// fill for a Calc sheet. The previous MAXREPEAT=1000 cap silently dropped
	// the entire column declaration, leaving the saved file with fewer columns
	// declared than rows actually use — which LO 24+ flags as "damaged file".
	RepeatVector<Tablecell> rv;
	auto fill = mkEmptyBase(1010);
	rv.append(fill);

	QCOMPARE(rv.size(), 1010);
	QVERIFY(rv.at(0) == fill);
	QVERIFY(rv.at(1009).isNull());
}

QTEST_MAIN(TestRepeatVector)
#include "test_repeatvector.moc"
