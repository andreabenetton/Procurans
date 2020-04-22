// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLEROW_H
#define TABLEROW_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QXmlStreamWriter>
#include <QSharedPointer>

#include "../tag.h"
#include "../istyleable.h"
#include "../irepeatable.h"

#include "tablecell.h"

namespace qoasis::table {

    class Tablerow : public Tag, public IStyleable, public IRepeatable
	{
	public:
        Tablerow(int repeat = 1, QString style = "");
        Tablerow(QXmlStreamReader& reader);
        Tablerow(const Tablerow &obj);
        ~Tablerow();

        static QSharedPointer<Tag> Builder(QXmlStreamReader& reader);

        static const QLatin1String kTag;

        QSharedPointer<Tablecell> GetCell(int index);
        int ScanForwardForBaseOfRepeatedCells(int index);
        int ScanBackwardForBaseOfRepeatedCells(int index);


		int GetLastDefined();
		int GetLastNonEmpty();

        // implements IRepeatable
        virtual QLatin1String RepeatTag();

        // implements Tag
        virtual QLatin1String InstanceTag();

    protected:
        // implements Tag
        virtual void ReadSubtag(QXmlStreamReader& reader);
        virtual void WriteAttributes(QXmlStreamWriter* writer);
        virtual void WriteSubtags(QXmlStreamWriter* writer);
        virtual void ReadAttribute(QStringRef attributename, QStringRef attributevalue);

	private:
        static const QLatin1String kRepeatAttribute;

        void Initialize();

        int lastdefined_;
        int lastnonempty_;
        QVector<QSharedPointer<Tablecell>>* cells_;

	};
}
#endif // TABLEROW_H
