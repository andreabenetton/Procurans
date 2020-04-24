// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECOLUMN_H
#define TABLECOLUMN_H

#include "../tag.h"
#include "../istyleable.h"
#include "../irepeatable.h"

namespace qoasis::table {

    class Tablecolumn : public Tag, public IStyleable, public IRepeatable
	{
	public:
        Tablecolumn(int repeat = 1, QString style = "", QString default_cell_style = "");
        Tablecolumn(QXmlStreamReader& reader);
        Tablecolumn(const Tablecolumn &obj);

        static QSharedPointer<Tablecolumn> builder(QXmlStreamReader& reader);

        static const QLatin1String kTag;
        static const QLatin1String kRepeatAttribute;
        static const QLatin1String kDefaultCellStyleAttribute;

		int getLastDefined() const;
		QString getDefaultCellStyle() const;
		void setDefaultCellStyle(QString default_cell_style);

        // implements IRepeatable
        QLatin1String repeatTag() override;

        // implements Tag
        QLatin1String instanceTag() override;

    protected:
        // implements Tag
        void writeAttributes(QXmlStreamWriter* writer) override;
        void readAttribute(QStringRef name, QStringRef value) override;

	private:
        int last_defined_{};
        QString default_cell_style_;

	};
}
#endif // TABLECOLUMN_H
