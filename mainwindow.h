#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <mainwidget.h>
#include <initalwidget.h>
#include <serverwidget.h>
#include <localwidget.h>
#include <QFrame>
#include <QStackedWidget>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

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
    void close_local_widget();
    void close_server_widget();

/*
    void validate_information();

    void save_command();
    void save_command_to_file();
    void close_program();
    void build_commands();
    void write_stamps();
*/


private:
    Ui::MainWindow *ui;
/*
    QFrame *main_frame;
    QStackedWidget *key_widget;
    MainWidget mainwidget;

    struct New_User
    {
        QString username = "";
        QString full_name = "";
        QString given_name = "";
        QString sur_name = "";
        QString display_name = "";
        QString email_address = "";
        QString password = "";
        QString cloud_prefix = "";
        QString sAMAccount = "";
        QString UPN = "";
        QString domain_name = "";
        QString enabled = "1";
        QString ou_clean = "";
        QString ou_path = "";
        QString template_user = "";
        QString user_groups = "";
        QString default_proxy = "";
        QString primary_proxy = "";
        QString secondary_proxy = "";
        QString middle_inital = "";

        QString user_command = "";
        QString GetGroups = "";
        QString SetProxy = "";
        QString ShiftOU = "";

        QString elevate = "";
    };


    void initialize_main_window();
    void initialize_connections();
    void initialize_actions();
    void initialize_menus();
    void initialize_warning_banner();
    void create_user(struct New_User s);
    void create_proxy_addresses(struct New_User s);
    void shift_ou(struct New_User s);
    void execute_command(QString param);

    void write_script(QString commands, QString _file);
    
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
*/

    void initialize_main_window();
    void initialize_connections();
    void initialize_actions();
    void initialize_menus();
    void initialize_warning_banner();
    void execute_command(QString param);
    void build_local_user();
    void create_local_user();

    QFrame *main_frame;
    QStackedWidget *key_widget;

    ServerWidget serverwidget;
    LocalWidget localwidget;
    InitalWidget initalwidget;
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



};
#endif // MAINWINDOW_H
