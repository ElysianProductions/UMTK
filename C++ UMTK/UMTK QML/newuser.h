#ifndef NEWUSER_H
#define NEWUSER_H
#include <QString>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QProcess>

#include "psintegration.h"

class NewUser : public PSIntegration
{
public:
    NewUser();
    bool validate_password(QString password, QString MinPasswordLength, QString ComplexityEnabled);

    QString password_generator(QString MinPasswordLength, QString ComplexityEnabled);
    QString get_UPN();
    QString get_Name();
    QString get_GivenName();
    QString get_SurName();
    QString get_OtherName();
    QString get_Initials();
    QString get_DisplayName();
    QString get_SamAccountName();
    QString get_OU_DN();
    QString get_OU_CN();
    QString get_Mail();
    QString does_user_exist(QString samname);
    QString clean_string(QString str);
    QString execute(QString param);
    QStringList get_Groups();
    QStringList get_GroupDNs();
    QString get_Identifier();
    QString get_User_DN();

    void set_password(QString password);
    void set_UPN(QString UPN);
    void set_Name(QString name);
    void set_GivenName(QString givenname);
    void set_SurName(QString surname);
    void set_OtherName(QString middlename);
    void set_Initials(QString initials);
    void set_DisplayName(QString displayname);
    void set_SamAccountName(QString samaccountname);
    void set_OU_DN(QString oudistinguishedname);
    void set_OU_CN(QString oucleanname);
    void set_Mail(QString mail);
    void set_Groups(QStringList groups);
    void set_GroupDNs(QStringList groupsdistinguished);
    void set_Identifier(QString identifier);
    void set_User_DN(QString DN);

    void set_attrib_upn(QString UPN);
    void set_attrib_name(QString name);
    void set_attrib_GivenName(QString givenname);
    void set_attrib_surname(QString surname);
    void set_attrib_Initials(QString initials);
    void set_attrib_DisplayName(QString displayname);
    void set_attrib_SamAccountName(QString samaccountname);
    void set_attrib_OUDistinguished(QString oudistinguishedname);
    void set_attrib_Mail(QString mail);
    void set_attrib_(QStringList groupdistinguishednames);
private:


    QString user_distinguished_name;
    QString Name;
    QString GivenName;
    QString SurName;
    QString OtherName;
    QString Initials;
    QString DisplayName;
    QString Password;
    QString SamAccountName;
    QString UserPrincipalName;
    QString OU_distinguished_name;
    QString OU_Clean_Name;
    QString Mail;
    QString Identifier;
    QStringList Groups;
    QStringList GroupsDistinguished;
    QString User_DN;


};

#endif // NEWUSER_H
