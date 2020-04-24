// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECELLCURRENCY_H
#define TABLECELLCURRENCY_H

#include "../currency.h"

#include "tablecell.h"

namespace qoasis::table
{
	class TablecellCurrency : public Tablecell
	{
	public:
		TablecellCurrency(Currency currency, double amount, int repeat = 1);
		TablecellCurrency(QXmlStreamReader& reader);
		TablecellCurrency(const TablecellCurrency& obj);

		static const QLatin1String kCellTypeValue;

		double getAmount();
		Currency getCurrency();

		// implements Tablecell
		QLatin1String instanceCellType() override;

	protected:
		// implements Tag
		void readAttribute(QStringRef name, QStringRef value) override;
		void writeAttributes(QXmlStreamWriter* writer) override;
		void writeSubtags(QXmlStreamWriter* writer) override;

	private:
		static const QLatin1String kCellTypeAttribute;
		static const QLatin1String kCellCurrencyAttribute;

		double value_amount_;
		Currency value_currency_;
	};
}
#endif // TABLECELLCURRENCY_H
