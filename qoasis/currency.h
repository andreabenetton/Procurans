// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef CURRENCY_H
#define CURRENCY_H

#include <QMap>

namespace qoasis
{
	class Currency
	{
	public:
		static const int EUR = 978;

		Currency(int constant = EUR);
		Currency(QString iso);
		Currency(const Currency& obj);

		static QString getSymbol(int constant);
		static QString getIso(int constant);
		static QString getSymbolFromIso(QString iso);

		bool getIsValid() const;
		QString getSymbol() const;
		QString getIso() const;
		QString formatAmount(double amount) const;

	private:
		static bool type_initialized_;
		static QMap<int, QString> symbols_;
		static QMap<int, QString> iso_;
		static QMap<int, int> decimals_;
		static void addCurrency(int constant, QString symbol, QString iso, int decimals);

		bool is_valid_ = false;
		int currency_;
	};
};
#endif // CURRENCY_H
