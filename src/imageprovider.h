#pragma once

#include <QQuickImageProvider>

class ImageProvider : public QQuickImageProvider {
public:
    explicit ImageProvider();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) final;
};
