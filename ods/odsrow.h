// Copyright 2019 - 2020, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef ODSROW_H
#define ODSROW_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QXmlStreamWriter>
#include <QSharedPointer>

#include <odsserializable.h>
#include <odsstyleable.h>
#include <odsrepeatable.h>

#include <odscell.h>

class ODSRow : public ODSSerializable, public ODSStyleable, public ODSRepeatable
{
public:
	ODSRow(int repeat = 1, QString style = "");

	static ODSRow* Builder(QXmlStreamReader& reader);

	static const QString TAG;

	int GetLastDefined();
	int GetLastNonEmpty();

	// implements ODSRepeatable
	QString RepeatTag();

	// implements ODSSerializable
	void Serialize(QXmlStreamWriter* writer);
	QString InstanceTag();

private:
	ODSRow(QXmlStreamReader& reader);

	static const QString REPEATTAG;

	void InitializeContainers();

	int _lastdefined;
	int _lastnonempty;
	QVector<QSharedPointer<ODSCell>>* _row;

	// implements ODSSerializable
	void Deserialize(QXmlStreamReader& reader);
	QString DeserializeSubitem(QXmlStreamReader& reader, int& c);
	void SerializeProperties(QXmlStreamWriter* writer);
	void SerializeSubitems(QXmlStreamWriter* writer);
	void DeserializeProperty(QStringRef attributename, QStringRef attributevalue);
};

#endif // ODSROW_H
