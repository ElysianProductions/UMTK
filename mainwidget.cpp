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
    primary_proxy_edit = new QLineEdit();
    secondary_proxy_edit = new QLineEdit();
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

    cloud_prefixes << "TOH_"
                   << "DCWIB_"
                   << "RWM_"
                   << "Taxpert_"
                   << "TFCPA_"
                   << "DSA_";



    //

    //
}



QWidget* MainWidget::initalize_widget()
{
    return create_widget(upn_combo, ou_combo, cloud_combobox, template_user_combo, domain_name_combo, employee_name_edit, user_edit, password_edit, email_edit, display_name_edit, primary_proxy_edit, secondary_proxy_edit, create_button, delete_button);
}

QWidget* MainWidget::create_widget(QComboBox *upn_combo, QComboBox *ou_combo, QComboBox *cloud_combobox, QComboBox *template_user_combo, QComboBox *domain_name_combo, QLineEdit *employee_name_edit, QLineEdit *user_edit, QLineEdit *password_edit, QLineEdit *email_edit, QLineEdit *display_name_edit, QLineEdit *primary_proxy_edit, QLineEdit *secondary_proxy_edit, QPushButton *create_button, QPushButton *delete_button)
{
        QVBoxLayout *main_layout = new QVBoxLayout();
        QWidget *primary_display = new QWidget();

        upn_combo->setToolTip("Please select the appropriate UPN from the window. If no UPNs are available use the domain name option");
        ad_upn_sufffixes = execute_command("Get-ADForest | Select-Object -ExpandProperty UPNSuffixes");
        upn_combo->addItems(ad_upn_sufffixes);
        if(ad_upn_sufffixes.count() > 0)
        {
            upn_combo->show();
        }
        else
        {
            upn_combo->hide();
        }


        cloud_combobox->setToolTip("If you're seeing this widget you need to seelct the cloud client from the list.");
        cloud_combobox->addItems(cloud_clients);
        cloud_combobox->hide();

        template_user_combo->setToolTip("Please select the appropriate template user.");
        ad_users = execute_command("Get-ADUser -Filter * | Select-Object -ExpandProperty Name");
        template_user_combo->addItems(ad_users);


        ou_combo->setToolTip("Please select the appropriate OU from the list.");
        ou_clean_names = execute_command("Get-ADOrganizationalUnit -Filter * | Select-Object -ExpandProperty Name");
        ou_distinguished_names = execute_command("Get-ADOrganizationalUnit -Filter * | Select-Object -ExpandProperty distinguishedName");
        ou_combo->addItems(ou_clean_names);


        domain_name_combo->setToolTip("Select domain name if applicable.");
        ad_domain_names = execute_command("Get-ADForest | Select-Object -ExpandProperty Domains");
        domain_name_combo->addItems(ad_domain_names);

        employee_name_edit->setPlaceholderText("Type the new employees full name.");

        user_edit->setPlaceholderText("Type the desired username.");

        password_edit->setPlaceholderText("Type the users password.");

        email_edit->setPlaceholderText("Type the employees email address");

        display_name_edit->setPlaceholderText("Type the desired display name if different from the default.");
        display_name_edit->hide();

        primary_proxy_edit->setPlaceholderText("Input your primary proxy address");
        primary_proxy_edit->setToolTip("You do not have to type SMTP: ");
        primary_proxy_edit->hide();

        secondary_proxy_edit->setPlaceholderText("Input your seocndary proxy address");
        secondary_proxy_edit->setToolTip("You do not have to type smtp: ");
        secondary_proxy_edit->hide();

        create_button->setText("Create");
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
        main_layout->addWidget(primary_proxy_edit);
        main_layout->addWidget(secondary_proxy_edit);
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

      QStringList return_list = QString(term_output).split("\n", QString::SkipEmptyParts);

      return return_list;


}
