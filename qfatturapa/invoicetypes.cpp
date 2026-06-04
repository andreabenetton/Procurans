// Copyright 2026 - 2026, the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "invoicetypes.h"

namespace qfatturapa
{
	namespace
	{
		// Insert key=value into the map iff value is non-empty. Saves the
		// caller from having to guard every toFlatMap() field.
		void insertIfSet(QMap<QString, QString>& m,
		                 const QString& key, const QString& value)
		{
			if (!value.isEmpty()) m.insert(key, value);
		}
	}

	QMap<QString, QString> InvoiceHeader::toFlatMap() const
	{
		QMap<QString, QString> m;
		insertIfSet(m, QStringLiteral("Denominazione"),  seller.denominazione);
		insertIfSet(m, QStringLiteral("Nome"),           seller.nome);
		insertIfSet(m, QStringLiteral("Cognome"),        seller.cognome);
		insertIfSet(m, QStringLiteral("IdPaese"),        seller.idPaese);
		insertIfSet(m, QStringLiteral("IdCodice"),       seller.idCodice);
		insertIfSet(m, QStringLiteral("Indirizzo"),      seller.indirizzo);
		insertIfSet(m, QStringLiteral("CAP"),            seller.cap);
		insertIfSet(m, QStringLiteral("Comune"),         seller.comune);
		insertIfSet(m, QStringLiteral("Data"),           document.data);
		insertIfSet(m, QStringLiteral("Numero"),         document.numero);
		insertIfSet(m, QStringLiteral("ImportoTotaleDocumento"),
		            document.importoTotaleDocumento);
		return m;
	}

	InvoiceHeader InvoiceHeader::fromFlatMap(const QMap<QString, QString>& m)
	{
		InvoiceHeader h;
		h.seller.denominazione = m.value(QStringLiteral("Denominazione"));
		h.seller.nome          = m.value(QStringLiteral("Nome"));
		h.seller.cognome       = m.value(QStringLiteral("Cognome"));
		h.seller.idPaese       = m.value(QStringLiteral("IdPaese"));
		h.seller.idCodice      = m.value(QStringLiteral("IdCodice"));
		h.seller.indirizzo     = m.value(QStringLiteral("Indirizzo"));
		h.seller.cap           = m.value(QStringLiteral("CAP"));
		h.seller.comune        = m.value(QStringLiteral("Comune"));
		h.document.data        = m.value(QStringLiteral("Data"));
		h.document.numero      = m.value(QStringLiteral("Numero"));
		h.document.importoTotaleDocumento =
			m.value(QStringLiteral("ImportoTotaleDocumento"));
		return h;
	}

	QMap<QString, QString> InvoiceLine::toFlatMap() const
	{
		QMap<QString, QString> m;
		insertIfSet(m, QStringLiteral("Descrizione"),    descrizione);
		insertIfSet(m, QStringLiteral("Quantita"),       quantita);
		insertIfSet(m, QStringLiteral("UnitaMisura"),    unitaMisura);
		insertIfSet(m, QStringLiteral("PrezzoUnitario"), prezzoUnitario);
		insertIfSet(m, QStringLiteral("PrezzoTotale"),   prezzoTotale);
		insertIfSet(m, QStringLiteral("AliquotaIVA"),    aliquotaIVA);
		insertIfSet(m, QStringLiteral("Natura"),         natura);
		return m;
	}

	InvoiceLine InvoiceLine::fromFlatMap(const QMap<QString, QString>& m)
	{
		InvoiceLine l;
		l.descrizione    = m.value(QStringLiteral("Descrizione"));
		l.quantita       = m.value(QStringLiteral("Quantita"));
		l.unitaMisura    = m.value(QStringLiteral("UnitaMisura"));
		l.prezzoUnitario = m.value(QStringLiteral("PrezzoUnitario"));
		l.prezzoTotale   = m.value(QStringLiteral("PrezzoTotale"));
		l.aliquotaIVA    = m.value(QStringLiteral("AliquotaIVA"));
		l.natura         = m.value(QStringLiteral("Natura"));
		return l;
	}

	QMap<QString, QString> Payment::toFlatMap() const
	{
		QMap<QString, QString> m;
		insertIfSet(m, QStringLiteral("ModalitaPagamento"),     modalitaPagamento);
		insertIfSet(m, QStringLiteral("DataScadenzaPagamento"), dataScadenzaPagamento);
		insertIfSet(m, QStringLiteral("ImportoPagamento"),      importoPagamento);
		insertIfSet(m, QStringLiteral("IBAN"),                  iban);
		return m;
	}

	Payment Payment::fromFlatMap(const QMap<QString, QString>& m)
	{
		Payment p;
		p.modalitaPagamento     = m.value(QStringLiteral("ModalitaPagamento"));
		p.dataScadenzaPagamento = m.value(QStringLiteral("DataScadenzaPagamento"));
		p.importoPagamento      = m.value(QStringLiteral("ImportoPagamento"));
		p.iban                  = m.value(QStringLiteral("IBAN"));
		return p;
	}

	QMap<QString, QString> SummaryRow::toFlatMap() const
	{
		QMap<QString, QString> m;
		insertIfSet(m, QStringLiteral("ImponibileImporto"), imponibileImporto);
		insertIfSet(m, QStringLiteral("Imposta"),           imposta);
		insertIfSet(m, QStringLiteral("AliquotaIVA"),       aliquotaIVA);
		insertIfSet(m, QStringLiteral("Natura"),            natura);
		return m;
	}

	SummaryRow SummaryRow::fromFlatMap(const QMap<QString, QString>& m)
	{
		SummaryRow s;
		s.imponibileImporto = m.value(QStringLiteral("ImponibileImporto"));
		s.imposta           = m.value(QStringLiteral("Imposta"));
		s.aliquotaIVA       = m.value(QStringLiteral("AliquotaIVA"));
		s.natura            = m.value(QStringLiteral("Natura"));
		return s;
	}
}
