#include "mainwidget.h"

MainWidget::MainWidget()
{
    server_button = new QPushButton();
    local_button = new QPushButton();
    edit_user_button = new QPushButton();
    disable_user_button = new QPushButton();
}

QWidget* MainWidget::get_widget()
{
    return initalize_widget(server_button, local_button, edit_user_button, disable_user_button);
}

QWidget* MainWidget::initalize_widget(QPushButton *server_button, QPushButton *local_button, QPushButton *edit_user_button, QPushButton *disable_user_button)
{

    server_button->setText("Create domain user");
    server_button->setToolTip("Click this button to create a new AD user on the Domain Controller.");


    local_button->setText("Create local user");
    local_button->setToolTip("Click this button to create a local user account.");

    edit_user_button->setText("Modify user account");
    edit_user_button->setToolTip("Click to modify a pre-existing user account.");

    disable_user_button->setText("Disable user account");
    disable_user_button->setToolTip("Click to disable a domain user account.");

    QVBoxLayout *primary_layout = new QVBoxLayout();

    primary_layout->addWidget(server_button);
    primary_layout->addWidget(local_button);
    primary_layout->addWidget(edit_user_button);
    primary_layout->addWidget(disable_user_button);

    QWidget *primary_widget = new QWidget();
    primary_widget->setLayout(primary_layout);
    primary_layout->setAlignment(Qt::AlignHCenter);
    return primary_widget;

}
