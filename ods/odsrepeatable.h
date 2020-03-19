// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef ODSREPEATABLE_H
#define ODSREPEATABLE_H

#include <QObject>

#include "odsserializable.h"

class ODSRepeatable : public ODSSerializable
{
public:
	ODSRepeatable(int repeat = 1);

	virtual QString RepeatTag() = 0;

	int GetRepeat();

protected:
	int _repeat;

	void DeserializeProperty(QStringRef attributename, QStringRef attributevalue);
	void SerializeProperties(QXmlStreamWriter* writer);
};

#endif // ODSREPEATABLE_H