// Copyright 2026 - 2026, the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include <QtTest>
#include <QObject>
#include <QBuffer>
#include <QByteArray>
#include <QString>

#include "qfatturapa/invoiceparser.h"

namespace
{
	bool runParse(const QByteArray& xml,
	              qfatturapa::ParsedInvoice& out,
	              QString* err = nullptr)
	{
		QBuffer src;
		src.setData(xml);
		if (!src.open(QIODevice::ReadOnly)) return false;
		return qfatturapa::parseInvoice(src, out, err);
	}
}

class TestInvoiceParser : public QObject
{
	Q_OBJECT

private slots:
	void parsesMinimalSellerHeader();
};

void TestInvoiceParser::parsesMinimalSellerHeader()
{
	const QByteArray xml = QByteArrayLiteral(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<FatturaElettronica>\n"
		"  <FatturaElettronicaHeader>\n"
		"    <CedentePrestatore>\n"
		"      <DatiAnagrafici>\n"
		"        <IdFiscaleIVA>\n"
		"          <IdPaese>IT</IdPaese>\n"
		"          <IdCodice>01234567890</IdCodice>\n"
		"        </IdFiscaleIVA>\n"
		"        <Anagrafica>\n"
		"          <Denominazione>ACME SRL</Denominazione>\n"
		"        </Anagrafica>\n"
		"      </DatiAnagrafici>\n"
		"      <Sede>\n"
		"        <Indirizzo>Via Roma 1</Indirizzo>\n"
		"        <CAP>20100</CAP>\n"
		"        <Comune>Milano</Comune>\n"
		"      </Sede>\n"
		"    </CedentePrestatore>\n"
		"  </FatturaElettronicaHeader>\n"
		"</FatturaElettronica>\n");

	qfatturapa::ParsedInvoice out;
	QString err;
	QVERIFY2(runParse(xml, out, &err), qPrintable(err));

	QCOMPARE(out.header.value(QStringLiteral("Denominazione")),
	         QStringLiteral("ACME SRL"));
	QCOMPARE(out.header.value(QStringLiteral("IdPaese")), QStringLiteral("IT"));
	QCOMPARE(out.header.value(QStringLiteral("IdCodice")),
	         QStringLiteral("01234567890"));
	QCOMPARE(out.header.value(QStringLiteral("Indirizzo")),
	         QStringLiteral("Via Roma 1"));
	QCOMPARE(out.header.value(QStringLiteral("CAP")), QStringLiteral("20100"));
	QCOMPARE(out.header.value(QStringLiteral("Comune")), QStringLiteral("Milano"));
}

QTEST_MAIN(TestInvoiceParser)
#include "test_invoiceparser.moc"
