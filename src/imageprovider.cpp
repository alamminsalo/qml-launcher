#include "imageprovider.h"
#include <QIcon>
#include <QDebug>

ImageProvider::ImageProvider() :
    QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    QIcon icon = QIcon::fromTheme(id);
    QPixmap pixmap = icon.pixmap(128,128);

    return pixmap;
}
