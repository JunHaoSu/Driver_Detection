#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>
#include <QThread>
#include "videoprocessorinit.h"
namespace Ui {
class MyDialog;
}

class MyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MyDialog(QWidget *parent = nullptr);
    ~MyDialog();

private:
    Ui::MyDialog *ui;
    videoprocessorInit *processor;
};

#endif // MYDIALOG_H
