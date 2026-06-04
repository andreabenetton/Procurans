// Copyright 2026 - 2026, the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef QFATTURAPA_INVOICETYPES_H
#define QFATTURAPA_INVOICETYPES_H

#include <QList>
#include <QMap>
#include <QString>

namespace qfatturapa
{
	// Field values are kept as QString — lossless from XML, no early
	// double/QDate parsing that could lose precision or hide an invalid
	// input. Convert at the consumer when needed.

	struct SellerIdentity
	{
		QString denominazione;
		QString nome;
		QString cognome;
		QString idPaese;
		QString idCodice;
		QString indirizzo;
		QString cap;
		QString comune;
	};

	struct DocumentInfo
	{
		QString data;
		QString numero;
		QString importoTotaleDocumento;
	};

	struct InvoiceHeader
	{
		SellerIdentity seller;
		DocumentInfo document;

		// Legacy "flat" view for code that still walks QMap<QString,QString>
		// by XML element name (the GridSchemaField-driven grids in
		// MainWindow). Eventually that machinery can move to typed access.
		QMap<QString, QString> toFlatMap() const;
		static InvoiceHeader fromFlatMap(const QMap<QString, QString>& m);
	};

	struct InvoiceLine
	{
		QString descrizione;
		QString quantita;
		QString unitaMisura;
		QString prezzoUnitario;
		QString prezzoTotale;
		QString aliquotaIVA;
		QString natura;

		QMap<QString, QString> toFlatMap() const;
		static InvoiceLine fromFlatMap(const QMap<QString, QString>& m);
	};

	struct Payment
	{
		QString modalitaPagamento;
		QString dataScadenzaPagamento;
		QString importoPagamento;
		QString iban;

		QMap<QString, QString> toFlatMap() const;
		static Payment fromFlatMap(const QMap<QString, QString>& m);
	};

	struct SummaryRow
	{
		QString imponibileImporto;
		QString imposta;
		QString aliquotaIVA;
		QString natura;

		QMap<QString, QString> toFlatMap() const;
		static SummaryRow fromFlatMap(const QMap<QString, QString>& m);
	};

	// Convenience: convert a typed list to a list of flat maps for grid
	// rendering. Defined as a free template so any of the row-shaped
	// structs above (which all provide toFlatMap) work uniformly.
	template <class T>
	QList<QMap<QString, QString>> toFlatMaps(const QList<T>& rows)
	{
		QList<QMap<QString, QString>> out;
		out.reserve(rows.size());
		for (const T& row : rows) out.append(row.toFlatMap());
		return out;
	}
}

#endif // QFATTURAPA_INVOICETYPES_H
