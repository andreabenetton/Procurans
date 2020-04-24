// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "istyleable.h"

namespace qoasis
{
	IStyleable::IStyleable(QString style)
	{
		style_ = style;
	}

	IStyleable::IStyleable(const IStyleable& obj)
	{
		style_ = obj.style_;
	}

	QString IStyleable::getStyle() const
	{
		return style_;
	}

	void IStyleable::setStyle(const QString style)
	{
		style_ = style;
	}

	QLatin1String IStyleable::styleTag()
	{
		return QLatin1String("table:style-name");
	}

	void IStyleable::readStyle(QStringRef value)
	{
		style_ = value.toString();
	}

	void IStyleable::writeStyle(QXmlStreamWriter* writer)
	{
		if (style_ != "")
		{
			writer->writeAttribute(styleTag(), style_);
		}
	}
}
