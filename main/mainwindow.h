// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QAction;
class QMenu;
class QTableView;
QT_END_NAMESPACE

#include "gridschemafield.h"
#include "qfatturapa/invoicetypes.h"
#include "qoasis/fileods.h"
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

public slots:

    void UpdateSummaryUI(const QModelIndex& topLeft, const QModelIndex& bottomRight);

private slots:
    void open();
    void about();
    void execute();

    void toggleElencoFatture();
    void toggleMastriniFornitori();
    void togglePrimaNota();
    void toggleScadenziario();
    void toggleBackupFile();

    void setPathElencoFatture();
    void setPathMastriniFornitori();
    void setPathPrimaNota();
    void setPathScadenziario();
    void setPathFattureElettroniche();

    void saveWindowSettings();

private:
    Ui::MainWindow *ui;

    QAction* openAct;
    QAction* exitAct;
    QAction* executeAct;
    QAction* elencoFattureAct;
    QAction* mastrinoFornitoriAct;
    QAction* primaNotaAct;
    QAction* scadenziarioAct;
    QAction* backupAct;
    QAction* pathFattureElettronicheAct;
    QAction* pathElencoFattureAct;
    QAction* pathMastrinoFornitoriAct;
    QAction* pathPrimaNotaAct;
    QAction* pathScadenziarioAct;
    QAction* pathSaveSettingsAct;
    QAction* aboutAct;
    QAction* aboutQtAct;

    bool billLoaded;

    void createActions();
    void setupMenuBar();
    void createStatusBar();

    QString strippedName(const QString &fullFileName);

    QList<GridSchemaField*> createDetailsGridSchema();
    QList<GridSchemaField*> createPaymentsGridSchema();
    QList<GridSchemaField*> createSummaryGridSchema();

    void onDetailsChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);
    void onPaymentsChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);
    void onSummaryChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);

    void LogOnModelChanged(QAbstractItemModel* detailsModel, const QModelIndex& topLeft, const QModelIndex& bottomRight);

    void addHeaderToUI(const qfatturapa::InvoiceHeader& headerData);
    void addDetailsToUI(const QList< QMap<QString,QString> >& detailsData, QList<GridSchemaField*> detailsSchema);
    void addPaymentsToUI(const QList< QMap<QString,QString> >& paymentData, QList<GridSchemaField*> paymentSchema);
    void addSummaryToUI(const QList< QMap<QString,QString> >& summaryData, QList<GridSchemaField*> summarySchema);

    double computeTotal(const QList< QMap<QString,QString> >& data, QString xmlfield);

    QString executeElencoFatture();
    QString executeMastriniFornitori();
    QString executePrimaNota();
    QString executeScadenziario();

    QString curFile;

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
                           "Dicembre"
                         };

};
#endif // MAINWINDOW_H
