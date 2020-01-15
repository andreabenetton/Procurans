/****************************************************************************
**
**    Procurans - An utility to import Italian electronic bill into odf files
**    Copyright (C) 2019  Andrea Benetton lnki5538@pm.me
**
**    This program is free software: you can redistribute it and/or modify
**    it under the terms of the GNU Affero General Public License as published
**    by the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**    This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU Affero General Public License for more details.
**
**    You should have received a copy of the GNU Affero General Public License
**    along with this program.  If not, see <https://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QAction;
class QMenu;
class QTableView;
class QXmlStreamReader;
QT_END_NAMESPACE

#include "gridschemaitem.h"
#include "settings.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void parseXMLFile(const QString &fileName);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void open();
    void about();
    void execute();

    void toggleElencoFatture();
    void toggleMastriniFornitori();
    void togglePrimaNota();
    void toggleScadenziario();

    void setPathElencoFatture();
    void setPathMastriniFornitori();
    void setPathPrimaNota();
    void setPathScadenziario();

    void saveWindowSettings();

private:
    Ui::MainWindow *ui;

    void createActions();
    void createStatusBar();

    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    bool validateBill(const QByteArray &document);

    bool isStartElementNamed(QXmlStreamReader& xml, const QString &tokenName);
    bool isNotEndElementNamed(QXmlStreamReader& xml, const QString &tokenName);
    void addSubelementsDataToMap(QXmlStreamReader& xml,
                                 const QString &tokenName,
                                 const QStringList &mapping,
                                 QMap<QString, QString> &header);

    QMap<QString, QString> parseHeader(QXmlStreamReader& xml);
    QMap<QString, QString> parseDocument(QXmlStreamReader& xml);
    QMap<QString, QString> parseDetail(QXmlStreamReader& xml);
    QMap<QString, QString> parsePayment(QXmlStreamReader& xml);

    void addElementDataToMap(QXmlStreamReader& xml,
                             QMap<QString, QString>& map) const;

    QList<GridSchemaItem*> createDetailsGridSchema();
    QList<GridSchemaItem*> createPaymentsGridSchema();

    void addHeaderToUI(QMap<QString,QString>& headerData);
    void addDetailsToUI(QList< QMap<QString,QString> >& detailsData, QList<GridSchemaItem*> detailsSchema);
    void addPaymentsToUI(QList< QMap<QString,QString> >& paymentData, QList<GridSchemaItem*> paymentSchema);

    void executeElencoFatture();
    void executeMastriniFornitori();
    void executePrimaNota();
    void executeScadenziario();

    QString curFile;
    QHash<QString, QString> paymentMethodType;
    Settings* m_setting;

    QString months[12] = { "Gennaio",
                           "Febbraio",
                           "Marzo",
                           "Aprile",
                           "Maggio",
                           "Giugno",
                           "Luglio",
                           "Agosto",
                           "Settembre",
                           "Ottobre",
                           "Novembre",
                           "Dicenbre"
                         };

};
#endif // MAINWINDOW_H
