#include "gridschemaitem.h"

#include <QtWidgets>

GridSchemaItem::GridSchemaItem(const QString columnName, const QString elementName, const ColumnType type, int truncAt, bool ignoreIfEmpty)
{
    this->translationTable = nullptr;
    this->elementName = elementName;
    this->columnName = columnName;
    this->columnType = type;
    this->truncAt = truncAt;
    this->ignoreIfEmpty = ignoreIfEmpty;
    this->toBeIgnored = false;
}

GridSchemaItem::GridSchemaItem(const QString columnName, const QString elementName, QHash<QString, QString> *translationtable)
{
    this->translationTable = translationtable;
    this->elementName = elementName;
    this->columnName = columnName;
    this->columnType = RelatedColumn;
    this->truncAt = 0;
    this->ignoreIfEmpty = false;
    this->toBeIgnored = false;
}

QString GridSchemaItem::getElementName()
{
    return elementName;
}

QString GridSchemaItem::getColumnName()
{
    return columnName;
}

ColumnType GridSchemaItem::getType()
{
    return columnType;
}

int GridSchemaItem::getTruncAt()
{
    return truncAt;
}

bool GridSchemaItem::getToBeIgnored() const
{
    return toBeIgnored;
}
void GridSchemaItem::resetToBeIgnored()
{
    toBeIgnored = false;
}

QStandardItem* GridSchemaItem::createGridItem(const QString &value)
{
    ColumnType ct = this->getType();
    int trunc = this->getTruncAt();
    QStandardItem* item = nullptr;

    switch( ct ) {
    case RelatedColumn: {
            QString touse = translationTable->value(value);
            item = new QStandardItem(touse);
        }
        break;

    case LStringColumn:
    case CStringColumn:
    case RStringColumn: {
            QString touse = value;
            if (trunc>0) {
                touse = value.left(trunc);
                //touse.append("...");
            }
            toBeIgnored = (ignoreIfEmpty)&&(touse=="");
            item = new QStandardItem(touse);      
        }
        if(ct==CStringColumn)
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        if(ct==RStringColumn)
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
        break;

    case FloatColumn: {
            bool flag;
            double num = value.toDouble(&flag);
            QString touse ;
            if (!flag)
                touse = "";
            else
                touse = QString::number(num, 'f', trunc);
            toBeIgnored = (ignoreIfEmpty)&&((!flag)||(num==0.0));
            item = new QStandardItem(touse);
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
        }
        break;
    case DateColumn: {
            QDate date = QDate::fromString(value.split(" ")[0],"yyyy-MM-dd");
            QString touse ;
            if (!date.isValid())
                touse = "###";
            else
                touse = date.toString("dd/MM/yyyy");
            toBeIgnored = (ignoreIfEmpty)&&(!date.isValid());
            item = new QStandardItem(touse);
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        }
        break;
    default :
       item = new QStandardItem(value);
    }
    return item;
}

