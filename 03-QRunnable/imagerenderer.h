#ifndef IMAGERENDERER_H
#define IMAGERENDERER_H

#include <QImage>
#include <QObject>
#include <QSharedPointer>

#include <atomic>

class ImageRenderer : public QObject {
  Q_OBJECT
public:
  ImageRenderer();
  void renderAsync(const QSize &size, qreal dpi);
  ~ImageRenderer();

signals:
  void imageRendered(QImage image);

private:
  QSharedPointer<std::atomic_flag> m_cancelFlag;
};

#endif // IMAGERENDERER_H