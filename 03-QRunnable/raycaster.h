#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <QRect>
#include <QImage>
#include <QSharedPointer>

class RayCaster
{
public:
    RayCaster();

    QImage render(const QSize &viewport, qreal dpi, QSharedPointer<std::atomic_flag> flag);
};

#endif // RAYCASTER_H
