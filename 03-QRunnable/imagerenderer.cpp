#include "imagerenderer.h"
#include "raycaster.h"

#include <QThreadPool>
#include <algorithm>

ImageRenderer::ImageRenderer() = default;

ImageRenderer::~ImageRenderer() {
  if (m_cancelFlag)
    m_cancelFlag->test_and_set();
}

void ImageRenderer::renderAsync(const QSize &size, qreal dpi) {
  if (m_cancelFlag)
    m_cancelFlag->test_and_set();

  m_cancelFlag.reset(new std::atomic_flag());
  QThreadPool::globalInstance()->start(
      QRunnable::create([this, cancelFlag = m_cancelFlag, size, dpi]() {
        RayCaster rayCaster;
        auto image = rayCaster.render(size, dpi, cancelFlag);
        if (!cancelFlag->test()) {
          emit imageRendered(image);
        }
      }));
}