#include "raycaster.h"

#include <QDebug>
#include <QVector3D>
#include <QImage>
#include <QRgb>
#include <QtMath>

#include <algorithm>

class Sphere {
public:
    Sphere(QVector3D center, qreal r) : m_center(std::move(center)), m_r(r) {

    }

    const QVector3D &center() const { return m_center; }

    const qreal &r() const { return m_r; }

private:
    QVector3D m_center;
    qreal m_r;
};

class Ray {
public:
    Ray(QVector3D start, QVector3D end) : m_start(std::move(start)), m_direction((end - m_start).normalized()) {
    }

    const QVector3D &start() const {
        return m_start;
    }


    const QVector3D &direction() const {
        return m_direction;
    }

private:
    QVector3D m_start;
    QVector3D m_direction;
};

RayCaster::RayCaster()
{

}

QImage RayCaster::render(const QSize &viewport, qreal dpi) {
    const int minDimension = std::min(viewport.width(), viewport.height());
    QRectF screenRectangle(-0.5 * (viewport.width() / qreal(minDimension)), -0.5 * ((viewport.height() / qreal(minDimension))), viewport.width() / qreal(minDimension), viewport.height() / qreal(minDimension));

    QSize scaledViewport = dpi * viewport;
    QImage image(scaledViewport, QImage::Format_RGB32);
    image.setDevicePixelRatio(dpi);

    const Sphere sphere(QVector3D(0, 0, 10), 0.5);
    const QVector3D directionalLight = Ray(QVector3D(1, 1, 0), QVector3D(0, 0, 0)).direction();

    const QPointF screenDelta(screenRectangle.width() / (scaledViewport.width() - 1), screenRectangle.height() / (scaledViewport.height() - 1));

    for (int y = 0; y < scaledViewport.height(); ++y) {
        auto scanLine = reinterpret_cast<uint *>(image.scanLine(y));
        for (int x = 0; x < scaledViewport.width(); ++x) {
            Ray ray(QVector3D(screenRectangle.x() + x * screenDelta.x(), screenRectangle.y() + y * screenDelta.y(), 0),
                    QVector3D(screenRectangle.x() + x * screenDelta.x(), screenRectangle.y() + y * screenDelta.y(), 1));

            const qreal a = ray.direction().x() * ray.direction().x() + ray.direction().y() * ray.direction().y() + ray.direction().z() * ray.direction().z();
            const qreal b = 2 * ray.direction().x() * (ray.start().x() - sphere.center().x()) +
                            2 * ray.direction().y() * (ray.start().y() - sphere.center().y()) +
                            2 * ray.direction().z() * (ray.start().z() - sphere.center().z());
            const qreal c = sphere.center().x() * sphere.center().x() + sphere.center().y() * sphere.center().y() + sphere.center().z() * sphere.center().z() +
                            ray.start().x() * ray.start().x() + ray.start().y() * ray.start().y() + ray.start().z() * ray.start().z() +
                            -2 * (sphere.center().x() * ray.start().x() + sphere.center().y() * ray.start().y() + sphere.center().z() * ray.start().z()) +
                            -(sphere.r() * sphere.r());

            const qreal delta = b * b - 4 * (a * c);

            if (delta >= 0) {
                const qreal tAtCollision = (-b - qSqrt(delta)) / (2 * a);
                const QVector3D pointOfCollision = ray.start() + tAtCollision * ray.direction();
                const QVector3D normalAtCollision = (pointOfCollision - sphere.center()).normalized();
                const qreal cosine = QVector3D::dotProduct(-directionalLight, normalAtCollision);

                int r = 255;

                *scanLine++ = qRgb(r * 0.3 + r * 0.7 * std::max(0., -cosine), 0, 0);
            } else {
                *scanLine++ = qRgb(0, 0, 0);
            }
        }
    }

    return image;
}
