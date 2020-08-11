#include "process.h"

#include <QDebug>

Process::Process(QObject *parent)
        : QProcess(parent) {
}

void Process::start(const QString &program, const QVariantList &arguments) {
    QStringList args;

    qDebug() << "Running" << program;
    // convert QVariantList from QML to QStringList for QProcess

    for (const auto &arg : arguments)
        args << arg.toString();

    //unused args

    QProcess::startDetached(program);
}

QByteArray Process::readAll() {
    return QProcess::readAll();
}
