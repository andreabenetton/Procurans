// Copyright 2019 - 2020, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef ODSSHEET_H
#define ODSSHEET_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QXmlStreamWriter>
#include <QSharedPointer>

#include <odsserializable.h>
#include <odsstyleable.h>

#include <odscolumn.h>
#include <odsrow.h>

class ODSSheet : public ODSSerializable, public ODSStyleable
{
public:
	ODSSheet(QString name = "");
	ODSSheet(QXmlStreamReader& reader);

	static ODSSheet* Builder(QXmlStreamReader& reader);
	static const QString TAG;

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

	QVector<QSharedPointer<ODSColumn>>* _columns;
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

#endif // ODSSHEET_H
