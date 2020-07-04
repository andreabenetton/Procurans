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
		TablecellCurrency(Currency currency, double amount, int repeat, QString style = "");
		TablecellCurrency(Currency currency, double amount, QString style = "");
		TablecellCurrency(QXmlStreamReader& reader);

		static const QString kCellTypeValue;

		double getAmount() const;
		Currency getCurrency() const;

		bool isEmpty() override;

		// implements Tablecell
		QString instanceCellType() override;

	private:
		static const QString kCellTypeAttribute;
		static const QString kCellCurrencyAttribute;

		double value_amount_;
		Currency value_currency_;

		// implements Tag
		void readAttribute(QStringRef name, QStringRef value) override;
		void writeAttributes(QXmlStreamWriter* writer) override;
		void writeSubtags(QXmlStreamWriter* writer) override;
	};
}
#endif // TABLECELLCURRENCY_H
