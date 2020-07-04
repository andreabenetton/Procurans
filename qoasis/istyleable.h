// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef ISTYLEABLE_H
#define ISTYLEABLE_H

#include <QXmlStreamWriter>

namespace qoasis
{
	class IStyleable
	{
	public:
		IStyleable(QString style = "");
		IStyleable(const IStyleable& obj);

		virtual QString styleTag();

		QString getStyle() const;
		void setStyle(QString style);

	protected:
		QString style_;

		void readStyle(QStringRef value);
		void writeStyle(QXmlStreamWriter* writer);
	};
}
#endif // ISTYLEABLE_H
