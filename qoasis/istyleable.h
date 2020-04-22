// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef ISTYLEABLE_H
#define ISTYLEABLE_H

#include <QObject>
#include <QXmlStreamWriter>

namespace qoasis {

    class IStyleable
	{
	public:
        IStyleable(QString style = "");
        IStyleable(const IStyleable &obj);

        virtual QLatin1String StyleTag();

		QString GetStyle();
		void SetStyle(QString style);

	protected:
        QString style_;

        void DeserializeProperty(QStringRef attributevalue);
        void SerializeProperties(QXmlStreamWriter* writer);
	};
}
#endif // ISTYLEABLE_H
