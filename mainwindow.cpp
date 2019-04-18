#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    processor = new VideoProcessor();

    processor->moveToThread(new QThread(this));

    connect(processor->thread(),
            SIGNAL(started()),
            processor,
            SLOT(startVideo()));


    connect(processor->thread(),
            SIGNAL(finished()),
            processor,
            SLOT(deleteLater()));

    connect(processor,
            SIGNAL(inDisplay(QPixmap)),
            ui->inVideo,
            SLOT(setPixmap(QPixmap)));

    connect(processor,
            SIGNAL(outDisplay(QPixmap)),
            ui->outVideo,
            SLOT(setPixmap(QPixmap)));

    processor->thread()->start();

    //创健关于对话框,非模态对话框
    connect(ui->actionAbout,&QAction::triggered,[=](){
        QDialog *about = new QDialog (this);
        about->resize(120,40);
        about->show();
        about->setAttribute(Qt::WA_DeleteOnClose);
    });



}

MainWindow::~MainWindow()
{
    processor->stopVideo();
    processor->thread()->quit();
    processor->thread()->wait();

    delete ui;
}
