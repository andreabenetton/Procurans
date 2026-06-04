// Copyright 2026 - 2026, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef BANKACCOUNTSDIALOG_H
#define BANKACCOUNTSDIALOG_H

#include <QDialog>
#include <QHash>
#include <QString>

QT_BEGIN_NAMESPACE
class QTableWidget;
QT_END_NAMESPACE

class BankAccountsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BankAccountsDialog(const QHash<QString, QString>& current,
                                QWidget* parent = nullptr);

    // Snapshot of the table at OK time, IBAN -> bank display name. Rows
    // with an empty IBAN are dropped; the bank name may be empty.
    QHash<QString, QString> result() const;

private slots:
    void addRow();
    void removeSelectedRow();

private:
    QTableWidget* table_;
};

#endif // BANKACCOUNTSDIALOG_H
