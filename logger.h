#ifndef LOGGER_H
#define LOGGER_H

#include <QApplication>
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

    void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    Logger(Logger const&)          = delete;
    void operator=(Logger const&)  = delete;

private:
    QScopedPointer<QFile>   logFile;
    Logger();
};

#endif // LOGGER_H
