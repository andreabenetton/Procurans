// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef IREPEATABLE_H
#define IREPEATABLE_H

#include <QXmlStreamWriter>

namespace qoasis
{
	class IRepeatable
	{
	public:
		IRepeatable(int repeat = 1);

		virtual QString repeatTag() = 0;
		
		int getRepeat() const;
		void setRepeat(int);
		void offsetRepeatBy(int);

	protected:
		int repeat_;

		void readRepeat(QStringRef value);
		void writeRepeat(QXmlStreamWriter* writer);
	};
}
#endif // IREPEATABLE_H
