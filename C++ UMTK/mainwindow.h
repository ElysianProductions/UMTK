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
#include "templateuser.h"
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
    void elevate_and_execute(QString param);
    void shift_ou(QString command, QString OU);

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

    // EXPERIMENTAL
    void fill_forms(QString template_user, QString employee_name);

    // EXPERIMENTAL


private:
    Ui::MainWindow *ui;
    MainWidget mainwidget;
    DomainWidget domainwidget;
    LocalWidget localwidget;
    EditWidget editwidget;


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

    struct Domain_User
    {
        QString employe_name; // Full employee name
        QString given_name; //Employee first name (GivenName)
        QString surname; // Employee last name (SurName)
        QString userpname; // Employee user princilple name (UserPrincepleName)
        QString password; // Employee password(AccountPassword)
        QString display_name; // Employee display name (DisplayName)
        QString email_address; // Employee email address (EmailAddress)
        QString sam_name; //Employee Sam Account (SamAccountName)
        QString is_enabled = "1"; // Account is enabled (Enabled)
        QStringList proxy_addresses; // User proxyAddresses
        QString ou_clean; // OU cleaned up name
        QString ou_actual; // OU Distingusihed name that's actually used
        QString UPN; // UPN
        QString template_user; // TEmplate to copy;
        QString user_distinguished_name;

        QString date_created; // Date that the account was made.
        QString time_created; // Time of day the account was made.
        QString created_by; // Created by <insert username>.

        QString complete_command; // Finialized PS command for creation
        QString set_groups_command; // String that holds the prebuilt PS command for setting group membership
        QString set_OU_command; // String that holds the prebuilt PS command for moving the users OU.

    };

    struct ADUser
    {
        QString DistinguishedName;  // : CN=Maria Franz,OU=IT Staff,OU=Elysium,DC=Elysium,DC=local
        QString Enabled;            // : True
        QString GivenName;          // : Maria
        QString Name;               // : Maria Franz
        QString ObjectClass;        // : user
        QString ObjectGUID;         // : 12657ca8-8bce-4815-83d1-26496d6662a4
        QString SamAccountName;     // : Mfranz
        QString SID;                // : S-1-5-21-1585572130-2828728486-3327902743-1219
        QString Surname;            // : Franz
        QString UserPrincipalName;  // : Mfranz@Elysium.local
        QString MiddleName;         // : Set-ADUser ((Get-ADUser -Filter {Name -Like "the name"} -Properties SamAccountName).SameAccountName) -Replace OtherName $middleName
        QString Initials;           // : A. J
        QString Mail;               // : example@example.com
        QStringList ProxyAddresses; // : (Get-ADUser -Filter {Name -like "Maria Franz"} -Properties Proxyaddresses).Proxyaddresses
    };


    void clear_ui();

    // EXPERIMENTAL
    void Automate();
    // EXPERIMENTAL
};
#endif // MAINWINDOW_H
