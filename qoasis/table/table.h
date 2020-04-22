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

namespace qoasis::table {

    class Table : public Tag, public IStyleable, public INameable
	{
	public:
        Table(QString name = "");
        Table(QXmlStreamReader& reader);
        Table(const Table &obj);
        ~Table();

        static QSharedPointer<Tag> Builder(QXmlStreamReader& reader);
        static const QLatin1String kTag;

        QString GetPrintRange();
        void SetPrintRange(QString name);

        QSharedPointer<Tablecolumn> GetColumn(int index);
        QSharedPointer<Tablerow> GetRow(int index);

        int ScanForwardForBaseOfRepeatedRows(int index);
        int ScanBackwardForBaseOfRepeatedRows(int index);
        int ScanForwardForBaseOfRepeatedColumns(int index);
        int ScanBackwardForBaseOfRepeatedColumns(int index);

        void AddRow(QSharedPointer<Tablerow> row);

        // implements INameable
        virtual QLatin1String NameTag();

        // implements Tag
        virtual QLatin1String InstanceTag();

    protected:
        // implements Tag
        virtual void Read(QXmlStreamReader& reader);
        virtual void ReadSubtag(QXmlStreamReader& reader);
        virtual void WriteAttributes(QXmlStreamWriter* writer);
        virtual void WriteSubtags(QXmlStreamWriter* writer);
        virtual void ReadAttribute(QStringRef attributename, QStringRef attributevalue);

	private:
        void Initialize();

        static const QLatin1String kNameAttribute;
        static const QLatin1String kPrintRangeAttribute;

        QString printranges_;
        QVector<QSharedPointer<Tablecolumn>>* columns_;
        int processedcolumns_ = 0;
        QVector<QSharedPointer<Tablerow>>* rows_;
        int processedrows_ = 0;
	};
}
#endif // TABLE_H
