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

        helpMenu = menuBar()->addMenu(tr("&Help"));
        helpMenu->addAction(helpMeAction);

        fileMenu->setToolTipsVisible(true);
        editMenu->setToolTipsVisible(true);
        settingsMenu->setToolTipsVisible(true);
        helpMenu->setToolTipsVisible(true);
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
