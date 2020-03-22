// Copyright 2019 - 2020, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef ODSSHEET_H
#define ODSSHEET_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QXmlStreamWriter>
#include <QSharedPointer>

#include <SerializableAbstract.h>
#include <StyleableAbstract.h>

#include <Column.h>
#include <Row.h>

namespace Ods {

	class Table : public SerializableAbstract, public StyleableAbstract
	{
	public:
		Table(QString name = "");
		Table(QXmlStreamReader& reader);

		static Table* Builder(QXmlStreamReader& reader);
		static const QString kTag;

		QString GetName();
		void SetName(QString name);

		QString GetPrintRange();
		void SetPrintRange(QString name);

		// implements ODSSerializable
		void Serialize(QXmlStreamWriter* writer);
		QString InstanceTag();

	private:
		static const QString NAMETAG;
		static const QString PRINTRANGETAG;

		QString _name;
		QString _printranges;

		QVector<QSharedPointer<Column>>* _columns;
		QVector<QSharedPointer<ODSRow>>* _rows;

		static int _sheetcount;

		void Initialize();

		// implements ODSSerializable
		void Deserialize(QXmlStreamReader& reader);
		QString DeserializeSubitem(QXmlStreamReader& reader, int& c);
		void SerializeProperties(QXmlStreamWriter* writer);
		void SerializeSubitems(QXmlStreamWriter* writer);
		void DeserializeProperty(QStringRef attributename, QStringRef attributevalue);
	};
}
#endif // ODSSHEET_H
