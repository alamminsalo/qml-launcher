#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QProcess>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QVariantMap>
#include <QIcon>

#include "imageprovider.h"
#include "process.h"

struct AppInfo {
    QString name;
    QString icon = "application";
    QString exec;
};

QVariantList apps() {
    QVariantList apps;

    QDir dir("/usr/share/applications");
    foreach (auto fn, dir.entryList(QStringList() << "*.desktop", QDir::Files)) {
       // qDebug() << "Reading" << dir.filePath(fn);
        QFile file(dir.filePath(fn));
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream in(&file);

            AppInfo app;

            bool foundDesktopEntry = false;

            while (!in.atEnd()) {
                QString line = in.readLine();

                if (line.trimmed().isEmpty())
                    continue;

                if (!foundDesktopEntry) {
                    if (line.contains("[Desktop Entry]"))
                        foundDesktopEntry = true;
                    continue;
                }
                else if (line.startsWith('[') && line.endsWith(']')) {
                    break;
                }

                QStringList values = line.split("=");
                QString name = values.takeFirst();
                QString value = QString(values.join('='));

                if (name == "Name") {
                    app.name = value;
                }

                if (name == "Icon") {
                    app.icon = value;
                    QIcon icon = QIcon::fromTheme(app.icon);
                    if (icon.isNull()) {
                        qDebug()<< "null icon:" << app.icon;
                    }
                }

                if (name == "Exec") {
                    app.exec = value.remove("\"").remove(QRegExp(" %."));
                }
            }

            apps.append(QStringList() << app.name << app.icon << app.exec);
        }
    }

    return apps;
}

int main(int argc, char *argv[])
{
    qputenv("QT_QUICK_CONTROLS_STYLE", "material");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.addImageProvider("icons", new ImageProvider());
    engine.rootContext()->setContextProperty("apps", apps());
    engine.rootContext()->setContextProperty("proc", new Process(&engine));
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
