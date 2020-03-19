// Copyright 2019 - 2020, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef ODSCOLUMN_H
#define ODSCOLUMN_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include <odsserializable.h>
#include <odsstyleable.h>
#include <odsrepeatable.h>

class ODSColumn : public ODSSerializable, public ODSStyleable, public ODSRepeatable
{
public:
	ODSColumn(int repeat = 1, QString columnstyle = "", QString defaultcellstyle = "");

	static ODSColumn* Builder(QXmlStreamReader& reader);

	static const QString TAG;
	static const QString DEFAULTCELLSTYLETAG;

	int GetLastDefined();
	QString GetDefaultCellStyle();
	void SetDefaultCellStyle(QString defaultstyle);

	// implements ODSRepeatable
	QString RepeatTag();

	// implements ODSSerializable
	void Serialize(QXmlStreamWriter* writer);
	QString InstanceTag();

private:
	ODSColumn(QXmlStreamReader& reader);

	int _lastdefined;
	QString _defaultcellstyle;

	// implements ODSSerializable
	void Deserialize(QXmlStreamReader& reader);
	QVariant DeserializeSubitem(QXmlStreamReader& reader, int& c);
	void SerializeProperties(QXmlStreamWriter* writer);
	void SerializeSubitems(QXmlStreamWriter* writer);
	void DeserializeProperty(QStringRef attributename, QStringRef attributevalue);
};

#endif // ODSCOLUMN_H
