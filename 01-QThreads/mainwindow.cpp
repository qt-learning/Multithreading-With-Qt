#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "raycaster.h"
#include <thread>

#include <QThread>

class RenderingThread: public QThread {
public:
   RenderingThread(QSize viewportSize, qreal dpi)
       : m_viewportSize(std::move(viewportSize)), m_dpi(dpi)
    {
        // Constructor runs on main thread
        qDebug() << QThread::currentThreadId() << " - RenderingThread() constructor";  // [1]
    }

    void run() override {
        // This part runs on the actual separate thread
        qDebug() << QThread::currentThreadId() << " - RenderingThread::run()";  // [2]
        RayCaster caster;
        m_result = caster.render(m_viewportSize, m_dpi);
    }

    const QImage& result() { return m_result; }

private:
    QSize m_viewportSize;
    qreal m_dpi;
    QImage m_result;
};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Type::Resize: {
        ui->viewport->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        ui->viewport->resize(this->geometry().size());
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
    auto *thread = new RenderingThread(ui->viewport->size(), devicePixelRatio());
    QObject::connect(thread, &QThread::finished, this, &MainWindow::threadFinished);
    thread->start();
}

void MainWindow::threadFinished() {
    auto *renderingThread = static_cast<RenderingThread *>(sender());
    const QImage& result = renderingThread->result();
     if (result.size() != ui->viewport->size() * devicePixelRatio())
        return;

    ui->viewport->setPixmap(QPixmap::fromImage(result));
}

