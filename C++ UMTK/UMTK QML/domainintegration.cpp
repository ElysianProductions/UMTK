#include "domainintegration.h"

DomainIntegration::DomainIntegration()
{

}

void DomainIntegration::setDAUserTemplate(const QString &templateuser)
{
    if(templateuser != template_user)
    {
        template_user = templateuser;
        emit da_TemplateUserChanged();
    }
}

void DomainIntegration::setDAFName(const QString &dafname)
{
    if(dafname != full_name)
    {
        full_name = dafname;
        emit da_FNameChanged();
    }
}

void DomainIntegration::setDAUName(const QString &dauname)
{
    if(dauname != username)
    {
        username = dauname;
        emit da_UNameChanged();
    }
}

void DomainIntegration::setDAEmail(const QString &daemail)
{
    if(daemail != email_address)
    {
        email_address = daemail;
        emit da_EmailChanged();
    }
}

void DomainIntegration::setDADisplayName(const QString &dadpname)
{
    if(dadpname != display_name)
    {
        display_name = dadpname;
        emit da_DisplayNameChanged();
    }
}

void DomainIntegration::setDAPassword(const QString &dapassword)
{
    if(dapassword != password)
    {
        password = dapassword;
        emit da_PasswordChanged();
    }
}

void DomainIntegration::setDAPrimaryProxy(const QString &dapproxy)
{
    if(dapproxy != primary_proxy)
    {
        primary_proxy = dapproxy;
        emit da_PrimaryProxyChanged();
    }
}

void DomainIntegration::setDASecondaryProxy(const QString &dasproxy)
{
    if(dasproxy != secondary_proxy)
    {
        secondary_proxy = dasproxy;
        emit da_SecondaryProxyChanged();
    }
}

void DomainIntegration::load_domain_information()
{
    all_forests = List_All_Forests();
    all_upns = List_All_UPNs();
    ou_names = List_All_OU_CNs();
    ou_dn_names = List_All_OU_DNs();
    all_users = List_All_Domain_Users();
}

void DomainIntegration::Automate()
{

}

QString DomainIntegration::da_template()
{
    return template_user;
}

QString DomainIntegration::da_fname()
{
    return full_name;
}

QString DomainIntegration::da_uname()
{
    return username;
}

QString DomainIntegration::da_email()
{
    return email_address;
}

QString DomainIntegration::da_displayname()
{
    return display_name;
}

QString DomainIntegration::da_password()
{
    return password;
}

QString DomainIntegration::da_pproxy()
{
    return primary_proxy;
}

QString DomainIntegration::da_sproxy()
{
    return secondary_proxy;
}

QString DomainIntegration::Execute(QString param)
{
    QProcess *process = new QProcess();
    QByteArray success;
    //QByteArray errors;
    QStringList params;
    params = QStringList({"-Command", QString("Start-Process -NoNewWindow -Verb runAs powershell; "), param});
    process->start("powershell", params);
    process->waitForFinished(-1);
    success.append(process->readAllStandardOutput());
    //errors.append(process->readAllStandardError());
    //qDebug() << errors;
    process->terminate();
    QString data = QString(success);
    return data;
}

QString DomainIntegration::Clean_String(QString str)
{
    bool bad_chars = true;
      while(bad_chars)
      {
          if(str.contains("\r"))
          {
              str = str.remove(QChar('\r'));
          }
          if(str.contains("\n"))
          {
              str = str.remove(QChar('\n'));
          }
          if(!str.contains("\r") && !str.contains("\n"))
          {
              bad_chars = false;
          }
      }
      return str;
}

QStringList DomainIntegration::Execute_Commands(QString param)
{
    QProcess *process = new QProcess();
    QByteArray term_output;
    QStringList params;
    params = QStringList({"-Command", QString("Start-Process -NoNewWindow -Verb runAs powershell; "), param});
    process->start("powershell", params);
    process->waitForFinished(-1);
    term_output.append(process->readAllStandardOutput());
    process->terminate();
    QStringList return_list = QString(term_output).split("\n", Qt::SkipEmptyParts);
    return return_list;
}

QStringList DomainIntegration::List_All_Forests()
{
    QStringList tmp = Execute_Commands("Get-ADForest | Select -ExpandProperty Domains");
    QStringList Forest_Names;
    for(auto &i : tmp)
    {
        Forest_Names << Clean_String(i);
    }
    return Forest_Names;
}

QStringList DomainIntegration::List_All_UPNs()
{
    QStringList tmp = Execute_Commands("Get-ADForest | Select -ExpandProperty UPNSuffixes");
    QStringList Domain_UPNS;
    for(auto &i : tmp)
    {
        Domain_UPNS << Clean_String(i);
    }
    return Domain_UPNS;
}

QStringList DomainIntegration::List_All_OU_CNs()
{
    QStringList tmp = Execute_Commands("Get-ADOrganizationalUnit -Filter * | Select -ExpandProperty Name");
    QStringList OU_Names;
    for(auto &i : tmp)
    {
        OU_Names << Clean_String(i);
    }
    return OU_Names;
}

QStringList DomainIntegration::List_All_OU_DNs()
{
    QStringList tmp = Execute_Commands("Get-ADOrganizationalUnit -Filter * | Select -ExpandProperty DistinguishedName");
    QStringList OU_DN_Names;
    for(auto &i : tmp)
    {
        OU_DN_Names << Clean_String(i);
    }
    return OU_DN_Names;
}

QStringList DomainIntegration::List_All_Domain_Users()
{
    QStringList tmp = Execute_Commands("$tmp = (Get-ADUser -Filter * | Select-Object Name, GivenName, SurName | Sort-Object SurName, GivenName); return $tmp.Name");
    QStringList AD_Users;
    for(auto &i : tmp)
    {
        AD_Users << Clean_String(i);
    }
    return AD_Users;
}

QStringList DomainIntegration::List_All_Group_CNs()
{
    QStringList dirty_groups = Execute_Commands("Get-ADGroup -Filter * | Select-Object -ExpandProperty Name");
    QStringList cleaned_groups;
    for(auto &i : dirty_groups)
    {
       cleaned_groups << Clean_String(i);
    }
    return cleaned_groups;
}

QStringList DomainIntegration::List_ALL_Group_DNs()
{
    QStringList dirty_groups_distinguished = Execute_Commands("Get-ADGroup -Filter * | Select-Object -ExpandProperty DistinguishedName");
    QStringList cleaned_group_distinguished;

    for(auto &i : dirty_groups_distinguished)
    {
        cleaned_group_distinguished << Clean_String(i);
    }
    return cleaned_group_distinguished;
}

QStringList DomainIntegration::List_User_Group_CNs(QString SamName)
{
    QStringList dirty_groups = Execute_Commands("Get-ADPrincipalGroupMembership -Identity " +  SamName + " | Select-Object -ExpandProperty Name");
    QStringList cleaned_groups;
    for(auto &i : dirty_groups)
    {
        cleaned_groups << Clean_String(i);
    }
    return cleaned_groups;
}

QStringList DomainIntegration::List_User_Group_DNs(QString SamName)
{
    QStringList dirty_groups_distinguished = Execute_Commands("Get-ADPrincipalGroupMembership -Identity " +  SamName + " | Select-Object -ExpandProperty DistinguishedName");
    QStringList cleaned_group_distinguished;

    for(auto &i : dirty_groups_distinguished)
    {
        cleaned_group_distinguished << Clean_String(i);
    }
    return cleaned_group_distinguished;
}








//
