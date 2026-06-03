// Copyright 2020 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef POSITIVEINTEGER_H
#define POSITIVEINTEGER_H

#include <QString>
#include <QXmlStreamReader>

namespace xsd
{
	//!  PositiveInteger class. 
	/*!
	  A more elaborate class description.
	*/
	class PositiveInteger
	{
	public:
		//! A constructor.
		/*!
		  A more elaborate description of the constructor.
		*/
		PositiveInteger(QString lexicalrepresentation);

	protected:
		PositiveInteger(QXmlStreamReader& reader);
	};
}
#endif // POSITIVEINTEGER_H