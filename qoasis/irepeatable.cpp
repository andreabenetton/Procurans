// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include <QDebug>

#include "irepeatable.h"

namespace qoasis
{
	// Constructors
	IRepeatable::IRepeatable(int repeat)
	{
		repeat_ = repeat;
	}

	// Methods
	int IRepeatable::getRepeat() const
	{
		return repeat_;
	}

	void IRepeatable::setRepeat(int repeat)
	{
		Q_ASSERT(repeat >= 0);
		repeat_ = repeat;
	}
	
	void IRepeatable::offsetRepeatBy(int offset)
	{
		repeat_ = repeat_ + offset >= 1? repeat_ + offset : 1;
	}

	void IRepeatable::readRepeat(const QStringRef value)
	{
		repeat_ = value.toInt(); 
		qDebug() << "Repeat attribute - name:" << repeatTag() << " value:" << value.toString();
	}

	void IRepeatable::writeRepeat(QXmlStreamWriter* writer)
	{
		if (repeat_ > 1)
		{
			writer->writeAttribute(repeatTag(), QString::number(repeat_));
		}
	}
}
