#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FileSystem.h"
#include "systemwindow.h"

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

int* MainWindow::on_allocateButton_clicked()
{
    this->close();
    systemWindow* newWind= new systemWindow(num_of_sectors, size_of_sectors, this);
    newWind->show();
}

void MainWindow::on_sizeSpinBox_valueChanged(int arg1)
{
    size_of_sectors=arg1;
}

void MainWindow::on_numSpinBox_valueChanged(int arg1)
{
    num_of_sectors=num_of_sectors_in_pool=arg1;
}
