#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  QObject::connect(&m_imageRenderer, &ImageRenderer::imageRendered, this,
                   &MainWindow::displayImage);
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
  m_imageRenderer.renderAsync(ui->viewport->size(), devicePixelRatio());
}

void MainWindow::displayImage(QImage image) {
  ui->viewport->setPixmap(QPixmap::fromImage(image));
}

#include "mainwindow.moc"