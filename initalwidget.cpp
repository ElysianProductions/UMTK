#include "initalwidget.h"

InitalWidget::InitalWidget()
{
    server_button = new QPushButton();
    local_button = new QPushButton();
    primary_layout = new QVBoxLayout();
}


QWidget* InitalWidget::create_widget()
{
    return initalize_widget(server_button, local_button, primary_layout);
}


QWidget* InitalWidget::initalize_widget(QPushButton *server_button, QPushButton *local_button, QVBoxLayout *layout)
{
        server_button->setText("Domain: New User");
        server_button->setToolTip("Click this button to create a new AD user on the Domain Controller.");


        local_button->setText("Workstation: New User");
        local_button->setToolTip("Click this button to create a local user account.");

        primary_layout->addWidget(server_button);
        primary_layout->addWidget(local_button);


        QWidget *primary_widget = new QWidget();
        primary_widget->setLayout(primary_layout);
        primary_layout->setAlignment(Qt::AlignHCenter);
        return primary_widget;
}
