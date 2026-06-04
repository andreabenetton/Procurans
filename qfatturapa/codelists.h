// Copyright 2026 - 2026, the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef QFATTURAPA_CODELISTS_H
#define QFATTURAPA_CODELISTS_H

#include <QHash>
#include <QString>

namespace qfatturapa
{
	// FatturaPA ModalitaPagamento (MP01..MP23) → Italian description.
	const QHash<QString, QString>& paymentMethodType();

	// FatturaPA Natura (N1..N7 plus N2.x, N3.x, N6.x sub-codes) → Italian
	// description. The plain N2 / N3 / N6 entries are retained for the
	// historical archive (the codes were retired for new invoices on
	// 2021-01-01 but pre-existing imports still reference them).
	const QHash<QString, QString>& naturaType();
}

#endif // QFATTURAPA_CODELISTS_H
