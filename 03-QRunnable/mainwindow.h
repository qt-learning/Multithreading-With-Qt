#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "imagerenderer.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool event(QEvent *event) override;

signals:
    void resized();

private:
    void renderImage();
    void displayImage(QImage image);

    Ui::MainWindow *ui;
    ImageRenderer m_imageRenderer;
};
#endif // MAINWINDOW_H
