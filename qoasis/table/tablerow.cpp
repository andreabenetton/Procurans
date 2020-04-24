// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablerow.h"
#include "tablecell.h"
#include "tablecellempty.h"

namespace qoasis::table
{
	// Constants
	const QLatin1String Tablerow::kTag = QLatin1String("table:table-row");
	const QLatin1String Tablerow::kRepeatAttribute = QLatin1String("table:number-rows-repeated");

	// Constructors
	Tablerow::Tablerow(int repeat, QString style) : Tag(), IStyleable(style), IRepeatable(repeat)
	{
	}

	Tablerow::Tablerow(QXmlStreamReader& reader)
	{
		Q_ASSERT(reader.qualifiedName() == Tablerow::kTag);
		Tag::read(reader);
	}

	Tablerow::Tablerow(const Tablerow& obj) : Tag(), IStyleable(obj), IRepeatable(obj)
	{
		for (int i = 0; i < cells_.length(); i++)
		{
			QSharedPointer<Tablecell> cell = obj.cells_.at(i);
			if (!cell.isNull())
			{
				cells_.replace(i, Tablecell::clone(cell));
			}
		}
		last_defined_ = obj.last_defined_;
		last_not_empty_ = obj.last_not_empty_;
	}

	// Static methods
	QSharedPointer<Tag> Tablerow::builder(QXmlStreamReader& reader)
	{
		Q_ASSERT(reader.qualifiedName() == Tablerow::kTag);
		return QSharedPointer<Tag>(new Tablerow(reader));
	}

	// Methods
	QSharedPointer<Tablecell> Tablerow::getCell(int index) const
	{
		Q_ASSERT(index >= 0);
		if (index > (cells_.length() - 1))
		{
			return QSharedPointer<Tablecell>(nullptr);
		}
		return cells_.at(index);
	}

	int Tablerow::scanBackwardForBaseOfRepeatedCells(int index) const
	{
		return IRepeatable::scanBackwardForNotNull<Tablecell>(index, cells_);
	}

	int Tablerow::scanForwardForBaseOfRepeatedCells(int index) const
	{
		return IRepeatable::scanForwardForNotNull<Tablecell>(index, cells_);
	}

	int Tablerow::getLastDefined() const
	{
		return last_defined_;
	}

	int Tablerow::getLastNonEmpty() const
	{
		return last_not_empty_;
	}

	// implements IRepeatable
	QLatin1String Tablerow::repeatTag()
	{
		return kRepeatAttribute;
	}

	// implements Tag
	QLatin1String Tablerow::instanceTag()
	{
		return Tablerow::kTag;
	}

	void Tablerow::readAttribute(QStringRef name, QStringRef value)
	{
		if (name == repeatTag())
		{
			IRepeatable::readRepeat(value);
			return;
		}
		if (name == styleTag())
		{
			IStyleable::readStyle(value);
			return;
		}
		// Deserialize present but unsupported attributes
		Tag::readAttribute(name, value);
	}

	void Tablerow::readSubtag(QXmlStreamReader& reader)
	{
		if (isStartElementNamed(reader, Tablecell::kTag))
		{
			QSharedPointer<Tablecell> cell = Tablecell::builder(reader);
			cells_[last_defined_] = cell;
			last_defined_ += cell->getRepeat();

			if (cell->instanceCellType() != TablecellEmpty::kCellTypeValue)
			{
				last_not_empty_ = last_defined_;
			}
			return;
		}
		// Deserialize present but unsupported subtags
		Tag::readSubtag(reader);
	}

	void Tablerow::writeAttributes(QXmlStreamWriter* writer)
	{
		IRepeatable::writeRepeat(writer);
		IStyleable::writeStyle(writer);

		// Serialize present but unsupported attributes
		Tag::writeAttributes(writer);
	}

	void Tablerow::writeSubtags(QXmlStreamWriter* writer)
	{
		for (auto& cell : cells_)
		{
			if (!cell.isNull())
			{
				cell->write(writer);
			}
		}
		// Serialize present but unsupported subtags
		Tag::writeSubtags(writer);
	}
}
