// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLEROW_H
#define TABLEROW_H

#include "../tag.h"
#include "../istyleable.h"
#include "../irepeatable.h"

#include "tablecell.h"

namespace qoasis::table
{
	class Tablerow : public Tag, public IStyleable, public IRepeatable
	{
	public:
		Tablerow(int repeat = 1, QString style = "");
		Tablerow(QXmlStreamReader& reader);
		Tablerow(const Tablerow& obj);

		static QSharedPointer<Tag> builder(QXmlStreamReader& reader);

		static const QLatin1String kTag;

		QSharedPointer<Tablecell> getCell(int index) const;
		int scanForwardForBaseOfRepeatedCells(int index) const;
		int scanBackwardForBaseOfRepeatedCells(int index) const;


		int getLastDefined() const;
		int getLastNonEmpty() const;

		// implements IRepeatable
		QLatin1String repeatTag() override;

		// implements Tag
		QLatin1String instanceTag() override;

	protected:
		// implements Tag
		void readSubtag(QXmlStreamReader& reader) override;
		void writeAttributes(QXmlStreamWriter* writer) override;
		void writeSubtags(QXmlStreamWriter* writer) override;
		void readAttribute(QStringRef name, QStringRef value) override;

	private:
		static const QLatin1String kRepeatAttribute;

		int last_defined_ = 0;
		int last_not_empty_ = 0;
		QVector<QSharedPointer<Tablecell>> cells_ =
			QVector<QSharedPointer<Tablecell>>(256, QSharedPointer<Tablecell>(nullptr));
	};
}
#endif // TABLEROW_H
