#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "raycaster.h"

#include <QThread>

class RenderingWorker : public QObject {
  Q_OBJECT
public:
  RenderingWorker(QSize viewportSize, qreal dpi)
      : m_viewportSize(std::move(viewportSize)), m_dpi(dpi) {}

  void doWork() {
    RayCaster caster;
    m_result = caster.render(m_viewportSize, m_dpi, &m_cancel);
    emit done();
  }

  QImage *result() { return &m_result; }

  void cancel() { m_cancel = true; }

signals:
  void done();

private:
  QSize m_viewportSize;
  qreal m_dpi;
  QImage m_result;
  bool m_cancel = false;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

bool MainWindow::event(QEvent *event) {
  switch (event->type()) {
  case QEvent::Type::Resize: {
    ui->viewport->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->viewport->resize(this->geometry().size());
    emit resized();
    renderImage();
    break;
  }
  default:
    break;
  }
  return QMainWindow::event(event);
}

void MainWindow::renderImage() {
  auto *thread = new QThread();
  auto *worker = new RenderingWorker(ui->viewport->size(), devicePixelRatio());
  worker->moveToThread(thread);
  QObject::connect(thread, &QThread::started, worker, &RenderingWorker::doWork);
  QObject::connect(worker, &RenderingWorker::done, this,
                   &MainWindow::threadFinished);
  QObject::connect(worker, &RenderingWorker::done, thread, &QThread::quit);
  QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);
  QObject::connect(this, &MainWindow::resized, worker,
                   &RenderingWorker::cancel);
  thread->start();
}

void MainWindow::threadFinished() {
  auto *renderingWorker = static_cast<RenderingWorker *>(sender());
  auto deleteWorkerLater =
      qScopeGuard([renderingWorker]() { renderingWorker->deleteLater(); });
  if (renderingWorker->result()->size() !=
      ui->viewport->size() * devicePixelRatio())
    return;
  ui->viewport->setPixmap(QPixmap::fromImage(*renderingWorker->result()));
}

#include "mainwindow.moc"