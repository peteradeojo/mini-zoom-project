#include "joincallwindow.h"
#include "ui_joincallwindow.h"

JoinCallWindow::JoinCallWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::JoinCallWindow)
{
    ui->setupUi(this);
}

JoinCallWindow::~JoinCallWindow()
{
    delete ui;
}
