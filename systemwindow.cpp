#include "systemwindow.h"
#include "ui_systemwindow.h"
#include "mainwindow.h"
#include "node.h"
#include "linkedList.h"
#include "myBlock.h"
#include "FileSystem.h"
#include "myFile.h"
#include "QDialog"
#include "QMessageBox"
#include <string>
#include <QInputDialog>
#include <QDebug>
#include <iomanip>

systemWindow::systemWindow( int num_of_sectors, int size_of_sectors,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::systemWindow)
{
    ui->setupUi(this);
    fsystem= new FileSystem(num_of_sectors, size_of_sectors);
    QString used;
    used=QString::number(fsystem->num_of_sectors_in_pool);
    used.append(" Sectors free of ");
    used.append(QString::number(num_of_sectors));
    used.append(" Sectors");
    ui->label_2->setText(used);
}

systemWindow::~systemWindow()
{
    delete ui;
}


void systemWindow::on_saveButton_clicked()
{
    QInputDialog* fnameBox= new QInputDialog(this);
    bool* ok_pressed= new bool(true);
    QString name=fnameBox->getText(this, "Enter File Name", "Name",QLineEdit::Normal, QString(), ok_pressed);
    if(*ok_pressed==true)
    {
        QInputDialog* fDataBox= new QInputDialog(this);
        bool* ok_pressed2= new bool(true);
        QString data=fnameBox->getMultiLineText(this, "Enter the file contents", "Contents", QString(), ok_pressed2);
        if(*ok_pressed2)
        {
            std::string fdata=data.toStdString();
            const char* myfdata=new char[fdata.size()+1];
            myfdata=fdata.c_str();
            char*hdata=new char[fdata.size()+1];
            strcpy(hdata, myfdata);
            if(fsystem->saveFile(name, hdata, fdata.size())==true)
            {
                // qDebug()<<"File save success"<<endl;
                QMessageBox succbox(this);
                succbox.information(this, "File Status", "File saved successfully!");

                int percent=fsystem->num_of_sectors_in_pool*fsystem->size_of_sectors*100;
                percent=percent/(fsystem->num_of_sectors*fsystem->size_of_sectors);
                ui->diskUsedBar->setValue(100-percent);

                QString used;
                used=QString::number(fsystem->num_of_sectors_in_pool);
                used.append(" Sectors free of ");
                used.append(QString::number(fsystem->num_of_sectors));
                used.append(" Sectors");
                ui->label_2->setText(used);
            }

            else
            {
                QMessageBox failbox(this);
                failbox.critical(this, "File Status", "Not enough space on disk!");
                //qDebug()<<"File not saved"<<endl;
            }
        }

    }

}

void systemWindow::on_displayButton_clicked()
{
    node<myFile> * iterator=fsystem->Files.head->next;
    QString file_list;

    while(iterator!=fsystem->Files.tail)
    {
        file_list.append(iterator->data.name);
        file_list.append("   ");
        file_list.append(QString::number(iterator->data.size));
        file_list.append(" Bytes\n");

        iterator=iterator->next;
    }

    QMessageBox * list_box= new QMessageBox(this);
    list_box->information(this, "Files on Disk", file_list);
    //list_box->show();
}

void systemWindow::on_quitButton_clicked()
{
    QMessageBox * confirm= new QMessageBox(this);
    confirm->setDefaultButton(QMessageBox::No);
    if(confirm->question(this, "Confirm Exit", "Are you sure you want to quit?")==QMessageBox::Yes)
    {
        this->close();
    }
}

void systemWindow::on_readButton_clicked()
{
    QInputDialog* fnameBox= new QInputDialog(this);
    bool * ok_pressed= new bool();
    QString name=fnameBox->getText(this, "Enter File Name", "Name",QLineEdit::Normal, QString(), ok_pressed);
    if(*ok_pressed==true)
    {
        QString result;
        if(fsystem->readFile(name,result)==true)
        {
            QMessageBox msg;
            msg.information(this, "File contents", result);
        }
        else
        {
            QMessageBox msg;
            msg.critical(this, "Invalid file name", "File does not exist!");
        }
    }
}

void systemWindow::on_deleteButton_clicked()
{
    QInputDialog* fnameBox= new QInputDialog(this);
    bool * ok_pressed= new bool();
    QString name=fnameBox->getText(this, "Enter File Name", "Name",QLineEdit::Normal, QString(), ok_pressed);
    if(*ok_pressed==true)
    {
        QString result;
        if(fsystem->deleteFile(name)==true)
        {
            QMessageBox msg;
            msg.information(this, "Deletion Status", "File deleted successfully");
            int percent=fsystem->num_of_sectors_in_pool*fsystem->size_of_sectors*100;
            percent=percent/(fsystem->num_of_sectors*fsystem->size_of_sectors);
            ui->diskUsedBar->setValue(100-percent);

            QString used;
            used=QString::number(fsystem->num_of_sectors_in_pool);
            used.append(" Sectors free of ");
            used.append(QString::number(fsystem->num_of_sectors));
            used.append(" Sectors");
            ui->label_2->setText(used);
        }
        else
        {
            QMessageBox msg;
            msg.critical(this, "Invalid file name", "File does not exist!");
        }
    }

}


