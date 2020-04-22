// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECOLUMN_H
#define TABLECOLUMN_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "../tag.h"
#include "../istyleable.h"
#include "../irepeatable.h"

namespace qoasis::table {

    class Tablecolumn : public Tag, public IStyleable, public IRepeatable
	{
	public:
        Tablecolumn(int repeat = 1, QString columnstyle = "", QString defaultcellstyle = "");
        Tablecolumn(QXmlStreamReader& reader);
        Tablecolumn(const Tablecolumn &obj);

        static QSharedPointer<Tablecolumn> Builder(QXmlStreamReader& reader);

        static const QLatin1String kTag;
        static const QLatin1String kRepeatAttribute;
        static const QLatin1String kDefaultCellStyleAttribute;

		int GetLastDefined();
		QString GetDefaultCellStyle();
		void SetDefaultCellStyle(QString defaultstyle);

        // implements IRepeatable
        virtual QLatin1String RepeatTag();

        // implements Tag
        virtual QLatin1String InstanceTag();

    protected:
        // implements Tag
        virtual void WriteAttributes(QXmlStreamWriter* writer);
        virtual void ReadAttribute(QStringRef attributename, QStringRef attributevalue);

	private:
        int lastdefined_;
        QString defaultcellstyle_;

	};
}
#endif // TABLECOLUMN_H
