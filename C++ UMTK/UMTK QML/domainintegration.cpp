#include "domainintegration.h"

DomainIntegration::DomainIntegration()
{
    load_domain_information();

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

void DomainIntegration::setDAAllUsers(const QStringList &daallusers)
{
    if(daallusers != all_users)
    {
        all_users = daallusers;
        emit da_AllUsersChanged();
    }
}

void DomainIntegration::setDAAllUPNs(const QStringList &daallupns)
{
    if(daallupns != all_upns)
    {
        all_upns = daallupns;
        emit da_AllUPNsChanged();
    }
}

void DomainIntegration::setDAAllOUCns(const QStringList &daalloucns)
{
    if(daalloucns != ou_names)
    {
        ou_names = daalloucns;
        emit da_AllOUCNsChanged();
    }
}

void DomainIntegration::setOUComboIndex(const int &daouselection)
{
    if(daouselection != ou_cn_selection)
    {
        ou_cn_selection = daouselection;
        emit da_OUComboIndexChanged();
    }
}

void DomainIntegration::setUPNComboIndex(const int &daupnselection)
{
    if(daupnselection != upn_selection)
    {
        upn_selection = daupnselection;
        emit da_UPNComboIndexChanged();
    }
}

void DomainIntegration::setDAComplexityPolicy(const QString &dacomplexity)
{
    if(dacomplexity != active_SP_Complexity)
    {
        active_SP_Complexity = dacomplexity;
        emit da_ComplexityChanged();
    }
}

void DomainIntegration::setDALengthPolicy(const QString &dalengthpolicy)
{
    if(dalengthpolicy != active_SP_MinLength)
    {
        active_SP_MinLength = dalengthpolicy;
        emit da_LengthPolicyChanged();
    }
}

void DomainIntegration::load_domain_information()
{
    all_forests = List_All_Forests();
    all_upns = List_All_UPNs();  
    if(all_upns.count() > 0)
    {
        setDAAllUPNs(all_upns);
    }
    else if(all_upns.count() <= 0)
    {
        setDAAllUPNs(all_forests);
    }
    setDAAllOUCns(List_All_OU_CNs());
    ou_dn_names = List_All_OU_DNs();
    setDAAllUsers(List_All_Domain_Users());
}

void DomainIntegration::automate()
{
    if(da_fname().length() > 0 && da_fname().contains(" "))
    {
        given_name = da_fname().split(" ").first();
        sur_name = da_fname().split(" ").last();
        if(da_displayname().length() <= 0)
        {
            setDADisplayName(da_fname());
        }
        setDAUName(SamAccountName = given_name.at(0).toUpper() + sur_name.toLower());
        setDAEmail(UserPrincipalName = SamAccountName + "@" + List_User_Identifier(Clean_String(da_template())));

        user_group_dns = List_User_Group_DNs(List_SamAccountName(Clean_String(da_template())));
        user_group_cns = List_User_Group_CNs(List_SamAccountName(Clean_String(da_template())));



        ou_distinguished_name = List_User_OU_DN(Clean_String(da_template()));
        ou_clean_name = List_User_OU_CN(Clean_String(da_template()));


        QString tmp_upn = List_User_Identifier(Clean_String(da_template()));
        for(auto i = 0; i < all_upns.count(); ++i)
        {
            if(tmp_upn == all_upns.at(i))
            {
                setUPNComboIndex(i);
            }
        }


        for(auto i = 0; i < ou_names.count(); ++i)
        {
            if(ou_clean_name == ou_names.at(i))
            {
                setOUComboIndex(i);
            }
        }

        List_Password_Policy(Clean_String(da_template()));

        // See lines 628 - 658 in original psintegration.cpp it just needs to be handled from within DomainAccountWidget.qml
    }
    else
    {

    }
}

void DomainIntegration::Set_FGPP_active(QString MinLength, QString Complexity)
{
    active_SP_MinLength = MinLength;
    active_SP_Complexity = Complexity;
}

void DomainIntegration::Set_DDPP_active(QString MinLength, QString Complexity)
{
    active_SP_MinLength = MinLength;
    active_SP_Complexity = Complexity;
}

void DomainIntegration::Set_APP_active(QString MinLength, QString Complexity)
{
    active_SP_MinLength = MinLength;
    active_SP_Complexity = Complexity;
}

void DomainIntegration::List_Password_Policy(QString name)
{
    QString probe = Clean_String(Execute("$var = (Get-Module -ListAvailable -Name " + QString("\"") + "ActiveDirectory" + QString("\"") + "); if($var -ne $null){return " + QString("\"") + "Domain" + QString("\"") +"}; $var"));
    if(probe == "Domain")
    {

        probe = Clean_String(Execute("$var = (Get-ADFineGrainedPasswordPolicy -Filter * -ErrorAction SilentlyContinue); if($var -ne $null){return " + QString("\"") + "Found Grain" + QString("\"") + "}; $var"));
        if(probe == "Found Grain")
        {
            QStringList tmp = Execute_Commands("Get-ADFineGrainedPasswordPolicy -Filter * | Select-Object -ExpandProperty AppliesTo");
            for(auto &i : tmp)
            {
                FGPP_AppliesTo << Clean_String(i);
            }
            int isGrained = 0;
            int counter = 0;
            if(List_User_Group_DNs(List_SamAccountName(name)).length() < FGPP_AppliesTo.length())
            {
                counter = List_User_Group_DNs(List_SamAccountName(name)).length();
                for(int i = 0; i < FGPP_AppliesTo.length(); ++i)
                {
                    for(int j = 0; j < counter; ++j)
                    {
                        if(List_User_Group_DNs(List_SamAccountName(name)).at(j) == FGPP_AppliesTo.at(i))
                        {
                            isGrained = 1;
                            FGPP_MatchGrainPolicy = i;
                        }
                        if(isGrained == 0 && j == counter && i != FGPP_AppliesTo.length())
                        {
                            j = 0;
                        }
                    }
                 }
            }
            else if(List_User_Group_DNs(List_SamAccountName(name)).length() > FGPP_AppliesTo.length())
            {
                counter = FGPP_AppliesTo.length();
                for(int i = 0; i < List_User_Group_DNs(List_SamAccountName(name)).length(); ++i)
                {
                    for(int j = 0; j < counter; ++j)
                    {
                        if(FGPP_AppliesTo.at(j) == List_User_Group_DNs(List_SamAccountName(name)).at(i))
                        {
                            isGrained = 1;
                            FGPP_MatchGrainPolicy = j;
                        }
                        if(isGrained == 0 && j == counter && i != List_User_Group_DNs(List_SamAccountName(name)).length())
                        {
                            j = 0;
                        }
                    }
                 }
            }
             if(isGrained == 1)
             {
                 QStringList tmp = Execute_Commands("Get-ADFineGrainedPasswordPolicy -Filter * | Select-Object -ExpandProperty Name");
                 for(auto &i : tmp)
                 {
                     FGPP_Names << Clean_String(i);
                 }
                 FGPP_ComplexityEnabled = Clean_String(Execute("Get-ADFineGrainedPasswordPolicy -Filter {Name -Like " + QString("\"") + FGPP_Names.at(FGPP_MatchGrainPolicy) + QString("\"") + "} | Select-Object -ExpandProperty ComplexityEnabled"));
                 FGPP_MinPasswordLength = Clean_String(Execute("Get-ADFineGrainedPasswordPolicy -Filter {Name -Like " + QString("\"") + FGPP_Names.at(FGPP_MatchGrainPolicy) + QString("\"") + "} | Select-Object -ExpandProperty MinPasswordLength"));
                 FGPP_DistinguishedName = Clean_String(Execute("Get-ADFineGrainedPasswordPolicy -Filter {Name -Like " + QString("\"") + FGPP_Names.at(FGPP_MatchGrainPolicy) + QString("\"") + "} | Select-Object -ExpandProperty DistinguishedName"));
                 Set_FGPP_active(FGPP_MinPasswordLength, FGPP_ComplexityEnabled);
             }
             else if(isGrained == 0)
             {
                DDPP_ComplexityEnabled = Clean_String(Execute("Get-ADDefaultDomainPasswordPolicy | Select-Object -ExpandProperty ComplexityEnabled"));
                DDPP_MinPasswordLength = Clean_String(Execute("Get-ADDefaultDomainPasswordPolicy | Select-Object -ExpandProperty MinPasswordLength"));
                Set_DDPP_active(DDPP_MinPasswordLength, DDPP_ComplexityEnabled);
             }
        }
        else
        {
            DDPP_ComplexityEnabled = Clean_String(Execute("Get-ADDefaultDomainPasswordPolicy | Select-Object -ExpandProperty ComplexityEnabled"));
            DDPP_MinPasswordLength = Clean_String(Execute("Get-ADDefaultDomainPasswordPolicy | Select-Object -ExpandProperty MinPasswordLength"));
            Set_DDPP_active(DDPP_MinPasswordLength, DDPP_ComplexityEnabled);
        }
    }
    else
    {
        Set_APP_active("10", "True");
    }
}

QString DomainIntegration::List_ActiveSP_length()
{
    return active_SP_MinLength;
}

QString DomainIntegration::List_ActiveSP_Complexity()
{
    return active_SP_Complexity;
}

QString DomainIntegration::List_User_OU_CN(QString name)
{
    return Clean_String(Execute("$temp = (Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "}); $t = $temp.DistinguishedName; $garbage, $OU = $t.split(',', 2); $clean, $junk = $OU.split(','); return $clean")).remove(0, 3);
}

QString DomainIntegration::List_User_OU_DN(QString name)
{
    return Clean_String(Execute("$temp = (Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "}); $t = $temp.DistinguishedName; $garbage, $OU = $t.split(',', 2); return $OU"));
}

QString DomainIntegration::List_SamAccountName(QString name)
{
    return Clean_String(Execute("(Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "} -Properties SamAccountName).SamAccountName"));
}

QString DomainIntegration::List_User_Identifier(QString name)
{
    if(all_upns.count() > 0)
    {
        return Clean_String(Execute("$temp = (Get-ADUser -Filter {Name -like " + QString("\"") + name + QString("\"") + " } -Properties UserPrincipalName).UserPrincipalName; $garbage, $upn = $temp.Split('@'); return $upn "));
    }
    else if(all_upns.count() == 0)
    {
        return all_forests.first(); // Needs to be fixed to work in multi-domain environments.
    }
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

QString DomainIntegration::da_complexitypolicy()
{
    return List_ActiveSP_Complexity();
}

QString DomainIntegration::da_lengthpolicy()
{
    return active_SP_MinLength;
}

int DomainIntegration::da_ouselect()
{
    return ou_cn_selection;
}

int DomainIntegration::da_upnselect()
{
    return upn_selection;
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

QStringList DomainIntegration::get_all_users()
{
    return all_users;
}

QStringList DomainIntegration::da_allusers()
{
    return all_users;
}

QStringList DomainIntegration::da_allupns()
{
    return all_upns;
}

QStringList DomainIntegration::da_alloucns()
{
    return ou_names;
}




//
