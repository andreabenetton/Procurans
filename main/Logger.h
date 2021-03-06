// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QDir>
#include <QScopedPointer>
#include <QTextStream>
#include <QDateTime>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(logDebug)
Q_DECLARE_LOGGING_CATEGORY(logInfo)
Q_DECLARE_LOGGING_CATEGORY(logWarning)
Q_DECLARE_LOGGING_CATEGORY(logCritical)

class Logger
{
public:
    static Logger *getInstance()
    {
        static Logger instance; // Guaranteed to be destroyed.
                                // Instantiated on first use.
        return &instance;
    }

    void Setup(QString appdir, QString logfilename);
    void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    Logger(Logger const&)          = delete;
    void operator=(Logger const&)  = delete;

private:
    QScopedPointer<QFile>   logFile;
    Logger();
};

#endif // LOGGER_H
