#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "raycaster.h"

#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::Type::Resize:
    {
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

void MainWindow::renderImage()
{
    RayCaster caster;
    QImage image = caster.render(ui->viewport->size(), devicePixelRatio());
    ui->viewport->setPixmap(QPixmap::fromImage(image));
}

#include "mainwindow.moc"