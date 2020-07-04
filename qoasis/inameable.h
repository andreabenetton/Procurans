// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef INAMEABLE_H
#define INAMEABLE_H

#include <QXmlStreamWriter>
#include <QMap>

namespace qoasis
{
	class INameable
	{
	public:
		INameable();
		INameable(QString name);
		INameable(const INameable& obj);

		virtual QString nameTag() = 0;
		virtual QString defaultName();

		QString getName() const;
		void setName(QString name);

	protected:
		QString name_;

		void readName(QStringRef value);
		void writeName(QXmlStreamWriter* writer);

	private:
		static QMap<QString, int> items_type_counter_;
	};
};
#endif // INAMEABLE_H
