// Copyright 2019 - 2020, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include <QtWidgets>
#include <QTableView>
#include <QDir>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Logger.h"
#include "settings.h"
#include "comboboxitemdelegate.h"
#include "gridschemafield.h"

#include "qfatturapa/codelists.h"
#include "qfatturapa/invoiceparser.h"
#include "qoasis/currency.h"
#include "qoasis/table/tablecell.h"
#include "qoasis/table/tablecellstring.h"
#include "qoasis/table/tablecelldate.h"
#include "qoasis/table/tablecellcurrency.h"


using namespace qoasis;
using namespace qoasis::office;
using namespace qoasis::table;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    billLoaded = false;

    m_setting = &Settings::getInstance();

    bankAccount["IT64U0503451861000000001728"] = "BPM";
    bankAccount["IT23P0503451861000000001817"] = "BPM Fotovoltaico";
    bankAccount["IT65Z0306951030615272528476"] = "Intesa";
    bankAccount["IT33U0843051030000000180277"] = "CRAC";

    ui->setupUi(this);

    createActions();
    setupMenuBar();
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
            QString r = executeElencoFatture();
            result.append("<p>");
            result.append(r);
            result.append("</p>");
            qInfo(logInfo()) << r;
        }

        if (m_setting->isExecute(Settings::Execute::mastrinifornitori))
        {
            QString r = executeMastriniFornitori();
            result.append("<p>");
            result.append(r);
            result.append("</p>");
            qInfo(logInfo()) << r;
        }

        if (m_setting->isExecute(Settings::Execute::primanota))
        {
            QString r = executePrimaNota();
            result.append("<p>");
            result.append(r);
            result.append("</p>");
            qInfo(logInfo()) << r;
        }

        if (m_setting->isExecute(Settings::Execute::scadenziario))
        {
            QString r = executeScadenziario();
            result.append("<p>");
            result.append(r);
            result.append("</p>");
            qInfo(logInfo()) << r;
        }

        QFile::rename(curFile, curFile + "_importato");
    }

    QMessageBox::information(this, tr("Esito dell'operazione"), result);
}

QString MainWindow::executeElencoFatture()
{
	const QString filenametemplate = "Fatt. %1 %2%3.ods";

	QDateEdit* qde = this->findChild<QDateEdit*>("dataemissioneEdit");

    QString entity = this->findChild<QLineEdit*>("entitaEdit")->text();
    QString numero = this->findChild<QLineEdit*>("numeroEdit")->text();

    QTableView *grid = ui->summaryTable;

    QString filename = filenametemplate.arg(months[qde->date().month() - 1], QString::number(qde->date().year()), "");
    QString pathandfilename = QDir(m_setting->getPath(Settings::Execute::elencofatture)).filePath(filename);

    //QString check_pathandfilename = pathandfilename;
    //check_pathandfilename.replace("/", "//");

    if (!QFileInfo::exists(pathandfilename)) {
        qInfo(logInfo()) << "Elenco Fatture not exists: " << pathandfilename;
        QString templatepathandfilename = QDir(m_setting->getPath(Settings::Execute::elencofatture)).filePath("Fatt. MODELLO.ods");
        QFile::copy(templatepathandfilename, pathandfilename);
    }

    if (!QFileInfo(pathandfilename).isFile()) {
        qWarning(logWarning()) << "A folder exists with same name: " << pathandfilename;
        return "Errore cartella con lo stesso nome del file ods";
    }

    //backupit
    if (m_setting->isExecute(Settings::Execute::backupfiles)) {
        QString backupfilename = filenametemplate.arg(months[qde->date().month() - 1], QString::number(qde->date().year()), QString::number(QDateTime::currentSecsSinceEpoch()));
        QString backuppath = QDir(m_setting->getPath(Settings::Execute::elencofatture)).filePath("old");
        QDir dir;
        if (dir.mkpath(backuppath)) {
            QFile::copy(pathandfilename, QDir(backuppath).filePath(backupfilename));
        }
    }
	
    FileOds book(pathandfilename);
    book.load();

    const auto table = book.getContent()->getBody()->getSpreadsheet()->getTable(0);

    table->removeEndingEmptyRows();

    const auto lastrow = table->getRow(table->getLastNonEmptyRow());

    int crow;

    for (crow = 0; crow < grid->model()->rowCount(); crow++) {
	    const QString riferimento = grid->model()->data(grid->model()->index(crow, 0)).toString();
	    const double imponibile = grid->model()->data(grid->model()->index(crow, 1)).toFloat();
	    const double imposta = grid->model()->data(grid->model()->index(crow, 3)).toFloat();

        auto row = QSharedPointer<Tablerow>(new Tablerow);

        row->appendCell(QSharedPointer<Tablecell>(new TablecellString(entity)));
        row->appendCell(QSharedPointer<Tablecell>(new TablecellString(riferimento)));
        row->appendCell(QSharedPointer<Tablecell>(new TablecellString(numero)));
        row->appendCell(QSharedPointer<Tablecell>(new TablecellCurrency(Currency::EUR, imponibile)));
        row->appendCell(QSharedPointer<Tablecell>(new TablecellCurrency(Currency::EUR, imposta)));
        row->appendCell(QSharedPointer<Tablecell>(new TablecellCurrency(Currency::EUR, imponibile + imposta)));

        row->restyleFromRow(lastrow);

        table->appendRow(row);
    }

    qInfo(logInfo()) << "Adding to elenco fatture: " << pathandfilename;

    if (!book.save()) {
        return "Errore nel salvataggio file fatture";
    }
    return "N. " + QString::number(crow) + " righe aggiunte al file fatture";
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

    filename = filenametemplate.arg(entity.replace(".", ""), "");
    QString pathandfilename = QDir(m_setting->getPath(Settings::Execute::mastrinifornitori)).filePath(filename);

    //QString check_pathandfilename = pathandfilename;
    //check_pathandfilename.replace("/", "//");

    if (!QFileInfo::exists(pathandfilename)) {
        qInfo(logInfo()) << "Mastrino Fornitore not exists: " << pathandfilename;
        QString templatepathandfilename = QDir(m_setting->getPath(Settings::Execute::mastrinifornitori)).filePath("Mastrino MODELLO.ods");
        QFile::copy(templatepathandfilename, pathandfilename);
    }

    if (!QFileInfo(pathandfilename).isFile()) {
        qWarning(logWarning()) << "A folder exists with same name: " << pathandfilename;
        return "Errore cartella con lo stesso nome del file ods";
    }

    //backupit
    QString backupfilename = filenametemplate.arg(entity.replace(".", ""), QString::number(QDateTime::currentSecsSinceEpoch()));
    if (m_setting->isExecute(Settings::Execute::backupfiles)) {
        QString backuppath = QDir(m_setting->getPath(Settings::Execute::mastrinifornitori)).filePath("old");
        QDir dir;
        if (dir.mkpath(backuppath)) {
            QFile::copy(pathandfilename, QDir(backuppath).filePath(backupfilename));
        }
    }

    FileOds book(pathandfilename);
    book.load();

    const auto table = book.getContent()->getBody()->getSpreadsheet()->getTable(0);

    table->removeEndingEmptyRows();

    const auto lastrow = table->getRow(table->getLastNonEmptyRow());
	
    int crow = 0;

    for (crow = 0; crow < grid->model()->rowCount(); crow++) {
	    auto modalita = grid->model()->data(grid->model()->index(crow, 0)).toString();
        QDate datascadenza = QDate::fromString(grid->model()->data(grid->model()->index(crow,1)).toString(),"dd/MM/yyyy");
        datascadenza = (datascadenza.isValid() ? datascadenza : dataemissione.addMonths(1));
	    const double importo = grid->model()->data(grid->model()->index(crow,2)).toFloat();
        const QString cassa = grid->model()->data(grid->model()->index(crow,3)).toString();

        auto row = QSharedPointer<Tablerow>(new Tablerow);

        row->appendCell(QSharedPointer<Tablecell>(crow == 0 ?
            QSharedPointer<Tablecell>(new TablecellDate(dataemissione)) :
            QSharedPointer<Tablecell>(new Tablecell)));
        row->appendCell(QSharedPointer<Tablecell>(new Tablecell));
        row->appendCell(crow == 0 ?
            QSharedPointer<Tablecell>(new TablecellString(numero)) :
            QSharedPointer<Tablecell>(new Tablecell));
        row->appendCell(QSharedPointer<Tablecell>(new Tablecell));
        row->appendCell(crow == 0 ?
            QSharedPointer<Tablecell>(new TablecellCurrency(Currency::EUR, totale)) :
            QSharedPointer<Tablecell>(new Tablecell));
        row->appendCell(QSharedPointer<Tablecell>(new Tablecell));
        const QHash<QString, QString>& pmt = qfatturapa::paymentMethodType();
        row->appendCell(modalita == "" ||
            modalita == pmt["MP01"] ||
            modalita == pmt["MP02"] ||
            modalita == pmt["MP03"] ||
            modalita == pmt["MP08"] ?
            QSharedPointer<Tablecell>(new TablecellString("RD")) :
            QSharedPointer<Tablecell>(new TablecellDate(datascadenza)));
        row->appendCell(QSharedPointer<Tablecell>(new Tablecell));
        row->appendCell(QSharedPointer<Tablecell>(new TablecellCurrency(Currency::EUR, importo)));
        row->appendCell(QSharedPointer<Tablecell>(new Tablecell));
        row->appendCell(QSharedPointer<Tablecell>(new TablecellString("")));
        row->appendCell(QSharedPointer<Tablecell>(new TablecellString(cassa)));

        row->restyleFromRow(lastrow);

        table->appendRow(row);
    }

    qInfo(logInfo())  << "Adding to mastrino fornitore: " << pathandfilename;

    if (!book.save())
        return "Errore nel salvataggio file mastrino fornitore";
	
    return "N. " + QString::number(crow) + " righe aggiunte al file mastrino fornitore";
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

    QMap<QString, QList<QSharedPointer<Tablerow>>*> filelist;

    int numberofrows = grid->model()->rowCount();

    int execrow = 0, crow = 0;

    for (crow = 0; crow < numberofrows; crow++) {

        QString modalita = grid->model()->data(grid->model()->index(crow, 0)).toString();
        QString cassa = grid->model()->data(grid->model()->index(crow, 3)).toString();

        if (cassa == "") {
            qInfo(logInfo()) << "Payment " << crow << ": prima nota skipped, bank not selected, payment " << modalita;
            break;
        }

        execrow++;

        QDate datascadenza = QDate::fromString(grid->model()->data(grid->model()->index(crow,1)).toString(),"dd/MM/yyyy");
        if ((!datascadenza.isValid())||datascadenza.isNull())
            datascadenza = dataemissione;

            
        double importo = grid->model()->data(grid->model()->index(crow,2)).toFloat();

        QString filename = filenametemplate.arg(cassa, months[datascadenza.month()-1], QString::number(datascadenza.year()), "");

        QList<QSharedPointer<Tablerow>>* rows;
        if (!filelist.contains(filename)) {
            rows = new QList<QSharedPointer<Tablerow>>();
            filelist.insert(filename,  rows);
        }
        else {
            rows = filelist.value(filename);
        }

        QString rowcomment = rowcommenttemplate.arg(entity, numero , dataemissione.toString("dd/MM/yyyy"),totale, QString::number(crow+1), QString::number(numberofrows));

        auto row = QSharedPointer<Tablerow>(new Tablerow);
        row->appendCell(QSharedPointer<Tablecell>(new TablecellDate(datascadenza)));
        row->appendCell(QSharedPointer<Tablecell>(new Tablecell));
        row->appendCell(QSharedPointer<Tablecell>(new TablecellString(rowcomment)));
        row->appendCell(QSharedPointer<Tablecell>(new Tablecell(4)));
        row->appendCell(QSharedPointer<Tablecell>(new TablecellCurrency(Currency::EUR, importo)));

        rows->append(row);
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
        //QString check_pathandfilename = pathandfilename;
        //check_pathandfilename.replace("/", "//");

        if(!QFileInfo::exists(pathandfilename)) {
            qInfo(logInfo())  << "Prima Nota not exists: " << pathandfilename;
            QString templatepathandfilename = QDir(m_setting->getPath(Settings::Execute::primanota)).filePath("PrimaNota MODELLO.ods");
            QFile::copy(templatepathandfilename, pathandfilename);
        }

        if(!QFileInfo(pathandfilename).isFile()){
            qWarning(logWarning())  << "A folder exists with same name: " << pathandfilename;
            return "Errore cartella con lo stesso nome del file ods";
        }

        //backupit
        if (m_setting->isExecute(Settings::Execute::backupfiles)) {
            QString backupfilename = filenametemplate.arg(QString::number(QDateTime::currentSecsSinceEpoch()), "", "");
            QString backuppath = QDir(m_setting->getPath(Settings::Execute::primanota)).filePath("old");
            QDir dir;
            if (dir.mkpath(backuppath))
                QFile::copy(pathandfilename, QDir(backuppath).filePath(backupfilename));
        }

        FileOds book(pathandfilename);
        book.load();

        const auto table = book.getContent()->getBody()->getSpreadsheet()->getTable(0);

        table->removeEndingEmptyRows();

        const auto lastrow = table->getRow(table->getLastNonEmptyRow());

        auto p = filelist.value(filename);
        for (int i = 0; i < p->size(); i++) {
            auto row = p->at(i);
            row->restyleFromRow(lastrow);
            table->appendRow(row);
        }

        qInfo(logInfo()) << "Adding to prima nota: " << pathandfilename;

        ok = (ok && book.save());
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

    QMap<QString, QList<QSharedPointer<Tablerow>>*> filelist;

    int numberofrows = grid->model()->rowCount();

    int execrow = 0, crow = 0;

    for (crow = 0; crow < numberofrows; crow++) {

        QString modalita = grid->model()->data(grid->model()->index(crow, 0)).toString();
        QDate datascadenza = QDate::fromString(grid->model()->data(grid->model()->index(crow,1)).toString(),"dd/MM/yyyy");
        if ((!datascadenza.isValid()) || datascadenza.isNull()) {
            if(modalita == qfatturapa::paymentMethodType()["MP05"]) {
                datascadenza = dataemissione.addMonths(1);
            }
            else {
                datascadenza = dataemissione;
            }
        }

        QString cassa = grid->model()->data(grid->model()->index(crow,3)).toString();
        double importo = grid->model()->data(grid->model()->index(crow,2)).toFloat();

        if (cassa == "") {
            qInfo(logInfo()) << "Payment " << crow << ": scadenziario skipped, bank not selected, payment " << modalita;
            break;
        }

        execrow++;

        QString filename = filenametemplate.arg(months[datascadenza.month()-1], QString::number(datascadenza.year()));
        QList<QSharedPointer<Tablerow>>* rows;
        if (!filelist.contains(filename)) {
            rows = new QList<QSharedPointer<Tablerow>>();
            filelist.insert(filename, rows);
        }
        else {
            rows = filelist.value(filename);
        }

        auto row = QSharedPointer<Tablerow>(new Tablerow);
        row->appendCell(QSharedPointer<Tablecell>(new TablecellString(numero)));
        row->appendCell(QSharedPointer<Tablecell>(new TablecellString(entity)));
        row->appendCell(QSharedPointer<Tablecell>(new Tablecell(2)));
        row->appendCell(QSharedPointer<Tablecell>(new TablecellDate(datascadenza)));
        row->appendCell(QSharedPointer<Tablecell>(new Tablecell()));

        Tablecell* p1;
        if (cassa == bankAccount["IT33U0843051030000000180277"])
            p1 = new TablecellCurrency(Currency::EUR, importo);
        else
            p1 = new Tablecell();
        row->appendCell(QSharedPointer<Tablecell>(p1));
        row->appendCell(QSharedPointer<Tablecell>(new Tablecell()));

        Tablecell* p2;
        if (cassa == bankAccount["IT65Z0306951030615272528476"])
            p2 = new TablecellCurrency(Currency::EUR, importo);
        else
            p2 = new Tablecell();
        row->appendCell(QSharedPointer<Tablecell>(p2));
        row->appendCell(QSharedPointer<Tablecell>(new Tablecell()));

        Tablecell* p3;
        if (cassa == bankAccount["IT64U0503451861000000001728"])
            p3 = new TablecellCurrency(Currency::EUR, importo);
        else
            p3 = new Tablecell();
        row->appendCell(QSharedPointer<Tablecell>(p3));
        rows->append(row);

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

        //backupit
        if (m_setting->isExecute(Settings::Execute::backupfiles)) {
            QString backupfilename = filenametemplate.arg(QString::number(QDateTime::currentSecsSinceEpoch()), "");
            QString backuppath = QDir(m_setting->getPath(Settings::Execute::scadenziario)).filePath("old");
            QDir dir;
            if (dir.mkpath(backuppath))
                QFile::copy(pathandfilename, QDir(backuppath).filePath(backupfilename));
        }

        FileOds book(pathandfilename);
        book.load();

        const auto table = book.getContent()->getBody()->getSpreadsheet()->getTable(0);

        table->removeEndingEmptyRows();

        const auto lastrow = table->getRow(table->getLastNonEmptyRow());

        auto p = filelist.value(filename);
        for (int i = 0; i < p->size(); i++) {
            auto row = p->at(i);
            row->restyleFromRow(lastrow);
            table->appendRow(row);
        }

        qInfo(logInfo()) << "Adding to scadenziario " << pathandfilename;

        ok = (ok && book.save());
    }

    if (ok)
        return "N. " + QString::number(execrow) + " righe aggiunte a N. " + QString::number(filelist.count()) + " files scadenziario";
    else
        return "Errore nel salvataggio di almeno un file scadenziario";
}

void MainWindow::createActions()
{
    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Apri una fattura elettronica"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    const QIcon exitIcon = QIcon::fromTheme("application-exit");
    exitAct = new QAction(exitIcon, tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Esci dall'applicazione"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    executeAct = new QAction(tr("&Esegui"), this);
    executeAct->setStatusTip(tr("Esporta i dati nei fogli ods selezionati"));
    connect(executeAct, &QAction::triggered, this, &MainWindow::execute);
    executeAct->setEnabled(false);

    elencoFattureAct = new QAction(tr("In Elenco Fatture"), this);
    elencoFattureAct->setCheckable(true);
    elencoFattureAct->setChecked(m_setting->isExecute(Settings::Execute::elencofatture));
    connect(elencoFattureAct, &QAction::triggered, this, &MainWindow::toggleElencoFatture);

    mastrinoFornitoriAct = new QAction(tr("In Mastrini Fornitori"), this);
    mastrinoFornitoriAct->setCheckable(true);
    mastrinoFornitoriAct->setChecked(m_setting->isExecute(Settings::Execute::mastrinifornitori));
    connect(mastrinoFornitoriAct, &QAction::triggered, this, &MainWindow::toggleMastriniFornitori);

    primaNotaAct = new QAction(tr("In Prima Nota"), this);
    primaNotaAct->setCheckable(true);
    primaNotaAct->setChecked(m_setting->isExecute(Settings::Execute::primanota));
    connect(primaNotaAct, &QAction::triggered, this, &MainWindow::togglePrimaNota);

    scadenziarioAct = new QAction(tr("In Scadenziario"), this);
    scadenziarioAct->setCheckable(true);
    scadenziarioAct->setChecked(m_setting->isExecute(Settings::Execute::scadenziario));
    connect(scadenziarioAct, &QAction::triggered, this, &MainWindow::toggleScadenziario);

    backupAct = new QAction(tr("Backup dei file ods"), this);
    backupAct->setCheckable(true);
    backupAct->setChecked(m_setting->isExecute(Settings::Execute::backupfiles));
    connect(backupAct, &QAction::triggered, this, &MainWindow::toggleBackupFile);

    pathFattureElettronicheAct = new QAction(tr("Cartella Fatture Elettroniche"), this);
    pathFattureElettronicheAct->setStatusTip(tr("Imposta la cartella in cui si trovano le Fatture Elettroniche"));
    connect(pathFattureElettronicheAct, &QAction::triggered, this, &MainWindow::setPathFattureElettroniche);

    pathElencoFattureAct = new QAction(tr("Cartella Elenco Fatture"), this);
    pathElencoFattureAct->setStatusTip(tr("Imposta la cartella del Elenco Fatture"));
    connect(pathElencoFattureAct, &QAction::triggered, this, &MainWindow::setPathElencoFatture);

    pathMastrinoFornitoriAct = new QAction(tr("Cartella Mastrini Fornitori"), this);
    pathMastrinoFornitoriAct->setStatusTip(tr("Imposta la cartella dei Mastrini Fornitori"));
    connect(pathMastrinoFornitoriAct, &QAction::triggered, this, &MainWindow::setPathMastriniFornitori);

    pathPrimaNotaAct = new QAction(tr("Cartella Prima Nota"), this);
    pathPrimaNotaAct->setStatusTip(tr("Imposta la cartella della Prima Nota"));
    connect(pathPrimaNotaAct, &QAction::triggered, this, &MainWindow::setPathPrimaNota);

    pathScadenziarioAct= new QAction(tr("Cartella Scadenziario"), this);
    pathScadenziarioAct->setStatusTip(tr("Imposta la cartella dello Scadenziario"));
    connect(pathScadenziarioAct, &QAction::triggered, this, &MainWindow::setPathScadenziario);

    pathSaveSettingsAct = new QAction(tr("Salva Impostazioni Finestra"), this);
    pathSaveSettingsAct->setStatusTip(tr("Cambia lo stato della finestra dell'applicazione"));
    connect(pathSaveSettingsAct, &QAction::triggered, this, &MainWindow::saveWindowSettings);
    
    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    aboutQtAct = new QAction(tr("About &Qt"), qApp);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, &QAction::triggered, this, &QApplication::aboutQt);
}

void MainWindow::setupMenuBar()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    menuBar()->addSeparator();

    QMenu* processMenu = menuBar()->addMenu(tr("&Process"));
    processMenu->addAction(executeAct);
    processMenu->addSeparator();
    processMenu->addAction(elencoFattureAct);
    processMenu->addAction(mastrinoFornitoriAct);
    processMenu->addAction(primaNotaAct);
    processMenu->addAction(scadenziarioAct);
    processMenu->addAction(backupAct);
    menuBar()->addSeparator();

    QMenu* toolsMenu = menuBar()->addMenu(tr("&Impostazioni"));
    toolsMenu->addAction(pathFattureElettronicheAct);
    toolsMenu->addSeparator();
    toolsMenu->addAction(pathElencoFattureAct);
    toolsMenu->addAction(pathMastrinoFornitoriAct);
    toolsMenu->addAction(pathPrimaNotaAct);
    toolsMenu->addAction(pathScadenziarioAct);
    toolsMenu->addSeparator();
    toolsMenu->addAction(pathSaveSettingsAct);
    menuBar()->addSeparator();

    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Pronto"), 0);
}

void MainWindow::parseXMLFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::critical(this, tr("Procurans"),
                             tr("Non posso leggere il file %1")
                             .arg(QDir::toNativeSeparators(fileName)),
                             QMessageBox::Ok);
        qWarning(logWarning())  << QString("Non posso leggere il file %1:\n%2.").arg(QDir::toNativeSeparators(fileName), file.errorString());
        return;
    }

    statusBar()->showMessage(tr("Fattura in caricamento ..."), 2000);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    qfatturapa::ParsedInvoice parsed;
    QString parseError;
    const bool ok = qfatturapa::parseInvoice(file, parsed, &parseError);
    file.close();

    if (!ok) {
        QMessageBox::critical(this,
                              tr("Procurans"),
                              tr("Error parsing XML file %1")
                              .arg(QDir::toNativeSeparators(fileName)),
                              QMessageBox::Ok);
        qWarning(logWarning())  << QString("Cannot parse file %1:\n%2.").arg(QDir::toNativeSeparators(fileName), parseError);
    }
    else {
        billLoaded = true;
        qInfo(logInfo())  << "XML File parsed: " << fileName;
    }

    // UI-layer convention: when the invoice has no DatiPagamento, synthesise
    // a single placeholder row carrying ImportoTotaleDocumento so the
    // payments grid isn't empty. Not the parser library's responsibility.
    if(parsed.payments.isEmpty()) {
        qfatturapa::Payment p;
        p.importoPagamento = parsed.header.document.importoTotaleDocumento;
        parsed.payments.append(p);
    }

    this->addHeaderToUI(parsed.header);
    this->addPaymentsToUI(qfatturapa::toFlatMaps(parsed.payments), createPaymentsGridSchema());
    this->addDetailsToUI(qfatturapa::toFlatMaps(parsed.details),   createDetailsGridSchema());
    this->addSummaryToUI(qfatturapa::toFlatMaps(parsed.summary),   createSummaryGridSchema());

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    curFile = fileName;

    this->setWindowTitle(QString(QApplication::applicationName() + ": " + strippedName(curFile)));

    statusBar()->showMessage(tr("Fattura caricata"), 2000);
    executeAct->setEnabled(true);
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
    for (int _row = 0; _row < rowN; _row++) {   
        QList<QStandardItem*> rowtemp;
        //QStandardItem* rowtemp[columnN];
        bool ignoreRow = false;

        for (int column = 0; column < columnN; column++) {
            QString elementName = schema.at(column)->getElementName();
            QString value = "";
            if (elementName!="") {
                value = data.at(_row).value(elementName, "");
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

void MainWindow::addHeaderToUI(const qfatturapa::InvoiceHeader& headerData)
{
    const qfatturapa::SellerIdentity& seller = headerData.seller;
    const qfatturapa::DocumentInfo&   doc    = headerData.document;

    QLineEdit* qle = this->findChild<QLineEdit*>("entitaEdit");
    if (qle != nullptr) {
        if (!seller.denominazione.isEmpty())
            qle->setText(seller.denominazione);
        else
            qle->setText(seller.cognome + " " + seller.nome);
    }

    qle = this->findChild<QLineEdit*>("pivaEdit");
    if (qle != nullptr)
        qle->setText(seller.idPaese + seller.idCodice);

    qle = this->findChild<QLineEdit*>("indirizzoEdit");
    if (qle != nullptr)
        qle->setText(seller.indirizzo);

    qle = this->findChild<QLineEdit*>("capEdit");
    if (qle != nullptr)
        qle->setText(seller.cap);

    qle = this->findChild<QLineEdit*>("comuneEdit");
    if (qle != nullptr)
        qle->setText(seller.comune);

    QDateEdit* qde = this->findChild<QDateEdit*>("dataemissioneEdit");
    if (qde != nullptr) {
        const QDate date = QDate::fromString(doc.data, "yyyy-MM-dd");
        qde->setDate(date);
    }

    qle = this->findChild<QLineEdit*>("numeroEdit");
    if (qle != nullptr)
        qle->setText(doc.numero);

    // imponibileEdit / impostaEdit are populated by UpdateSummaryUI from
    // the summary grid totals; there's no ImponibileImporto / Imposta in
    // the header block. Leave those line edits to be updated by the
    // summary path.

    qle = this->findChild<QLineEdit*>("totaleEdit");
    if (qle != nullptr)
        qle->setText(doc.importoTotaleDocumento);
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
    schema.append(new GridSchemaField(QObject::tr("Modalita"), "ModalitaPagamento", &qfatturapa::paymentMethodType()));
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
    schema.append(new GridSchemaField(QObject::tr("Natura"), "Natura", &qfatturapa::naturaType()));

    return schema;
}

double MainWindow::computeTotal(const QList< QMap<QString,QString> >& data, QString xmlfield)
{
    double result = 0.0;
    int rowN = data.size();
    for (int _row = 0; _row < rowN; _row++) {
        result += data.at(_row).value(xmlfield).toDouble();
    }
    return result;
}

void MainWindow::addSummaryToUI(const QList< QMap<QString,QString> >& summaryData, QList<GridSchemaField*> summarySchema)
{
    QTableView *grid = ui->summaryTable;

    QStandardItemModel* itemmodel = createGridModel(summaryData, summarySchema);
    itemmodel->setObjectName("summaryModel");
    grid->setModel(itemmodel);

    bool ok = connect(
        itemmodel, &QStandardItemModel::dataChanged,
        this, &MainWindow::onSummaryChanged
    );

    Q_ASSERT(ok);

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

void MainWindow::LogOnModelChanged(QAbstractItemModel* model, const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    for (int row = topLeft.row(); row <= bottomRight.row(); row++) {
        for (int column = topLeft.column(); column <= bottomRight.column(); column++) {
            qInfo() << model->objectName() << " item at row " << row << " col " << column << " has changed to:" << model->data(model->index(row, column)).toString();
        }
    }
}

void MainWindow::onDetailsChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    QAbstractItemModel* model = ui->detailsTable->model();
    LogOnModelChanged(model, topLeft, bottomRight);
    UpdateSummaryUI(topLeft, bottomRight);
}

void MainWindow::onPaymentsChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    QAbstractItemModel* model = ui->paymentsTable->model();
    LogOnModelChanged(model, topLeft, bottomRight);
}

void MainWindow::onSummaryChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    QAbstractItemModel*  model = ui->summaryTable->model();
    LogOnModelChanged(model, topLeft, bottomRight);
}

void MainWindow::addDetailsToUI(const QList< QMap<QString,QString> >& detailsData, QList<GridSchemaField*> detailsSchema)
{
    QTableView *grid = ui->detailsTable;

    QStandardItemModel* itemmodel = createGridModel(detailsData, detailsSchema);
    itemmodel->setObjectName("detailsModel");
    grid->setModel(itemmodel);

    bool ok = connect(
        itemmodel, &QStandardItemModel::dataChanged,
        this, &MainWindow::onDetailsChanged
    );

    Q_ASSERT(ok);

    grid->resizeColumnsToContents();
    //grid->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}



void MainWindow::addPaymentsToUI(const QList< QMap<QString,QString> >& paymentData, QList<GridSchemaField*> paymentSchema)
{
    QTableView *grid = ui->paymentsTable;

    QStandardItemModel* itemmodel = createGridModel(paymentData, paymentSchema);
    itemmodel->setObjectName("paymentsModel");
    grid->setModel(itemmodel);

    bool ok = connect(
        itemmodel, &QStandardItemModel::dataChanged,
        this, &MainWindow::onPaymentsChanged
    );

    Q_ASSERT(ok);

    QStringList z(qfatturapa::paymentMethodType().values());
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
