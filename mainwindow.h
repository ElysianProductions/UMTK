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

    void save_command();
    void save_command_to_file();
    void close_program();
    void build_commands();
    void write_stamps();

private:
    Ui::MainWindow *ui;

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
