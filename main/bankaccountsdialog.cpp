// Copyright 2026 - 2026, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "bankaccountsdialog.h"

#include <QtWidgets>
#include <algorithm>

BankAccountsDialog::BankAccountsDialog(const QHash<QString, QString>& current,
                                       QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Conti bancari"));
    resize(640, 320);

    table_ = new QTableWidget(this);
    table_->setColumnCount(2);
    table_->setHorizontalHeaderLabels({ tr("IBAN"), tr("Banca") });
    table_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    table_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table_->verticalHeader()->setVisible(false);
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setEditTriggers(QAbstractItemView::DoubleClicked
                          | QAbstractItemView::EditKeyPressed
                          | QAbstractItemView::AnyKeyPressed);

    QList<QString> ibans = current.keys();
    std::sort(ibans.begin(), ibans.end());
    table_->setRowCount(ibans.size());
    int row = 0;
    for (const QString& iban : ibans) {
        table_->setItem(row, 0, new QTableWidgetItem(iban));
        table_->setItem(row, 1, new QTableWidgetItem(current.value(iban)));
        ++row;
    }

    QPushButton* addBtn    = new QPushButton(tr("Aggiungi"), this);
    QPushButton* removeBtn = new QPushButton(tr("Rimuovi"), this);
    connect(addBtn,    &QPushButton::clicked,
            this,      &BankAccountsDialog::addRow);
    connect(removeBtn, &QPushButton::clicked,
            this,      &BankAccountsDialog::removeSelectedRow);

    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QHBoxLayout* btnRow = new QHBoxLayout;
    btnRow->addWidget(addBtn);
    btnRow->addWidget(removeBtn);
    btnRow->addStretch();

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(table_);
    layout->addLayout(btnRow);
    layout->addWidget(buttons);
}

QHash<QString, QString> BankAccountsDialog::result() const
{
    QHash<QString, QString> out;
    for (int row = 0; row < table_->rowCount(); ++row) {
        const QTableWidgetItem* ibanItem = table_->item(row, 0);
        const QTableWidgetItem* nameItem = table_->item(row, 1);
        if (ibanItem == nullptr) continue;
        const QString iban = ibanItem->text().trimmed();
        if (iban.isEmpty()) continue;
        const QString name = nameItem != nullptr
                           ? nameItem->text().trimmed()
                           : QString();
        out.insert(iban, name);
    }
    return out;
}

void BankAccountsDialog::addRow()
{
    const int row = table_->rowCount();
    table_->insertRow(row);
    table_->setItem(row, 0, new QTableWidgetItem());
    table_->setItem(row, 1, new QTableWidgetItem());
    table_->setCurrentCell(row, 0);
    table_->editItem(table_->item(row, 0));
}

void BankAccountsDialog::removeSelectedRow()
{
    const int row = table_->currentRow();
    if (row >= 0) table_->removeRow(row);
}
