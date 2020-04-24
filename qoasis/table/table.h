// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLE_H
#define TABLE_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QXmlStreamWriter>
#include <QSharedPointer>

#include "../tag.h"
#include "../istyleable.h"
#include "../inameable.h"

#include "tablecolumn.h"
#include "tablerow.h"

namespace qoasis::table
{
	class Table : public Tag, public IStyleable, public INameable
	{
	public:
		Table(QString name = "", QString style = "");
		Table(QXmlStreamReader& reader);
		Table(const Table& obj);

		static QSharedPointer<Tag> Builder(QXmlStreamReader& reader);
		static const QLatin1String kTag;

		QString getPrintRange();
		void setPrintRange(QString name);

		QSharedPointer<Tablecolumn> GetColumn(int index);
		QSharedPointer<Tablerow> GetRow(int index);

		auto scanForwardForBaseOfRepeatedRows(int index) const -> int;
		auto scanBackwardForBaseOfRepeatedRows(int index) const -> int;
		int scanForwardForBaseOfRepeatedColumns(int index);
		int scanBackwardForBaseOfRepeatedColumns(int index);

		void AddRow(QSharedPointer<Tablerow> row);

		// implements INameable
		virtual QLatin1String nameTag();

		// implements Tag
		virtual QLatin1String instanceTag();

	protected:
		// implements Tag
		virtual void read(QXmlStreamReader& reader);
		virtual void readSubtag(QXmlStreamReader& reader);
		virtual void writeAttributes(QXmlStreamWriter* writer);
		virtual void writeSubtags(QXmlStreamWriter* writer);
		virtual void readAttribute(QStringRef attributename, QStringRef attributevalue);

	private:
		static const QLatin1String kNameAttribute;
		static const QLatin1String kPrintRangeAttribute;

		QString printranges_ = "";
		QVector<QSharedPointer<Tablecolumn>> columns_ = QVector<QSharedPointer<Tablecolumn>>(
			256, QSharedPointer<Tablecolumn>(nullptr));
		int processedcolumns_ = 0;
		QVector<QSharedPointer<Tablerow>> rows_ = QVector<QSharedPointer<Tablerow>>(
			256, QSharedPointer<Tablerow>(nullptr));
		int processedrows_ = 0;
	};
}
#endif // TABLE_H
