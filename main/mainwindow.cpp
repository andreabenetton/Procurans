// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include <QtWidgets>
#include <QtXmlPatterns>
#include <QTableView>
#include <QDir>
#include <QDebug>
#include <QStringList>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logger.h"
#include "settings.h"
#include "comboboxitemdelegate.h"
#include "gridschemafield.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    billLoaded = false;

    m_setting = &Settings::getInstance();

    paymentMethodType["MP01"] = "contanti";
    paymentMethodType["MP02"] = "assegno";
    paymentMethodType["MP03"] = "assegno circolare";
    paymentMethodType["MP04"] = "contanti presso Tesoreria";
    paymentMethodType["MP05"] = "bonifico";
    paymentMethodType["MP06"] = "vaglia cambiario";
    paymentMethodType["MP07"] = "bollettino bancario";
    paymentMethodType["MP08"] = "carta di pagamento";
    paymentMethodType["MP09"] = "RID";
    paymentMethodType["MP10"] = "RID utenze";
    paymentMethodType["MP11"] = "RID veloce";
    paymentMethodType["MP12"] = "RIBA";
    paymentMethodType["MP13"] = "MAV";
    paymentMethodType["MP14"] = "quietanza erario";
    paymentMethodType["MP15"] = "giroconto su conti di contabilità speciale";
    paymentMethodType["MP16"] = "domiciliazione bancaria";
    paymentMethodType["MP17"] = "domiciliazione postale";
    paymentMethodType["MP18"] = "bollettino di c/c postale";
    paymentMethodType["MP19"] = "SEPA Direct Debit";
    paymentMethodType["MP20"] = "SEPA Direct Debit CORE";
    paymentMethodType["MP21"] = "SEPA Direct Debit B2B";
    paymentMethodType["MP22"] = "Trattenuta su somme già riscosse";
    paymentMethodType["MP23"] = "PagoPA";

    naturaType["N1"] = "Escluse ex. art. 15";
    naturaType["N2"] = "Non soggette";
    naturaType["N3"] = "Non Imponibili";
    naturaType["N3.1"] = "Non Imponibili - esportazioni";
    naturaType["N3.2"] = "Non Imponibili - cessioni intracomunitarie";
    naturaType["N3.3"] = "Non Imponibili - cessioni verso San Marino";
    naturaType["N3.4"] = "Non Imponibili - operazioni assimilate alle cessioni all'esportazione";
    naturaType["N3.5"] = "Non Imponibili - a seguito di dichiarazioni d'intento";
    naturaType["N3.6"] = "Non Imponibili - altre operazioni che non concorrono alla formazione del plafond";
    naturaType["N4"] = "Esenti";
    naturaType["N5"] = "Regime del margine";
    naturaType["N6"] = "Inversione contabile";
    naturaType["N6.1"] = "Inversione contabile - cessione di rottami e altri materiali di recupero";
    naturaType["N6.2"] = "Inversione contabile - cessione di oro e argento puro";
    naturaType["N6.3"] = "Inversione contabile - subappalto nel settore edile";
    naturaType["N6.4"] = "Inversione contabile - cessione di fabbricati";
    naturaType["N6.5"] = "Inversione contabile - cessione di telefoni cellulari";
    naturaType["N6.6"] = "Inversione contabile - cessione di prodotti elettronici";
    naturaType["N6.7"] = "Inversione contabile - prestazioni comparto edile e settori connessi";
    naturaType["N6.8"] = "Inversione contabile - operazioni settore energetico";
    naturaType["N6.9"] = "Inversione contabile - altri casi";
    naturaType["N7"] = "IVA assolta in altro stato UE";

    bankAccount["IT64U0503451861000000001728"] = "BPM";
    bankAccount["IT23P0503451861000000001817"] = "BPM Fotovoltaico";
    bankAccount["IT65Z0306951030615272528476"] = "Intesa";
    bankAccount["IT33U0843051030000000180277"] = "CRAC";

    ui->setupUi(this);

    createActions();
    createStatusBar();

    m_setting->load();
    m_setting->restore(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_setting->save();
    event->accept();
}

void MainWindow::open()
{  
    QString fileName = QFileDialog::getOpenFileName(
          this,
          "Apri fattura",
          m_setting->getPath(Settings::Execute::fattureelettroniche),
          "Fattura in XML (*.xml)"
    );

    if (!fileName.isEmpty())
        parseXMLFile(fileName);
    qInfo(logInfo())  << "File selected via dialog: " << fileName;
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Procurans"),
        tr("<p>An utility to import Italian electronic bill into odf files</p>"
           "<p>Copyright © 2019  Andrea Benetton lnki5538@pm.me</p>"
           "<p>This program is free software: you can redistribute it and/or "
           "modify it under the terms of the GNU Affero General Public License "
           "as published by the Free Software Foundation, either version 3 "
           "of the License, or (at your option) any later version.</p>"
           "<p>This program is distributed in the hope that it will be useful, "
           "but WITHOUT ANY WARRANTY; without even the implied warranty "
           "of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.</p>"
           "<p>See the GNU Affero General Public License for more details.</p>"
           "<p>You should have received a copy of the license along with this "
           "program. If not, see https://www.gnu.org/licenses/.</p>"
           "<p>It use parts from ODS2 library Copyright © 2014 f35f22fan@gmail.com</p>"

           ));
}

void MainWindow::toggleElencoFatture()
{
    m_setting->toggleExecute(Settings::Execute::elencofatture);
}

void MainWindow::toggleMastriniFornitori()
{
    m_setting->toggleExecute(Settings::Execute::mastrinifornitori);
}

void MainWindow::togglePrimaNota()
{
    m_setting->toggleExecute(Settings::Execute::primanota);
}

void MainWindow::toggleScadenziario()
{
    m_setting->toggleExecute(Settings::Execute::scadenziario);
}

void MainWindow::toggleBackupFile()
{
    m_setting->toggleExecute(Settings::Execute::backupfiles);
}

void MainWindow::saveWindowSettings()
{
    m_setting->backup(this);
    statusBar()->showMessage(tr("Impostazioni della finestra dell'applicazione salvati"), 2000);
}

void MainWindow::setPathElencoFatture()
{
    m_setting->setPath(this, Settings::Execute::elencofatture);
    statusBar()->showMessage(tr("Posizione della Cartella Elenco Fatture salvata"), 2000);
}

void MainWindow::setPathMastriniFornitori()
{
    m_setting->setPath(this, Settings::Execute::mastrinifornitori);
    statusBar()->showMessage(tr("Posizione della Cartella Mastrini Fornitori salvata"), 2000);
}

void MainWindow::setPathPrimaNota()
{
    m_setting->setPath(this, Settings::Execute::primanota);
    statusBar()->showMessage(tr("Posizione della Cartella Prima Nota salvata"), 2000);
}

void MainWindow::setPathScadenziario()
{
    m_setting->setPath(this, Settings::Execute::scadenziario);
    statusBar()->showMessage(tr("Posizione della Cartella Scadenziario salvata"), 2000);
}

void MainWindow::setPathFattureElettroniche()
{
    m_setting->setPath(this, Settings::Execute::fattureelettroniche);
    statusBar()->showMessage(tr("Posizione della Cartella Fatture Elettroniche salvata"), 2000);
}

void MainWindow::execute()
{
    QString result;

    QDateEdit* qde = this->findChild<QDateEdit*>("dataemissioneEdit");
    if (qde->date().isNull() || !(qde->date().isValid())) {
        result = "<p>Errore data di emissione non valida</p>";
    }
    else {

        if (m_setting->isExecute(Settings::Execute::elencofatture))
        {
            result.append("<p>");
            result.append(executeElencoFatture());
            result.append("</p>");
        }

        if (m_setting->isExecute(Settings::Execute::mastrinifornitori))
        {
            result.append("<p>");
            result.append(executeMastriniFornitori());
            result.append("</p>");
        }

        if (m_setting->isExecute(Settings::Execute::primanota))
        {
            result.append("<p>");
            result.append(executePrimaNota());
            result.append("</p>");
        }

        if (m_setting->isExecute(Settings::Execute::scadenziario))
        {
            result.append("<p>");
            result.append(executeScadenziario());
            result.append("</p>");
        }

        QFile::rename(curFile, curFile + "_importato");
    }

    QMessageBox::information(this, tr("Esito dell'operazione"), result);
}

QString MainWindow::executeElencoFatture()
{
    QString filenametemplate = "Fatt. %1 %2%3.ods";
    QString filename;

    QDateEdit* qde = this->findChild<QDateEdit*>("dataemissioneEdit");

    QString entity = this->findChild<QLineEdit*>("entitaEdit")->text();
    QString numero = this->findChild<QLineEdit*>("numeroEdit")->text();

    QTableView *grid = ui->summaryTable;

    QList<QList<QSharedPointer<ODSCell>>> rows;

    int crow = 0;

    for (crow = 0; crow < grid->model()->rowCount(); crow++) {

        QList<QSharedPointer<ODSCell>> columns;

        {
            QSharedPointer<ODSCell> pt(new ODSCellString(entity));
            columns.append(pt);
        }
        {
            QString riferimento = grid->model()->data(grid->model()->index(crow,0)).toString();
            QSharedPointer<ODSCell> pt(new ODSCellString(riferimento));
            columns.append(pt);
        }
        {
            QSharedPointer<ODSCell> pt(new ODSCellString(numero));
            columns.append(pt);
        }
        {
            double imponibile = grid->model()->data(grid->model()->index(crow,1)).toFloat();
            QSharedPointer<ODSCell> pt(new ODSCellCurrency(ODSCurrency::EUR, imponibile));
            columns.append(pt);

            double imposta = grid->model()->data(grid->model()->index(crow,3)).toFloat();
            QSharedPointer<ODSCell> pt1(new ODSCellCurrency(ODSCurrency::EUR, imposta));
            columns.append(pt1);

            QSharedPointer<ODSCell> pt2(new ODSCellCurrency(ODSCurrency::EUR, imponibile + imposta));
            columns.append(pt2);
        }

        rows.append(columns);
    }

    filename = filenametemplate.arg(months[qde->date().month()-1], QString::number(qde->date().year()), "");
    QString pathandfilename = QDir(m_setting->getPath(Settings::Execute::elencofatture)).filePath(filename);

    if(!QFileInfo::exists(pathandfilename)) {
        qInfo(logInfo())  << "Elenco Fatture not exists: " << pathandfilename;
        QString templatepathandfilename = QDir(m_setting->getPath(Settings::Execute::elencofatture)).filePath("Fatt. MODELLO.ods");
        QFile::copy(templatepathandfilename, pathandfilename);
    }

    if(!QFileInfo(pathandfilename).isFile()){
        qWarning(logWarning())  << "A folder exists with same name: " << pathandfilename;
        return "Errore cartella con lo stesso nome del file ods";
    }

    qInfo(logInfo())  << "Adding to elenco fatture: " << pathandfilename;

    ODSDocument book;
    book.Load(pathandfilename);

    //backupit
    if(m_setting->isExecute(Settings::Execute::backupfiles)){
        QString backupfilename = filenametemplate.arg(months[qde->date().month()-1], QString::number(qde->date().year()), QString::number(QDateTime::currentSecsSinceEpoch()));
        QString backuppath = QDir(m_setting->getPath(Settings::Execute::elencofatture)).filePath("old");
        QDir dir;
        if (dir.mkpath(backuppath))
            dir.rename(pathandfilename, QDir(backuppath).filePath(backupfilename));
    }
    book.AddRowToContent(&rows);
    if(book.Save(pathandfilename))
        return "N. " + QString::number(crow) + " righe aggiunte al file fatture";
    else
        return "Errore nel salvataggio file fatture";

}

QString MainWindow::executeMastriniFornitori()
{
    QString filenametemplate = "%1%2.ods";
    QString filename;

    QDateEdit* qde = this->findChild<QDateEdit*>("dataemissioneEdit");

    QDate dataemissione = qde->date();

    QString entity = this->findChild<QLineEdit*>("entitaEdit")->text();
    QString numero = this->findChild<QLineEdit*>("numeroEdit")->text();
    double totale = (this->findChild<QLineEdit*>("totaleEdit")->text()).toDouble();

    QTableView *grid = ui->paymentsTable;

    QList<QList<QSharedPointer<ODSCell>>> rows;

    int crow = 0;

    for (crow = 0; crow < grid->model()->rowCount(); crow++) {

        QDate datascadenza = QDate::fromString(grid->model()->data(grid->model()->index(crow,1)).toString(),"dd/MM/yyyy");
        double importo = grid->model()->data(grid->model()->index(crow,2)).toFloat();
        QString cassa = grid->model()->data(grid->model()->index(crow,3)).toString();

        QList<QSharedPointer<ODSCell>> columns;

        if (crow == 0) {
            QSharedPointer<ODSCell> pt(new ODSCellDate(dataemissione));
            columns.append(pt);
        }
        else {
            QSharedPointer<ODSCell> pt(new ODSCellEmpty);
            columns.append(pt);
        }

        {
            QSharedPointer<ODSCell> pt(new ODSCellEmpty);
            columns.append(pt);
        }

        if (crow == 0) {
            QSharedPointer<ODSCell> pt(new ODSCellString(numero));
            columns.append(pt);
        }
        else {
            QSharedPointer<ODSCell> pt(new ODSCellEmpty);
            columns.append(pt);
        }

        {
            QSharedPointer<ODSCell> pt(new ODSCellEmpty);
            columns.append(pt);
        }

        if (crow == 0) {
            QSharedPointer<ODSCell> pt(new ODSCellCurrency(ODSCurrency::EUR, totale));
            columns.append(pt);
        }
        else {
            QSharedPointer<ODSCell> pt(new ODSCellEmpty);
            columns.append(pt);
        }

        {
            QSharedPointer<ODSCell> pt(new ODSCellEmpty);
            columns.append(pt);
        }

        {
            QSharedPointer<ODSCell> pt(new ODSCellDate(datascadenza));
            columns.append(pt);
        }

        {
            QSharedPointer<ODSCell> pt(new ODSCellEmpty);
            columns.append(pt);
        }

        {
            QSharedPointer<ODSCell> pt(new ODSCellCurrency(ODSCurrency::EUR, importo));
            columns.append(pt);
        }

        {
            QSharedPointer<ODSCell> pt(new ODSCellEmpty);
            columns.append(pt);
        }

        {
            QSharedPointer<ODSCell> pt(new ODSCellString(""));
            columns.append(pt);
        }

        {
            QSharedPointer<ODSCell> pt(new ODSCellString(cassa));
            columns.append(pt);
        }

        rows.append(columns);
    }

    filename = filenametemplate.arg(entity.replace(".", ""), "");
    QString pathandfilename = QDir(m_setting->getPath(Settings::Execute::mastrinifornitori)).filePath(filename);

    if(!QFileInfo::exists(pathandfilename)) {
        qInfo(logInfo())  << "Mastrino Fornitore not exists: " << pathandfilename;
        QString templatepathandfilename = QDir(m_setting->getPath(Settings::Execute::mastrinifornitori)).filePath("Mastrino MODELLO.ods");
        QFile::copy(templatepathandfilename, pathandfilename);
    }

    if(!QFileInfo(pathandfilename).isFile()){
        qWarning(logWarning())  << "A folder exists with same name: " << pathandfilename;
        return "Errore cartella con lo stesso nome del file ods";
    }

    qInfo(logInfo())  << "Adding to mastrino fornitore: " << pathandfilename;

    ODSDocument book;
    book.Load(pathandfilename);

    //backupit
    QString backupfilename = filenametemplate.arg(entity.replace(".", ""), QString::number(QDateTime::currentSecsSinceEpoch()));
    if(m_setting->isExecute(Settings::Execute::backupfiles)){
        QString backuppath = QDir(m_setting->getPath(Settings::Execute::mastrinifornitori)).filePath("old");
        QDir dir;
        if (dir.mkpath(backuppath))
            dir.rename(pathandfilename, QDir(backuppath).filePath(backupfilename));
    }
    book.AddRowToContent(&rows);
    if (book.Save(pathandfilename))
        return "N. " + QString::number(crow) + " righe aggiunte al file mastrino fornitore";
    else
        return "Errore nel salvataggio file mastrino fornitore";

}

QString MainWindow::executePrimaNota()
{
    QString filenametemplate = "%1 %2 %3.ods";
    QString rowcommenttemplate = "A Favore di %1 Saldo Rata/Fatt. %2 del %3 di %4, rata %5/%6";

    QDateEdit* qde = this->findChild<QDateEdit*>("dataemissioneEdit");

    QDate dataemissione = qde->date();

    QString entity = this->findChild<QLineEdit*>("entitaEdit")->text();
    QString numero = this->findChild<QLineEdit*>("numeroEdit")->text();
    QString totale = this->findChild<QLineEdit*>("totaleEdit")->text();

    QTableView *grid = ui->paymentsTable;

    QMap<QString, QList<QList<QSharedPointer<ODSCell>>>*> filelist;

    int numberofrows = grid->model()->rowCount();

    int execrow, crow = 0;

    for (crow = 0; crow < numberofrows; crow++) {

        QString modalita = grid->model()->data(grid->model()->index(crow, 0)).toString();

        if (modalita == paymentMethodType["MP12"]) {

            execrow++;

            QDate datascadenza = QDate::fromString(grid->model()->data(grid->model()->index(crow,1)).toString(),"dd/MM/yyyy");
            if ((!datascadenza.isValid())||datascadenza.isNull())
                datascadenza = dataemissione;

            QString cassa = grid->model()->data(grid->model()->index(crow,3)).toString();
            double importo = grid->model()->data(grid->model()->index(crow,2)).toFloat();


            QString filename = filenametemplate.arg(cassa, months[datascadenza.month()-1], QString::number(datascadenza.year()), "");

            QList<QList<QSharedPointer<ODSCell>>>* rows;
            if (!filelist.contains(filename)) {
                rows = new QList<QList<QSharedPointer<ODSCell>>>();
                filelist.insert(filename,  rows);
            }
            else {
                rows = filelist.value(filename);
            }

            QString rowcomment = rowcommenttemplate.arg(entity, numero , dataemissione.toString("dd/MM/yyyy"),totale, QString::number(crow+1), QString::number(numberofrows));

            QList<QSharedPointer<ODSCell>> columns;

            {
                QSharedPointer<ODSCell> pt(new ODSCellDate(datascadenza));
                columns.append(pt);
            }

            {
                QSharedPointer<ODSCell> pt(new ODSCellEmpty);
                columns.append(pt);
            }

            {
                QSharedPointer<ODSCell> pt(new ODSCellString(rowcomment));
                columns.append(pt);
            }

            {
                QSharedPointer<ODSCell> pt(new ODSCellEmpty(4));
                columns.append(pt);
            }

            {
                QSharedPointer<ODSCell> pt(new ODSCellCurrency(ODSCurrency::EUR, importo));
                columns.append(pt);
            }

            rows->append(columns);
        }
    }

    bool ok = true;

    for(QString filename : filelist.keys())
    {
        QString year = filename.split(".").value(0).right(4);

        QString yearpath = QDir(m_setting->getPath(Settings::Execute::primanota)).filePath(year);
        if (!QDir(yearpath).exists()) {
            QDir().mkdir(yearpath);
        }

        QString pathandfilename = QDir(yearpath).filePath(filename);

        if(!QFileInfo::exists(pathandfilename)) {
            qInfo(logInfo())  << "Prima Nota not exists: " << pathandfilename;
            QString templatepathandfilename = QDir(m_setting->getPath(Settings::Execute::primanota)).filePath("PrimaNota MODELLO.ods");
            QFile::copy(templatepathandfilename, pathandfilename);
        }

        if(!QFileInfo(pathandfilename).isFile()){
            qWarning(logWarning())  << "A folder exists with same name: " << pathandfilename;
            return "Errore cartella con lo stesso nome del file ods";
        }

        qInfo(logInfo())  << "Adding to prima nota: " << pathandfilename;

        ODSDocument book;
        book.Load(pathandfilename);

        //backupit
        if(m_setting->isExecute(Settings::Execute::backupfiles)){

            QString backupfilename = filenametemplate.arg(QString::number(QDateTime::currentSecsSinceEpoch()),"","");
            QString backuppath = QDir(m_setting->getPath(Settings::Execute::primanota)).filePath("old");
            QDir dir;
            if (dir.mkpath(backuppath))
                dir.rename(pathandfilename, QDir(backuppath).filePath(backupfilename));
        }

        book.AddRowToContent(filelist.value(filename));
        ok = (ok && book.Save(pathandfilename));
    }

    if (ok)
        return "N. " + QString::number(execrow) + " righe aggiunte a N. " + QString::number(filelist.count()) + " files prima nota";
    else
        return "Errore nel salvataggio di almeno un file prima nota";

}

QString MainWindow::executeScadenziario()
{
    QString filenametemplate = "%1 %2.ods";

    QString entity = this->findChild<QLineEdit*>("entitaEdit")->text();
    QString numero = this->findChild<QLineEdit*>("numeroEdit")->text();

    QDate dataemissione = this->findChild<QDateEdit*>("dataemissioneEdit")->date();

    QTableView *grid = ui->paymentsTable;

    QMap<QString, QList<QList<QSharedPointer<ODSCell>>>*> filelist;

    int numberofrows = grid->model()->rowCount();

    int execrow, crow = 0;

    for (crow = 0; crow < numberofrows; crow++) {

        QString modalita = grid->model()->data(grid->model()->index(crow, 0)).toString();
        QDate datascadenza = QDate::fromString(grid->model()->data(grid->model()->index(crow,1)).toString(),"dd/MM/yyyy");
        if ((!datascadenza.isValid()) || datascadenza.isNull())
            if(modalita == paymentMethodType["MP05"]) {
                datascadenza = dataemissione.addMonths(1);
            }
            else {
                datascadenza = dataemissione;
            }

        QString cassa = grid->model()->data(grid->model()->index(crow,3)).toString();
        double importo = grid->model()->data(grid->model()->index(crow,2)).toFloat();

        if((modalita == paymentMethodType["MP12"])||(modalita == paymentMethodType["MP05"])) {

            execrow++;

            QString filename = filenametemplate.arg(months[datascadenza.month()-1], QString::number(datascadenza.year()));
            QList<QList<QSharedPointer<ODSCell>>>* rows;
            if (!filelist.contains(filename)) {
                rows = new QList<QList<QSharedPointer<ODSCell>>>();
                filelist.insert(filename,  rows);
            }
            else {
                rows = filelist.value(filename);
            }


            QList<QSharedPointer<ODSCell>> columns;

            {
                QSharedPointer<ODSCell> pt(new ODSCellString(numero));
                columns.append(pt);
            }

            {
                QSharedPointer<ODSCell> pt(new ODSCellString(entity));
                columns.append(pt);
            }

            {
                QSharedPointer<ODSCell> pt(new ODSCellEmpty(2));
                columns.append(pt);
            }

            {
                QSharedPointer<ODSCell> pt(new ODSCellDate(datascadenza));
                columns.append(pt);
            }

            {
                QSharedPointer<ODSCell> pt(new ODSCellEmpty());
                columns.append(pt);
            }

            {
            ODSCell* p;
                if(cassa==bankAccount["IT33U0843051030000000180277"])
                    p = new ODSCellCurrency(ODSCurrency::EUR, importo);
                else
                    p = new ODSCellEmpty();
                QSharedPointer<ODSCell> pt(p);
                columns.append(pt);
            }

            {
                QSharedPointer<ODSCell> pt(new ODSCellEmpty());
                columns.append(pt);
            }

            {
            ODSCell* p;
                if(cassa==bankAccount["IT65Z0306951030615272528476"])
                    p = new ODSCellCurrency(ODSCurrency::EUR, importo);
                else
                    p = new ODSCellEmpty();
                QSharedPointer<ODSCell> pt(p);
                columns.append(pt);
            }

            {
                QSharedPointer<ODSCell> pt(new ODSCellEmpty());
                columns.append(pt);
            }

            {
            ODSCell* p;
                if(cassa==bankAccount["IT64U0503451861000000001728"])
                    p = new ODSCellCurrency(ODSCurrency::EUR, importo);
                else
                    p = new ODSCellEmpty();
                QSharedPointer<ODSCell> pt(p);
                columns.append(pt);
            }

            rows->append(columns);
        }
    }

    bool ok = true;

    for(QString filename : filelist.keys())
    {
        QString year = filename.split(".").value(0).right(4);

        QString yearpath = QDir(m_setting->getPath(Settings::Execute::scadenziario)).filePath(year);
        if (!QDir(yearpath).exists()) {
            QDir().mkdir(yearpath);
        }

        QString pathandfilename = QDir(yearpath).filePath(filename);

        if(!QFileInfo::exists(pathandfilename)) {
            qInfo(logInfo())  << "Scadenziario not exists: " << pathandfilename;
            QString templatepathandfilename = QDir(m_setting->getPath(Settings::Execute::scadenziario)).filePath("Scadenziario MODELLO.ods");
            QFile::copy(templatepathandfilename, pathandfilename);
        }

        if(!QFileInfo(pathandfilename).isFile()){
            qWarning(logWarning())  << "A folder exists with same name: " << pathandfilename;
            return "Errore cartella con lo stesso nome del file ods";
        }

        qInfo(logInfo())  << "Adding to scadenziario " << pathandfilename;

        ODSDocument book;
        book.Load(pathandfilename);

        //backupit
        if(m_setting->isExecute(Settings::Execute::backupfiles)){
            QString backupfilename = filenametemplate.arg(QString::number(QDateTime::currentSecsSinceEpoch()),"");
            QString backuppath = QDir(m_setting->getPath(Settings::Execute::scadenziario)).filePath("old");
            QDir dir;
            if (dir.mkpath(backuppath))
                dir.rename(pathandfilename, QDir(backuppath).filePath(backupfilename));
        }

        book.AddRowToContent(filelist.value(filename));
        ok = (ok && book.Save(pathandfilename));
    }

    if (ok)
        return "N. " + QString::number(execrow) + " righe aggiunte a N. " + QString::number(filelist.count()) + " files scadenziario";
    else
        return "Errore nel salvataggio di almeno un file scadenziario";
}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    //QToolBar *fileToolBar = addToolBar(tr("File"));

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Apri una fattura elettronica"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    //fileToolBar->addAction(openAct);

    fileMenu->addSeparator();

    const QIcon exitIcon = QIcon::fromTheme("application-exit");
    QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Esci dall'applicazione"));

    menuBar()->addSeparator();

    QMenu *processMenu = menuBar()->addMenu(tr("&Process"));

    QAction *executeAct = processMenu->addAction(tr("&Esegui"), this, &MainWindow::execute);
    executeAct->setStatusTip(tr("Esporta i dati nei fogli ods selezionati"));

    processMenu->addSeparator();

    QAction *elencoFattureAct = processMenu->addAction(tr("In Elenco Fatture"), this, &MainWindow::toggleElencoFatture);
    elencoFattureAct->setCheckable(true);
    elencoFattureAct->setChecked(m_setting->isExecute(Settings::Execute::elencofatture));

    QAction *mastrinoFornitoriAct = processMenu->addAction(tr("In Mastrini Fornitori"), this, &MainWindow::toggleMastriniFornitori);
    mastrinoFornitoriAct->setCheckable(true);
    mastrinoFornitoriAct->setChecked(m_setting->isExecute(Settings::Execute::mastrinifornitori));

    QAction *primaNotaAct = processMenu->addAction(tr("In Prima Nota"), this, &MainWindow::togglePrimaNota);
    primaNotaAct->setCheckable(true);
    primaNotaAct->setChecked(m_setting->isExecute(Settings::Execute::primanota));

    QAction *scadenziarioAct = processMenu->addAction(tr("In Scadenziario"), this, &MainWindow::toggleScadenziario);
    scadenziarioAct->setCheckable(true);
    scadenziarioAct->setChecked(m_setting->isExecute(Settings::Execute::scadenziario));

    QAction *backupAct = processMenu->addAction(tr("Backup dei file ods"), this, &MainWindow::toggleBackupFile);
    backupAct->setCheckable(true);
    backupAct->setChecked(m_setting->isExecute(Settings::Execute::backupfiles));

    menuBar()->addSeparator();

    QMenu *toolsMenu = menuBar()->addMenu(tr("&Impostazioni"));

    QAction* pathFattureElettronicheAct = toolsMenu->addAction(tr("Cartella Fatture Elettroniche"), this, &MainWindow::setPathFattureElettroniche);
    pathFattureElettronicheAct->setStatusTip(tr("Imposta la cartella in cui si trovano le Fatture Elettroniche"));

    toolsMenu->addSeparator();

    QAction *pathElencoFattureAct = toolsMenu->addAction(tr("Cartella Elenco Fatture"), this, &MainWindow::setPathElencoFatture);
    pathElencoFattureAct->setStatusTip(tr("Imposta la cartella del Elenco Fatture"));

    QAction *pathMastrinoFornitoriAct = toolsMenu->addAction(tr("Cartella Mastrini Fornitori"), this, &MainWindow::setPathMastriniFornitori);
    pathMastrinoFornitoriAct->setStatusTip(tr("Imposta la cartella dei Mastrini Fornitori"));

    QAction *pathPrimaNotaAct = toolsMenu->addAction(tr("Cartella Prima Nota"), this, &MainWindow::setPathPrimaNota);
    pathPrimaNotaAct->setStatusTip(tr("Imposta la cartella della Prima Nota"));

    QAction *pathScadenziarioAct= toolsMenu->addAction(tr("Cartella Scadenziario"), this, &MainWindow::setPathScadenziario);
    pathScadenziarioAct->setStatusTip(tr("Imposta la cartella dello Scadenziario"));

    toolsMenu->addSeparator();

    QAction *pathSaveSettingsAct = toolsMenu->addAction(tr("Salva Impostazioni Finestra"), this, &MainWindow::saveWindowSettings);
    pathSaveSettingsAct->setStatusTip(tr("Cambia lo stato della finestra dell'applicazione"));
    menuBar()->addSeparator();

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));

    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"), 0);
}

void MainWindow::parseXMLFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::critical(this, tr("Procurans"),
                             tr("Cannot read file %1")
                             .arg(QDir::toNativeSeparators(fileName)),
                             QMessageBox::Ok);
        qWarning(logWarning())  << QString("Cannot read file %1:\n%2.").arg(QDir::toNativeSeparators(fileName), file.errorString());
        return;
    }

    statusBar()->showMessage(tr("Fattura in caricamento ..."), 2000);

    QXmlStreamReader xml(&file);
    QList< QMap<QString,QString> > detailsData;
    QList< QMap<QString,QString> > paymentsData;
    QList< QMap<QString,QString> > summaryData;
    QMap<QString,QString> headerData;

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    /* We'll parse the XML until we reach end of it.*/
    QStringRef latestValid;
    while(!xml.atEnd() &&
            !xml.hasError()) {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = xml.readNext();
        /* If token is just StartDocument, we'll go to next.*/
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if(isStartElementNamed(xml, "CedentePrestatore")) {
            headerData = this->parseHeader(xml);
        }
        if(isStartElementNamed(xml, "DatiGeneraliDocumento")) {
            headerData.unite(this->parseDocument(xml));
        }

        if(isStartElementNamed(xml, "DettaglioLinee")) {
            detailsData.append(this->parseDetail(xml));
        }
        if(isStartElementNamed(xml, "DettaglioPagamento")) {
            paymentsData.append(this->parsePayment(xml));
        }
        if(isStartElementNamed(xml, "DatiRiepilogo")) {
            summaryData.append(this->parseSummary(xml));
        }
    }
    /* Error handling. */
    if(xml.hasError()) {
        QMessageBox::critical(this,
                              tr("Procurans"),
                              tr("Error parsing XML file %1")
                              .arg(QDir::toNativeSeparators(fileName)),
                              QMessageBox::Ok);
        qWarning(logWarning())  << QString("Cannot parse file %1:\n%2.").arg(QDir::toNativeSeparators(fileName), xml.errorString());

    }
    else {
        billLoaded = true;
        qInfo(logInfo())  << "XML File parsed: " << fileName;
        //QMenu e = menuBar()->findChild<QMenu>("test");
    }
    /* Removes any device() or data from the reader
     * and resets its internal state to the initial state. */
    xml.clear();
    file.close();

    if(paymentsData.isEmpty()) {
        QMap<QString, QString> payment;
       // "ModalitaPagamento",
       //     "DataScadenzaPagamento",

        payment.insert("ImportoPagamento", headerData.value("ImportoTotaleDocumento"));
        paymentsData.append(payment);
    }

    this->addHeaderToUI(headerData);
    this->addPaymentsToUI(paymentsData, createPaymentsGridSchema());
    this->addDetailsToUI(detailsData, createDetailsGridSchema());
    this->addSummaryToUI(summaryData, createSummaryGridSchema());

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    curFile = fileName;

   /* if (!validateBill(fileContent.toUtf8())) {
        QMessageBox::warning(this, tr("Procurans"),
                             tr("Validation Fail %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }
    statusBar()->showMessage(tr("Fattura validata"), 2000);
*/
    this->setWindowTitle(QString(QApplication::applicationName() + ": " + strippedName(curFile)));

    statusBar()->showMessage(tr("Fattura caricata"), 2000);
}

inline bool MainWindow::isStartElementNamed(QXmlStreamReader& xml, const QString &tokenName)
{
    return ((xml.tokenType() == QXmlStreamReader::StartElement) && (xml.name() == tokenName));
}

inline bool MainWindow::isNotEndElementNamed(QXmlStreamReader& xml, const QString &tokenName)
{
    return !((xml.tokenType() == QXmlStreamReader::EndElement) && (xml.name() == tokenName));
}

inline void MainWindow::addSubelementsDataToMap(QXmlStreamReader& xml,
                                                const QString& tokenName,
                                                const QStringList& subTokenName,
                                                QMap<QString, QString>& header)
{
    if(isStartElementNamed(xml, tokenName)) {
        do {
            /* Next element... */
            xml.readNext();

            for (int i = 0; i < subTokenName.size(); ++i) {
                if(isStartElementNamed(xml, subTokenName.at(i))) {
                    this->addElementDataToMap(xml, header);
                }
            }
        }
        while(isNotEndElementNamed(xml, tokenName));

    }
}

QMap<QString, QString> MainWindow::parseDocument(QXmlStreamReader& xml)
{
    QMap<QString, QString> document;

    if(xml.tokenType() != QXmlStreamReader::StartElement &&
            xml.name() == "DatiGeneraliDocumento") {
        return document;
    }

    addSubelementsDataToMap(xml, "DatiGeneraliDocumento", { "Data", "Numero", "ImportoTotaleDocumento" }, document);

    return document;
}

QMap<QString, QString> MainWindow::parseSummary(QXmlStreamReader& xml)
{
    QMap<QString, QString> summary;

    if(xml.tokenType() != QXmlStreamReader::StartElement &&
            xml.name() == "DatiRiepilogo") {
        return summary;
    }

    addSubelementsDataToMap(xml, "DatiRiepilogo", { "ImponibileImporto", "Imposta", "AliquotaIVA", "Natura" }, summary);

    return summary;
}

QMap<QString, QString> MainWindow::parseHeader(QXmlStreamReader& xml)
{
    QMap<QString, QString> header;

    if(xml.tokenType() != QXmlStreamReader::StartElement &&
            xml.name() == "CedentePrestatore") {
        return header;
    }

    do {
        /* Next element... */
        xml.readNext();

        if(isStartElementNamed(xml, "DatiAnagrafici")) {
            do {
                /* Next element... */
                xml.readNext();

                addSubelementsDataToMap(xml, "Anagrafica", { "Denominazione", "Nome", "Cognome" }, header);
                addSubelementsDataToMap(xml, "IdFiscaleIVA", { "IdPaese", "IdCodice" }, header);
            }
            while(isNotEndElementNamed(xml,"DatiAnagrafici"));
        }
        addSubelementsDataToMap(xml, "Sede", { "Indirizzo", "CAP", "Comune" }, header);
    }
    while(isNotEndElementNamed(xml,"CedentePrestatore"));

    return header;
}

QMap<QString, QString> MainWindow::parseDetail(QXmlStreamReader& xml)
{
    QMap<QString, QString> detail;

    if(xml.tokenType() != QXmlStreamReader::StartElement &&
            xml.name() == "DettaglioLinee") {
        return detail;
    }

    addSubelementsDataToMap(xml, "DettaglioLinee", { "Descrizione",
                                                     "Quantita",
                                                     "UnitaMisura",
                                                     "PrezzoUnitario",
                                                     "PrezzoTotale",
                                                     "AliquotaIVA",
                                                     "Natura" }, detail);
    return detail;
}

QMap<QString, QString> MainWindow::parsePayment(QXmlStreamReader& xml)
{
    QMap<QString, QString> payment;

    if(xml.tokenType() != QXmlStreamReader::StartElement &&
            xml.name() == "DettaglioPagamento") {
        return payment;
    }

    addSubelementsDataToMap(xml, "DettaglioPagamento", { "ModalitaPagamento",
                                                         "DataScadenzaPagamento",
                                                         "ImportoPagamento",
                                                         "IBAN"}, payment);
    return payment;
}

void MainWindow::addElementDataToMap(QXmlStreamReader& xml, QMap<QString, QString>& map) const
{
    /* We need a start element, like <foo> */
    if(xml.tokenType() != QXmlStreamReader::StartElement) {
        return;
    }
    /* Let's read the name... */
    QString elementName = xml.name().toString();
    /* ...go to the next. */
    xml.readNext();
    /*
     * This elements needs to contain Characters so we know it's
     * actually data, if it's not we'll leave.
     */
    if(xml.tokenType() != QXmlStreamReader::Characters) {
        return;
    }
    /* Now we can add it to the map.*/
    map.insert(elementName, xml.text().toString());
}

QStandardItemModel* createGridModel(const QList< QMap<QString,QString> >  &data,
                                    const QList<GridSchemaField*> &schema)
{
    int columnN = schema.size();
    int rowN = data.size();
    QStandardItemModel* model = new QStandardItemModel(rowN, columnN);

    for (int column = 0; column < columnN; column++) {
        model->setHeaderData(column, Qt::Horizontal, schema.at(column)->getColumnName());
    }

    int outRow = 0;
    for (int row = 0; row < rowN; row++) {   
        QList<QStandardItem*> rowtemp;
        //QStandardItem* rowtemp[columnN];
        bool ignoreRow = false;

        for (int column = 0; column < columnN; column++) {
            QString elementName = schema.at(column)->getElementName();
            QString value = "";
            if (elementName!="") {
                value = data.at(row).value(elementName, "");
            }
            rowtemp.append(schema.at(column)->createGridItem(value));
            ignoreRow = (ignoreRow && schema.at(column)->getToBeIgnored());
        }

        if (!ignoreRow) {
            for (int column = 0; column < columnN; column++)
                model->setItem(outRow, column, rowtemp.at(column));
            outRow++;
        }
        else {
            for (int column = 0; column < columnN; column++)
                schema.at(column)->resetToBeIgnored();
        }
    }
    return model;
}

void MainWindow::addHeaderToUI(QMap<QString,QString>& headerData)
{
    QLineEdit* qle = this->findChild<QLineEdit*>("entitaEdit");
    if (qle != nullptr){
        if (headerData.contains("Denominazione"))
            qle->setText(headerData.value("Denominazione"));
        else
            qle->setText(headerData.value("Cognome") + " " + headerData.value("Nome")) ;
    }

    qle = this->findChild<QLineEdit*>("pivaEdit");
    if (qle != nullptr)
        qle->setText(headerData.value("IdPaese") + headerData.value("IdCodice"));

    qle = this->findChild<QLineEdit*>("indirizzoEdit");
    if (qle != nullptr)
        qle->setText(headerData.value("Indirizzo"));

    qle = this->findChild<QLineEdit*>("capEdit");
    if (qle != nullptr)
        qle->setText(headerData.value("CAP"));

    qle = this->findChild<QLineEdit*>("comuneEdit");
    if (qle != nullptr)
        qle->setText(headerData.value("Comune"));

    QDateEdit* qde = this->findChild<QDateEdit*>("dataemissioneEdit");
    if (qde != nullptr) {
        QDate date = QDate::fromString(headerData.value("Data"),"yyyy-MM-dd");
        qde->setDate(date);
    }

    qle = this->findChild<QLineEdit*>("numeroEdit");
    if (qle != nullptr)
        qle->setText(headerData.value("Numero"));

    qle = this->findChild<QLineEdit*>("imponibileEdit");
    if (qle != nullptr)
        qle->setText(headerData.value("ImponibileImporto"));

    qle = this->findChild<QLineEdit*>("impostaEdit");
    if (qle != nullptr)
        qle->setText(headerData.value("Imposta"));

    qle = this->findChild<QLineEdit*>("totaleEdit");
    if (qle != nullptr)
        qle->setText(headerData.value("ImportoTotaleDocumento"));
}

QList<GridSchemaField*> MainWindow::createDetailsGridSchema()
{
    QList<GridSchemaField*> schema;
    schema.append(new GridSchemaField(QObject::tr("Descrizione"), "Descrizione"));
    schema.append(new GridSchemaField(QObject::tr("Quantita"), "Quantita", FloatColumn, 2));
    schema.append(new GridSchemaField(QObject::tr("UdM"), "UnitaMisura", CStringColumn, 0, true));
    schema.append(new GridSchemaField(QObject::tr("P.Unitario"), "PrezzoUnitario", FloatColumn, 2, true));
    schema.append(new GridSchemaField(QObject::tr("P.Totale"), "PrezzoTotale", FloatColumn, 2, true));
    schema.append(new GridSchemaField(QObject::tr("IVA"), "AliquotaIVA", FloatColumn, 0));
    schema.append(new GridSchemaField(QObject::tr("Riferimento")));

    return schema;
}

QList<GridSchemaField*> MainWindow::createPaymentsGridSchema()
{
    QList<GridSchemaField*> schema;
    schema.append(new GridSchemaField(QObject::tr("Modalita"), "ModalitaPagamento", &paymentMethodType));
    schema.append(new GridSchemaField(QObject::tr("Scadenza"), "DataScadenzaPagamento", DateColumn));
    schema.append(new GridSchemaField(QObject::tr("Importo"), "ImportoPagamento", FloatColumn, 2));
    schema.append(new GridSchemaField(QObject::tr("Banca"), "IBAN", &bankAccount));

    return schema;
}

QList<GridSchemaField*> MainWindow::createSummaryGridSchema()
{
    QList<GridSchemaField*> schema;
    schema.append(new GridSchemaField(QObject::tr("Riferimento"), "Riferimento"));
    schema.append(new GridSchemaField(QObject::tr("Imponibile"), "ImponibileImporto", FloatColumn, 2));
    schema.append(new GridSchemaField(QObject::tr("Aliquota"), "AliquotaIVA", FloatColumn, 2));
    schema.append(new GridSchemaField(QObject::tr("Imposta"), "Imposta", FloatColumn, 2));
    schema.append(new GridSchemaField(QObject::tr("Natura"), "Natura", &naturaType));

    return schema;
}

double MainWindow::computeTotal(QList< QMap<QString,QString> >& data, QString xmlfield)
{
    double result = 0.0;
    int rowN = data.size();
    for (int row = 0; row < rowN; row++) {
        auto item = data.at(row);
        result += item[xmlfield].toDouble();
    }
    return result;
}

void MainWindow::addSummaryToUI(QList< QMap<QString,QString> >& summaryData, QList<GridSchemaField*> summarySchema)
{
    QTableView *grid = ui->summaryTable;

    grid->setModel(createGridModel(summaryData, summarySchema));

    //QStringList z(naturaType.values());
    //z.sort(Qt::CaseInsensitive);
    //ComboBoxItemDelegate* cbid = new ComboBoxItemDelegate(z, grid);

    //grid->setItemDelegateForColumn(4, cbid);

    float fImponibile = computeTotal(summaryData, "ImponibileImporto");
    QLineEdit* qle = this->findChild<QLineEdit*>("imponibileEdit");
    if (qle != nullptr) {
        QString imponibile = QString::number(fImponibile, 'f', 2);
        qle->setText(imponibile);
    }

    float fImposta = computeTotal(summaryData, "Imposta");
    qle = this->findChild<QLineEdit*>("impostaEdit");
    if (qle != nullptr) {
        QString imposta = QString::number(fImposta, 'f', 2);
        qle->setText(imposta);
    }

    qle = this->findChild<QLineEdit*>("totaleEdit");
    if (qle != nullptr) {
        QString totale = QString::number(fImponibile + fImposta, 'f', 2);
        qle->setText(totale);
    }
    
}

void MainWindow::UpdateSummaryUI(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    if((topLeft.column()==5)||(topLeft.column()==6))
    {
        QAbstractItemModel *detailsModel = ui->detailsTable->model();

        QMap< QString, QMap<QString,QString>*> summaryData;

        QMap<QString,QString> rowName;

        for (int detailRow = 0; detailRow < detailsModel->rowCount(); ++detailRow) {

            QString PrezzoTotale = detailsModel->data(detailsModel->index(detailRow, 4)).toString();
            QString AliquotaIVA = detailsModel->data(detailsModel->index(detailRow, 5)).toString();
            QString Riferimento = detailsModel->data(detailsModel->index(detailRow, 6)).toString();

            QString rowName = AliquotaIVA + Riferimento;

            QMap<QString,QString>* m = new QMap<QString,QString>;

            float fPrezzoTotale = PrezzoTotale.toFloat();
            float fImposta = fPrezzoTotale * AliquotaIVA.toFloat() / 100.00;

            if (!summaryData.contains(rowName)) {
                summaryData.insert(rowName, m);
                m->insert("ImponibileImporto", QString::number(fPrezzoTotale));
                m->insert("Riferimento", Riferimento);
                m->insert("AliquotaIVA", AliquotaIVA);
                m->insert("Imposta", QString::number(fImposta));
            }
            else {
                m = summaryData.value(rowName);
                float fLastPrezzoTotale = m->value("ImponibileImporto").toFloat();
                m->remove("ImponibileImporto");
                m->insert("ImponibileImporto", QString::number(fLastPrezzoTotale + fPrezzoTotale));
                float fLastImposta = m->value("Imposta").toFloat();
                m->remove("Imposta");
                m->insert("Imposta", QString::number(fLastImposta + fImposta));
            }

        }

        QList< QMap<QString,QString> > summaryData2;

        for (auto var : summaryData) {
            summaryData2.append(*var);
        }

        addSummaryToUI(summaryData2, createSummaryGridSchema());

        for (auto var : summaryData) {
            delete var;
        }
    }
}

void MainWindow::addDetailsToUI(QList< QMap<QString,QString> >& detailsData, QList<GridSchemaField*> detailsSchema)
{
    QTableView *grid = ui->detailsTable;

    QStandardItemModel* itemmodel = createGridModel(detailsData, detailsSchema);
    grid->setModel(itemmodel);

    bool ok = connect(
        itemmodel, &QStandardItemModel::dataChanged,
        this, &MainWindow::UpdateSummaryUI
    );

    Q_ASSERT( ok );

    grid->resizeColumnsToContents();
    //grid->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}



void MainWindow::addPaymentsToUI(QList< QMap<QString,QString> >& paymentData, QList<GridSchemaField*> paymentSchema)
{
    QTableView *grid = ui->paymentsTable;

    grid->setModel(createGridModel(paymentData, paymentSchema));

    QStringList z(paymentMethodType.values());
    z.sort(Qt::CaseInsensitive);
    ComboBoxItemDelegate* cbid = new ComboBoxItemDelegate(z, grid);

    grid->setItemDelegateForColumn(0, cbid);

    QStringList q(bankAccount.values());
    q.append("");
    q.sort(Qt::CaseInsensitive);
    ComboBoxItemDelegate* cbid1 = new ComboBoxItemDelegate(q, grid);

    grid->setItemDelegateForColumn(3, cbid1);

}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
