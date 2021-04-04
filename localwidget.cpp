#include "localwidget.h"

LocalWidget::LocalWidget()
{
    username_edit = new QLineEdit();
    fullname_edit = new QLineEdit();
    emailaddress_edit = new QLineEdit();
    password_edit = new QLineEdit();
    create_button = new QPushButton();
    cancel_button = new QPushButton();
    admin_checkbox = new QCheckBox();
}


QWidget* LocalWidget::create_widget()
{
    return initalize_widget(username_edit, fullname_edit, emailaddress_edit, password_edit, create_button, cancel_button, admin_checkbox);
}

QWidget* LocalWidget::initalize_widget(QLineEdit *username_edit, QLineEdit *fullname_edit, QLineEdit *emailaddress_edit, QLineEdit *password_edit, QPushButton *create_button, QPushButton *cancel_buton, QCheckBox *admin_checkbox)
{
    username_edit->setPlaceholderText("Input username.");
    username_edit->setToolTip("Type the users desired username.");
    fullname_edit->setPlaceholderText("Input full name.");
    fullname_edit->setToolTip("Type the users full name.");
    emailaddress_edit->setPlaceholderText("Input mail address.");
    emailaddress_edit->setToolTip("Type the users emaill address.\nOr, leave blank if empty.");
    password_edit->setPlaceholderText("Input password.");
    password_edit->setToolTip("Enter the users password.");
    password_edit->setEchoMode(QLineEdit::Password);

    create_button->setText("Create");
    create_button->setToolTip("Click to create the user account.");
    cancel_buton->setText("Cancel");
    cancel_buton->setToolTip("Click to close and return to the main menu.");

    admin_checkbox->setText("Administrator");
    admin_checkbox->setToolTip("Check this box if the user should be an Administrator.\nIf not, the default non-administrator privleges will be assigned.");

    QGridLayout *primary_layout = new QGridLayout();
    primary_layout->addWidget(username_edit, 0, 0);
    primary_layout->addWidget(fullname_edit, 0, 1);
    primary_layout->addWidget(emailaddress_edit, 1, 0);
    primary_layout->addWidget(password_edit, 1, 1);
    primary_layout->addWidget(admin_checkbox, 2, 0);
    primary_layout->addWidget(create_button, 3, 0);
    primary_layout->addWidget(cancel_buton, 3, 1);

    QWidget *primary_display = new QWidget();
    primary_display->setLayout(primary_layout);
    primary_layout->setAlignment(Qt::AlignHCenter);

    return primary_display;
}




//
