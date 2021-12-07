#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <QStackedWidget>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

#include "mainwidget.h"
#include "domainwidget.h"
#include "localwidget.h"
#include "editwidget.h"
#include "newuser.h"

#ifdef __linux__
    // Linux shit
#elif _WIN32
    #include "windows.h"
    #include "combaseapi.h"
    #include "shellapi.h"
    #include "LMaccess.h"
#elif __APPLE__
    // MAC shit
#endif


/*
    __linux__       Defined on Linux
    __sun           Defined on Solaris
    __FreeBSD__     Defined on FreeBSD
    __NetBSD__      Defined on NetBSD
    __OpenBSD__     Defined on OpenBSD
    __APPLE__       Defined on Mac OS X
    __hpux          Defined on HP-UX
    __osf__         Defined on Tru64 UNIX (formerly DEC OSF1)
    __sgi           Defined on Irix
    _AIX            Defined on AIX
    _WIN32          Defined on Windows
 */

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void launch_local_widget();
    void launch_server_widget();
    void launch_edit_user_widget();;
    void close_local_widget();
    void close_server_widget();
    void close_edit_user_widget();
    void initialize_main_window();
    void initialize_connections();
    void initialize_actions();
    void initialize_menus();
    void initialize_warning_banner();
    void create_local_user();
    void create_domain_user();

    QFrame *main_frame;
    QStackedWidget *key_widget;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *settingsMenu;
    QMenu *helpMenu;
    QAction *saveFileAction;
    QAction *saveFileAsAction;
    QAction *closeProgramAction;
    QAction *setProxyAction;
    QAction *setDisplayNameAction;
    QAction *helpMeAction;
    QMessageBox *warning_banner;



private:
    Ui::MainWindow *ui;
    MainWidget mainwidget;
    DomainWidget domainwidget;
    LocalWidget localwidget;
    EditWidget editwidget;
    NewUser user;

    struct Local_User
    {
        QString username;
        QString employee_name;
        QString email_address;
        QString password;
        QString account_enabled = "1";
        QString is_administrator = "0";

        QString date_created;
        QString time_created;
        QString created_by;
        QString final_command;


        QString whoami;
    };




    void clear_ui();
    void Automate();

};
#endif // MAINWINDOW_H
