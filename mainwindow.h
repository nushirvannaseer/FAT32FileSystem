#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class FileSystem;
public:
    int size_of_sectors, num_of_sectors, num_of_sectors_in_pool;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
   // void on_buttonBox_clicked(QAbstractButton *button);

    int * on_allocateButton_clicked();

    void on_sizeSpinBox_valueChanged(int arg1);

    void on_numSpinBox_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
