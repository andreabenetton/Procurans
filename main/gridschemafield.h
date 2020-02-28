// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef GRIDSCHEMA_H
#define GRIDSCHEMA_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QStandardItem;
QT_END_NAMESPACE

enum GridSchemaFieldType {
  RelatedColumn,
  LStringColumn,
  CStringColumn,
  RStringColumn,
  FloatColumn,
  DateColumn
};

class GridSchemaField
{
public:
    GridSchemaField(const QString columnName, const QString elementName = "", const GridSchemaFieldType type = GridSchemaFieldType::LStringColumn, int truncAt = 0, bool ignoreIfEmpty = false);
    GridSchemaField(const QString columnName, const QString elementName, QHash<QString, QString> *translationtable);
    QString getElementName();
    QString getColumnName();
    GridSchemaFieldType getType();
    int getTruncAt();
    bool getToBeIgnored() const;
    void resetToBeIgnored();
    QStandardItem* createGridItem(const QString &value);

private:
    QString elementName;
    QString columnName;
    GridSchemaFieldType columnType;
    int truncAt;
    bool ignoreIfEmpty;
    bool toBeIgnored;
    QHash<QString, QString> *translationTable;
};

#endif // GRIDSCHEMA_H
