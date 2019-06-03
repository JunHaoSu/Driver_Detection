#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    processor = new VideoProcessor();

    processor->moveToThread(new QThread(this));

    connect(processor->thread(),
            SIGNAL(finished()),
            processor,
            SLOT(deleteLater()));

    connect(ui->Stop,
            SIGNAL(clicked()),
            processor,
            SLOT(stopVideo()),
            Qt::DirectConnection);//由于程序死循环，并没有进入事件监听循环，所以使用这个参数强制调用槽函数

    connect(ui->Start,
            SIGNAL(clicked()),
            processor,
            SLOT(startVideo()));//由于暂停函数只是改变了状态，并不是死循环，此时程序是在监听事件的，所以不需要第五个参数


    connect(processor,
            SIGNAL(inDisplay(QPixmap)),
            ui->inVideo,
            SLOT(setPixmap(QPixmap)));

    connect(processor,
            SIGNAL(outDisplay(QPixmap)),
            ui->outVideo,
            SLOT(setPixmap(QPixmap)));



    processor->thread()->start();

    //创建关于对话框
     connect(ui->actionAbout,
             &QAction::triggered,
             this,
             &MainWindow::onAbout);

     //相机标定对话框
      connect(ui->actionOpenCamera,
              &QAction::triggered,
              this,
              &MainWindow::openCamera);

}

MainWindow::~MainWindow()
{
    processor->stopVideo();
    processor->thread()->quit();
    processor->thread()->wait();
    delete ui;
}


//自定义about槽函数
void MainWindow::onAbout()
{
    QString text = QString("<h1>驾驶员检测系统</h1><p><b>Version 1.0.0</b><br />"
            "Using OpenCV and Dlib</p>"
            "<p>Copyright © 2019 JunHao SU"
            "<a href='http://blog.chdbwtx.cn'>@sujunhao</a></p>"
            "<p>License: <a href='http://www.gnu.org/licenses/gpl-3.0.html'>"
            "GNU GENERAL PUBLIC LICENSE Version 3</a><br />"
            "Project Hosted at "
            "<a href='https://github.com/JunHaoSu/Driver_Detection'>"
            "GitHub</a></p>");
    QMessageBox::about(this, tr("About"), text);
}

void MainWindow::openCamera()
{
    camera_dialog = new MyDialog();
    camera_dialog->setAttribute(Qt::WA_DeleteOnClose);//关闭窗口后立即释放资源
    camera_dialog->setWindowTitle("相机初始位置标定");
    camera_dialog->show();
}
