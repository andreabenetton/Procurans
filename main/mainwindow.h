// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

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

#include "gridschemafield.h"
#include "ods/odsdocument.h"
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

    bool billLoaded;

    void createActions();
    void createStatusBar();

    QString strippedName(const QString &fullFileName);
    //bool validateBill(const QByteArray &document);

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
    QMap<QString, QString> parseSummary(QXmlStreamReader& xml);

    void addElementDataToMap(QXmlStreamReader& xml,
                             QMap<QString, QString>& map) const;

    QList<GridSchemaField*> createDetailsGridSchema();
    QList<GridSchemaField*> createPaymentsGridSchema();
    QList<GridSchemaField*> createSummaryGridSchema();

    void addHeaderToUI(QMap<QString,QString>& headerData);
    void addDetailsToUI(QList< QMap<QString,QString> >& detailsData, QList<GridSchemaField*> detailsSchema);
    void addPaymentsToUI(QList< QMap<QString,QString> >& paymentData, QList<GridSchemaField*> paymentSchema);
    void addSummaryToUI(QList< QMap<QString,QString> >& summaryData, QList<GridSchemaField*> summarySchema);

    double computeTotal(QList< QMap<QString,QString> >& data, QString xmlfield);

    QString executeElencoFatture();
    QString executeMastriniFornitori();
    QString executePrimaNota();
    QString executeScadenziario();

    QString curFile;
    QHash<QString, QString> paymentMethodType;
    QHash<QString, QString> naturaType;
    QHash<QString, QString> bankAccount;

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
