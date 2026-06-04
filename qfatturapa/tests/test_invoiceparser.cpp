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
	void parsesDatiGeneraliDocumento();
	void parsesMultipleDettaglioLinee();
	void parsesDettaglioPagamento();
	void parsesDatiRiepilogoNaturaN21();
	void reportsErrorOnMalformedXml();
	void synthesizesNoPaymentsCaseLeftToCaller();
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

void TestInvoiceParser::parsesDatiGeneraliDocumento()
{
	const QByteArray xml = QByteArrayLiteral(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<FatturaElettronica><FatturaElettronicaBody><DatiGenerali>"
		"<DatiGeneraliDocumento>"
		"<Data>2026-06-04</Data>"
		"<Numero>FT-00042</Numero>"
		"<ImportoTotaleDocumento>1234.56</ImportoTotaleDocumento>"
		"</DatiGeneraliDocumento>"
		"</DatiGenerali></FatturaElettronicaBody></FatturaElettronica>\n");

	qfatturapa::ParsedInvoice out;
	QString err;
	QVERIFY2(runParse(xml, out, &err), qPrintable(err));
	QCOMPARE(out.header.value(QStringLiteral("Data")), QStringLiteral("2026-06-04"));
	QCOMPARE(out.header.value(QStringLiteral("Numero")), QStringLiteral("FT-00042"));
	QCOMPARE(out.header.value(QStringLiteral("ImportoTotaleDocumento")),
	         QStringLiteral("1234.56"));
}

void TestInvoiceParser::parsesMultipleDettaglioLinee()
{
	const QByteArray xml = QByteArrayLiteral(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<FatturaElettronica><FatturaElettronicaBody><DatiBeniServizi>"
		"<DettaglioLinee>"
		"<Descrizione>Consulenza</Descrizione>"
		"<Quantita>2.00</Quantita>"
		"<PrezzoUnitario>100.00</PrezzoUnitario>"
		"<PrezzoTotale>200.00</PrezzoTotale>"
		"<AliquotaIVA>22.00</AliquotaIVA>"
		"</DettaglioLinee>"
		"<DettaglioLinee>"
		"<Descrizione>Materiale esente</Descrizione>"
		"<Quantita>1.00</Quantita>"
		"<PrezzoTotale>50.00</PrezzoTotale>"
		"<AliquotaIVA>0.00</AliquotaIVA>"
		"<Natura>N4</Natura>"
		"</DettaglioLinee>"
		"</DatiBeniServizi></FatturaElettronicaBody></FatturaElettronica>\n");

	qfatturapa::ParsedInvoice out;
	QString err;
	QVERIFY2(runParse(xml, out, &err), qPrintable(err));
	QCOMPARE(out.details.size(), 2);
	QCOMPARE(out.details.at(0).value(QStringLiteral("Descrizione")),
	         QStringLiteral("Consulenza"));
	QCOMPARE(out.details.at(0).value(QStringLiteral("PrezzoTotale")),
	         QStringLiteral("200.00"));
	QCOMPARE(out.details.at(1).value(QStringLiteral("Natura")), QStringLiteral("N4"));
	QCOMPARE(out.details.at(1).value(QStringLiteral("AliquotaIVA")),
	         QStringLiteral("0.00"));
}

void TestInvoiceParser::parsesDettaglioPagamento()
{
	const QByteArray xml = QByteArrayLiteral(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<FatturaElettronica><FatturaElettronicaBody><DatiPagamento>"
		"<DettaglioPagamento>"
		"<ModalitaPagamento>MP05</ModalitaPagamento>"
		"<DataScadenzaPagamento>2026-07-31</DataScadenzaPagamento>"
		"<ImportoPagamento>1234.56</ImportoPagamento>"
		"<IBAN>IT60X0542811101000000123456</IBAN>"
		"</DettaglioPagamento>"
		"</DatiPagamento></FatturaElettronicaBody></FatturaElettronica>\n");

	qfatturapa::ParsedInvoice out;
	QString err;
	QVERIFY2(runParse(xml, out, &err), qPrintable(err));
	QCOMPARE(out.payments.size(), 1);
	const auto& p = out.payments.at(0);
	QCOMPARE(p.value(QStringLiteral("ModalitaPagamento")), QStringLiteral("MP05"));
	QCOMPARE(p.value(QStringLiteral("DataScadenzaPagamento")),
	         QStringLiteral("2026-07-31"));
	QCOMPARE(p.value(QStringLiteral("ImportoPagamento")), QStringLiteral("1234.56"));
	QCOMPARE(p.value(QStringLiteral("IBAN")),
	         QStringLiteral("IT60X0542811101000000123456"));
}

void TestInvoiceParser::parsesDatiRiepilogoNaturaN21()
{
	// N2.1 was introduced in spec v1.5 (2021). The parser doesn't interpret
	// the code, it just passes the literal string through into summary[].
	const QByteArray xml = QByteArrayLiteral(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<FatturaElettronica><FatturaElettronicaBody><DatiBeniServizi>"
		"<DatiRiepilogo>"
		"<AliquotaIVA>0.00</AliquotaIVA>"
		"<Natura>N2.1</Natura>"
		"<ImponibileImporto>100.00</ImponibileImporto>"
		"<Imposta>0.00</Imposta>"
		"</DatiRiepilogo>"
		"</DatiBeniServizi></FatturaElettronicaBody></FatturaElettronica>\n");

	qfatturapa::ParsedInvoice out;
	QString err;
	QVERIFY2(runParse(xml, out, &err), qPrintable(err));
	QCOMPARE(out.summary.size(), 1);
	QCOMPARE(out.summary.at(0).value(QStringLiteral("Natura")), QStringLiteral("N2.1"));
	QCOMPARE(out.summary.at(0).value(QStringLiteral("ImponibileImporto")),
	         QStringLiteral("100.00"));
}

void TestInvoiceParser::reportsErrorOnMalformedXml()
{
	const QByteArray xml = QByteArrayLiteral("<FatturaElettronica><not-closed>");
	qfatturapa::ParsedInvoice out;
	QString err;
	QVERIFY(!runParse(xml, out, &err));
	QVERIFY2(!err.isEmpty(), "Error string should be populated on parse failure");
}

void TestInvoiceParser::synthesizesNoPaymentsCaseLeftToCaller()
{
	// The "no DatiPagamento → synthesise placeholder row" convention lives in
	// MainWindow, not in the library. Here we just confirm the library leaves
	// payments empty when the input has no DatiPagamento.
	const QByteArray xml = QByteArrayLiteral(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<FatturaElettronica><FatturaElettronicaBody><DatiGenerali>"
		"<DatiGeneraliDocumento>"
		"<Data>2026-06-04</Data><Numero>F1</Numero>"
		"<ImportoTotaleDocumento>10.00</ImportoTotaleDocumento>"
		"</DatiGeneraliDocumento>"
		"</DatiGenerali></FatturaElettronicaBody></FatturaElettronica>\n");

	qfatturapa::ParsedInvoice out;
	QString err;
	QVERIFY2(runParse(xml, out, &err), qPrintable(err));
	QVERIFY(out.payments.isEmpty());
}

QTEST_MAIN(TestInvoiceParser)
#include "test_invoiceparser.moc"
