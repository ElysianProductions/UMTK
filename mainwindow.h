#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <mainwidget.h>
#include <QFrame>
#include <QStackedWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QFrame *main_frame;
    QStackedWidget *key_widget;

    MainWidget mainwidget;


    void initialize_main_window();
    void initialize_connections();
};
#endif // MAINWINDOW_H
