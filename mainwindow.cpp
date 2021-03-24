#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    main_frame = new QFrame(this);
    key_widget = new QStackedWidget();
    key_widget->addWidget(mainwidget.initalize_widget());
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
    setWindowTitle("Standardized New User Creations");
}


void MainWindow::initialize_connections()
{
      connect(saveFileAction, &QAction::triggered, this, &MainWindow::save_command);
      connect(enableCloudCreationAction, &QAction::triggered, [this] {if(enableCloudCreationAction->isChecked()) { mainwidget.cloud_combobox->show();} if(!enableCloudCreationAction->isChecked()) {mainwidget.cloud_combobox->hide();} });
      connect(setProxyAction, &QAction::triggered, [this] {if (setProxyAction->isChecked()) { mainwidget.primary_proxy_edit->show(); mainwidget.secondary_proxy_edit->show(); } if(!setProxyAction->isChecked()) {mainwidget.primary_proxy_edit->hide(); mainwidget.secondary_proxy_edit->hide(); } } );
      connect(setDisplayNameAction, &QAction::triggered, [this] { if(setDisplayNameAction->isChecked()) { mainwidget.display_name_edit->show(); } if(!setDisplayNameAction->isChecked()) { mainwidget.display_name_edit->hide(); } } );
      connect(mainwidget.create_button, &QPushButton::clicked, this, &MainWindow::initialize_warning_banner);






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

     enableCloudCreationAction = new QAction("Enable Cloud");
     enableCloudCreationAction->setStatusTip("Select this option to enable the ability to create cloud accounts.");
     enableCloudCreationAction->setToolTip("Select this option to enable the ability to create cloud accounts.");
     enableCloudCreationAction->setCheckable(true);

     addCloudClientsAction = new QAction(tr("Add clients"));
     addCloudClientsAction->setStatusTip("Click here to add cloud clients to a text file for use later on.");
     addCloudClientsAction->setToolTip("Click here to add cloud clients to a text file for use later on.");

     addCloudPefixesAction = new QAction("Add prefixes");
     addCloudPefixesAction->setStatusTip("Click to add your cloud prefixes.");
     addCloudPefixesAction->setToolTip("Click to add your cloud prefixes.");

     loadCloudClients = new QAction("Load clients");
     loadCloudClients->setToolTip("Click here to load a text file continaing your cloud clients.");
     loadCloudClients->setStatusTip("Click here to load a text file continaing your cloud clients.");

     loadCloudPrefixes = new QAction("Load prefixes");
     loadCloudPrefixes->setToolTip("Click here to load a text file containing your cloud prefixes.");
     loadCloudPrefixes->setStatusTip("Click here to load a text file containing your cloud prefixes.");

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
        editMenu->addAction(addCloudClientsAction);
        editMenu->addSeparator();
        editMenu->addAction(addCloudPefixesAction);


        settingsMenu = menuBar()->addMenu(tr("&Settings"));
        settingsMenu->addAction(enableCloudCreationAction);
        settingsMenu->addSeparator();
        settingsMenu->addAction(loadCloudClients);
        settingsMenu->addSeparator();
        settingsMenu->addAction(loadCloudPrefixes);
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

 void MainWindow::initialize_warning_banner()
 {
     warning_banner = new QMessageBox();
     warning_banner->setText("Confirm the following information by clicking 'OK'\nTo cancel and redo the information, click the button labeled 'Cancel'");

     QString employee_name = mainwidget.employee_name_edit->text();
     QString username = mainwidget.user_edit->text();
     QString user_upn = "";
     QString display_name = "";
     QString email_address = "";
     QString primary_proxy = "";
     QString secondary_proxy = "";
     QString cloud_prefix = "";
     if(mainwidget.upn_combo->isVisible())
     {
         user_upn = mainwidget.upn_combo->currentText();
     }
     if(mainwidget.display_name_edit->isVisible())
     {
         display_name = mainwidget.display_name_edit->text();
     }
     if(mainwidget.display_name_edit->isHidden())
     {
         display_name = employee_name;
     }
     email_address = mainwidget.email_edit->text();
     if(mainwidget.primary_proxy_edit->isVisible())
     {
         primary_proxy = "SMTP:" + mainwidget.primary_proxy_edit->text();
         if(mainwidget.secondary_proxy_edit->text().length() > 0)
         {
             secondary_proxy = "smtp:" + mainwidget.secondary_proxy_edit->text();

         }
     }
     if(mainwidget.primary_proxy_edit->isHidden())
     {
         primary_proxy = "SMTP: " + email_address;
     }
     if(mainwidget.cloud_combobox->isVisible())
     {
         cloud_prefix = mainwidget.cloud_prefixes[mainwidget.cloud_combobox->currentIndex()];
         username = cloud_prefix + username;
     }
     if(user_upn.length() <= 0)
     {
        if(mainwidget.primary_proxy_edit->isHidden())
        {
             warning_banner->setInformativeText("Employee name: " + employee_name + "\nUsername: " + username + "\nDisplay name: " + display_name + "\nEmail address: " + email_address + "\nPrimary proxy: " + primary_proxy);
        }
        if(mainwidget.primary_proxy_edit->isVisible())
        {
             if(mainwidget.secondary_proxy_edit->text().length() <= 0)
             {
                 warning_banner->setInformativeText("Employee name: " + employee_name + "\nUsername: " + username + "\nDisplay name: " + display_name + "\nEmail address: " + email_address + "\nPrimary Proxy: " + primary_proxy);
             }
             else
             {
                 warning_banner->setInformativeText("Employee name: " + employee_name + "\nUsername: " + username + "\nDisplay name: " + display_name + "\nEmail address: " + email_address + "\nPrimary Proxy: " + primary_proxy + "\nSecondary Proxy: " + secondary_proxy);
             }
        }
     }

     QPushButton *okay_button = warning_banner->addButton(tr("Okay"), QMessageBox::ActionRole);
     QPushButton *cancel_button = warning_banner->addButton(tr("Cancel"), QMessageBox::ActionRole);

     warning_banner->exec();

     if (warning_banner->clickedButton() == okay_button)
     {
         // build command
         qDebug() << "1";
     }
     else if (warning_banner->clickedButton() == cancel_button)
     {
         // cancel and clear everything
         qDebug() << "2";
     }


 }


 void MainWindow::validate_information()
 {

 }

 void MainWindow::create_user()
 {

 }

 void MainWindow::set_proxy_addresses()
 {

 }

 void MainWindow::shift_ou()
 {

 }

 void MainWindow::save_command()
 {

 }

 void MainWindow::save_command_to_file()
 {

 }

 void MainWindow::close_program()
 {

 }
