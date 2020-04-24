// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "inameable.h"

namespace qoasis
{
	QMap<QLatin1String, int> INameable::items_type_counter_;

	INameable::INameable(QString name)
	{
		const QLatin1String default_name = defaultName();

		int count = 1;
		if (!items_type_counter_.contains(default_name))
		{
			items_type_counter_.insert(default_name, count);
		}
		else
		{
			count = items_type_counter_.value(default_name);
		}

		if (name != "")
		{
			name_ = name;
		}
		else
		{
			name_ = default_name + QString::number(count);
			items_type_counter_.insert(default_name, ++count);
		}
	}

	INameable::INameable(const INameable& obj)
	{
		name_ = "Copy of " + obj.name_;
	}

	QLatin1String INameable::defaultName()
	{
		return nameTag();
	}

	QString INameable::getName() const
	{
		return name_;
	}

	void INameable::setName(QString name)
	{
		name_ = name;
	}

	void INameable::readName(QStringRef value)
	{
		name_ = value.toString();
	}

	void INameable::writeName(QXmlStreamWriter* writer)
	{
		if (name_ != "")
		{
			writer->writeAttribute(nameTag(), name_);
		}
	}
}
