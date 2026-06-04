// Copyright 2026 - 2026, the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "codelists.h"

namespace qfatturapa
{
	const QHash<QString, QString>& paymentMethodType()
	{
		static const QHash<QString, QString> map = {
			{ QStringLiteral("MP01"), QStringLiteral("contanti") },
			{ QStringLiteral("MP02"), QStringLiteral("assegno") },
			{ QStringLiteral("MP03"), QStringLiteral("assegno circolare") },
			{ QStringLiteral("MP04"), QStringLiteral("contanti presso Tesoreria") },
			{ QStringLiteral("MP05"), QStringLiteral("bonifico") },
			{ QStringLiteral("MP06"), QStringLiteral("vaglia cambiario") },
			{ QStringLiteral("MP07"), QStringLiteral("bollettino bancario") },
			{ QStringLiteral("MP08"), QStringLiteral("carta di pagamento") },
			{ QStringLiteral("MP09"), QStringLiteral("RID") },
			{ QStringLiteral("MP10"), QStringLiteral("RID utenze") },
			{ QStringLiteral("MP11"), QStringLiteral("RID veloce") },
			{ QStringLiteral("MP12"), QStringLiteral("RIBA") },
			{ QStringLiteral("MP13"), QStringLiteral("MAV") },
			{ QStringLiteral("MP14"), QStringLiteral("quietanza erario") },
			{ QStringLiteral("MP15"), QStringLiteral("giroconto su conti di contabilità speciale") },
			{ QStringLiteral("MP16"), QStringLiteral("domiciliazione bancaria") },
			{ QStringLiteral("MP17"), QStringLiteral("domiciliazione postale") },
			{ QStringLiteral("MP18"), QStringLiteral("bollettino di c/c postale") },
			{ QStringLiteral("MP19"), QStringLiteral("SEPA Direct Debit") },
			{ QStringLiteral("MP20"), QStringLiteral("SEPA Direct Debit CORE") },
			{ QStringLiteral("MP21"), QStringLiteral("SEPA Direct Debit B2B") },
			{ QStringLiteral("MP22"), QStringLiteral("Trattenuta su somme già riscosse") },
			{ QStringLiteral("MP23"), QStringLiteral("PagoPA") },
		};
		return map;
	}

	const QHash<QString, QString>& naturaType()
	{
		static const QHash<QString, QString> map = {
			{ QStringLiteral("N1"), QStringLiteral("Escluse ex. art. 15") },
			{ QStringLiteral("N2"), QStringLiteral("Non soggette") },
			{ QStringLiteral("N2.1"), QStringLiteral("Non soggette - artt. da 7 a 7-septies DPR 633/72") },
			{ QStringLiteral("N2.2"), QStringLiteral("Non soggette - altri casi") },
			{ QStringLiteral("N3"), QStringLiteral("Non Imponibili") },
			{ QStringLiteral("N3.1"), QStringLiteral("Non Imponibili - esportazioni") },
			{ QStringLiteral("N3.2"), QStringLiteral("Non Imponibili - cessioni intracomunitarie") },
			{ QStringLiteral("N3.3"), QStringLiteral("Non Imponibili - cessioni verso San Marino") },
			{ QStringLiteral("N3.4"), QStringLiteral("Non Imponibili - operazioni assimilate alle cessioni all'esportazione") },
			{ QStringLiteral("N3.5"), QStringLiteral("Non Imponibili - a seguito di dichiarazioni d'intento") },
			{ QStringLiteral("N3.6"), QStringLiteral("Non Imponibili - altre operazioni che non concorrono alla formazione del plafond") },
			{ QStringLiteral("N4"), QStringLiteral("Esenti") },
			{ QStringLiteral("N5"), QStringLiteral("Regime del margine") },
			{ QStringLiteral("N6"), QStringLiteral("Inversione contabile") },
			{ QStringLiteral("N6.1"), QStringLiteral("Inversione contabile - cessione di rottami e altri materiali di recupero") },
			{ QStringLiteral("N6.2"), QStringLiteral("Inversione contabile - cessione di oro e argento puro") },
			{ QStringLiteral("N6.3"), QStringLiteral("Inversione contabile - subappalto nel settore edile") },
			{ QStringLiteral("N6.4"), QStringLiteral("Inversione contabile - cessione di fabbricati") },
			{ QStringLiteral("N6.5"), QStringLiteral("Inversione contabile - cessione di telefoni cellulari") },
			{ QStringLiteral("N6.6"), QStringLiteral("Inversione contabile - cessione di prodotti elettronici") },
			{ QStringLiteral("N6.7"), QStringLiteral("Inversione contabile - prestazioni comparto edile e settori connessi") },
			{ QStringLiteral("N6.8"), QStringLiteral("Inversione contabile - operazioni settore energetico") },
			{ QStringLiteral("N6.9"), QStringLiteral("Inversione contabile - altri casi") },
			{ QStringLiteral("N7"), QStringLiteral("IVA assolta in altro stato UE") },
		};
		return map;
	}
}
