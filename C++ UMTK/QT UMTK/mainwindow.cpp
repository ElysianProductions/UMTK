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
    key_widget->insertWidget(3, editwidget.get_widget());
    key_widget->insertWidget(4, disableuser.get_widget());
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
      //connect(setDisplayNameAction, &QAction::triggered, [this] {if(setDisplayNameAction->isChecked()) { domainwidget.display_name_edit->show();} if(!setDisplayNameAction->isChecked()) {domainwidget.display_name_edit->hide();} });
      //connect(setProxyAction, &QAction::triggered, [this] {if (setProxyAction->isChecked()) { domainwidget.primary_proxy_edit->show(); domainwidget.secondary_proxy_edit->show(); } if(!setProxyAction->isChecked()) {domainwidget.primary_proxy_edit->hide(); domainwidget.secondary_proxy_edit->hide(); } } );
      //connect(setDisplayNameAction, &QAction::triggered, [this] { if(setDisplayNameAction->isChecked()) { domainwidget.display_name_edit->show(); } if(!setDisplayNameAction->isChecked()) { domainwidget.display_name_edit->hide(); } } );
      connect(domainwidget.advanced_button, &QPushButton::clicked, [this] {
        if(domainwidget.display_name_edit->isVisible() && domainwidget.primary_proxy_edit->isVisible() && domainwidget.secondary_proxy_edit->isVisible())
          {
              domainwidget.display_name_edit->hide();
              domainwidget.primary_proxy_edit->hide();
              domainwidget.secondary_proxy_edit->hide();
          }
          else if(!domainwidget.display_name_edit->isVisible() && !domainwidget.primary_proxy_edit->isVisible() && !domainwidget.secondary_proxy_edit->isVisible())
          {
              domainwidget.display_name_edit->show();
              domainwidget.primary_proxy_edit->show();
              domainwidget.secondary_proxy_edit->show();
          }
       } );

      connect(mainwidget.server_button, &QPushButton::clicked, this, &MainWindow::launch_server_widget);
      connect(domainwidget.create_button, &QPushButton::clicked, this, &MainWindow::create_domain_user);
      connect(domainwidget.cancel_button, &QPushButton::clicked, this, &MainWindow::close_server_widget);
      connect(mainwidget.local_button, &QPushButton::clicked, this, &MainWindow::launch_local_widget);
      connect(localwidget.cancel_button, &QPushButton::clicked, this, &MainWindow::close_local_widget);
      connect(localwidget.create_button, &QPushButton::clicked, this, &MainWindow::create_local_user);
      connect(domainwidget.generate_button, &QPushButton::clicked, this, &MainWindow::Automate);
      connect(mainwidget.edit_user_button, &QPushButton::clicked, this, &MainWindow::launch_edit_user_widget);
      connect(editwidget.cancel_button, &QPushButton::clicked, this, &MainWindow::close_edit_user_widget);
      connect(editwidget.select_action, qOverload<int>(&QComboBox::currentIndexChanged), [=] (int var) { emit(Edit_User_Widget(var) );  } );
      connect(mainwidget.disable_user_button, &QPushButton::clicked, this, &MainWindow::launch_disable_user_widget);
      connect(disableuser.cancel_button, &QPushButton::clicked, this, &MainWindow::close_disable_user_widget);
      connect(disableuser.disable_button, &QPushButton::clicked, this, &MainWindow::disable_user);

}

void MainWindow::initialize_actions()
 {
     /*saveFileAction = new QAction(tr("Save Command"));
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
     helpMeAction->setToolTip("Click to view the help documentation.");*/

 }

void MainWindow::initialize_menus()
 {
        //fileMenu = menuBar()->addMenu(tr("&File"));
        //fileMenu->addAction(saveFileAction);
        //fileMenu->addSeparator();
        //fileMenu->addAction(saveFileAsAction);
        //fileMenu->addSeparator();
        //fileMenu->addAction(closeProgramAction);
        //fileMenu->setToolTipsVisible(true);

        //editMenu = menuBar()->addMenu(tr("&Edit"));



        //settingsMenu = menuBar()->addMenu(tr("&Settings"));
        //settingsMenu->addAction(setProxyAction);
        //settingsMenu->addSeparator();
        //settingsMenu->addAction(setDisplayNameAction);

       // helpMenu = menuBar()->addMenu(tr("&Help"));
        //helpMenu->addAction(helpMeAction);

        //fileMenu->setToolTipsVisible(true);
        //editMenu->setToolTipsVisible(true);
        //settingsMenu->setToolTipsVisible(true);
        //helpMenu->setToolTipsVisible(true);
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
      key_widget->setCurrentIndex(3);
}

void MainWindow::launch_disable_user_widget()
{
    key_widget->setCurrentIndex(4);
}

void MainWindow::close_local_widget()
 {
     key_widget->setCurrentIndex(0);
 }

void MainWindow::close_server_widget()
 {
     key_widget->setCurrentIndex(0);
     clear_ui();
     domainwidget.user_edit->hide();
     domainwidget.password_edit->hide();
     domainwidget.email_edit->hide();
     domainwidget.informational->setTextColor("Black");
     domainwidget.informational->setText("");
     domainwidget.upn_combo->setCurrentIndex(0);
     domainwidget.template_user_combo->setCurrentIndex(0);
     domainwidget.ou_combo->setCurrentIndex(0);
     domainwidget.ou_combo->hide();
     domainwidget.primary_proxy_edit->hide();
     domainwidget.secondary_proxy_edit->hide();
     domainwidget.display_name_edit->hide();

 }

void MainWindow::close_edit_user_widget()
 {
     key_widget->setCurrentIndex(0);
 }

void MainWindow::close_disable_user_widget()
{
    key_widget->setCurrentIndex(0);
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
     /*
     if(localwidget.emailaddress_edit->text().length() <= 0)
     {
         localwidget.emailaddress_edit->setStyleSheet("color: black; background-color: red");
         localwidget.emailaddress_edit->setPlaceholderText("VALUE CANNOT BE EMPTY");
     }
     else if(localwidget.emailaddress_edit->text().length() > 0)
     {
         localwidget.emailaddress_edit->setStyleSheet("color: black; background-color: white");
         luser.email_address = localwidget.emailaddress_edit->text();
     }*/
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

     if(luser.username.length() > 0 && luser.employee_name.length() > 0 /*&& luser.email_address.length() > 0*/ && luser.password.length() > 0)
     {
         QString tmp = "net user " + luser.username + " " + luser.password + " /add";
         user.Execute(tmp);

         if(luser.is_administrator == "1")
         {
             QString tmp = "net localgroup Administrators " + luser.username + " /add";
             user.Execute(tmp);
             user.Set_URL_Image_Path("");
             user.Dump_User_Form("<html> <h1> <center> The following information pertains to the new user request that you have submitted: </center> </h1> <br><br><br> <body> <strong> Employee name: </strong> " + luser.employee_name +
                                 "<br> <strong> Username: </strong> " + luser.username +
                                 "<br> <strong> Password: </strong> " + localwidget.password_edit->text() + " <br> <strong> Groups: Administrators</strong> " +
                                 "</body> </html>", user.List_URL_Image_Path(), luser.employee_name
                         );

         }
         else if(luser.is_administrator == "0")
         {
             user.Set_URL_Image_Path("");
             user.Dump_User_Form("<html> <h1> <center> The following information pertains to the new user request that you have submitted: </center> </h1> <br><br><br> <body> <strong> Employee name: </strong> " + luser.employee_name +
                                 "<br> <strong> Username: </strong> " + luser.username +
                                 "<br> <strong> Password: </strong> " + localwidget.password_edit->text() + " <br> <strong> Groups: None</strong> " +
                                 "</body> </html>", user.List_URL_Image_Path(), luser.employee_name
                         );
         }
     }
 }

void MainWindow::create_domain_user()
 {
     QString error = "";
     bool errorDetected = false;

     if(user.User_Exists(user.get_SamAccountName()) == "Yes")
     {
         error = error + "\nWARNING: The username " + user.get_SamAccountName() + " already exists. Please select a new one.\n";
         errorDetected = true;
         domainwidget.user_edit->setText("");
     }
     if(domainwidget.password_edit->text().length() <= 0 || domainwidget.password_edit->text().length() < user.List_ActiveSP_length().toInt())
     {
         errorDetected = true;
         error = error + "\nWARNING: The password " + domainwidget.password_edit->text() + " does not meet the minimum length requirements.\n Your password must be at least " + user.List_ActiveSP_length() + " characters long.\n";
         domainwidget.password_edit->setText("");
     }
     if(user.List_ActiveSP_Complexity() == "True" && user.Validate_Password(domainwidget.password_edit->text(), user.List_ActiveSP_length(), user.List_ActiveSP_Complexity()) == false)
     {
         errorDetected = true;
         error = error + "\nWARNING: The password " + domainwidget.password_edit->text() + " does not meet the complexity requirements.\n Your password must have at least one of the following: \nOne upper case letter"
                                                                                           "\n One lower case letter. \nOne number\nOne Special character\n";
         domainwidget.password_edit->setText("");
     }
     if(user.Employee_Name_Exists(user.List_All_Domain_Users(), user.get_Name()))
     {
         error = error + "\nWARNING: We found a person with the same name as " + user.get_Name() + " Please change the new users name. Consider using their initials or some other variation of their name.\n";
         errorDetected = true;
         domainwidget.employee_name_edit->setText("");
     }
     // insert check for duplicate email address
     // insert check for duplicate upn
     if(domainwidget.display_name_edit->text().length() > 0)
     {
         user.set_DisplayName(domainwidget.display_name_edit->text());
     }
     if(user.get_UPN().length() <= 0)
     {
         error = error + "\nWARNING: Your UPN appears to be empty. Please try again.\n";
         errorDetected = true;
     }
     if(!user.Validate_User_Status(domainwidget.template_user_combo->currentText()))
     {
         error = error + "\nWARNING: The template user " + domainwidget.template_user_combo->currentText() + " is disabled. This means it likely has no groups. Please confirm that the user has the groups needed before creating this user.\n";
         errorDetected = true;
     }
     if(errorDetected == true)
     {
         domainwidget.informational->setTextColor("Red");
         domainwidget.informational->setText(error);
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
                 QString azure = user.Run_Azure_Sync(user.Get_Azure_Status());
                 domainwidget.informational->setText("SUCCESS - The following user has been created and a PDF named " + user.get_SamAccountName() + ".pdf has been generated and saved on your desktop.\nPresent it via encrypted email to the end user.\n\n\nEmployee name: " + user.get_Name() +"\nUsername: " + user.get_SamAccountName() + "\nEmail address: " + user.get_Mail() + "\nDisplay name: " + domainwidget.display_name_edit->text() +
                                                     "\nOrganizational unit: " + user.get_OU_CN() + "\nUser Principal Name: " + user.get_UPN() + "\nGroups: " + user.get_Groups().join(" , ") + "\nPassword: " + domainwidget.password_edit->text() + "\n\n" + azure);


                 user.Set_URL_Image_Path("");
                 user.Dump_User_Form("<html> <h1> <center> The following information pertains to the new user request that you have submitted: </center> </h1> <br><br><br> <body> <strong> Employee name: </strong> " + user.get_Name() +
                                     "<br> <strong> Username: </strong> " + user.get_SamAccountName() + " <br> <strong> Email address: </strong> " + user.get_Mail() +
                                     "<br> <strong> Password: </strong> " + domainwidget.password_edit->text() + " <br> <strong> Groups: </strong> " + user.get_Groups().join(" , ") +
                                     "<br> <strong> Template user provided: </strong> " + domainwidget.template_user_combo->currentText() +
                                     "</body> </html>", user.List_URL_Image_Path(), user.get_Name()
                             );
                 clear_ui();

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
                 QString azure = user.Run_Azure_Sync(user.Get_Azure_Status());
                 domainwidget.informational->setText("SUCCESS - The following user has been created and a PDF named " + user.get_SamAccountName() + ".pdf has been generated and saved on your desktop.\nPresent it via encrypted email to the end user.\n\n\nEmployee name: " + user.get_Name() +"\nUsername: " + user.get_SamAccountName() + "\nEmail address: " + user.get_Mail() + "\nDisplay name: " + user.get_DisplayName() +
                                                     "\nOrganizational unit: " + user.get_OU_CN() + "\nUser Principal Name: " + user.get_UPN() + "\nGroups: " + user.get_Groups().join(" , ") + "\nPassword: " + domainwidget.password_edit->text() + "\n\n" + azure);


                 user.Set_URL_Image_Path("");
                 user.Dump_User_Form("<html> <h1> <center> The following information pertains to the new user request that you have submitted: </center> </h1> <br><br><br> <body> <strong> Employee name: </strong> " + user.get_Name() +
                                     "<br> <strong> Username: </strong> " + user.get_SamAccountName() + " <br> <strong> Email address: </strong> " + user.get_Mail() +
                                     "<br> <strong> Password: </strong> " + domainwidget.password_edit->text() + " <br> <strong> Groups: </strong> " + user.get_Groups().join(" , ") +
                                     "<br> <strong> Template user provided: </strong> " + domainwidget.template_user_combo->currentText() +
                                     "</body> </html>", user.List_URL_Image_Path(), user.get_Name()
                             );
                 clear_ui();
             }
             if(domainwidget.primary_proxy_edit->text().length() > 0 && domainwidget.secondary_proxy_edit->text().length() <= 0 && domainwidget.display_name_edit->text().length() > 0)
             {
                 QString p = "$p = " + QString("\"") + domainwidget.password_edit->text() + QString("\"") + "; $sec = $p | ConvertTo-SecureString -AsPlainText -Force; ";

                 /*user.Execute(p + "New-ADUser -Name " + "\"" + user.get_Name() + "\"" + " -GivenName " + "\"" + user.get_GivenName() + "\"" +
                              " -Surname " + "\"" + user.get_SurName() + "\"" + " -AccountPassword $sec -UserPrincipalName " + "\"" + user.get_UPN() + "\"" +
                              " -DisplayName " + "\"" + domainwidget.display_name_edit->text() + "\"" + " -EmailAddress " + "\"" + user.get_Mail() + "\"" + " -SamAccountName " +
                              "\"" + user.get_SamAccountName() + "\"" + " -Enabled 1; exit");*/



                 if(user.get_OtherName().length() > 0)
                 {
                     user.Execute(p + "New-ADUser -Name " + "\"" + user.get_Name() + "\"" + " -GivenName " + "\"" + user.get_GivenName() + "\"" +
                                  " -Surname " + "\"" + user.get_SurName() + "\"" + " -OtherName" + "\"" + user.get_OtherName() + "\"" + " -AccountPassword $sec -UserPrincipalName " + "\"" + user.get_UPN() + "\"" +
                                  " -DisplayName " + "\"" + domainwidget.display_name_edit->text() + "\"" + " -EmailAddress " + "\"" + user.get_Mail() + "\"" + " -SamAccountName " +
                                  "\"" + user.get_SamAccountName() + "\"" + " -Enabled 1; exit");
                 }
                 else if(user.get_OtherName().length() <= 0)
                 {
                     user.Execute(p + "New-ADUser -Name " + "\"" + user.get_Name() + "\"" + " -GivenName " + "\"" + user.get_GivenName() + "\"" +
                                  " -Surname " + "\"" + user.get_SurName() + "\"" + " -AccountPassword $sec -UserPrincipalName " + "\"" + user.get_UPN() + "\"" +
                                  " -DisplayName " + "\"" + domainwidget.display_name_edit->text() + "\"" + " -EmailAddress " + "\"" + user.get_Mail() + "\"" + " -SamAccountName " +
                                  "\"" + user.get_SamAccountName() + "\"" + " -Enabled 1; exit");
                 }


                 user.Execute(user.Clean_String("$tmp = (Get-ADUser -Filter {Name -like \"" + domainwidget.template_user_combo->currentText() + "\"}); "
                              "$groups = (Get-ADUser $tmp -Properties MemberOf).MemberOf; $usr = \"" + user.get_SamAccountName() + "\"; "
                              "Foreach ($group in $groups) {Add-ADGroupMember -Identity (Get-ADGroup $group).name -Members $usr}; exit "));

                 user.Execute("Set-ADUser -Identity \"" + user.get_SamAccountName() + "\" -Add @{Proxyaddresses = " + "\"SMTP:" + domainwidget.primary_proxy_edit->text() + "\"}");
                 user.set_User_DN(user.List_User_DN(user.get_Name()));
                 user.Move_ADUser_Orgranizational_Unit(user.get_User_DN(), user.get_OU_DN());

                 // insert auto print to pdf

                 domainwidget.informational->setTextColor("Green");
                 QString azure = user.Run_Azure_Sync(user.Get_Azure_Status());
                 domainwidget.informational->setText("SUCCESS - The following user has been created and a PDF named " + user.get_SamAccountName() + ".pdf has been generated and saved on your desktop.\nPresent it via encrypted email to the end user.\n\n\nEmployee name: " + user.get_Name() +"\nUsername: " + user.get_SamAccountName() + "\nEmail address: " + user.get_Mail() + "\nDisplay name: " + domainwidget.display_name_edit->text() +
                                                     "\nOrganizational unit: " + user.get_OU_CN() + "\nUser Principal Name: " + user.get_UPN() + "\nGroups: " + user.get_Groups().join(" , ") + "\nPassword: " + domainwidget.password_edit->text() + "\n\n" + azure);


                 user.Set_URL_Image_Path("");
                 user.Dump_User_Form("<html> <h1> <center> The following information pertains to the new user request that you have submitted: </center> </h1> <br><br><br> <body> <strong> Employee name: </strong> " + user.get_Name() +
                                     "<br> <strong> Username: </strong> " + user.get_SamAccountName() + " <br> <strong> Email address: </strong> " + user.get_Mail() +
                                     "<br> <strong> Password: </strong> " + domainwidget.password_edit->text() + " <br> <strong> Groups: </strong> " + user.get_Groups().join(" , ") +
                                     "<br> <strong> Template user provided: </strong> " + domainwidget.template_user_combo->currentText() +
                                     "</body> </html>", user.List_URL_Image_Path(), user.get_Name()
                             );
                 clear_ui();
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
                 QString azure = user.Run_Azure_Sync(user.Get_Azure_Status());
                 domainwidget.informational->setText("SUCCESS - The following user has been created and a PDF named " + user.get_SamAccountName() + ".pdf has been generated and saved on your desktop.\nPresent it via encrypted email to the end user.\n\n\nEmployee name: " + user.get_Name() +"\nUsername: " + user.get_SamAccountName() + "\nEmail address: " + user.get_Mail() + "\nDisplay name: " + domainwidget.display_name_edit->text() +
                                                     "\nOrganizational unit: " + user.get_OU_CN() + "\nUser Principal Name: " + user.get_UPN() + "\nGroups: " + user.get_Groups().join(" , ") + "\nPassword: " + domainwidget.password_edit->text() + "\n\n" + azure);


                 user.Set_URL_Image_Path("");
                 user.Dump_User_Form("<html> <h1> <center> The following information pertains to the new user request that you have submitted: </center> </h1> <br><br><br> <body> <strong> Employee name: </strong> " + user.get_Name() +
                                     "<br> <strong> Username: </strong> " + user.get_SamAccountName() + " <br> <strong> Email address: </strong> " + user.get_Mail() +
                                     "<br> <strong> Password: </strong> " + domainwidget.password_edit->text() + " <br> <strong> Groups: </strong> " + user.get_Groups().join(" , ") +
                                     "<br> <strong> Template user provided: </strong> " + domainwidget.template_user_combo->currentText() +
                                     "</body> </html>", user.List_URL_Image_Path(), user.get_Name()
                             );
                 clear_ui();

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
                 QString azure = user.Run_Azure_Sync(user.Get_Azure_Status());
                 domainwidget.informational->setText("SUCCESS - The following user has been created and a PDF named " + user.get_SamAccountName() + ".pdf has been generated and saved on your desktop.\nPresent it via encrypted email to the end user.\n\n\nEmployee name: " + user.get_Name() +"\nUsername: " + user.get_SamAccountName() + "\nEmail address: " + user.get_Mail() + "\nDisplay name: " + domainwidget.display_name_edit->text() +
                                                     "\nOrganizational unit: " + user.get_OU_CN() + "\nUser Principal Name: " + user.get_UPN() + "\nGroups: " + user.get_Groups().join(" , ") + "\nPassword: " + domainwidget.password_edit->text() + "\n\n" + azure);


                 user.Set_URL_Image_Path("");
                 user.Dump_User_Form("<html> <h1> <center> The following information pertains to the new user request that you have submitted: </center> </h1> <br><br><br> <body> <strong> Employee name: </strong> " + user.get_Name() +
                                     "<br> <strong> Username: </strong> " + user.get_SamAccountName() + " <br> <strong> Email address: </strong> " + user.get_Mail() +
                                     "<br> <strong> Password: </strong> " + domainwidget.password_edit->text() + " <br> <strong> Groups: </strong> " + user.get_Groups().join(" , ") +
                                     "<br> <strong> Template user provided: </strong> " + domainwidget.template_user_combo->currentText() +
                                     "</body> </html>", user.List_URL_Image_Path(), user.get_Name()
                             );
                 clear_ui();


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

                 QString azure = user.Run_Azure_Sync(user.Get_Azure_Status());
                 domainwidget.informational->setText("SUCCESS - The following user has been created and a PDF named " + user.get_SamAccountName() + ".pdf has been generated and saved on your desktop.\nPresent it via encrypted email to the end user.\n\n\nEmployee name: " + user.get_Name() +"\nUsername: " + user.get_SamAccountName() + "\nEmail address: " + user.get_Mail() + "\nDisplay name: " + user.get_DisplayName() +
                                                     "\nOrganizational unit: " + user.get_OU_CN() + "\nUser Principal Name: " + user.get_UPN() + "\nGroups: " + user.get_Groups().join(" , ") + "\nPassword: " + domainwidget.password_edit->text() + "\n\n" + azure);




                 user.Set_URL_Image_Path("");
                 user.Dump_User_Form("<html> <h1> <center> The following information pertains to the new user request that you have submitted: </center> </h1> <br><br><br> <body> <strong> Employee name: </strong> " + user.get_Name() +
                                     "<br> <strong> Username: </strong> " + user.get_SamAccountName() + " <br> <strong> Email address: </strong> " + user.get_Mail() +
                                     "<br> <strong> Password: </strong> " + domainwidget.password_edit->text() + " <br> <strong> Groups: </strong> " + user.get_Groups().join(" , ") +
                                     "<br> <strong> Template user provided: </strong> " + domainwidget.template_user_combo->currentText() +
                                     "</body> </html>", user.List_URL_Image_Path(), user.get_Name()
                             );
                 clear_ui();

             }
         }
         else if (warning_banner->clickedButton() == cancel_button)
         {
             clear_ui();
         }

     }

 }

void MainWindow::edit_domain_user()
{

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
        if(Names.count() >= 2)
        {
            user.set_OtherName(Names[1]);
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
        domainwidget.password_edit->setPlaceholderText("Minimum password length " + user.List_ActiveSP_length() + " Complexity on: " + user.List_ActiveSP_Complexity());
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

/*void MainWindow::Automate_Edit_User()
{

     if(editwidget.select_action->currentIndex() == 0)
     {

     }
     else if(editwidget.select_action->currentIndex() == 1)
     {

     }
     else if(editwidget.select_action->currentIndex() == 2)
     {

     }
     else if(editwidget.select_action->currentIndex() == 3)
     {

     }
     else if(editwidget.select_action->currentIndex() == 4)
     {

     }
     else if(editwidget.select_action->currentIndex() == 5)
     {

     }
     else if(editwidget.select_action->currentIndex() == 6)
     {

     }
     else if(editwidget.select_action->currentIndex() == 7)
     {

     }


    if(editwidget.user_combo->currentText().length() > 0)
     {
        editwidget.givenname_edit->setText(editwidget.user_combo->currentText().split(" ").first());
        editwidget.surname_edit->setText(editwidget.user_combo->currentText().split(" ").last());
        editwidget.username_edit->setText(user.List_SamAccountName(editwidget.user_combo->currentText()));
        editwidget.password_edit->setPlaceholderText("Input new password");
        editwidget.email_edit->setText(user.List_Mail(editwidget.user_combo->currentText()));
        editwidget.displayname_edit->setText(user.List_User_DisplayName(editwidget.user_combo->currentText()));
        QStringList tmp_upn = user.List_All_UPNs();

        for(int i = 0; i < editwidget.upn_combo->count(); ++i)
        {
            if(user.List_User_Identifier(user.List_Name(editwidget.user_combo->currentText())) == tmp_upn.at(i))
            {
                editwidget.upn_combo->setCurrentIndex(i);
            }
        }

        QStringList tmp_ou_cn = user.List_All_OU_CNs();

        for(int i = 0; i < editwidget.ou_combo->count(); ++i)
        {
            if(user.List_User_OU_CN(editwidget.user_combo->currentText()) == tmp_ou_cn.at(i))
            {
                editwidget.ou_combo->setCurrentIndex(i);
            }
        }



        editwidget.surname_edit->show();
        editwidget.givenname_edit->show();
        editwidget.username_edit->show();
        editwidget.upn_combo->show();
        editwidget.ou_combo->show();
        //editwidget.current_groups_combo->show();
        editwidget.password_edit->show();
        editwidget.email_edit->show();
     }
     else
     {

     }


}*/

void MainWindow::Edit_User_Widget(int choice)
{
    if(choice == 0)
    {
        qDebug() << choice;
    }
    if(choice == 1)
    {
        editwidget.informational->setTextColor("Black");
        editwidget.informational->setText("Informational Event: Please be aware that this will change all of the following values:\nName\nGiven name\nSurname\nMiddle name\nEmail address\nSamAccountName\nUserPrincipalName\nDisplay name");
        editwidget.user_combo->show();
        editwidget.givenname_edit->show();
        editwidget.middlename_edit->show();
        editwidget.surname_edit->show();
        editwidget.edit_button->show();
        connect(editwidget.edit_button, &QPushButton::clicked, [this] {user.Edit_Name(editwidget.user_combo->currentText(), editwidget.givenname_edit->text(), editwidget.middlename_edit->text(), editwidget.surname_edit->text()); } );
    }
    if(choice == 2)
    {


        //hide
        editwidget.givenname_edit->hide();
        editwidget.middlename_edit->hide();
        editwidget.surname_edit->hide();
    }
    if(choice == 3)
    {


        //hide
        editwidget.givenname_edit->hide();
        editwidget.middlename_edit->hide();
        editwidget.surname_edit->hide();
    }
    if(choice == 4)
    {

        //hide
        editwidget.givenname_edit->hide();
        editwidget.middlename_edit->hide();
        editwidget.surname_edit->hide();
    }
    if(choice == 5)
    {
        editwidget.password_edit->show();

        user.List_Password_Policy(user.List_Name(editwidget.user_combo->currentText()));
        editwidget.password_edit->setPlaceholderText("Minimum password length " + user.List_ActiveSP_length() + " Complexity on: " + user.List_ActiveSP_Complexity());


        //hide
        editwidget.givenname_edit->hide();
        editwidget.middlename_edit->hide();
        editwidget.surname_edit->hide();
    }
    if(choice == 6)
    {


        //hide
        editwidget.givenname_edit->hide();
        editwidget.middlename_edit->hide();
        editwidget.surname_edit->hide();
    }
    if(choice == 7)
    {



        //hide
        editwidget.givenname_edit->hide();
        editwidget.middlename_edit->hide();
        editwidget.surname_edit->hide();
    }
    if(choice == 8)
    {


        //hide
        editwidget.givenname_edit->hide();
        editwidget.middlename_edit->hide();
        editwidget.surname_edit->hide();
    }
}

void MainWindow::disable_user()
{
    QString name = disableuser.user_selection->currentText();
    QString username = user.List_SamAccountName(name);
    QString email = user.List_Mail(name);
    QString OU_Clean = user.List_User_OU_CN(name);
    QStringList Groups_Clean = user.List_User_Group_CNs(user.List_SamAccountName(name));
    user.Execute("$user = " + QString("\"") + user.List_SamAccountName(name) + QString("\"") + "; Get-ADPrincipalGroupMembership $user | Foreach {Remove-ADGroupMember $_ -Members $user -Confirm:$false}");
    user.Edit_Disable_Description(name);
    user.Edit_User_Status(name);
    disableuser.informational->setText("The user " + name + " has been disabled.");
    user.Dump_User_Form("<html> <h1> <center> The following information pertains to the disable user request that you have submitted: </center> </h1> <br><br><br> <body> <strong> Employee name: </strong> " + name +
                        "<br> <strong> Username: </strong> " + username + " <br> <strong> Email address: </strong> " + email +
                        "<br> <strong> Groups: </strong> " + Groups_Clean.join(" , ") +
                        "</body> </html>", QUrl(""), name
                );
}
