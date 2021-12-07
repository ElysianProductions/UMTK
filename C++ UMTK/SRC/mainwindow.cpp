#include "mainwindow.h"
#include "./ui_mainwindow.h"


#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    main_frame = new QFrame(this);
    key_widget = new QStackedWidget();
    key_widget->addWidget(mainwidget.get_widget());
    key_widget->insertWidget(1, domainwidget.get_widget());
    key_widget->insertWidget(2, localwidget.get_widget());
    //key_widget->insertWidget(2, editwidget.get_widget());
    initialize_main_window();
    initialize_actions();
    initialize_menus();
    initialize_connections();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialize_main_window()
{
    setCentralWidget(key_widget);
    setWindowTitle("User Maintenance Tool Kit");
}

void MainWindow::initialize_connections()
{
      //connect(saveFileAction, &QAction::triggered, this, &MainWindow::save_command);
      connect(setDisplayNameAction, &QAction::triggered, [this] {if(setDisplayNameAction->isChecked()) { domainwidget.display_name_edit->show();} if(!setDisplayNameAction->isChecked()) {domainwidget.display_name_edit->hide();} });
      connect(setProxyAction, &QAction::triggered, [this] {if (setProxyAction->isChecked()) { domainwidget.primary_proxy_edit->show(); domainwidget.secondary_proxy_edit->show(); } if(!setProxyAction->isChecked()) {domainwidget.primary_proxy_edit->hide(); domainwidget.secondary_proxy_edit->hide(); } } );
      connect(setDisplayNameAction, &QAction::triggered, [this] { if(setDisplayNameAction->isChecked()) { domainwidget.display_name_edit->show(); } if(!setDisplayNameAction->isChecked()) { domainwidget.display_name_edit->hide(); } } );
      connect(mainwidget.server_button, &QPushButton::clicked, this, &MainWindow::launch_server_widget);
      connect(domainwidget.create_button, &QPushButton::clicked, this, &MainWindow::create_domain_user);
      connect(domainwidget.cancel_button, &QPushButton::clicked, this, &MainWindow::close_server_widget);

      connect(mainwidget.local_button, &QPushButton::clicked, this, &MainWindow::launch_local_widget);
      connect(localwidget.cancel_button, &QPushButton::clicked, this, &MainWindow::close_local_widget);
      connect(localwidget.create_button, &QPushButton::clicked, this, &MainWindow::create_local_user);

      connect(domainwidget.generate_button, &QPushButton::clicked, this, &MainWindow::Automate);

}

void MainWindow::initialize_actions()
 {
     saveFileAction = new QAction(tr("Save Command"));
     saveFileAction->setShortcuts(QKeySequence::New);
     saveFileAction->setStatusTip(tr("Click this to save your current command."));
     saveFileAction->setToolTip("Save your current working command to a StringList for future use.");


     saveFileAsAction = new QAction(tr("Save to file"));
     saveFileAsAction->setStatusTip(tr("Click this to save your current command to an existing ps script."));
     saveFileAsAction->setToolTip("Save your current working command to a new ps script.");

     closeProgramAction = new QAction(tr("Close program"));
     closeProgramAction->setStatusTip("Click this to close the program");
     closeProgramAction->setToolTip("Click to close the program.");

     setProxyAction = new QAction("Set aditonal proxy");
     setProxyAction->setToolTip("Check this to show a widget that will enable you to add a non-standard primary\nandsecondary proxy address.");
     setProxyAction->setStatusTip("Check this to show a widget that will enable you to add a non-standard primary\nandsecondary proxy address.");
     setProxyAction->setCheckable(true);

     setDisplayNameAction = new QAction("Set display name");
     setDisplayNameAction->setToolTip("Check this to enable a widget to modify the display name.");
     setDisplayNameAction->setStatusTip("Check this to enable a widget to modify the display name.");
     setDisplayNameAction->setCheckable(true);

     helpMeAction = new QAction(tr("Help"));
     helpMeAction->setToolTip("Click to view the help documentation.");
     helpMeAction->setToolTip("Click to view the help documentation.");

 }

void MainWindow::initialize_menus()
 {
        fileMenu = menuBar()->addMenu(tr("&File"));
        fileMenu->addAction(saveFileAction);
        fileMenu->addSeparator();
        fileMenu->addAction(saveFileAsAction);
        fileMenu->addSeparator();
        fileMenu->addAction(closeProgramAction);
        fileMenu->setToolTipsVisible(true);

        editMenu = menuBar()->addMenu(tr("&Edit"));



        settingsMenu = menuBar()->addMenu(tr("&Settings"));
        settingsMenu->addAction(setProxyAction);
        settingsMenu->addSeparator();
        settingsMenu->addAction(setDisplayNameAction);

        helpMenu = menuBar()->addMenu(tr("&Help"));
        helpMenu->addAction(helpMeAction);

        fileMenu->setToolTipsVisible(true);
        editMenu->setToolTipsVisible(true);
        settingsMenu->setToolTipsVisible(true);
        helpMenu->setToolTipsVisible(true);
 }

void MainWindow::launch_local_widget()
 {
     key_widget->setCurrentIndex(2);
 }

void MainWindow::launch_server_widget()
 {
     key_widget->setCurrentIndex(1);
 }

void MainWindow::launch_edit_user_widget()
 {

 }

void MainWindow::close_local_widget()
 {
     key_widget->setCurrentIndex(0);
 }

void MainWindow::close_server_widget()
 {
     key_widget->setCurrentIndex(0);
 }

void MainWindow::close_edit_user_widget()
 {

 }

void MainWindow::create_local_user()
 {
     Local_User luser;
     if(localwidget.username_edit->text().length() <= 0)
     {
         localwidget.username_edit->setStyleSheet("color: black; background-color: red");
         localwidget.username_edit->setPlaceholderText("VALUE CANNOT BE EMPTY");
     }
     else if(localwidget.username_edit->text().length() > 0)
     {
         localwidget.username_edit->setStyleSheet("color: black; background-color: white");
         luser.username = localwidget.username_edit->text();
     }

     if(localwidget.fullname_edit->text().length() <= 0)
     {
         localwidget.fullname_edit->setStyleSheet("color: black; background-color: red");
         localwidget.fullname_edit->setPlaceholderText("VALUE CANNOT BE EMPTY");
     }
     else if(localwidget.fullname_edit->text().length() > 0)
     {
         localwidget.fullname_edit->setStyleSheet("color: black; background-color: white");
         luser.employee_name = localwidget.fullname_edit->text();
     }

     if(localwidget.emailaddress_edit->text().length() <= 0)
     {
         localwidget.emailaddress_edit->setStyleSheet("color: black; background-color: red");
         localwidget.emailaddress_edit->setPlaceholderText("VALUE CANNOT BE EMPTY");
     }
     else if(localwidget.emailaddress_edit->text().length() > 0)
     {
         localwidget.emailaddress_edit->setStyleSheet("color: black; background-color: white");
         luser.email_address = localwidget.emailaddress_edit->text();
     }
     if(localwidget.password_edit->text().length() <= 0)
     {
         localwidget.password_edit->setStyleSheet("color: black; background-color: red");
         localwidget.password_edit->setPlaceholderText("VALUE CANNOT BE EMPTY");
     }
     else if(localwidget.password_edit->text().length() > 0)
     {
         localwidget.password_edit->setStyleSheet("color: black; background-color: white");
         luser.password = localwidget.password_edit->text();
     }

     if(localwidget.admin_checkbox->isChecked())
     {
         luser.is_administrator = "1";
     }
     else if(!localwidget.admin_checkbox->isChecked())
     {
         luser.is_administrator = "0";
     }

     if(luser.username.length() > 0 && luser.employee_name.length() > 0 && luser.email_address.length() > 0 && luser.password.length() > 0)
     {
         QString tmp = "net user /add " + luser.username + " " + luser.password;
         user.Execute(tmp);
     }
 }

void MainWindow::create_domain_user()
 {
    /*Domain_User duser;
    if(domainwidget.employee_name_edit->text().length() <= 0)
    {
        domainwidget.employee_name_edit->setStyleSheet("color: black; background-color: red");
        domainwidget.employee_name_edit->setPlaceholderText("VALUE CANNOT BE EMPTY");
    }
    else if(domainwidget.employee_name_edit->text() > 0)
    {
        duser.employe_name = domainwidget.employee_name_edit->text();
        duser.display_name = domainwidget.employee_name_edit->text();
        QStringList name = duser.employe_name.split(" ");
        duser.given_name = name.first();
        duser.surname = name.last();
    }
    if(domainwidget.user_edit->text().length() <= 0)
    {
        domainwidget.user_edit->setStyleSheet("color: black; background-color: red");
        domainwidget.user_edit->setPlaceholderText("VALUE CANNOT BE EMPTY");
    }
    else if(domainwidget.user_edit->text().length() > 0)
    {
        duser.sam_name = domainwidget.user_edit->text();
    }
    if(domainwidget.password_edit->text().length() <= 0)
    {
        domainwidget.password_edit->setStyleSheet("color: black; background-color: red");
        domainwidget.password_edit->setPlaceholderText("VALUE CANNOT BE EMPTY");
    }
    else if(domainwidget.password_edit->text().length() > 0)
    {
        duser.password = "\"" + domainwidget.password_edit->text() + "\"";
    }
    if(domainwidget.email_edit->text().length() <= 0)
    {
        domainwidget.email_edit->setStyleSheet("color: black; background-color: red");
        domainwidget.email_edit->setPlaceholderText("VALUE CANNOT BE EMPTY");
    }
    else if(domainwidget.email_edit->text().length() > 0)
    {
        duser.email_address = domainwidget.email_edit->text();
        duser.proxy_addresses << domainwidget.email_edit->text();
    }
    if(duser.employe_name.length() > 0 && duser.sam_name.length() > 0 && duser.password.length() > 0 && duser.email_address.length() > 0)
    {

        QString p = "$p = " + duser.password + "; $sec = $p | ConvertTo-SecureString -AsPlainText -Force; ";
        duser.ou_clean = domainwidget.ou_combo->currentText();


        duser.ou_actual = "\"" + domainwidget.OU_DN_Names.at(domainwidget.ou_combo->currentIndex()) + "\"";

        bool bad_chars = true;
        while(bad_chars)
        {
            if(duser.ou_actual.contains("\r"))
            {
                duser.ou_actual = duser.ou_actual.remove(QChar('\r'));
            }
            if(duser.ou_actual.contains("\n"))
            {
                duser.ou_actual = duser.ou_actual.remove(QChar('\n'));
            }
            if(!duser.ou_actual.contains("\r") && !duser.ou_actual.contains("\n"))
            {
                bad_chars = false;
            }
        }

        duser.template_user = domainwidget.template_user_combo->currentText();

        bad_chars = true;
        while(bad_chars)
        {
            if(duser.template_user.contains("\r"))
            {
                duser.template_user = duser.template_user.remove(QChar('\r'));
            }
            if(duser.template_user.contains("\n"))
            {
                duser.template_user = duser.template_user.remove(QChar('\n'));
            }
            if(!duser.template_user.contains("\r") && !duser.template_user.contains("\n"))
            {
                bad_chars = false;
            }
        }



        if(duser.UPN.length() <= 0)
        {
            duser.userpname = duser.sam_name + "@" + domainwidget.domain_name_combo->currentText();
            duser.userpname.remove(QChar('"'));
        }
        else if(duser.UPN.length() > 0)
        {
            duser.userpname = duser.sam_name + duser.UPN;
        }

        if(domainwidget.primary_proxy_edit->text().length() > 0)
        {
            duser.proxy_addresses << domainwidget.primary_proxy_edit->text();
        }
        else if(domainwidget.primary_proxy_edit->text().length() <= 0)
        {
            duser.proxy_addresses << "SMTP:" + duser.email_address;
        }

        if(domainwidget.secondary_proxy_edit->text().length() > 0)
        {
            duser.proxy_addresses << domainwidget.secondary_proxy_edit->text();
        }

        if(domainwidget.display_name_edit->isHidden() && domainwidget.display_name_edit->text().length() > 0)
        {
            duser.display_name = domainwidget.display_name_edit->text();
        }
        else if(!domainwidget.display_name_edit->isHidden() && domainwidget.display_name_edit->text().length() > 0)
        {
            duser.display_name = domainwidget.display_name_edit->text();
        }

        duser.complete_command = p + "New-ADUser -Name " + "\"" + duser.employe_name +"\"" + " -GivenName " + "\"" + duser.given_name + "\""
                + " -Surname " + "\"" + duser.surname + "\"" + " -AccountPassword $sec " + " -UserPrincipalName " + "\"" + duser.userpname + "\""
                " -DisplayName " + "\"" + duser.display_name + "\"" + " -EmailAddress " + "\"" + duser.email_address + "\"" + " -SamAccountName " +
                "\"" + duser.sam_name + "\"" + " -Enabled " + duser.is_enabled + "; exit";


        duser.set_groups_command = "$tmp = (Get-ADUser -Filter {Name -like \"" + duser.template_user + "\"}); "
                                   "$groups = (Get-ADUser $tmp -Properties MemberOf).MemberOf; $usr = \"" + duser.sam_name + "\"; "
                                   "Foreach ($group in $groups) {Add-ADGroupMember -Identity (Get-ADGroup $group).name -Members $usr}; exit ";

        warning_banner = new QMessageBox();
        QPushButton *okay_button = warning_banner->addButton(tr("Okay"), QMessageBox::ActionRole);
        QPushButton *cancel_button = warning_banner->addButton(tr("Cancel"), QMessageBox::ActionRole);
        warning_banner->setText("Confirm the following by clicking 'Okay'\nTo cancel and redo, click 'Cancel'");

        if (domainwidget.primary_proxy_edit->isHidden())
        {
            if(domainwidget.primary_proxy_edit->text().length() > 0 && domainwidget.secondary_proxy_edit->text().length() > 0)
            {
                if(domainwidget.display_name_edit->isHidden())
                {
                    warning_banner->setInformativeText("Employee Name: " + duser.employe_name +"\nUsername: " + duser.sam_name + "\nPassword: " + duser.password + "\nEmail Address: " + duser.email_address + "\nPrimary proxy: " + duser.proxy_addresses.first() + "\nSecondary Proxy: " + duser.proxy_addresses.last() + "; exit");
                }
                else if(!domainwidget.display_name_edit->isHidden() && domainwidget.display_name_edit->text().length() > 0)
                {
                    warning_banner->setInformativeText("Employee Name: " + duser.employe_name +"\nUsername: " + duser.sam_name + "\nPassword: " + duser.password + "\nEmail Address: " + duser.email_address + "\nDisplay name: " + duser.display_name + "\nPrimary proxy: " + duser.proxy_addresses.first() + "\nSecondary Proxy: " + duser.proxy_addresses.last() + "; exit");
                }
            }
            if(domainwidget.primary_proxy_edit->text().length() <= 0 && domainwidget.secondary_proxy_edit->text().length() > 0)
            {
                if(domainwidget.display_name_edit->isHidden())
                {
                    warning_banner->setInformativeText("Employee Name: " + duser.employe_name +"\nUsername: " + duser.sam_name + "\nPassword: " + duser.password + "\nEmail Address: " + duser.email_address + "\nPrimary proxy: " + "SMTP:" + duser.email_address + "\nSecondary Proxy: " + duser.proxy_addresses.last() + "; exit");
                }
                else if(!domainwidget.display_name_edit->isHidden() && domainwidget.display_name_edit->text().length() > 0)
                {
                    warning_banner->setInformativeText("Employee Name: " + duser.employe_name +"\nUsername: " + duser.sam_name + "\nPassword: " + duser.password + "\nEmail Address: " + duser.email_address + "\nDisplay name: " + duser.display_name + "\nPrimary proxy: " + "SMTP:" + duser.email_address + "\nSecondary Proxy: " + duser.proxy_addresses.last() + "; exit");
                }
            }
            if(domainwidget.primary_proxy_edit->text().length() > 0 && domainwidget.secondary_proxy_edit->text().length() <= 0)
            {
                if(domainwidget.display_name_edit->isHidden())
                {
                    warning_banner->setInformativeText("Employee Name: " + duser.employe_name +"\nUsername: " + duser.sam_name + "\nPassword: " + duser.password + "\nEmail Address: " + duser.email_address + "\nPrimary proxy: " + duser.proxy_addresses.first() + "; exit");
                }
                else if(!domainwidget.display_name_edit->isHidden() && domainwidget.display_name_edit->text().length() > 0)
                {
                    warning_banner->setInformativeText("Employee Name: " + duser.employe_name +"\nUsername: " + duser.sam_name + "\nPassword: " + duser.password + "\nEmail Address: " + duser.email_address + "\nDisplay name: " + duser.display_name + "\nPrimary proxy: " + duser.proxy_addresses.first() + "; exit");
                }
            }
            if(domainwidget.primary_proxy_edit->text().length() <= 0 && domainwidget.secondary_proxy_edit->text().length() <= 0)
            {
                if(domainwidget.display_name_edit->isHidden())
                {
                    warning_banner->setInformativeText("Employee Name: " + duser.employe_name +"\nUsername: " + duser.sam_name + "\nPassword: " + duser.password + "\nEmail Address: " + duser.email_address + "\nDefault proxy: " + "SMTP:" + duser.email_address + "; exit");
                }
                else if(!domainwidget.display_name_edit->isHidden() && domainwidget.display_name_edit->text().length() > 0)
                {
                    warning_banner->setInformativeText("Employee Name: " + duser.employe_name +"\nUsername: " + duser.sam_name + "\nPassword: " + duser.password + "\nEmail Address: " + duser.email_address + "\nDisplay name: " + duser.display_name + "\nDefault proxy: " + "SMTP:" + duser.email_address + "; exit");
                }
            }

        }
        else if(!domainwidget.primary_proxy_edit->isHidden())
        {
            if(domainwidget.primary_proxy_edit->text().length() > 0 && domainwidget.secondary_proxy_edit->text().length() > 0)
            {
                if(domainwidget.display_name_edit->isHidden())
                {
                    warning_banner->setInformativeText("Employee Name: " + duser.employe_name +"\nUsername: " + duser.sam_name + "\nPassword: " + duser.password + "\nEmail Address: " + duser.email_address + "\nPrimary proxy: " + duser.proxy_addresses.first() + "\nSecondary Proxy: " + duser.proxy_addresses.last() + "; exit");
                }
                else if(!domainwidget.display_name_edit->isHidden() && domainwidget.display_name_edit->text().length() > 0)
                {
                    warning_banner->setInformativeText("Employee Name: " + duser.employe_name +"\nUsername: " + duser.sam_name + "\nPassword: " + duser.password + "\nEmail Address: " + duser.email_address + "\nDisplay name: " + duser.display_name + "\nPrimary proxy: " + duser.proxy_addresses.first() + "\nSecondary Proxy: " + duser.proxy_addresses.last() + "; exit");
                }
            }
            if(domainwidget.primary_proxy_edit->text().length() <= 0 && domainwidget.secondary_proxy_edit->text().length() > 0)
            {
                if(domainwidget.display_name_edit->isHidden())
                {
                    warning_banner->setInformativeText("Employee Name: " + duser.employe_name +"\nUsername: " + duser.sam_name + "\nPassword: " + duser.password + "\nEmail Address: " + duser.email_address + "\nPrimary proxy: " + "SMTP:" + duser.email_address + "\nSecondary Proxy: " + duser.proxy_addresses.last() + "; exit");
                }
                else if(!domainwidget.display_name_edit->isHidden() && domainwidget.display_name_edit->text().length() > 0)
                {
                    warning_banner->setInformativeText("Employee Name: " + duser.employe_name +"\nUsername: " + duser.sam_name + "\nPassword: " + duser.password + "\nEmail Address: " + duser.email_address + "\nDisplay name: " + duser.display_name + "\nPrimary proxy: " + "SMTP:" + duser.email_address + "\nSecondary Proxy: " + duser.proxy_addresses.last() + "; exit");
                }
            }
            if(domainwidget.primary_proxy_edit->text().length() > 0 && domainwidget.secondary_proxy_edit->text().length() <= 0)
            {
                if(domainwidget.display_name_edit->isHidden())
                {
                    warning_banner->setInformativeText("Employee Name: " + duser.employe_name +"\nUsername: " + duser.sam_name + "\nPassword: " + duser.password + "\nEmail Address: " + duser.email_address + "\nPrimary proxy: " + duser.proxy_addresses.first() + "; exit");
                }
                else if(!domainwidget.display_name_edit->isHidden() && domainwidget.display_name_edit->text().length() > 0)
                {
                    warning_banner->setInformativeText("Employee Name: " + duser.employe_name +"\nUsername: " + duser.sam_name + "\nPassword: " + duser.password + "\nEmail Address: " + duser.email_address + "\nDisplay name: " + duser.display_name + "\nPrimary proxy: " + duser.proxy_addresses.first() + "; exit");
                }
            }
            if(domainwidget.primary_proxy_edit->text().length() <= 0 && domainwidget.secondary_proxy_edit->text().length() <= 0)
            {
                if(domainwidget.display_name_edit->isHidden())
                {
                    warning_banner->setInformativeText("Employee Name: " + duser.employe_name +"\nUsername: " + duser.sam_name + "\nPassword: " + duser.password + "\nEmail Address: " + duser.email_address + "\nDefault proxy: " + "SMTP:" + duser.email_address + "; exit");
                }
                else if(!domainwidget.display_name_edit->isHidden() && domainwidget.display_name_edit->text().length() > 0)
                {
                    warning_banner->setInformativeText("Employee Name: " + duser.employe_name +"\nUsername: " + duser.sam_name + "\nPassword: " + duser.password + "\nEmail Address: " + duser.email_address + "\nDisplay name: " + duser.display_name + "\nDefault proxy: " + "SMTP:" + duser.email_address + "; exit");
                }
            }
        }


        warning_banner->exec();
        if(warning_banner->clickedButton() == okay_button)
        {
            elevate_and_execute(duser.complete_command);

            elevate_and_execute(duser.set_groups_command);


            if(domainwidget.primary_proxy_edit->text().length() > 0 && domainwidget.secondary_proxy_edit->text().length() > 0)
            {
                elevate_and_execute("Set-ADUser -Identity " + QString("\"") + duser.sam_name + QString("\"") + " -Add @{Proxyaddresses = " + "\"SMTP:" + duser.proxy_addresses.first() + "\"}; exit");
                elevate_and_execute("Set-ADUser -Identity " + QString("\"") + duser.sam_name + QString("\"") +  " -Add @{Proxyaddresses = " + "\"smtp:" + duser.proxy_addresses.last() + "\"}; exit");
            }
            else if(domainwidget.primary_proxy_edit->text().length() > 0 && domainwidget.secondary_proxy_edit->text().length() <= 0)
            {
                elevate_and_execute("Set-ADUser -Identity " + QString("\"") + duser.sam_name + QString("\"") + " -Add @{Proxyaddresses = " + "\"SMTP:" + duser.proxy_addresses.first() +"\"}; exit");
            }
            else if(domainwidget.secondary_proxy_edit->text().length() > 0 && domainwidget.primary_proxy_edit->text().length() <= 0)
            {
                elevate_and_execute("Set-ADUser -Identity " + QString("\"") + duser.sam_name + QString("\"") +  " -Add @{Proxyaddresses = " + "\"SMTP:" + duser.email_address +"\"}; exit");
                elevate_and_execute("Set-ADUser -Identity " + QString("\"") + duser.sam_name + QString("\"") +  " -Add @{Proxyaddresses = " + "\"smtp:" + duser.proxy_addresses.last() +"\"}; exit");
            }
            else if(domainwidget.secondary_proxy_edit->text().length() <= 0 && domainwidget.primary_proxy_edit->text().length() <= 0)
            {

                elevate_and_execute("Set-ADUser -Identity \"" + duser.sam_name + "\" -Add @{Proxyaddresses = " + "\"SMTP:" + duser.email_address +"\"}");
            }

            shift_ou("$user = (Get-ADUser -Filter {SamAccountName -like \"" + duser.sam_name + "\"} | Select-Object -ExpandProperty DistinguishedName);", duser.ou_actual + "; exit");
        }
        else if (warning_banner->clickedButton() == cancel_button)
        {
            clear_ui();
        }


    }*/


     if(user.User_Exists(user.get_SamAccountName()) == "Yes")
     {
         domainwidget.informational->setTextColor("Red");
         domainwidget.informational->setText("WARNING: The username " + user.get_SamAccountName() + " already exists. Please select a new one.");
         domainwidget.user_edit->setText("");
     }
     if(domainwidget.password_edit->text().length() <= 0 || domainwidget.password_edit->text().length() < user.List_ActiveSP_length().toInt())
     {
         domainwidget.informational->setTextColor("Red");
         domainwidget.informational->setText("WARNING: The password " + domainwidget.password_edit->text() + " does not meet the minimum length requirements.\n Your password must be at least " + user.List_ActiveSP_length() + " characters long.");
         domainwidget.password_edit->setText("");
     }
     if(user.List_ActiveSP_Complexity() == "True" && user.Validate_Password(domainwidget.password_edit->text(), user.List_ActiveSP_length(), user.List_ActiveSP_Complexity()) == false)
     {
         domainwidget.informational->setTextColor("Red");
         domainwidget.informational->setText("WARNING: The password " + domainwidget.password_edit->text() + " does not meet the complexity requirements.\n Your password must have at least one of the following: \nOne upper case letter"
                                                                                                             "\n One lower case letter. \nOne number\nOne Special character");
         domainwidget.password_edit->setText("");
     }
     if(user.Employee_Name_Exists(user.List_All_Domain_Users(), user.get_Name()))
     {
         domainwidget.informational->setTextColor("Red");
         domainwidget.informational->setText("WARNING: We found a person with the same name as " + user.get_Name() + " Please change the new users name. Consider using their initials or some other variation of their name.");
         domainwidget.employee_name_edit->setText("");
     }
     // insert check for duplicate email address
     // insert check for duplicate upn
     if(user.get_UPN().length() <= 0)
     {
         domainwidget.informational->setTextColor("Red");
         domainwidget.informational->setText("WARNING: Your UPN appears to be empty. Please try again.");
     }
     if(!user.Validate_User_Status(domainwidget.template_user_combo->currentText()))
     {
         domainwidget.informational->setTextColor("Red");
         domainwidget.informational->setText("WARNING: The template user " + domainwidget.template_user_combo->currentText() + " is disabled. This means it likely has no groups. Please confirm that the user has the groups needed before creating this user.");
     }

     if(user.User_Exists(user.get_SamAccountName()) == "No" && domainwidget.password_edit->text().length() >= user.List_ActiveSP_length().toInt() && user.Validate_Password(domainwidget.password_edit->text(), user.List_ActiveSP_length(), user.List_ActiveSP_Complexity()) == true && user.Employee_Name_Exists(user.List_All_Domain_Users(), user.get_Name()) == false && user.get_UPN().length() > 0)
     {
         domainwidget.informational->setTextColor("Black");

         warning_banner = new QMessageBox();
         QPushButton *okay_button = warning_banner->addButton(tr("Okay"), QMessageBox::ActionRole);
         QPushButton *cancel_button = warning_banner->addButton(tr("Cancel"), QMessageBox::ActionRole);
         QString data = domainwidget.informational->toPlainText();
         QString tmp = user.get_Groups().join("\n");
         warning_banner->setText("Confirm the following by clicking 'Okay'\nTo cancel and redo, click 'Cancel'\n\n" + data + "\nGroups: \n" + tmp + "\nPassword: " + domainwidget.password_edit->text());
         warning_banner->exec();

         if(warning_banner->clickedButton() == okay_button)
         {
             if(domainwidget.primary_proxy_edit->text().length() > 0 && domainwidget.secondary_proxy_edit->text().length() > 0 && domainwidget.display_name_edit->text().length() > 0)
             {
                 QString p = "$p = " + QString("\"") + domainwidget.password_edit->text() + QString("\"") + "; $sec = $p | ConvertTo-SecureString -AsPlainText -Force; ";

                 user.Execute(p + "New-ADUser -Name " + "\"" + user.get_Name() + "\"" + " -GivenName " + "\"" + user.get_GivenName() + "\"" +
                              " -Surname " + "\"" + user.get_SurName() + "\"" + " -AccountPassword $sec -UserPrincipalName " + "\"" + user.get_UPN() + "\"" +
                              " -DisplayName " + "\"" + domainwidget.display_name_edit->text() + "\"" + " -EmailAddress " + "\"" + user.get_Mail() + "\"" + " -SamAccountName " +
                              "\"" + user.get_SamAccountName() + "\"" + " -Enabled 1; exit");

                 user.Execute("$tmp = (Get-ADUser -Filter {Name -like \"" + domainwidget.template_user_combo->currentText() + "\"}); "
                              "$groups = (Get-ADUser $tmp -Properties MemberOf).MemberOf; $usr = \"" + user.get_SamAccountName() + "\"; "
                              "Foreach ($group in $groups) {Add-ADGroupMember -Identity (Get-ADGroup $group).name -Members $usr}; exit ");

                 user.Execute("Set-ADUser -Identity \"" + user.get_SamAccountName() + "\" -Add @{Proxyaddresses = " + "\"SMTP:" + domainwidget.primary_proxy_edit->text() + "\"}");
                 user.Execute("Set-ADUser -Identity \"" + user.get_SamAccountName() + "\" -Add @{Proxyaddresses = " + "\"smtp:" + domainwidget.secondary_proxy_edit->text() + "\"}");
                 user.set_User_DN(user.List_User_DN(user.get_Name()));
                 user.Move_ADUser_Orgranizational_Unit(user.get_User_DN(), user.get_OU_DN());

                 // insert auto print to pdf

                 domainwidget.informational->setTextColor("Green");
                 domainwidget.informational->setText("SUCCESS - The following user has been created and a PDF named " + user.get_SamAccountName() + ".pdf has been generated and saved on your desktop <file path>.\nPresent it via encrypted email to the end user.\n\n\nEmployee name: " + user.get_Name() +"\nUsername: " + user.get_SamAccountName() + "\nEmail address: " + user.get_Mail() + "\nDisplay name: " + domainwidget.display_name_edit->text() +
                                                     "\nOrganizational unit: " + user.get_OU_CN() + "\nUser Principal Name: " + user.get_UPN() + "\nGroups: " + user.get_Groups().join(" , ") + "\nPassword: " + domainwidget.password_edit->text());

             }
             if(domainwidget.primary_proxy_edit->text().length() > 0 && domainwidget.secondary_proxy_edit->text().length() > 0 && domainwidget.display_name_edit->text().length() <= 0)
             {
                 QString p = "$p = " + QString("\"") + domainwidget.password_edit->text() + QString("\"") + "; $sec = $p | ConvertTo-SecureString -AsPlainText -Force; ";

                 user.Execute(p + "New-ADUser -Name " + "\"" + user.get_Name() + "\"" + " -GivenName " + "\"" + user.get_GivenName() + "\"" +
                              " -Surname " + "\"" + user.get_SurName() + "\"" + " -AccountPassword $sec -UserPrincipalName " + "\"" + user.get_UPN() + "\"" +
                              " -DisplayName " + "\"" + user.get_DisplayName() + "\"" + " -EmailAddress " + "\"" + user.get_Mail() + "\"" + " -SamAccountName " +
                              "\"" + user.get_SamAccountName() + "\"" + " -Enabled 1; exit");

                 user.Execute("$tmp = (Get-ADUser -Filter {Name -like \"" + domainwidget.template_user_combo->currentText() + "\"}); "
                              "$groups = (Get-ADUser $tmp -Properties MemberOf).MemberOf; $usr = \"" + user.get_SamAccountName() + "\"; "
                              "Foreach ($group in $groups) {Add-ADGroupMember -Identity (Get-ADGroup $group).name -Members $usr}; exit ");

                 user.Execute("Set-ADUser -Identity \"" + user.get_SamAccountName() + "\" -Add @{Proxyaddresses = " + "\"SMTP:" + domainwidget.primary_proxy_edit->text() + "\"}");
                 user.Execute("Set-ADUser -Identity \"" + user.get_SamAccountName() + "\" -Add @{Proxyaddresses = " + "\"smtp:" + domainwidget.secondary_proxy_edit->text() + "\"}");
                 user.set_User_DN(user.List_User_DN(user.get_Name()));
                 user.Move_ADUser_Orgranizational_Unit(user.get_User_DN(), user.get_OU_DN());

                 // insert auto print to pdf

                 domainwidget.informational->setTextColor("Green");
                 domainwidget.informational->setText("SUCCESS - The following user has been created and a PDF named " + user.get_SamAccountName() + ".pdf has been generated and saved on your desktop <file path>.\nPresent it via encrypted email to the end user.\n\n\nEmployee name: " + user.get_Name() +"\nUsername: " + user.get_SamAccountName() + "\nEmail address: " + user.get_Mail() + "\nDisplay name: " + user.get_DisplayName() +
                                                     "\nOrganizational unit: " + user.get_OU_CN() + "\nUser Principal Name: " + user.get_UPN() + "\nGroups: " + user.get_Groups().join(" , ") + "\nPassword: " + domainwidget.password_edit->text());
             }
             if(domainwidget.primary_proxy_edit->text().length() > 0 && domainwidget.secondary_proxy_edit->text().length() <= 0 && domainwidget.display_name_edit->text().length() > 0)
             {
                 QString p = "$p = " + QString("\"") + domainwidget.password_edit->text() + QString("\"") + "; $sec = $p | ConvertTo-SecureString -AsPlainText -Force; ";

                 user.Execute(p + "New-ADUser -Name " + "\"" + user.get_Name() + "\"" + " -GivenName " + "\"" + user.get_GivenName() + "\"" +
                              " -Surname " + "\"" + user.get_SurName() + "\"" + " -AccountPassword $sec -UserPrincipalName " + "\"" + user.get_UPN() + "\"" +
                              " -DisplayName " + "\"" + domainwidget.display_name_edit->text() + "\"" + " -EmailAddress " + "\"" + user.get_Mail() + "\"" + " -SamAccountName " +
                              "\"" + user.get_SamAccountName() + "\"" + " -Enabled 1; exit");

                 user.Execute(user.Clean_String("$tmp = (Get-ADUser -Filter {Name -like \"" + domainwidget.template_user_combo->currentText() + "\"}); "
                              "$groups = (Get-ADUser $tmp -Properties MemberOf).MemberOf; $usr = \"" + user.get_SamAccountName() + "\"; "
                              "Foreach ($group in $groups) {Add-ADGroupMember -Identity (Get-ADGroup $group).name -Members $usr}; exit "));

                 user.Execute("Set-ADUser -Identity \"" + user.get_SamAccountName() + "\" -Add @{Proxyaddresses = " + "\"SMTP:" + domainwidget.primary_proxy_edit->text() + "\"}");
                 user.set_User_DN(user.List_User_DN(user.get_Name()));
                 user.Move_ADUser_Orgranizational_Unit(user.get_User_DN(), user.get_OU_DN());

                 // insert auto print to pdf

                 domainwidget.informational->setTextColor("Green");
                 domainwidget.informational->setText("SUCCESS - The following user has been created and a PDF named " + user.get_SamAccountName() + ".pdf has been generated and saved on your desktop <file path>.\nPresent it via encrypted email to the end user.\n\n\nEmployee name: " + user.get_Name() +"\nUsername: " + user.get_SamAccountName() + "\nEmail address: " + user.get_Mail() + "\nDisplay name: " + domainwidget.display_name_edit->text() +
                                                     "\nOrganizational unit: " + user.get_OU_CN() + "\nUser Principal Name: " + user.get_UPN() + "\nGroups: " + user.get_Groups().join(" , ") + "\nPassword: " + domainwidget.password_edit->text());
             }
             if(domainwidget.primary_proxy_edit->text().length() <= 0 && domainwidget.secondary_proxy_edit->text().length() > 0 && domainwidget.display_name_edit->text().length() > 0)
             {
                 QString p = "$p = " + QString("\"") + domainwidget.password_edit->text() + QString("\"") + "; $sec = $p | ConvertTo-SecureString -AsPlainText -Force; ";

                 user.Execute(p + "New-ADUser -Name " + "\"" + user.get_Name() + "\"" + " -GivenName " + "\"" + user.get_GivenName() + "\"" +
                              " -Surname " + "\"" + user.get_SurName() + "\"" + " -AccountPassword $sec -UserPrincipalName " + "\"" + user.get_UPN() + "\"" +
                              " -DisplayName " + "\"" + domainwidget.display_name_edit->text() + "\"" + " -EmailAddress " + "\"" + user.get_Mail() + "\"" + " -SamAccountName " +
                              "\"" + user.get_SamAccountName() + "\"" + " -Enabled 1; exit");

                 user.Execute("$tmp = (Get-ADUser -Filter {Name -like \"" + domainwidget.template_user_combo->currentText() + "\"}); "
                              "$groups = (Get-ADUser $tmp -Properties MemberOf).MemberOf; $usr = \"" + user.get_SamAccountName() + "\"; "
                              "Foreach ($group in $groups) {Add-ADGroupMember -Identity (Get-ADGroup $group).name -Members $usr}; exit ");

                 user.Execute("Set-ADUser -Identity \"" + user.get_SamAccountName() + "\" -Add @{Proxyaddresses = " + "\"smtp:" + domainwidget.secondary_proxy_edit->text() + "\"}");
                 user.set_User_DN(user.List_User_DN(user.get_Name()));
                 user.Move_ADUser_Orgranizational_Unit(user.get_User_DN(), user.get_OU_DN());

                 // insert auto print to pdf

                 domainwidget.informational->setTextColor("Green");
                 domainwidget.informational->setText("SUCCESS - The following user has been created and a PDF named " + user.get_SamAccountName() + ".pdf has been generated and saved on your desktop <file path>.\nPresent it via encrypted email to the end user.\n\n\nEmployee name: " + user.get_Name() +"\nUsername: " + user.get_SamAccountName() + "\nEmail address: " + user.get_Mail() + "\nDisplay name: " + domainwidget.display_name_edit->text() +
                                                     "\nOrganizational unit: " + user.get_OU_CN() + "\nUser Principal Name: " + user.get_UPN() + "\nGroups: " + user.get_Groups().join(" , ") + "\nPassword: " + domainwidget.password_edit->text());

             }
             if(domainwidget.primary_proxy_edit->text().length() <= 0 && domainwidget.secondary_proxy_edit->text().length() <= 0 && domainwidget.display_name_edit->text().length() > 0)
             {
                 QString p = "$p = " + QString("\"") + domainwidget.password_edit->text() + QString("\"") + "; $sec = $p | ConvertTo-SecureString -AsPlainText -Force; ";

                 user.Execute(p + "New-ADUser -Name " + "\"" + user.get_Name() + "\"" + " -GivenName " + "\"" + user.get_GivenName() + "\"" +
                              " -Surname " + "\"" + user.get_SurName() + "\"" + " -AccountPassword $sec -UserPrincipalName " + "\"" + user.get_UPN() + "\"" +
                              " -DisplayName " + "\"" + domainwidget.display_name_edit->text() + "\"" + " -EmailAddress " + "\"" + user.get_Mail() + "\"" + " -SamAccountName " +
                              "\"" + user.get_SamAccountName() + "\"" + " -Enabled 1; exit");

                 user.Execute("$tmp = (Get-ADUser -Filter {Name -like \"" + domainwidget.template_user_combo->currentText() + "\"}); "
                              "$groups = (Get-ADUser $tmp -Properties MemberOf).MemberOf; $usr = \"" + user.get_SamAccountName() + "\"; "
                              "Foreach ($group in $groups) {Add-ADGroupMember -Identity (Get-ADGroup $group).name -Members $usr}; exit ");

                 user.Execute("Set-ADUser -Identity \"" + user.get_SamAccountName() + "\" -Add @{Proxyaddresses = " + "\"SMTP:" + user.get_Mail() +"\"}");
                 user.set_User_DN(user.List_User_DN(user.get_Name()));
                 user.Move_ADUser_Orgranizational_Unit(user.get_User_DN(), user.get_OU_DN());

                 // insert auto print to pdf

                 domainwidget.informational->setTextColor("Green");
                 domainwidget.informational->setText("SUCCESS - The following user has been created and a PDF named " + user.get_SamAccountName() + ".pdf has been generated and saved on your desktop <file path>.\nPresent it via encrypted email to the end user.\n\n\nEmployee name: " + user.get_Name() +"\nUsername: " + user.get_SamAccountName() + "\nEmail address: " + user.get_Mail() + "\nDisplay name: " + domainwidget.display_name_edit->text() +
                                                     "\nOrganizational unit: " + user.get_OU_CN() + "\nUser Principal Name: " + user.get_UPN() + "\nGroups: " + user.get_Groups().join(" , ") + "\nPassword: " + domainwidget.password_edit->text());


             }
             if(domainwidget.primary_proxy_edit->text().length() <= 0 && domainwidget.secondary_proxy_edit->text().length() <= 0 && domainwidget.display_name_edit->text().length() <= 0)
             {

                 QString p = "$p = " + QString("\"") + domainwidget.password_edit->text() + QString("\"") + "; $sec = $p | ConvertTo-SecureString -AsPlainText -Force; ";

                 user.Execute(p + "New-ADUser -Name " + "\"" + user.get_Name() + "\"" + " -GivenName " + "\"" + user.get_GivenName() + "\"" +
                              " -Surname " + "\"" + user.get_SurName() + "\"" + " -AccountPassword $sec -UserPrincipalName " + "\"" + user.get_UPN() + "\"" +
                              " -DisplayName " + "\"" + user.get_DisplayName() + "\"" + " -EmailAddress " + "\"" + user.get_Mail() + "\"" + " -SamAccountName " +
                              "\"" + user.get_SamAccountName() + "\"" + " -Enabled 1; exit");

                 user.Execute("$tmp = (Get-ADUser -Filter {Name -like \"" + domainwidget.template_user_combo->currentText() + "\"}); "
                              "$groups = (Get-ADUser $tmp -Properties MemberOf).MemberOf; $usr = \"" + user.get_SamAccountName() + "\"; "
                              "Foreach ($group in $groups) {Add-ADGroupMember -Identity (Get-ADGroup $group).name -Members $usr}; exit ");

                 user.Execute("Set-ADUser -Identity \"" + user.get_SamAccountName() + "\" -Add @{Proxyaddresses = " + "\"SMTP:" + user.get_Mail() +"\"}");
                 user.set_User_DN(user.List_User_DN(user.get_Name()));
                 user.Move_ADUser_Orgranizational_Unit(user.get_User_DN(), user.get_OU_DN());

                 // insert auto print to pdf

                 domainwidget.informational->setTextColor("Green");
                 domainwidget.informational->setText("SUCCESS - The following user has been created and a PDF named " + user.get_SamAccountName() + ".pdf has been generated and saved on your desktop <file path>.\nPresent it via encrypted email to the end user.\n\n\nEmployee name: " + user.get_Name() +"\nUsername: " + user.get_SamAccountName() + "\nEmail address: " + user.get_Mail() + "\nDisplay name: " + user.get_DisplayName() +
                                                     "\nOrganizational unit: " + user.get_OU_CN() + "\nUser Principal Name: " + user.get_UPN() + "\nGroups: " + user.get_Groups().join(" , ") + "\nPassword: " + domainwidget.password_edit->text());

             }
         }
         else if (warning_banner->clickedButton() == cancel_button)
         {
             clear_ui();
         }

     }

 }

void MainWindow::clear_ui()
 {
     domainwidget.employee_name_edit->setText("");
     domainwidget.user_edit->setText("");
     domainwidget.password_edit->setText("");
     domainwidget.email_edit->setText("");
     domainwidget.display_name_edit->setText("");
     domainwidget.primary_proxy_edit->setText("");
     domainwidget.secondary_proxy_edit->setText("");
 }

void MainWindow::Automate()
{
    if(domainwidget.employee_name_edit->text().length() > 0)
    {

        user.set_Name(domainwidget.employee_name_edit->text());

        QStringList Names = domainwidget.employee_name_edit->text().split(" ");
        user.set_GivenName(Names.first());
        user.set_SurName(Names.last());
        user.set_DisplayName(user.get_Name());
        if(Names.count() > 2)
        {
            user.set_OtherName(Names[2]);
            qDebug() << user.get_OtherName();
        }
        user.set_SamAccountName(Names.first().at(0).toUpper() + Names.last().toLower());
        user.set_Identifier(user.List_User_Identifier(user.List_Name(domainwidget.template_user_combo->currentText())));
        user.set_UPN(user.get_SamAccountName() + "@" + user.get_Identifier());
        user.set_Mail(user.get_UPN());
        user.set_Groups(user.List_User_Group_CNs(user.List_SamAccountName(domainwidget.template_user_combo->currentText())));
        user.set_GroupDNs(user.List_User_Group_DNs(user.List_SamAccountName(domainwidget.template_user_combo->currentText())));
        user.set_OU_DN(user.List_User_OU_DN(domainwidget.template_user_combo->currentText()));
        user.set_OU_CN(user.List_User_OU_CN(domainwidget.template_user_combo->currentText()));
        user.List_Password_Policy(user.List_Name(domainwidget.template_user_combo->currentText()));


        domainwidget.ou_combo->show();
        domainwidget.upn_combo->show();
        QStringList tmp_upn = user.List_All_UPNs();

        for(int i = 0; i < domainwidget.upn_combo->count(); ++i)
        {
            if(user.get_Identifier() == tmp_upn.at(i))
            {
                domainwidget.upn_combo->setCurrentIndex(i);
            }
        }

        QStringList tmp_ou_cn = user.List_All_OU_CNs();

        for(int i = 0; i < domainwidget.ou_combo->count(); ++i)
        {
            if(user.get_OU_CN() == tmp_ou_cn.at(i))
            {
                domainwidget.ou_combo->setCurrentIndex(i);
            }
        }

        domainwidget.employee_name_edit->show();
        domainwidget.employee_name_edit->setText(user.get_Name());
        domainwidget.user_edit->show();
        domainwidget.user_edit->setText(user.get_SamAccountName());
        domainwidget.password_edit->show();
        domainwidget.email_edit->show();
        domainwidget.email_edit->setText(user.get_Mail());
        domainwidget.create_button->show();



        domainwidget.informational->setText("Employee name: " + user.get_Name() +"\nUsername: " + user.get_SamAccountName() + "\nEmail address: " + user.get_Mail() + "\nDisplay name: " + user.get_DisplayName() +
                                            "\nOrganizational unit: " + user.get_OU_CN() + "\nUser Principal Name: " + user.get_UPN());
    }
    else if(domainwidget.employee_name_edit->text().length() <= 0)
    {
        // throw error

    }
}


