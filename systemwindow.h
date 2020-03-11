#ifndef SYSTEMWINDOW_H
#define SYSTEMWINDOW_H

#include <QMainWindow>
#include "newfiledialog.h"
#include "FileSystem.h"
namespace Ui {
class systemWindow;
}

class systemWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit systemWindow(int num_of_sectors, int size_of_sectors, QWidget *parent = nullptr);
    ~systemWindow();

private slots:
   // void on_Button_clicked();

    void on_saveButton_clicked();

    void on_displayButton_clicked();

    void on_quitButton_clicked();

    void on_readButton_clicked();

    void on_deleteButton_clicked();

private:
    Ui::systemWindow *ui;
    FileSystem * fsystem;
    friend class newFileDialog;
};

#endif // SYSTEMWINDOW_H
