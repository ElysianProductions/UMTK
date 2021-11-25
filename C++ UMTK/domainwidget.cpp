#include "domainwidget.h"
DomainWidget::DomainWidget()
{
    upn_combo = new QComboBox();
    ou_combo = new QComboBox();
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
    cancel_button = new QPushButton();
    generate_button = new QPushButton();
}

QWidget* DomainWidget::get_widget()
{
    return initalize_widget(upn_combo, ou_combo, template_user_combo, domain_name_combo,
                            employee_name_edit, user_edit, password_edit, email_edit,
                            display_name_edit, primary_proxy_edit, secondary_proxy_edit,
                            create_button, cancel_button, generate_button);
}

QWidget* DomainWidget::initalize_widget(QComboBox *upn_combo, QComboBox *ou_combo, QComboBox *template_user_combo, QComboBox *domain_name_combo, QLineEdit *employee_name_edit, QLineEdit *user_edit, QLineEdit *password_edit, QLineEdit *email_edit, QLineEdit *display_name_edit, QLineEdit *primary_proxy_edit, QLineEdit *secondary_proxy_edit, QPushButton *create_button, QPushButton *cancel_button, QPushButton *generate_button)
{
        QVBoxLayout *main_layout = new QVBoxLayout();
        QWidget *primary_display = new QWidget();
        load_domain_information();

        upn_combo->setToolTip("Please select the appropriate UPN from the window. If no UPNs are available use the domain name option");
        upn_combo->addItems(Domain_UPNS);
        upn_combo->hide();

        template_user_combo->setToolTip("Please select the appropriate template user.");
        template_user_combo->addItems(AD_Users);

        ou_combo->setToolTip("Please select the appropriate OU from the list.");
        ou_combo->addItems(OU_Names);
        ou_combo->hide();

        domain_name_combo->setToolTip("Select domain name if applicable.");
        domain_name_combo->addItems(Domain_Name);
        domain_name_combo->hide();

        employee_name_edit->setPlaceholderText("Type the new employees full name.");

        user_edit->setPlaceholderText("Type the desired username.");
        user_edit->hide();

        password_edit->setPlaceholderText("Type the users password.");
        password_edit->setEchoMode(QLineEdit::Password);
        password_edit->hide();

        email_edit->setPlaceholderText("Type the employees email address");
        email_edit->hide();

        display_name_edit->setPlaceholderText("Type the desired display name if different from the default.");
        display_name_edit->hide();

        primary_proxy_edit->setPlaceholderText("Input your primary proxy address");
        primary_proxy_edit->setToolTip("You do not have to type SMTP: ");
        primary_proxy_edit->hide();

        secondary_proxy_edit->setPlaceholderText("Input your secondary proxy address");
        secondary_proxy_edit->setToolTip("You do not have to type smtp: ");
        secondary_proxy_edit->hide();

        create_button->setText("Create");
        create_button->hide();

        generate_button->setText("Generate");

        cancel_button->setText("Cancel");

        main_layout->addWidget(upn_combo);
        main_layout->addWidget(ou_combo);
        main_layout->addWidget(template_user_combo);
        main_layout->addWidget(domain_name_combo);
        main_layout->addWidget(employee_name_edit);
        main_layout->addWidget(user_edit);
        main_layout->addWidget(password_edit);
        main_layout->addWidget(email_edit);
        main_layout->addWidget(display_name_edit);
        main_layout->addWidget(primary_proxy_edit);
        main_layout->addWidget(secondary_proxy_edit);
        main_layout->addWidget(create_button);
        main_layout->addWidget(generate_button);
        main_layout->addWidget(cancel_button);
        primary_display->setLayout(main_layout);
        main_layout->setAlignment(Qt::AlignHCenter);

        return primary_display;
}

QStringList DomainWidget::execute_command(QString param)
{
      QProcess *process = new QProcess();
      QByteArray term_output;
      QStringList params;
      params = QStringList({"-Command", QString("Start-Process -NoNewWindow -Verb runAs powershell; "), param});
      process->start("powershell", params);
      process->waitForFinished();
      term_output.append(process->readAllStandardOutput());
      process->terminate();
      QStringList return_list = QString(term_output).split("\n", QString::SkipEmptyParts);
      return return_list;
}

QString DomainWidget::execute(QString param)
{
    QProcess *process = new QProcess();
    QByteArray term_output;
    QStringList params;
    params = QStringList({"-Command", QString("Start-Process -NoNewWindow -Verb runAs powershell; "), param});
    process->start("powershell", params);
    process->waitForFinished();
    term_output.append(process->readAllStandardOutput());
    process->terminate();
    QString data = QString(term_output);
    return data;
}

void DomainWidget::load_domain_information()
{
    Domain_Name = execute_command("Get-ADForest | Select -ExpandProperty Domains");
    Domain_UPNS = execute_command("Get-ADForest | Select -ExpandProperty UPNSuffixes");
    OU_Names = execute_command("Get-ADOrganizationalUnit -Filter * | Select -ExpandProperty Name");
    OU_DN_Names = execute_command("Get-ADOrganizationalUnit -Filter * | Select -ExpandProperty DistinguishedName");
    AD_Users = execute_command("Get-ADUser -Filter * | Select-Object -ExpandProperty Name");
}
