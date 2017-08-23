#pragma once
#include <QProcess>
#include <QVariant>
#include <QDebug>

class Process : public QProcess {
    Q_OBJECT

public:
    Process(QObject *parent = 0);

    Q_INVOKABLE void start(const QString &program, const QVariantList &arguments = QVariantList());

    Q_INVOKABLE QByteArray readAll();
};
