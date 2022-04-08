#include "disablewidget.h"

DisableWidget::DisableWidget()
{
    user_selection = new QComboBox();
    disable_button = new QPushButton();
    cancel_button = new QPushButton();
    informational = new QTextBrowser();
}



QWidget* DisableWidget::get_widget()
{
   return initalize_widget(user_selection, disable_button, cancel_button, informational);
}

QWidget* DisableWidget::initalize_widget(QComboBox *user_selection, QPushButton *disable_button, QPushButton *cancel_button, QTextBrowser *informational)
{
    QVBoxLayout *main_layout = new QVBoxLayout();
    QWidget *primary_display = new QWidget();
    QSettings *MultiCompanyEnabled = new QSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Elysian Productions\\UMTK-Classic\\Company Settings\\", QSettings::Registry64Format);
    setAllADUsers(Execute_Command("$tmp = (Get-ADUser -Filter * | Select-Object Name, GivenName, SurName | Sort-Object SurName, GivenName); return $tmp.Name"));
    if(MultiCompanyEnabled->value("MultiCompanyEnabled").toBool())
    {
        mapOUToCompany();
        mapCompanyToPrefix();
        mapUserToOU();
        mapUserToCompany();
    }

//    QStringList AD_Users = getAllADUsers();

    user_selection->setToolTip("Please select the appropriate template user.");
    user_selection->addItems(getAllADUsers());

    disable_button->setText("Disable");


    cancel_button->setText("Cancel");


    main_layout->addWidget(user_selection);
    main_layout->addWidget(disable_button);
    main_layout->addWidget(cancel_button);
    main_layout->addWidget(informational);
    primary_display->setLayout(main_layout);
    main_layout->setAlignment(Qt::AlignHCenter);

    return primary_display;
}
