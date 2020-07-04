// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecellcurrency.h"

namespace qoasis::table
{
	const QString TablecellCurrency::kCellTypeValue = QString("currency");
	const QString TablecellCurrency::kCellTypeAttribute = QString("office:value");
	const QString TablecellCurrency::kCellCurrencyAttribute = QString("office:currency");

	TablecellCurrency::TablecellCurrency(Currency currency, double number, int repeat, QString style) : Tablecell(repeat, style)
	{
		value_amount_ = number;
		value_currency_ = currency;
	}

	TablecellCurrency::TablecellCurrency(Currency currency, double number, QString style) : TablecellCurrency(currency, number, 1 , style) {}

	TablecellCurrency::TablecellCurrency(QXmlStreamReader& reader) 
	{
		Q_ASSERT(reader.qualifiedName() == TablecellCurrency::kTag);
		value_amount_ = 0.0;
		value_currency_ = Currency(Currency::EUR);
		Tag::read(reader);
	}


	double TablecellCurrency::getAmount() const
	{
		return value_amount_;
	}

	Currency TablecellCurrency::getCurrency() const
	{
		return value_currency_;
	}

	QString TablecellCurrency::instanceCellType()
	{
		return kCellTypeValue;
	}

	// implements IRepeatable
	bool TablecellCurrency::isEmpty()
	{
		return false;
	}

	// implements Tag
	void TablecellCurrency::readAttribute(QStringRef name, QStringRef value)
	{
		if (name.toString() == kCellTypeAttribute) {
			value_amount_ = value.toFloat();
			return;
		}
		if (name.toString() == kCellCurrencyAttribute) {
			value_currency_ = Currency(value.toString());
			return;
		}
		Tablecell::readAttribute(name, value);
	}

	void TablecellCurrency::writeAttributes(QXmlStreamWriter* writer)
	{
		writer->writeAttribute(kCellTypeAttribute, QString::number(value_amount_, 'f', 2));
		writer->writeAttribute(kCellCurrencyAttribute, value_currency_.getIso());
		Tablecell::writeAttributes(writer);
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
