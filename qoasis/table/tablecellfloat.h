// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECELLFLOAT_H
#define TABLECELLFLOAT_H

#include "tablecell.h"

namespace qoasis::table {

    class TablecellFloat : public Tablecell
    {
    public:
        TablecellFloat(double number, int repeat = 1);
        TablecellFloat(QXmlStreamReader& reader);
        TablecellFloat(const TablecellFloat &obj);

        static const QLatin1String kCellTypeValue;

        double getDouble() const;

        // implements Tablecell
        QLatin1String instanceCellType() override;

    protected:
        // implements Tag
        void readAttribute(QStringRef name, QStringRef value) override;
        void writeAttributes(QXmlStreamWriter* writer) override;
        void writeSubtags(QXmlStreamWriter* writer) override;

    private:
        static const QLatin1String kCellTypeAttribute;

        double value_number_;
    };
}
#endif // TABLECELLFLOAT_H
