#include "templateuser.h"

TemplateUser::TemplateUser()
{
    user_distinguished_name = "";
    SamAccountName = "";
    UserPrincipalName = "";
    OU_distinguished_name = "";
    OU_Clean_Name = "";
    Mail = "";
    Name = "";
}

QString TemplateUser::get_template_user_dn()
{
    return user_distinguished_name;
}

QString TemplateUser::get_name()
{
    return Name;
}

QString TemplateUser::get_samaccount_name()
{
    return SamAccountName;
}

QString TemplateUser::get_userprincipal_name()
{
    return UserPrincipalName;
}

QString TemplateUser::get_mail()
{
    return Mail;
}

QStringList TemplateUser::get_groups()
{
    return Groups;
}

QStringList TemplateUser::get_GroupDNs()
{
    return GroupsDistinguished;
}

QString TemplateUser::get_OrganizationalUnitDN()
{
    return OU_distinguished_name;
}

QString TemplateUser::get_OrganizationalUnitCN()
{
    return OU_Clean_Name;
}

void TemplateUser::set_template_user_dn(QString str) // Migrated to PSIntegration class
{
    user_distinguished_name = clean_string(execute("(Get-ADUser -Filter {Name -Like " + QString("\"") + str + QString("\"") + "} -Properties DistinguishedName).DistinguishedName"));
}

void TemplateUser::set_name(QString str) // Migrated to PSIntegration class.
{
     Name = clean_string(str);
    //Name = execute_command("(Get-ADUser -Filter {Name -Like " + QString("\"") + str + QString("\"") + "} -Properties Name).Name");
}

void TemplateUser::set_samaccount_name(QString str) // Migrated to PSIntegration class
{
    SamAccountName = clean_string(execute("(Get-ADUser -Filter {Name -Like " + QString("\"") + str + QString("\"") + "} -Properties SamAccountName).SamAccountName"));
}

void TemplateUser::set_userprincipal_name(QStringList UPNs, QStringList Domains, QString name) // Migrated to PSIntegration
{
    QStringList cleaned_upns;
    QStringList cleaned_domains;
    for(auto &i : UPNs)
    {
        cleaned_upns << clean_string(i);
    }
    for(auto &i : Domains)
    {
        cleaned_domains << clean_string(i);
    }

    if(cleaned_upns.count() > 0)
    {
        UserPrincipalName = "@" + clean_string(execute("$temp = (Get-ADUser -Filter {Name -like " + QString("\"") + name + QString("\"") + " } -Properties UserPrincipalName).UserPrincipalName; $garbage, $upn = $temp.Split('@'); return $upn "));
    }
    else if(cleaned_upns.count() == 0)
    {
        UserPrincipalName = "@" + cleaned_domains.first(); // Needs to be fixed to work in multi-domain environments.
    }
}

void TemplateUser::set_mail(QString str) // Migrated to PSIntegration class.
{
    Mail = clean_string(execute("(Get-ADUser -Filter {Name -Like " + QString("\"") + str + QString("\"") + "} -Properties Mail).Mail"));
}

void TemplateUser::set_groups(QString str) // Migrated to PSIntegration class.
{
    QStringList tmp = execute_command("Get-ADPrincipalGroupMembership -Identity " +  str + " | Select-Object -ExpandProperty Name");
    QStringList tmp_two = execute_command("Get-ADPrincipalGroupMembership -Identity " +  str + " | Select-Object -ExpandProperty DistinguishedName");
    for(auto &i : tmp)
    {
        Groups << clean_string(i);
    }
    for(auto &j : tmp_two)
    {
        GroupsDistinguished << clean_string(j);
    }
}

void TemplateUser::set_OrganizationalUnitDN(QString name) // Migrated to PSIntegration class
{
    OU_distinguished_name = clean_string(execute("$temp = (Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "}); $t = $temp.DistinguishedName; $garbage, $OU = $t.split(',', 2); return $OU"));
    set_OrganizationalUnitCN(name);
}

void TemplateUser::set_OrganizationalUnitCN(QString name) // Migrated to PSIntegration class
{
   OU_Clean_Name = clean_string(execute("$temp = (Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "}); $t = $temp.DistinguishedName; $garbage, $OU = $t.split(',', 2); $clean, $junk = $OU.split(','); return $clean")).remove(0, 3);
}

QStringList TemplateUser::execute_command(QString param) // Migrated to PSIntegration class
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

QString TemplateUser::execute(QString param) // Migrated to PSIntegration class
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

QString TemplateUser::get_ActiveSP_length()
{
    return active_SP_MinLength;
}

QString TemplateUser::get_ActiveSP_Complexity()
{
    return active_SP_Complexity;
}

void TemplateUser::set_FGPP_active(QString MinLength, QString Complexity)
{
    active_SP_MinLength = MinLength;
    active_SP_Complexity = Complexity;
}

void TemplateUser::set_DDPP_active(QString MinLength, QString Complexity)
{
    active_SP_MinLength = MinLength;
    active_SP_Complexity = Complexity;
}

void TemplateUser::set_APP_active(QString MinLength, QString Complexity)
{
    active_SP_MinLength = MinLength;
    active_SP_Complexity = Complexity;
}

void TemplateUser::detect_password_policy(QString name) // Migrated to PSIntegration class.
{
    QString probe = clean_string(execute("$var = (Get-Module -ListAvailable -Name " + QString("\"") + "ActiveDirectory" + QString("\"") + "); if($var -ne $null){return " + QString("\"") + "Domain" + QString("\"") +"}; $var"));
    if(probe == "Domain")
    {

        probe = clean_string(execute("$var = (Get-ADFineGrainedPasswordPolicy -Filter * -ErrorAction SilentlyContinue); if($var -ne $null){return " + QString("\"") + "Found Grain" + QString("\"") + "}; $var"));
        if(probe == "Found Grain")
        {
            QStringList tmp = execute_command("Get-ADFineGrainedPasswordPolicy -Filter * | Select-Object -ExpandProperty AppliesTo");
            for(auto &i : tmp)
            {
                FGPP_AppliesTo << clean_string(i);
            }
            int isGrained = 0;
            int counter = 0;
            if(GroupsDistinguished.length() < FGPP_AppliesTo.length())
            {
                counter = GroupsDistinguished.length();
                for(int i = 0; i < FGPP_AppliesTo.length(); ++i)
                {
                    for(int j = 0; j < counter; ++j)
                    {
                        if(GroupsDistinguished.at(j) == FGPP_AppliesTo.at(i))
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
            else if(GroupsDistinguished.length() > FGPP_AppliesTo.length())
            {
                counter = FGPP_AppliesTo.length();
                for(int i = 0; i < GroupsDistinguished.length(); ++i)
                {
                    for(int j = 0; j < counter; ++j)
                    {
                        if(FGPP_AppliesTo.at(j) == GroupsDistinguished.at(i))
                        {
                            isGrained = 1;
                            FGPP_MatchGrainPolicy = j;
                        }
                        if(isGrained == 0 && j == counter && i != GroupsDistinguished.length())
                        {
                            j = 0;
                        }
                    }
                 }
            }
             if(isGrained == 1)
             {
                 QStringList tmp = execute_command("Get-ADFineGrainedPasswordPolicy -Filter * | Select-Object -ExpandProperty Name");
                 for(auto &i : tmp)
                 {
                     FGPP_Names << clean_string(i);
                 }
                 FGPP_ComplexityEnabled = clean_string(execute("Get-ADFineGrainedPasswordPolicy -Filter {Name -Like " + QString("\"") + FGPP_Names.at(FGPP_MatchGrainPolicy) + QString("\"") + "} | Select-Object -ExpandProperty ComplexityEnabled"));
                 FGPP_MinPasswordLength = clean_string(execute("Get-ADFineGrainedPasswordPolicy -Filter {Name -Like " + QString("\"") + FGPP_Names.at(FGPP_MatchGrainPolicy) + QString("\"") + "} | Select-Object -ExpandProperty MinPasswordLength"));
                 FGPP_DistinguishedName = clean_string(execute("Get-ADFineGrainedPasswordPolicy -Filter {Name -Like " + QString("\"") + FGPP_Names.at(FGPP_MatchGrainPolicy) + QString("\"") + "} | Select-Object -ExpandProperty DistinguishedName"));
                 set_FGPP_active(FGPP_MinPasswordLength, FGPP_ComplexityEnabled);
             }
             else if(isGrained == 0)
             {
                DDPP_ComplexityEnabled = clean_string(execute("Get-ADDefaultDomainPasswordPolicy | Select-Object -ExpandProperty ComplexityEnabled"));
                DDPP_MinPasswordLength = clean_string(execute("Get-ADDefaultDomainPasswordPolicy | Select-Object -ExpandProperty MinPasswordLength"));
                set_DDPP_active(DDPP_MinPasswordLength, DDPP_ComplexityEnabled);
             }
        }
        else
        {
            DDPP_ComplexityEnabled = clean_string(execute("Get-ADDefaultDomainPasswordPolicy | Select-Object -ExpandProperty ComplexityEnabled"));
            DDPP_MinPasswordLength = clean_string(execute("Get-ADDefaultDomainPasswordPolicy | Select-Object -ExpandProperty MinPasswordLength"));
            set_DDPP_active(DDPP_MinPasswordLength, DDPP_ComplexityEnabled);
        }
    }
    else
    {
        set_APP_active("10", "True");
    }
}

QString TemplateUser::clean_string(QString str) // Migrated to PSIntegration class
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


//
