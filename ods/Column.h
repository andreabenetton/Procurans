// Copyright 2019 - 2020, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef ODSCOLUMN_H
#define ODSCOLUMN_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "SerializableAbstract.h"
#include "StyleableAbstract.h"
#include "RepeatableAbstract.h"

namespace Ods {

	class Column : public SerializableAbstract, public StyleableAbstract, public RepeatableAbstract
	{
	public:
		Column(int repeat = 1, QString columnstyle = "", QString defaultcellstyle = "");

		static Column* Builder(QXmlStreamReader& reader);

		static const QString kTag;
		static const QString kRepeatAttribute;
		static const QString kDefaultCellStyleAttribute;

		int GetLastDefined();
		QString GetDefaultCellStyle();
		void SetDefaultCellStyle(QString defaultstyle);

		// implements ODSRepeatable
		QString RepeatTag();

		// implements ODSSerializable
		void Serialize(QXmlStreamWriter* writer);
		QString InstanceTag();

	private:
		Column(QXmlStreamReader& reader);

		int _lastdefined;
		QString _defaultcellstyle;

		// implements ODSSerializable
		void Deserialize(QXmlStreamReader& reader);
		QString DeserializeSubitem(QXmlStreamReader& reader, int& c);
		void SerializeProperties(QXmlStreamWriter* writer);
		void SerializeSubitems(QXmlStreamWriter* writer);
		void DeserializeProperty(QStringRef attributename, QStringRef attributevalue);
	};
}
#endif // ODSCOLUMN_H
