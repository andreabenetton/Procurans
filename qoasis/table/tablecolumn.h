// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECOLUMN_H
#define TABLECOLUMN_H

#include "../tag.h"
#include "../istyleable.h"
#include "../irepeatable.h"

namespace qoasis::table
{
	class Tablecolumn : public Tag, public IStyleable, public IRepeatable
	{
	public:
		Tablecolumn(int repeat = 1, QString style = "", QString default_cell_style = "");
		Tablecolumn(QXmlStreamReader& reader);

		static QSharedPointer<Tablecolumn> builder(QXmlStreamReader& reader);
		static QSharedPointer<Tablecolumn> placeholder(int repeat = 1);

		static const QString kTag;
		static const QString kRepeatAttribute;
		static const QString kDefaultCellStyleAttribute;

		QString getDefaultCellStyle() const;
		void setDefaultCellStyle(QString default_cell_style);

		// implements IRepeatable
		QString repeatTag() override;
		bool isEmpty() override;

		// implements Tag
		QString instanceTag() override;

	protected:
		// implements Tag
		void writeAttributes(QXmlStreamWriter* writer) override;
		void readAttribute(QStringRef name, QStringRef value) override;

	private:
		QString default_cell_style_;
	};
}
#endif // TABLECOLUMN_H
