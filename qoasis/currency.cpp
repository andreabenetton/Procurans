// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "currency.h"

namespace qoasis
{
	bool Currency::type_initialized_ = false;
	QMap<int, QString> Currency::symbols_;
	QMap<int, QString> Currency::iso_;
	QMap<int, int> Currency::decimals_;

	Currency::Currency(int constant)
	{
		if (type_initialized_ == false)
		{
			addCurrency(EUR, "€", "EUR", 2);
			type_initialized_ = true;
		}
		if (symbols_.contains(constant))
		{
			currency_ = constant;
			is_valid_ = true;
		}
	}

	Currency::Currency(QString iso)
	{
		if (type_initialized_ == false)
		{
			addCurrency(EUR, "€", "EUR", 2);
			type_initialized_ = true;
		}
		currency_ = iso_.key(iso, 0);
		if (currency_ != 0)
		{
			is_valid_ = true;
		}
	}

	Currency::Currency(const Currency& obj)
	{
		is_valid_ = obj.is_valid_;
		currency_ = obj.currency_;
	}

	void Currency::addCurrency(int constant, const QString symbol, const QString iso, const int decimals)
	{
		symbols_.insert(constant, symbol);
		iso_.insert(constant, iso);
		decimals_.insert(constant, decimals);
	}

	bool Currency::getIsValid() const
	{
		return is_valid_;
	}

	QString Currency::getSymbol() const
	{
		return symbols_.value(currency_);
	}

	QString Currency::getIso() const
	{
		return iso_.value(currency_);
	}

	QString Currency::getSymbol(const int constant)
	{
		return symbols_.value(constant);
	}

	auto Currency::getSymbolFromIso(const QString iso) -> QString
	{
		const auto constant = iso_.key(iso);
		return symbols_.value(constant);
	}

	QString Currency::getIso(int constant)
	{
		return iso_.value(constant);
	}

	QString Currency::formatAmount(double amount) const
	{
		const QString pattern = "%'." + QString::number(decimals_.value(currency_)) + "f";
		QString out = "";
		if (amount < 0) out = "-";
		out += symbols_.value(currency_);
		if (amount < 0)
			out += QString::asprintf(qPrintable(pattern), -amount);
		else
			out += QString::asprintf(qPrintable(pattern), amount);

		out.replace(".", "!");
		out.replace(",", ".");
		out.replace("!", ",");
		return out;
	}
}
