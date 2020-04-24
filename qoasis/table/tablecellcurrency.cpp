// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecellcurrency.h"

namespace qoasis::table
{
	const QLatin1String TablecellCurrency::kCellTypeValue = QLatin1String("currency");
	const QLatin1String TablecellCurrency::kCellTypeAttribute = QLatin1String("office:value");
	const QLatin1String TablecellCurrency::kCellCurrencyAttribute = QLatin1String("office:currency");

	TablecellCurrency::TablecellCurrency(Currency currency, double number, int repeat) : Tablecell(repeat)
	{
		value_amount_ = number;
		value_currency_ = currency;
	}

	TablecellCurrency::TablecellCurrency(QXmlStreamReader& reader) : Tablecell(reader)
	{
		value_amount_ = 0.0;
		value_currency_ = Currency(Currency::EUR);

		Tag::read(reader);
	}

	TablecellCurrency::TablecellCurrency(const TablecellCurrency& obj): Tablecell(obj)
	{
		value_amount_ = obj.value_amount_;
		value_currency_ = Currency(obj.value_currency_);
	}


	double TablecellCurrency::getAmount()
	{
		return value_amount_;
	}

	Currency TablecellCurrency::getCurrency()
	{
		return value_currency_;
	}

	QLatin1String TablecellCurrency::instanceCellType()
	{
		return kCellTypeValue;
	}

	// implements Tag
	void TablecellCurrency::readAttribute(QStringRef name, QStringRef value)
	{
		if (name == kCellTypeAttribute)
		{
			value_amount_ = value.toFloat();
			return;
		}
		if (name == kCellCurrencyAttribute)
		{
			value_currency_ = Currency(value.toString());
			return;
		}
		Tablecell::readAttribute(name, value);
	}

	void TablecellCurrency::writeAttributes(QXmlStreamWriter* writer)
	{
		Tablecell::writeAttributes(writer);
		writer->writeAttribute(kCellTypeAttribute, QString::number(value_amount_, 'f'));
		writer->writeAttribute(kCellCurrencyAttribute, value_currency_.getIso());
	}

	void TablecellCurrency::writeSubtags(QXmlStreamWriter* writer)
	{
		QString out = "";
		if (value_amount_ < 0) out = "-";
		out += value_currency_.getSymbol();
		if (value_amount_ < 0)
			out += QString::asprintf("%'.2f", -value_amount_);
		else
			out += QString::asprintf("%'.2f", value_amount_);

		out.replace(".", "!");
		out.replace(",", ".");
		out.replace("!", ",");
		writer->writeTextElement(Tablecell::kTextPTag, out);
	}
}
