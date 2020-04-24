// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecell.h"
#include "tablecellempty.h"
#include "tablecellstring.h"
#include "tablecelldate.h"
#include "tablecellfloat.h"
#include "tablecellcurrency.h"

namespace qoasis::table
{
	// Constants
	const QLatin1String Tablecell::kTag = QLatin1String("table:table-cell");
	const QLatin1String Tablecell::kCellTypeAttribute = QLatin1String("office:value-type");
	const QLatin1String Tablecell::kRepeatAttribute = QLatin1String("table:number-columns-repeated");
	const QLatin1String Tablecell::kTextPTag = QLatin1String("text:p");
	const QLatin1String Tablecell::kCalcextValueType = QLatin1String("calcext:value-type");

	// Constructors
	Tablecell::Tablecell(int repeat, QString style) : IStyleable(style), IRepeatable(repeat)
	{
	}

	Tablecell::Tablecell(QXmlStreamReader& reader) : IStyleable(""), IRepeatable(1)
	{
		Q_ASSERT(reader.qualifiedName() == Tablecell::kTag);
	}

	Tablecell::Tablecell(const Tablecell& obj): IStyleable(obj), IRepeatable(obj)
	{
		_valueText = obj._valueText;
	}

	// Static methods
	QSharedPointer<Tablecell> Tablecell::clone(QSharedPointer<Tablecell> obj)
	{
		Q_ASSERT(obj->instanceTag() == Tablecell::kTag);

		if (obj->instanceCellType() == TablecellString::kCellTypeValue)
		{
			const QSharedPointer<TablecellString> cell = qSharedPointerCast<TablecellString>(obj);
			return QSharedPointer<Tablecell>(new TablecellString(*cell));
		}
		if (obj->instanceCellType() == TablecellDate::kCellTypeValue)
		{
			const QSharedPointer<TablecellDate> cell = qSharedPointerCast<TablecellDate>(obj);
			return QSharedPointer<Tablecell>(new TablecellDate(*cell));
		}
		if (obj->instanceCellType() == TablecellFloat::kCellTypeValue)
		{
			const QSharedPointer<TablecellFloat> cell = qSharedPointerCast<TablecellFloat>(obj);
			return QSharedPointer<Tablecell>(new TablecellFloat(*cell));
		}
		if (obj->instanceCellType() == TablecellCurrency::kCellTypeValue)
		{
			const QSharedPointer<TablecellCurrency> cell = qSharedPointerCast<TablecellCurrency>(obj);
			return QSharedPointer<Tablecell>(new TablecellCurrency(*cell));
		}
		const QSharedPointer<TablecellEmpty> cell = qSharedPointerCast<TablecellEmpty>(obj);
		return QSharedPointer<Tablecell>(new TablecellEmpty(*cell));
	}

	QSharedPointer<Tablecell> Tablecell::builder(QXmlStreamReader& reader)
	{
		Q_ASSERT(reader.qualifiedName() == Tablecell::kTag);

		if (!reader.attributes().hasAttribute(Tablecell::kCellTypeAttribute))
		{
			return QSharedPointer<Tablecell>(new TablecellEmpty(reader));
		}

		QString type = reader.attributes().value(Tablecell::kCellTypeAttribute).toString();
		if (type.isEmpty())
		{
			return QSharedPointer<Tablecell>(new TablecellEmpty(reader));
		}
		if (type == TablecellString::kCellTypeValue)
		{
			return QSharedPointer<Tablecell>(new TablecellString(reader));
		}
		if (type == TablecellDate::kCellTypeValue)
		{
			return QSharedPointer<Tablecell>(new TablecellDate(reader));
		}
		if (type == TablecellFloat::kCellTypeValue)
		{
			return QSharedPointer<Tablecell>(new TablecellFloat(reader));
		}
		if (type == TablecellCurrency::kCellTypeValue)
		{
			return QSharedPointer<Tablecell>(new TablecellCurrency(reader));
		}
		Q_ASSERT(false);
		return {};
	}

	// Methods
	QString Tablecell::getText() const
	{
		return _valueText;
	}

	// implements IRepeatable
	QLatin1String Tablecell::repeatTag()
	{
		return kRepeatAttribute;
	}

	// implements Tag
	QLatin1String Tablecell::instanceTag()
	{
		return kTag;
	}

	void Tablecell::readAttribute(QStringRef name, QStringRef value)
	{
		if (name == styleTag())
		{
			IStyleable::readStyle(value);
			return;
		}
		if (name == repeatTag())
		{
			IRepeatable::readRepeat(value);
			return;
		}
		// Deserialize present but unsupported attributes
		Tag::readAttribute(name, value);
	}

	void Tablecell::readSubtag(QXmlStreamReader& reader)
	{
		if (reader.qualifiedName() == kTextPTag)
		{
			_valueText = reader.readElementText(QXmlStreamReader::IncludeChildElements);
			return;
		}
		// Deserialize present but unsupported subtags
		Tag::readSubtag(reader);
	}

	void Tablecell::writeAttributes(QXmlStreamWriter* writer)
	{
		writer->writeAttribute(Tablecell::kCellTypeAttribute, instanceCellType());
		writer->writeAttribute(Tablecell::kCalcextValueType, instanceCellType());
		// Serialize present but unsupported attributes
		Tag::writeAttributes(writer);
	}

	void Tablecell::writeSubtags(QXmlStreamWriter* writer)
	{
		if (!(_valueText.isNull() || _valueText.isEmpty()))
		{
			writer->writeTextElement(Tablecell::kTextPTag, _valueText);
		}
		// Serialize present but unsupported subtags
		Tag::writeSubtags(writer);
	}
}
