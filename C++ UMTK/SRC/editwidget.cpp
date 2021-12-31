#include "editwidget.h"

EditWidget::EditWidget()
{
    select_action = new QComboBox();
    upn_combo = new QComboBox();
    ou_combo = new QComboBox();
    user_combo = new QComboBox();
    forest_combo = new QComboBox();
    current_groups_combo = new QComboBox();
    username_edit = new QLineEdit();
    password_edit = new QLineEdit();
    email_edit = new QLineEdit();
    displayname_edit = new QLineEdit();
    givenname_edit = new QLineEdit();
    surname_edit = new QLineEdit();
    primary_proxy_edit = new QLineEdit();
    secondary_proxy_edit = new QLineEdit();
    edit_button = new QPushButton();
    cancel_button = new QPushButton();
    load_button = new QPushButton();
    informational = new QTextBrowser();
    middlename_edit = new QLineEdit();
}

QWidget* EditWidget::get_widget()
{
    return initalize_widget(select_action, upn_combo, ou_combo, user_combo, forest_combo,
                            username_edit, password_edit, email_edit,
                            displayname_edit, primary_proxy_edit, secondary_proxy_edit,
                            edit_button, givenname_edit, surname_edit, current_groups_combo,
                            cancel_button, load_button, informational, middlename_edit);
}

QWidget* EditWidget::initalize_widget(QComboBox *select_action, QComboBox *upn_combo, QComboBox *ou_combo, QComboBox *user_combo, QComboBox *forest_combo, QLineEdit *username_edit, QLineEdit *password_edit, QLineEdit *email_edit, QLineEdit *displayname_edit,
                                      QLineEdit *primary_proxy_edit, QLineEdit *secondary_proxy_edit, QPushButton *edit_button, QLineEdit *givenname_edit, QLineEdit *surname_edit, QComboBox *current_groups_combo, QPushButton *cancel_button,
                                      QPushButton *load_button, QTextBrowser *informational, QLineEdit *middlename_edit)
{
        QVBoxLayout *main_layout = new QVBoxLayout();
        QWidget *primary_display = new QWidget();

        QStringList actions;
        actions << "Edit full name" << "Edit username" << "Edit display name" << "Edit email address" << "Edit password" << "Edit user principal name" << "Edit organizational unit" << "Edit group membership";
        select_action->addItems(actions);

        upn_combo->setToolTip("Please select the appropriate UPN from the window. If no UPNs are available use the domain name option");
        upn_combo->addItems(List_All_UPNs());
        upn_combo->hide();

        user_combo->setToolTip("Please select the appropriate user.");
        user_combo->addItems(List_All_Domain_Users());
        user_combo->hide();

        ou_combo->setToolTip("Please select the appropriate OU from the list.");
        ou_combo->addItems(List_All_OU_CNs());
        ou_combo->hide();

        forest_combo->setToolTip("Select domain name if applicable.");
        forest_combo->addItems(List_All_Forests());
        forest_combo->hide();

        givenname_edit->setToolTip("Use this field to change the users first name");
        givenname_edit->setPlaceholderText("Input new first name");
        givenname_edit->hide();

        middlename_edit->setToolTip("Use this field to change the users middle name");
        middlename_edit->setPlaceholderText("Input new middle name");
        middlename_edit->hide();

        surname_edit->setToolTip("Use this field to change the users last name");
        surname_edit->setPlaceholderText("Input new last name");
        surname_edit->hide();                

        current_groups_combo->hide();
        current_groups_combo->setToolTip("");

        username_edit->setPlaceholderText("Type the desired username.");
        username_edit->hide();

        password_edit->setPlaceholderText("Type the users password.");
        password_edit->setEchoMode(QLineEdit::Password);
        password_edit->hide();

        email_edit->setPlaceholderText("Type the employees email address");
        email_edit->hide();

        displayname_edit->setPlaceholderText("Type the desired display name if different from the default.");
        displayname_edit->hide();

        primary_proxy_edit->setPlaceholderText("Input your primary proxy address");
        primary_proxy_edit->setToolTip("You do not have to type SMTP: ");
        primary_proxy_edit->hide();

        secondary_proxy_edit->setPlaceholderText("Input your secondary proxy address");
        secondary_proxy_edit->setToolTip("You do not have to type smtp: ");
        secondary_proxy_edit->hide();

        edit_button->setText("Create");
        edit_button->hide();

        load_button->setText("Load user");

        cancel_button->setText("Cancel");

        main_layout->addWidget(select_action);
        main_layout->addWidget(upn_combo);
        main_layout->addWidget(ou_combo);
        main_layout->addWidget(user_combo);
        main_layout->addWidget(forest_combo);
        main_layout->addWidget(current_groups_combo);
        main_layout->addWidget(givenname_edit);
        main_layout->addWidget(middlename_edit);
        main_layout->addWidget(surname_edit);
        main_layout->addWidget(username_edit);
        main_layout->addWidget(password_edit);
        main_layout->addWidget(email_edit);
        main_layout->addWidget(displayname_edit);
        main_layout->addWidget(primary_proxy_edit);
        main_layout->addWidget(secondary_proxy_edit);
        main_layout->addWidget(edit_button);
        main_layout->addWidget(load_button);
        main_layout->addWidget(cancel_button);
        main_layout->addWidget(informational);
        primary_display->setLayout(main_layout);
        main_layout->setAlignment(Qt::AlignHCenter);

        return primary_display;
}
