#include "imageprovider.h"

#include <QIcon>
#include <QDebug>

ImageProvider::ImageProvider()
        : QQuickImageProvider(QQuickImageProvider::Pixmap) {
}

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *, const QSize &requestedSize) {
    QIcon icon = QIcon::fromTheme(id);

    if (requestedSize.isValid())
        return icon.pixmap(requestedSize);

    return icon.pixmap(128, 128);
}
