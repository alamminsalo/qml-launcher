#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QProcess>
#include <QFile>
#include <QVariantMap>
#include <QIcon>
#include <QDirIterator>
#include <QSettings>

#include "imageprovider.h"
#include "process.h"

struct AppInfo {
    QString name;
    QString icon{"application"};
    QString exec;
};

constexpr auto DESKTOP_FILE_SYSTEM_DIR = "/usr/share/applications";
constexpr auto DESKTOP_FILE_USER_DIR = "%1/.local/share/applications";
constexpr auto DESKTOP_ENTRY_STRING = "Desktop Entry";

class SettingsGroupRaii {
public:
    SettingsGroupRaii(QSettings &settings, const QString &groupName)
            : m_settings(settings) {
        m_settings.beginGroup(groupName);
    }

    ~SettingsGroupRaii() {
        m_settings.endGroup();
    }

private:
    QSettings &m_settings;
};

QVariantList createAppsList(const QString &path) {
    QDirIterator it(path, {"*.desktop"}, QDir::NoFilter, QDirIterator::Subdirectories);
    QVariantList ret;

    while (it.hasNext()) {
        const auto filename = it.next();
        QSettings desktopFile(filename, QSettings::IniFormat);

        if (!desktopFile.childGroups().contains(DESKTOP_ENTRY_STRING))
            continue;
        
        SettingsGroupRaii raii(desktopFile, DESKTOP_ENTRY_STRING);

        if (desktopFile.contains("NoDisplay"))
            if (desktopFile.value("NoDisplay").toBool() == 1)
                continue;
        
        AppInfo app;
        app.exec = desktopFile.value("Exec").toString().remove("\"").remove(QRegExp(" %."));
        app.icon = desktopFile.value("Icon", "application").toString();
        app.name = desktopFile.value("Name").toString();

        ret.append(QStringList{app.name, app.icon, app.exec});
    }

    return ret;
}

QVariantList apps() {
    QVariantList ret;
    ret.append(createAppsList(DESKTOP_FILE_SYSTEM_DIR));
    ret.append(createAppsList(QString(DESKTOP_FILE_USER_DIR).arg(QDir::homePath())));
    return ret;
}

int main(int argc, char *argv[]) {
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
