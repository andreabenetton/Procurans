// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef ODSSTYLEABLE_H
#define ODSSTYLEABLE_H

#include <QObject>

#include "SerializableAbstract.h"

namespace Ods {

	class StyleableAbstract : public SerializableAbstract
	{
	public:
		StyleableAbstract(QString style = "");

		virtual QString StyleTag();

		QString GetStyle();
		void SetStyle(QString style);

	protected:
		QString _style;

		void DeserializeProperty(QStringRef attributename, QStringRef attributevalue);
		void SerializeProperties(QXmlStreamWriter* writer);
	};
}
#endif // ODSSTYLEABLE_H
