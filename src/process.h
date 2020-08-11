#pragma once

#include <QProcess>
#include <QVariant>

class Process : public QProcess {
    Q_OBJECT

public:
    Process(QObject *parent = nullptr);

    Q_INVOKABLE void start(const QString &program, const QVariantList &arguments = {});
    Q_INVOKABLE QByteArray readAll();
};
