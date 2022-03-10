#include "advancedsettings.h"

AdvancedSettings::AdvancedSettings()
{

}

void AdvancedSettings::setOUDName(const QString &ou_dn)
{
    if(ou_dn != distinguished_name)
    {
        distinguished_name = ou_dn;
        Q_EMIT _DNameChanged();
    }
}

void AdvancedSettings::setCompanyName(const QString &c_name)
{
    if(c_name != company_name)
    {
        company_name = c_name;
        Q_EMIT _CompanyNameChanged();
    }
}


void AdvancedSettings::setUserPrefix(const QString &u_prefix)
{
    if(u_prefix != user_prefix)
    {
        user_prefix = u_prefix;
        Q_EMIT _UserPrefixChanged();
    }
}

void AdvancedSettings::insert_data(const QString &ou, const QString &company, const QString &user)
{
    qDebug() << "Distinguished name - " + ou << "Company name - " + company << "User prefix - " + user;
}

void AdvancedSettings::create_database(QString path)
{

}

bool AdvancedSettings::dbExists(QString path)
{
    QFile file(path);
    if(file.exists())
    {
        return true;
    }
    else if(!file.exists())
    {
        return false;
    }
}

QString AdvancedSettings::get_ou_dn()
{
    return distinguished_name;
}

QString AdvancedSettings::get_company_name()
{
    return company_name;
}

QString AdvancedSettings::get_user_prefix()
{
    return user_prefix;
}
