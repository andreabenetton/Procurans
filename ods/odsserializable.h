// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef ODSSERIALIZABLE_H
#define ODSSERIALIZABLE_H

#include <QObject>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QVariant>

class ODSSerializable
{
public:
	ODSSerializable();
	virtual void Serialize(QXmlStreamWriter* writer) = 0;
	virtual QString InstanceTag() = 0;

protected:
	ODSSerializable(QXmlStreamReader& reader);

	virtual void Deserialize(QXmlStreamReader& reader) = 0;

	void LoopForProperties(QXmlStreamReader& reader, int& numberofloopeditems);
	virtual void DeserializeProperty(QStringRef attributename, QStringRef attributevalue) = 0;
	void LoopForSubitems(QXmlStreamReader& reader, int& numberofloopeditems);
	virtual QString DeserializeSubitem(QXmlStreamReader& reader, int& numberofdeserializeitems) = 0;

	void SerializeStart(QXmlStreamWriter* writer);
	virtual void SerializeProperties(QXmlStreamWriter* writer) = 0;
	virtual void SerializeSubitems(QXmlStreamWriter* writer) = 0;
	void SerializeEnd(QXmlStreamWriter* writer);
};

#endif // ODSSERIALIZABLE_H
