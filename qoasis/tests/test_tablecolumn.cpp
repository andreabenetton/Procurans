// Copyright 2026 - 2026, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include <QtTest>
#include <QObject>

#include "qoasis/table/tablecolumn.h"

using qoasis::table::Tablecolumn;

class TestTablecolumn : public QObject
{
	Q_OBJECT

private slots:
	void isEmptyDistinguishesTrivialFromStyled();
};

void TestTablecolumn::isEmptyDistinguishesTrivialFromStyled()
{
	Tablecolumn trivial;
	QVERIFY(trivial.isEmpty());

	Tablecolumn styled(1, QStringLiteral("co1"));
	QVERIFY(!styled.isEmpty());

	Tablecolumn repeated(5);
	QVERIFY(!repeated.isEmpty());

	Tablecolumn withDefaultCellStyle(1, QString(), QStringLiteral("ce1"));
	QVERIFY(!withDefaultCellStyle.isEmpty());
}

QTEST_MAIN(TestTablecolumn)
#include "test_tablecolumn.moc"
