#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <mainwidget.h>
#include <QFrame>
#include <QStackedWidget>
#include <QMenu>
#include <QAction>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();




    void save_command();
    void save_command_to_file();
    void close_program();

private:
    Ui::MainWindow *ui;

    QFrame *main_frame;
    QStackedWidget *key_widget;

    MainWidget mainwidget;


    void initialize_main_window();
    void initialize_connections();
    void initialize_actions();
    void initialize_menus();

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *settingsMenu;
    QMenu *helpMenu;



    QAction *saveFileAction;
    QAction *saveFileAsAction;
    QAction *closeProgramAction;

    QAction *addCloudClientsAction;
    QAction *addCloudPefixesAction;
    QAction *enableCloudCreationAction;

    QAction *helpMeAction;




};
#endif // MAINWINDOW_H
