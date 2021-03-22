#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <mainwidget.h>
#include <QFrame>
#include <QStackedWidget>
#include <QMenu>
#include <QAction>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    void validate_information();
    void create_user();
    void set_proxy_addresses();
    void shift_ou();


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
    void initialize_warning_banner();



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
    QAction *loadCloudClients;
    QAction *loadCloudPrefixes;
    QAction *setProxyAction;
    QAction *setDisplayNameAction;

    QAction *helpMeAction;

    QMessageBox *warning_banner;



};
#endif // MAINWINDOW_H
