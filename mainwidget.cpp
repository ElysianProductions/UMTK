#include "mainwidget.h"

MainWidget::MainWidget()
{

    upn_combo = new QComboBox();
    ou_combo = new QComboBox();
    cloud_combobox = new QComboBox();
    template_user_combo = new QComboBox();
    domain_name_combo = new QComboBox();
    employee_name_edit = new QLineEdit();
    user_edit = new QLineEdit();
    password_edit = new QLineEdit();
    email_edit = new QLineEdit();
    display_name_edit = new QLineEdit();
    create_button = new QPushButton();
    delete_button= new QPushButton();

    /* commands << "Get-ADOrganizationalUnit -Filter * | select-object -expandproperty Name"
             << "Get-ADOrganizationalUnit -Filter * | Select-Object -ExpandProperty distinguishedName"
             << "Get-ADForest | Select-Object -ExpandProperty UPNSuffixes"
             << "Get-ADDomain | Select-Object -expandproperty Forest"
             << "Get-ADUser -Filter * | Select-Object -ExpandProperty Name"
             << "(Get-ADUser $usr -Properties MemberOf).MemberOf";*/


    cloud_clients << "Taylor Oil & Heat"
                  << "Dutchess County Workforce Investment Board"
                  << "Rosenthal Wine Merchants"
                  << "Taxpert"
                  << "Tom Fucito CPA"
                  << "Dana Sperry CPA";

    cloud_suffixes << "TOH_"
                   << "DCWIB_"
                   << "RWM_"
                   << "Taxpert_"
                   << "TFCPA_"
                   << "DSA_";
}



QWidget* MainWidget::initalize_widget()
{
    return create_widget(upn_combo, ou_combo, cloud_combobox, template_user_combo, domain_name_combo, employee_name_edit, user_edit, password_edit, email_edit, display_name_edit, create_button, delete_button);
}

QWidget* MainWidget::create_widget(QComboBox *upn_combo, QComboBox *ou_combo, QComboBox *cloud_combobox, QComboBox *template_user_combo, QComboBox *domain_name_combo, QLineEdit *employee_name_edit, QLineEdit *user_edit, QLineEdit *password_edit, QLineEdit *email_edit, QLineEdit *display_name_edit, QPushButton *create_button, QPushButton *delete_button)
{
        QVBoxLayout *main_layout = new QVBoxLayout();
        QWidget *primary_display = new QWidget();

        upn_combo->setToolTip("Please select the appropriate UPN from the window. If no UPNs are available use the domain name option");
        upn_combo->addItems(execute_command("Get-ADForest | Select-Object -ExpandProperty UPNSuffixes"));


        cloud_combobox->setToolTip("If you're seeing this widget you need to seelct the cloud client from the list.");
        cloud_combobox->addItems(cloud_clients);

        template_user_combo->setToolTip("Please select the appropriate template user.");
        template_user_combo->setToolTip("Get-ADUser -Filter * | Select-Object -ExpandProperty Name");

        ou_combo->setToolTip("Please select the appropriate OU from the list.");
        execute_command("Get-ADOrganizationalUnit -Filter * | select-object -expandproperty Name");

        domain_name_combo->setToolTip("Select domain name if applicable.");
        execute_command("Get-ADDomain | Select-Object -expandproperty Forest");

        employee_name_edit->setPlaceholderText("Type the new employees full name.");

        user_edit->setPlaceholderText("Type the desired username.");

        password_edit->setPlaceholderText("Type the users password.");

        email_edit->setPlaceholderText("Type the employees email address");

        display_name_edit->setPlaceholderText("Type the desired display name if different from the default.");

        create_button->setText("Create user");

        delete_button->setText("Cancel");

        main_layout->addWidget(upn_combo);
        main_layout->addWidget(ou_combo);
        main_layout->addWidget(template_user_combo);
        main_layout->addWidget(cloud_combobox);
        main_layout->addWidget(domain_name_combo);
        main_layout->addWidget(employee_name_edit);
        main_layout->addWidget(user_edit);
        main_layout->addWidget(password_edit);
        main_layout->addWidget(email_edit);
        main_layout->addWidget(display_name_edit);
        main_layout->addWidget(create_button);
        main_layout->addWidget(delete_button);
        primary_display->setLayout(main_layout);
        main_layout->setAlignment(Qt::AlignHCenter);

        return primary_display;
}

QStringList MainWidget::execute_command(QString param)
{
   QProcess *process = new QProcess();
   QByteArray term_output;
   QStringList params;
   QString command = "powershell";
   params << "-c" << param;
   process->start(command, params);
   process->waitForFinished();
   term_output.append(process->readAllStandardOutput());
   process->close();

   QStringList return_list = QString(term_output).split(" ", QString::SkipEmptyParts);

   return return_list;


}
