#ifndef DOMAININTEGRATION_H
#define DOMAININTEGRATION_H


#include <QObject>
#include <QProcess>
#include <QString>
#include <QStringList>


#include <QDebug>

class DomainIntegration : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString da_template READ da_template WRITE setDAUserTemplate NOTIFY da_TemplateUserChanged) // Property 1.
    Q_PROPERTY(QString da_fname READ da_fname WRITE setDAFName NOTIFY da_FNameChanged) // Property 2.
    Q_PROPERTY(QString da_uname READ da_uname WRITE setDAUName NOTIFY da_UNameChanged) // Property 3.
    Q_PROPERTY(QString da_email READ da_email WRITE setDAEmail NOTIFY da_EmailChanged) // Property 4.
    Q_PROPERTY(QString da_displayname READ da_displayname WRITE setDADisplayName NOTIFY da_DisplayNameChanged) // Property 5.
    Q_PROPERTY(QString da_password READ da_password WRITE setDAPassword NOTIFY da_PasswordChanged) // Property 6.
    Q_PROPERTY(QString da_pproxy READ da_pproxy WRITE setDAPrimaryProxy NOTIFY da_PrimaryProxyChanged) // Property 7.
    Q_PROPERTY(QString da_sproxy READ da_sproxy WRITE setDASecondaryProxy NOTIFY da_SecondaryProxyChanged) // Property 8.

public:
    DomainIntegration();

    void setDAUserTemplate(const QString &templateuser); // See Q_PROPERTY labeled Property 1.
    void setDAFName(const QString &dafname); // See Q_PROPERTY labeled Property 2.
    void setDAUName(const QString &dauname); // See Q_PROPERTY labeled Property 3.
    void setDAEmail(const QString &daemail); // See Q_PROPERTY labeled Property 4.
    void setDADisplayName(const QString &dadpname); // See Q_PROPERTY labeled Property 5.
    void setDAPassword(const QString &dapassword); // See Q_PROPERTY labeled Property 6.
    void setDAPrimaryProxy(const QString &dapproxy); // See Q_PROPERTY labeled Property 7.
    void setDASecondaryProxy(const QString &dasproxy); // See Q_PROPERTY labeled Property 8.

    QString da_template(); // See Q_PROPERTY labeled Property 1.
    QString da_fname(); // See Q_PROPERTY labeled Property 2.
    QString da_uname(); // See Q_PROPERTY labeled Property 3.
    QString da_email(); // See Q_PROPERTY labeled property 4.
    QString da_displayname(); // See Q_PROPERTY labeled property 5.
    QString da_password(); // See Q_PROPERTY labeled property 6.
    QString da_pproxy(); // See Q_PROPERTY labeled Property 7.
    QString da_sproxy(); // See Q_PROPERTY labeled Property 8.

    QString Execute(QString param);
    QStringList Execute_Commands(QString param);

protected:

public slots:
    void Automate();


signals:
    void da_TemplateUserChanged(); // User selection in combobox changed.
    void da_FNameChanged(); // Fullname in textedit changed.
    void da_UNameChanged(); // Username in textedit changed.
    void da_EmailChanged(); // Email in textedit changed.
    void da_DisplayNameChanged(); // Display name in textedit changed.
    void da_PasswordChanged(); // Password in textedit changed.
    void da_PrimaryProxyChanged(); // Primary SMTP proxy address in textedit changed.
    void da_SecondaryProxyChanged(); // Secondary SMTP proxy address in textedit changed.


private:
    QString template_user; // Template user to copy select from the combo box and set by setDAUserTemplate.
    QString full_name; //Full name of new employee.
    QString username; // Username of new employee.
    QString display_name; // Display name of new employee (by default set to the full name).
    QString email_address; // The email of the new employee (byt default full_name + @ + domain/upn.
    QString password; // Password of the new employee.
    QString primary_proxy; // Primary proxy address of the new employee (by default the generated/edited email address).
    QString secondary_proxy; // Secondarty proxy address of the new employee.

    QString Clean_String(QString str);

    void load_domain_information();

    QStringList all_users; // All users found in the domain.
    QStringList all_forests; // All forests found in the domain.
    QStringList all_upns; // All UPNs found in the domain.
    QStringList ou_names; // All cleaned organizational names in the domain.
    QStringList ou_dn_names; // All distinguished names of all the organizational units found in the domain.
    QStringList user_group_cns; // All cleaned group names of the template user.
    QStringList user_group_dns; // All distinguished names of the template users groups.
    QStringList all_group_dns; // All group distinguished names in the domain.
    QStringList all_group_cns; // All clean group names in the domain.


    QStringList List_All_Forests();
    QStringList List_All_UPNs();
    QStringList List_All_OU_CNs();
    QStringList List_All_OU_DNs();
    QStringList List_All_Domain_Users();
    QStringList List_All_Group_CNs();
    QStringList List_ALL_Group_DNs();
    QStringList List_User_Group_CNs(QString SamName);
    QStringList List_User_Group_DNs(QString SamName);

};

#endif // DOMAININTEGRATION_H
