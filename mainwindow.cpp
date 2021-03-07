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
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::initialize_main_window()
{
    setCentralWidget(key_widget);
    initialize_connections();
}


void MainWindow::initialize_connections()
{

}
