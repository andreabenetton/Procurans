// Copyright 2026 - 2026, the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef QFATTURAPA_INVOICEPARSER_H
#define QFATTURAPA_INVOICEPARSER_H

#include <QList>
#include <QString>

#include "invoicetypes.h"

QT_BEGIN_NAMESPACE
class QIODevice;
QT_END_NAMESPACE

namespace qfatturapa
{
	struct ParsedInvoice
	{
		InvoiceHeader         header;
		QList<InvoiceLine>    details;
		QList<Payment>        payments;
		QList<SummaryRow>     summary;
	};

	/**
	 * \brief Parse a FatturaPA invoice XML document from \a source into \a out.
	 *
	 * Drives a QXmlStreamReader over the source. Returns false on parse
	 * error; \a err (if non-null) is set to the QXmlStreamReader::errorString.
	 * Partial results may already be present in \a out on failure.
	 *
	 * Does NOT apply any UI-layer conventions (e.g. synthesising a payment
	 * row when DatiPagamento is absent) — that responsibility stays with the
	 * caller.
	 */
	bool parseInvoice(QIODevice& source,
	                  ParsedInvoice& out,
	                  QString* err = nullptr);
}

#endif // QFATTURAPA_INVOICEPARSER_H
