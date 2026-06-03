// Copyright 2020 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef P_H
#define P_H

#include "../tag.h"

namespace qoasis::table
{
	// The <text:p> element represents a paragraph, which is the basic unit of text in an OpenDocument file.
	// https://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#__RefHeading__1415138_253892949
	// 
	class P : public Tag
	{
	public:
		static const QLatin1String kTag;

	protected:
		P(QXmlStreamReader& reader);
	};
}
#endif // TABLECELL_H