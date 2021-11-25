#ifndef DOMAINWIDGET_H
#define DOMAINWIDGET_H

#include <QObject>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QWidget>
#include <QGridLayout>
#include <QString>
#include <QStringList>
#include <QProcess>

class DomainWidget : public QObject
{
    Q_OBJECT

public:
    DomainWidget();
    QWidget *get_widget();



    QComboBox *upn_combo;
    QComboBox *ou_combo;
    QComboBox *template_user_combo;
    QComboBox *domain_name_combo;
    QLineEdit *employee_name_edit;
    QLineEdit *user_edit;
    QLineEdit *password_edit;
    QLineEdit *email_edit;
    QLineEdit *display_name_edit;
    QLineEdit *primary_proxy_edit;
    QLineEdit *secondary_proxy_edit;
    QPushButton *create_button;
    QPushButton *cancel_button;
    QPushButton *generate_button;

    QStringList commands;

    // Experimental
    void load_domain_information();


    QStringList AD_Users;
    QStringList Domain_UPNS;
    QStringList OU_Names;
    QStringList OU_DN_Names;
    QStringList Domain_Name;

private:
    QStringList execute_command(QString param);
    QString execute(QString param);

    QWidget* initalize_widget(QComboBox *upn_combo, QComboBox *ou_combo, QComboBox *template_user_combo,
                              QComboBox *domain_name_combo, QLineEdit *employee_name_edit, QLineEdit *user_edit,
                              QLineEdit *password_edit, QLineEdit *email_edit, QLineEdit *display_name_edit,
                              QLineEdit *primary_proxy_edit, QLineEdit *secondary_proxy_edit, QPushButton *create_button,
                              QPushButton *cancel_button, QPushButton *generate_button);


    // EXPERIMENTAL
    void load_template_user(QString user);



    struct
    {
        QString DistinguishedName;  // : CN=Maria Franz,OU=IT Staff,OU=Elysium,DC=Elysium,DC=local
        QString Enabled;            // : True
        QString GivenName;          // : Maria
        QString Name;               // : Maria Franz
        QString ObjectClass;        // : user
        QString ObjectGUID;         // : 12657ca8-8bce-4815-83d1-26496d6662a4
        QString SamAccountName;     // : Mfranz
        QString SID;                // : S-1-5-21-1585572130-2828728486-3327902743-1219
        QString Surname;            // : Franz
        QString UserPrincipalName;  // : Mfranz@Elysium.local
        QString MiddleName;         // : Set-ADUser ((Get-ADUser -Filter {Name -Like "the name"} -Properties SamAccountName).SameAccountName) -Replace OtherName $middleName
        QString Initials;           // : A. J
        QString Mail;               // : example@example.com
        QStringList ProxyAddresses; // : (Get-ADUser -Filter {Name -like "Maria Franz"} -Properties Proxyaddresses).Proxyaddresses
    } ADTemplateUSer;
};

#endif // DOMAINWIDGET_H
