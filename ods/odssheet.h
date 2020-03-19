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

	static const QString TAG;
	QString InstanceTag();

	QString GetName();
	void SetName(QString name);

private:
	void DeserializeProperty(QXmlStreamReader& reader);

	void WriteStartAttribute(QXmlStreamWriter* writer);

	QString _name;
	QString _printranges;

	QVector<ODSColumn>* _columns;
	QVector<ODSRow>* _rows;

	static int _sheetcount;
};

#endif // ODSSHEET_H
