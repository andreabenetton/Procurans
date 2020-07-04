// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include <QDebug>

#include "inameable.h"

namespace qoasis
{
	QMap<QString, int> INameable::items_type_counter_;

	INameable::INameable()
	{
		const QString default_name = defaultName();

		int count = 1;
		if (!items_type_counter_.contains(default_name))
		{
			items_type_counter_.insert(default_name, count);
		}
		else
		{
			count = items_type_counter_.value(default_name);
			items_type_counter_.insert(default_name, ++count);
		}
		name_ = default_name + QString::number(count);

	}

	INameable::INameable(QString name)
	{
		name_ = name;
	}

	INameable::INameable(const INameable& obj)
	{
		name_ = "Copy of " + obj.name_;
	}

	QString INameable::defaultName()
	{
		return QString("Sheet");
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
		qDebug() << "Name attribute - name:" << nameTag() << " value:" << value.toString();
	}

	void INameable::writeName(QXmlStreamWriter* writer)
	{
		if (name_ != "")
		{
			writer->writeAttribute(nameTag(), name_);
		}
	}
}
