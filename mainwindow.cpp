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

#include <QtWidgets>
#include <QtXmlPatterns>
#include <QTableView>
#include <QDir>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logger.h"
#include "comboboxitemdelegate.h"
#include "gridschemaitem.h"

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

    naturaType["N1"] = "Escluse ex. art. 15";
    naturaType["N2"] = "Non soggette";
    naturaType["N3"] = "Non Imponibili";
    naturaType["N4"] = "Esenti";
    naturaType["N5"] = "Regime del margine";
    naturaType["N6"] = "Reverse charge";
    naturaType["N7"] = "IVA assolta in altro stato UE";

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
    QString fileName = QFileDialog::getOpenFileName
    (
          this,
          "Apri fattura",
          QDir::currentPath(),
          "Fattura in XML (*.xml)");
    if (!fileName.isEmpty())
        parseXMLFile(fileName);
    qInfo(logInfo())  << "File selected via dialog: " << fileName;
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Procurans"),
        tr("<p>An utility to import Italian electronic bill into odf files</p>"
           "<p>Copyright (C) 2019  Andrea Benetton lnki5538@pm.me</p>"
           "<p>This program is free software: you can redistribute it and/or "
           "modify it under the terms of the GNU Affero General Public License "
           "as published by the Free Software Foundation, either version 3 "
           "of the License, or (at your option) any later version.</p>"
           "<p>This program is distributed in the hope that it will be useful, "
           "but WITHOUT ANY WARRANTY; without even the implied warranty "
           "of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.</p>"
           "<p>See the GNU Affero General Public License for more details.</p>"
           "<p>You should have received a copy of the license along with this "
           "program. If not, see https://www.gnu.org/licenses/.</p>"));
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

void MainWindow::execute()
{
    if(m_setting->isExecute(Settings::Execute::elencofatture))
        executeElencoFatture();
    if(m_setting->isExecute(Settings::Execute::mastrinifornitori))
        executeMastriniFornitori();
    if(m_setting->isExecute(Settings::Execute::primanota))
        executePrimaNota();
    if(m_setting->isExecute(Settings::Execute::scadenziario))
        executeScadenziario();
}

//int MainWindow::findFirstStartingBlankRow(ods::Sheet *sheet)
//{
//    int countresult = sheet->CountRows();
//    int lastvalid = 0;

//    if(countresult>0) {
//        for (lastvalid = 0; lastvalid < countresult; lastvalid++) {
//            ods::Cell *cell = sheet->GetRow(lastvalid)->GetCell(0);

//            if (cell == nullptr) {
//                break;
//            }
//            else {
//                if (cell->ValueToString() == "") {
//                    break;
//                }
//            }
//        }
//    }
//    return lastvalid;
//}

void MainWindow::executeElencoFatture()
{
    QString filenametemplate = "Fatt. %1 %2%3.ods";
    QString filename;

    QTableView *grid = ui->summaryTable;

    QDateEdit* qde = this->findChild<QDateEdit*>("dataemissioneEdit");

    if (qde != nullptr) {
        filename = filenametemplate.arg(months[qde->date().month()-1], QString::number(qde->date().year()), "");

        QString pathandfilename = QDir(m_setting->getPath(Settings::Execute::elencofatture)).filePath(filename);

//        ods::Book* book;

//        if(QFileInfo::exists(pathandfilename) && QFileInfo(pathandfilename).isFile()) {
//            book = ods::Book::FromFile(pathandfilename);
//            QString backupfilename = filenametemplate.arg(months[qde->date().month()-1], QString::number(qde->date().year()), QString::number(QDateTime::currentSecsSinceEpoch()));
//            QString backuppath = QDir(m_setting->getPath(Settings::Execute::elencofatture)).filePath("old");
//            QDir dir;
//            if (dir.mkpath(backuppath)) {
//                dir.rename(pathandfilename, QDir(backuppath).filePath(backupfilename));
//            }
//        }
//        else {
//            book = ods::Book::New();
////            auto *spreadsheet = book->spreadsheet();
////            auto *sheet = spreadsheet->NewSheet("Sheet name");
////            auto *row = sheet->NewRowAt(0);
////            auto *cell0 = row->NewCellAt(0);
////            cell0->SetDouble(5.0);
//        }

//        auto *spreadsheet = book->spreadsheet();
//        auto *sheet = spreadsheet->GetSheet(0);


//        int lastvalid = findFirstStartingBlankRow(sheet);


//        for (int crow = 0; crow < grid->size().height(); crow++) {
//            auto *row = sheet->NewRowAt(lastvalid + crow - 1);

//            if(row != nullptr){
//                auto *cell0 = row->NewCellAt(0);

//                QLineEdit* qle = this->findChild<QLineEdit*>("entitaEdit");
//                if (qle != nullptr) {
//                    cell0->SetFirstString(qle->text());
//                }

//                //riferimento
//                auto *cell1 = row->NewCellAt(1);
//                cell1->SetFirstString(grid->model()->data(grid->model()->index(crow,3)).toString());

//                auto *cell2 = row->NewCellAt(2);

//                qle = this->findChild<QLineEdit*>("numeroEdit");
//                if (qle != nullptr) {
//                    cell2->SetFirstString(qle->text());
//                }

//                //imponibile
//                auto *cell3 = row->NewCellAt(3);
//                cell3->SetFirstString(grid->model()->data(grid->model()->index(crow,0)).toString());

//                //iva
//                auto *cell4 = row->NewCellAt(4);
//                cell4->SetFirstString(grid->model()->data(grid->model()->index(crow,2)).toString());

//                //totale

//            }
//        }

//        QFile file(pathandfilename);
//        book->Save(file);
    }
}

void MainWindow::executeMastriniFornitori()
{

}

void MainWindow::executePrimaNota()
{

}

void MainWindow::executeScadenziario()
{

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

    menuBar()->addSeparator();

    QMenu *toolsMenu = menuBar()->addMenu(tr("&Impostazioni"));

    QAction *pathElencoFattureAct = toolsMenu->addAction(tr("Cartella Elenco Fatture"), this, &MainWindow::setPathElencoFatture);
    pathElencoFattureAct->setStatusTip(tr("Imposta la cartella in cui sono presenti le sottocartelle contenenti le fatture fornitori"));

    QAction *pathMastrinoFornitoriAct = toolsMenu->addAction(tr("Cartella Mastrini Fornitori"), this, &MainWindow::setPathMastriniFornitori);
    pathMastrinoFornitoriAct->setStatusTip(tr("Cambia le impostazioni dell'applicazione"));

    QAction *pathPrimaNotaAct = toolsMenu->addAction(tr("Cartella Prima Nota"), this, &MainWindow::setPathPrimaNota);
    pathPrimaNotaAct->setStatusTip(tr("Cambia le impostazioni dell'applicazione"));

    QAction *pathScadenziarioAct= toolsMenu->addAction(tr("Cartella Scadenziario"), this, &MainWindow::setPathScadenziario);
    pathScadenziarioAct->setStatusTip(tr("Cambia le impostazioni dell'applicazione"));

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
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
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

    this->addHeaderToUI(headerData);
    this->addPaymentsToUI(paymentsData, createPaymentsGridSchema());
    this->addDetailsToUI(detailsData, createDetailsGridSchema());
    this->addSummaryToUI(summaryData, createSummaryGridSchema());

#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif

   /* if (!validateBill(fileContent.toUtf8())) {
        QMessageBox::warning(this, tr("Procurans"),
                             tr("Validation Fail %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }
    statusBar()->showMessage(tr("Fattura validata"), 2000);
*/

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
                                                         "ImportoPagamento" }, payment);
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
                                    const QList<GridSchemaItem*> &schema)
{
    int columnN = schema.size();
    int rowN = data.size();
    QStandardItemModel* model = new QStandardItemModel(rowN, columnN);

    for (int column = 0; column < columnN; column++) {
        model->setHeaderData(column, Qt::Horizontal, schema.at(column)->getColumnName());
    }

    int outRow = 0;
    for (int row = 0; row < rowN; row++) {    
        QStandardItem* rowtemp[columnN];
        bool ignoreRow = false;

        for (int column = 0; column < columnN; column++) {
            QString elementName = schema.at(column)->getElementName();
            QString value = "";
            if (elementName!="") {
                value = data.at(row).value(elementName, "");
            }
            rowtemp[column] = schema.at(column)->createGridItem(value);
            ignoreRow = (ignoreRow && schema.at(column)->getToBeIgnored());
        }

        if (!ignoreRow) {
            for (int column = 0; column < columnN; column++)
                model->setItem(outRow, column, rowtemp[column]);
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

QList<GridSchemaItem*> MainWindow::createDetailsGridSchema()
{
    QList<GridSchemaItem*> schema;
    schema.append(new GridSchemaItem(QObject::tr("Descrizione"), "Descrizione"));
    schema.append(new GridSchemaItem(QObject::tr("Quantita"), "Quantita", FloatColumn, 2));
    schema.append(new GridSchemaItem(QObject::tr("UdM"), "UnitaMisura", CStringColumn, 0, true));
    schema.append(new GridSchemaItem(QObject::tr("P.Unitario"), "PrezzoUnitario", FloatColumn, 2, true));
    schema.append(new GridSchemaItem(QObject::tr("P.Totale"), "PrezzoTotale", FloatColumn, 2, true));
    schema.append(new GridSchemaItem(QObject::tr("IVA"), "AliquotaIVA", FloatColumn, 0));

    return schema;
}

QList<GridSchemaItem*> MainWindow::createPaymentsGridSchema()
{
    QList<GridSchemaItem*> schema;
    schema.append(new GridSchemaItem(QObject::tr("Modalita"), "ModalitaPagamento", &paymentMethodType));
    schema.append(new GridSchemaItem(QObject::tr("Scadenza"), "DataScadenzaPagamento", DateColumn));
    schema.append(new GridSchemaItem(QObject::tr("Importo"), "ImportoPagamento", FloatColumn, 2));
    schema.append(new GridSchemaItem(QObject::tr("Banca"), ""));

    return schema;
}

QList<GridSchemaItem*> MainWindow::createSummaryGridSchema()
{
    QList<GridSchemaItem*> schema;
    schema.append(new GridSchemaItem(QObject::tr("Imponibile"), "ImponibileImporto", FloatColumn, 2));
    schema.append(new GridSchemaItem(QObject::tr("Aliquota"), "AliquotaIVA", FloatColumn, 2));
    schema.append(new GridSchemaItem(QObject::tr("Imposta"), "Imposta", FloatColumn, 2));
    schema.append(new GridSchemaItem(QObject::tr("Natura"), "Natura", &naturaType));

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

void MainWindow::addSummaryToUI(QList< QMap<QString,QString> >& summaryData, QList<GridSchemaItem*> summarySchema)
{
    QTableView *grid = ui->summaryTable;

    grid->setModel(createGridModel(summaryData, summarySchema));

    QStringList z(naturaType.values());
    z.sort(Qt::CaseInsensitive);
    ComboBoxItemDelegate* cbid = new ComboBoxItemDelegate(z, grid);

    grid->setItemDelegateForColumn(3, cbid);

    QLineEdit* qle = this->findChild<QLineEdit*>("imponibileEdit");
    if (qle != nullptr) {
        QString imponibile = QString::number(computeTotal(summaryData, "ImponibileImporto"));
        qle->setText(imponibile);
    }

    qle = this->findChild<QLineEdit*>("impostaEdit");
    if (qle != nullptr) {
        QString imposta = QString::number(computeTotal(summaryData, "Imposta"));
        qle->setText(imposta);
    }
}

void MainWindow::addDetailsToUI(QList< QMap<QString,QString> >& detailsData, QList<GridSchemaItem*> detailsSchema)
{
    QTableView *grid = ui->detailsTable;

    grid->setModel(createGridModel(detailsData, detailsSchema));

    grid->resizeColumnsToContents();
    //grid->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::addPaymentsToUI(QList< QMap<QString,QString> >& paymentData, QList<GridSchemaItem*> paymentSchema)
{
    QTableView *grid = ui->paymentsTable;

    grid->setModel(createGridModel(paymentData, paymentSchema));

    QStringList z(paymentMethodType.values());
    z.sort(Qt::CaseInsensitive);
    ComboBoxItemDelegate* cbid = new ComboBoxItemDelegate(z, grid);

    grid->setItemDelegateForColumn(0, cbid);

    QStringList q = { "", "Banca BPM", "Banca Intesa", "CRAC" };
    q.sort(Qt::CaseInsensitive);
    ComboBoxItemDelegate* cbid1 = new ComboBoxItemDelegate(q, grid);

    grid->setItemDelegateForColumn(3, cbid1);

}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
