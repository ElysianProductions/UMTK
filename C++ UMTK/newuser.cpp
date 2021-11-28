#include "newuser.h"

NewUser::NewUser()
{

}



bool NewUser::validate_password(QString password, QString MinPasswordLength, QString ComplexityEnabled)
{
    if(password.length() >= MinPasswordLength.toInt())
    {
        if(ComplexityEnabled == "True")
        {
            QRegularExpression re;
            QRegularExpressionMatch match;
            re.setPattern("(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*[^A-Za-z0-9])");
            match = re.match(password);
            bool matching = match.hasMatch();
            if(matching == true)
            {
                return true;
            }
            else if(matching == false)
            {
                return false;
            }
        }
        else if(ComplexityEnabled == "False")
        {
            QRegularExpression re;
            QRegularExpressionMatch match;
            re.setPattern("(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*[^A-Za-z0-9])");
            match = re.match(password);
            bool matching = match.hasMatch();
            if(matching == true)
            {
                return true;
            }
            else if(matching == false)
            {
                return false;
            }
        }
    }
    if(password.length() < MinPasswordLength.toInt())
    {
        return false;
    }
}

QString NewUser::password_generator(QString MinPasswordLength, QString ComplexityEnabled)
{
  // remove
    return QString("");
}

QString NewUser::get_UPN()
{
    return UserPrincipalName;
}

QString NewUser::get_Name()
{
    return Name;
}

QString NewUser::get_GivenName()
{
    return GivenName;
}

QString NewUser::get_SurName()
{
    return SurName;
}

QString NewUser::get_OtherName()
{
    return OtherName;
}

QString NewUser::get_Initials()
{
    return Initials;
}

QString NewUser::get_DisplayName()
{
    return DisplayName;
}

QString NewUser::get_SamAccountName()
{
    return SamAccountName;
}

QString NewUser::get_OU_DN()
{
    return OU_distinguished_name;
}

QString NewUser::get_OU_CleanName()
{
    return OU_Clean_Name;
}

QString NewUser::get_Mail()
{
    return Mail;
}

QString NewUser::get_Identifier()
{
    return Identifier;
}

QStringList NewUser::get_Groups()
{
    return Groups;
}

QStringList NewUser::get_GroupDNs()
{
    return GroupsDistinguished;
}

void NewUser::set_password(QString password)
{
    Password = password;
}

void NewUser::set_UPN(QString UPN)
{
    UserPrincipalName = UPN;
}

void NewUser::set_Name(QString name)
{
    Name = name;
}

void NewUser::set_GivenName(QString givenname)
{
    GivenName = givenname;
}

void NewUser::set_SurName(QString surname)
{
    SurName = surname;
}

void NewUser::set_OtherName(QString middlename)
{
    OtherName = middlename;
}

void NewUser::set_Initials(QString initials)
{
 Initials = initials;
}

void NewUser::set_DisplayName(QString displayname)
{
    DisplayName = displayname;
}

void NewUser::set_SamAccountName(QString samaccountname)
{
    SamAccountName = samaccountname;
}

void NewUser::set_OU_DN(QString oudistinguishedname)
{
    OU_distinguished_name = oudistinguishedname;
}

void NewUser::set_OU_CleanName(QString oucleanname)
{
    OU_Clean_Name = oucleanname;
}

void NewUser::set_Mail(QString mail)
{
    Mail = mail;
}

void NewUser::set_Identifier(QString identifier)
{
    Identifier = identifier;
}

void NewUser::set_Groups(QStringList groups)
{
    Groups = groups;
}

void NewUser::set_GroupDNs(QStringList groupsdistinguished)
{
    GroupsDistinguished = groupsdistinguished;
}
//
