#include "mydialog.h"
#include "ui_mydialog.h"

MyDialog::MyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyDialog)
{
    ui->setupUi(this);


    processor = new videoprocessorInit();
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

    connect(ui->Ok,
            SIGNAL(clicked()),
            processor,
            SLOT(beginCameraInit()),
            Qt::DirectConnection);

    connect(ui->Cancel,
            SIGNAL(clicked()),
            this,
            SLOT(close()));


    processor->thread()->start();
}

MyDialog::~MyDialog()
{
    processor->stopVideo();
    processor->thread()->quit();
    processor->thread()->wait();
    delete ui;
}
