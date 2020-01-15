#ifndef GRIDSCHEMA_H
#define GRIDSCHEMA_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QStandardItem;
QT_END_NAMESPACE

enum ColumnType {
  RelatedColumn,
  LStringColumn,
  CStringColumn,
  RStringColumn,
  FloatColumn,
  DateColumn
};

class GridSchemaItem
{
public:
    GridSchemaItem(const QString columnName, const QString elementName = "", const ColumnType type = ColumnType::LStringColumn, int truncAt = 0, bool ignoreIfEmpty = false);
    GridSchemaItem(const QString columnName, const QString elementName, QHash<QString, QString> *translationtable);
    QString getElementName();
    QString getColumnName();
    ColumnType getType();
    int getTruncAt();
    bool getToBeIgnored() const;
    void resetToBeIgnored();
    QStandardItem* createGridItem(const QString &value);

private:
    QString elementName;
    QString columnName;
    ColumnType columnType;
    int truncAt;
    bool ignoreIfEmpty;
    bool toBeIgnored;
    QHash<QString, QString> *translationTable;
};

#endif // GRIDSCHEMA_H
