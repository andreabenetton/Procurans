// Copyright 2026 - 2026, the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "invoiceparser.h"

#include <QIODevice>
#include <QXmlStreamReader>

namespace qfatturapa
{
	bool parseInvoice(QIODevice& /*source*/,
	                  ParsedInvoice& /*out*/,
	                  QString* err)
	{
		// Stub — filled in by the migration commit that moves MainWindow's
		// parser into this library.
		if (err) *err = QStringLiteral("parseInvoice not yet implemented");
		return false;
	}
}
