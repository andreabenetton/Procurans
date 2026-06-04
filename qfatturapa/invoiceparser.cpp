// Copyright 2026 - 2026, the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "invoiceparser.h"

#include <QIODevice>
#include <QStringList>
#include <QXmlStreamReader>

namespace qfatturapa
{
	namespace
	{
		bool isStartElementNamed(QXmlStreamReader& xml, const QString& tokenName)
		{
			return xml.tokenType() == QXmlStreamReader::StartElement
			    && xml.name() == tokenName;
		}

		bool isNotEndElementNamed(QXmlStreamReader& xml, const QString& tokenName)
		{
			return !(xml.tokenType() == QXmlStreamReader::EndElement
			      && xml.name() == tokenName);
		}

		void addElementDataToMap(QXmlStreamReader& xml,
		                         QMap<QString, QString>& map)
		{
			if (xml.tokenType() != QXmlStreamReader::StartElement) return;
			const QString elementName = xml.name().toString();
			xml.readNext();
			if (xml.tokenType() != QXmlStreamReader::Characters) return;
			map.insert(elementName, xml.text().toString());
		}

		void addSubelementsDataToMap(QXmlStreamReader& xml,
		                             const QString& tokenName,
		                             const QStringList& subTokenName,
		                             QMap<QString, QString>& out)
		{
			if (!isStartElementNamed(xml, tokenName)) return;
			do {
				xml.readNext();
				for (int i = 0; i < subTokenName.size(); ++i) {
					if (isStartElementNamed(xml, subTokenName.at(i))) {
						addElementDataToMap(xml, out);
					}
				}
			}
			while (isNotEndElementNamed(xml, tokenName));
		}

		QMap<QString, QString> parseHeader(QXmlStreamReader& xml)
		{
			QMap<QString, QString> header;
			if (xml.tokenType() != QXmlStreamReader::StartElement
			    && xml.name() == "CedentePrestatore") {
				return header;
			}
			do {
				xml.readNext();
				if (isStartElementNamed(xml, QStringLiteral("DatiAnagrafici"))) {
					do {
						xml.readNext();
						addSubelementsDataToMap(xml, QStringLiteral("Anagrafica"),
						    { QStringLiteral("Denominazione"),
						      QStringLiteral("Nome"),
						      QStringLiteral("Cognome") }, header);
						addSubelementsDataToMap(xml, QStringLiteral("IdFiscaleIVA"),
						    { QStringLiteral("IdPaese"),
						      QStringLiteral("IdCodice") }, header);
					}
					while (isNotEndElementNamed(xml, QStringLiteral("DatiAnagrafici")));
				}
				addSubelementsDataToMap(xml, QStringLiteral("Sede"),
				    { QStringLiteral("Indirizzo"),
				      QStringLiteral("CAP"),
				      QStringLiteral("Comune") }, header);
			}
			while (isNotEndElementNamed(xml, QStringLiteral("CedentePrestatore")));
			return header;
		}

		QMap<QString, QString> parseDocument(QXmlStreamReader& xml)
		{
			QMap<QString, QString> document;
			if (xml.tokenType() != QXmlStreamReader::StartElement
			    && xml.name() == "DatiGeneraliDocumento") {
				return document;
			}
			addSubelementsDataToMap(xml, QStringLiteral("DatiGeneraliDocumento"),
			    { QStringLiteral("Data"),
			      QStringLiteral("Numero"),
			      QStringLiteral("ImportoTotaleDocumento") }, document);
			return document;
		}

		QMap<QString, QString> parseDetail(QXmlStreamReader& xml)
		{
			QMap<QString, QString> detail;
			if (xml.tokenType() != QXmlStreamReader::StartElement
			    && xml.name() == "DettaglioLinee") {
				return detail;
			}
			addSubelementsDataToMap(xml, QStringLiteral("DettaglioLinee"),
			    { QStringLiteral("Descrizione"),
			      QStringLiteral("Quantita"),
			      QStringLiteral("UnitaMisura"),
			      QStringLiteral("PrezzoUnitario"),
			      QStringLiteral("PrezzoTotale"),
			      QStringLiteral("AliquotaIVA"),
			      QStringLiteral("Natura") }, detail);
			return detail;
		}

		QMap<QString, QString> parsePayment(QXmlStreamReader& xml)
		{
			QMap<QString, QString> payment;
			if (xml.tokenType() != QXmlStreamReader::StartElement
			    && xml.name() == "DettaglioPagamento") {
				return payment;
			}
			addSubelementsDataToMap(xml, QStringLiteral("DettaglioPagamento"),
			    { QStringLiteral("ModalitaPagamento"),
			      QStringLiteral("DataScadenzaPagamento"),
			      QStringLiteral("ImportoPagamento"),
			      QStringLiteral("IBAN") }, payment);
			return payment;
		}

		QMap<QString, QString> parseSummary(QXmlStreamReader& xml)
		{
			QMap<QString, QString> summary;
			if (xml.tokenType() != QXmlStreamReader::StartElement
			    && xml.name() == "DatiRiepilogo") {
				return summary;
			}
			addSubelementsDataToMap(xml, QStringLiteral("DatiRiepilogo"),
			    { QStringLiteral("ImponibileImporto"),
			      QStringLiteral("Imposta"),
			      QStringLiteral("AliquotaIVA"),
			      QStringLiteral("Natura") }, summary);
			return summary;
		}

		void mergeInto(QMap<QString, QString>& dst,
		               const QMap<QString, QString>& src)
		{
			for (auto it = src.constBegin(); it != src.constEnd(); ++it) {
				dst.insert(it.key(), it.value());
			}
		}
	}

	bool parseInvoice(QIODevice& source,
	                  ParsedInvoice& out,
	                  QString* err)
	{
		QXmlStreamReader xml(&source);

		// The XML scan still produces intermediate QMap<QString,QString>
		// rows (the addSubelementsDataToMap helper is shape-agnostic). We
		// fold the seller and document blocks into one header map first
		// and convert each row to the typed struct at the boundary below.
		QMap<QString, QString> header_flat;

		while (!xml.atEnd() && !xml.hasError()) {
			const QXmlStreamReader::TokenType token = xml.readNext();
			if (token == QXmlStreamReader::StartDocument) {
				continue;
			}
			if (isStartElementNamed(xml, QStringLiteral("CedentePrestatore"))) {
				mergeInto(header_flat, parseHeader(xml));
			}
			if (isStartElementNamed(xml, QStringLiteral("DatiGeneraliDocumento"))) {
				mergeInto(header_flat, parseDocument(xml));
			}
			if (isStartElementNamed(xml, QStringLiteral("DettaglioLinee"))) {
				out.details.append(InvoiceLine::fromFlatMap(parseDetail(xml)));
			}
			if (isStartElementNamed(xml, QStringLiteral("DettaglioPagamento"))) {
				out.payments.append(Payment::fromFlatMap(parsePayment(xml)));
			}
			if (isStartElementNamed(xml, QStringLiteral("DatiRiepilogo"))) {
				out.summary.append(SummaryRow::fromFlatMap(parseSummary(xml)));
			}
		}

		out.header = InvoiceHeader::fromFlatMap(header_flat);

		if (xml.hasError()) {
			if (err) *err = xml.errorString();
			return false;
		}
		return true;
	}
}
