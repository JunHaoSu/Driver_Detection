#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "videoprocessor.h"
#include <QThread>
#include <QDialog>
#include "mydialog.h"
#include <QtMultimedia/QSoundEffect>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    VideoProcessor *processor;
    MyDialog *camera_dialog;

private slots:
    void onAbout();
    void openCamera();


};

#endif // MAINWINDOW_H
