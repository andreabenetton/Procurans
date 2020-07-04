// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLEROW_H
#define TABLEROW_H

#include "../repeatvector.h"
#include "../tag.h"
#include "../istyleable.h"
#include "../irepeatable.h"

#include "tablecell.h"

namespace qoasis::table
{
	class Tablerow : public Tag, public IStyleable, public IRepeatable
	{
	public:
		Tablerow(int repeat = 1, QString style = "", int array_size = 0, QSharedPointer<Tablecell> cells[] = nullptr);
		Tablerow(QXmlStreamReader& reader);

		static QSharedPointer<Tablerow> builder(QXmlStreamReader& reader);
		static QSharedPointer<Tablerow> placeholder(int repeat = 1);

		static const QString kTag;

		QSharedPointer<Tablecell> getCell(int index) const;

		void removeCell(int index);
		void insertCell(int index, QSharedPointer<Tablecell> cell);
		void appendCell(QSharedPointer<Tablecell> cell);
		void replaceCell(int index, QSharedPointer<Tablecell> cell);

		void replaceCells(int start, int end, QSharedPointer<Tablecell> cells[]);

		void restyleFromRow(QSharedPointer<Tablerow> fromRow);

		int getLastNonEmptyCell() const;

		// implements IRepeatable
		QString repeatTag() override;

		// implements Tag
		QString instanceTag() override;
		bool isEmpty() override;

	protected:
		// implements Tag
		void readSubtag(QXmlStreamReader& reader) override;
		void writeAttributes(QXmlStreamWriter* writer) override;
		void writeSubtags(QXmlStreamWriter* writer) override;
		void readAttribute(QStringRef name, QStringRef value) override;

	private:
		static const QString kRepeatAttribute;

		RepeatVector<Tablecell> cells_;
	};
}
#endif // TABLEROW_H
