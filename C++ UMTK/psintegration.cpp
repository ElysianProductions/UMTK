#include "psintegration.h"

PSIntegration::PSIntegration()
{

}


QStringList PSIntegration::Execute_Command(QString param)
{
    /*
     *
     *
     *
     */

    QProcess *process = new QProcess();
    QByteArray term_output;
    QStringList params;
    params = QStringList({"-Command", QString("Start-Process -NoNewWindow -Verb runAs powershell; "), param});
    process->start("powershell", params);
    process->waitForFinished();
    term_output.append(process->readAllStandardOutput());
    process->terminate();
    QStringList return_list = QString(term_output).split("\n", Qt::SkipEmptyParts);
    return return_list;
}

QStringList PSIntegration::List_All_Group_DNs(QString SamName)
{
    /*
     *
     *
     */

    QStringList dirty_groups_distinguished = Execute_Command("Get-ADGroup -Filter * | Select-Object -ExpandProperty DistinguishedName");
    QStringList cleaned_group_distinguished;

    for(auto &i : dirty_groups_distinguished)
    {
        cleaned_group_distinguished << Clean_String(i);
    }
    return cleaned_group_distinguished;
}

QStringList PSIntegration::List_All_Group_CNs(QString SamName)
{
    /*
     *
     *
     */

    QStringList dirty_groups = Execute_Command("Get-ADGroup -Filter * | Select-Object -ExpandProperty Name");
    QStringList cleaned_groups;
    for(auto &i : dirty_groups)
    {
        dirty_groups << Clean_String(i);
    }
    return cleaned_groups;
}

QStringList PSIntegration::List_User_Group_DNs(QString SamName)
{
    /*
     *
     *
     */

    QStringList dirty_groups_distinguished = Execute_Command("Get-ADPrincipalGroupMembership -Identity " +  SamName + " | Select-Object -ExpandProperty DistinguishedName");
    QStringList cleaned_group_distinguished;

    for(auto &i : dirty_groups_distinguished)
    {
        cleaned_group_distinguished << Clean_String(i);
    }
    return cleaned_group_distinguished;
}

QStringList PSIntegration::List_User_Group_CNs(QString SamName)
{
    /*
     *
     *
     */

    QStringList dirty_groups = Execute_Command("Get-ADPrincipalGroupMembership -Identity " +  SamName + " | Select-Object -ExpandProperty Name");
    QStringList cleaned_groups;
    for(auto &i : dirty_groups)
    {
        dirty_groups << Clean_String(i);
    }
    return cleaned_groups;
}

QStringList PSIntegration::List_All_UPNs()
{
    QStringList tmp = Execute_Command("Get-ADForest | Select -ExpandProperty UPNSuffixes");
    QStringList Domain_UPNS;
    for(auto &i : tmp)
    {
        Domain_UPNS << Clean_String(i);
    }
    return Domain_UPNS;
}

QStringList PSIntegration::List_All_Forests()
{
    QStringList tmp = Execute_Command("Get-ADForest | Select -ExpandProperty Domains");
    QStringList Forest_Names;
    for(auto &i : tmp)
    {
        Forest_Names << Clean_String(i);
    }
    return Forest_Names;
}

QStringList PSIntegration::List_All_OU_CNs()
{

    /*
     *
     *
     */

    QStringList tmp = Execute_Command("Get-ADOrganizationalUnit -Filter * | Select -ExpandProperty Name");
    QStringList OU_Names;
    for(auto &i : tmp)
    {
        OU_Names << Clean_String(i);
    }
    return OU_Names;
}

QStringList PSIntegration::List_All_OU_DNs()
{
    /*
     *
     *
     */

    QStringList tmp = Execute_Command("Get-ADOrganizationalUnit -Filter * | Select -ExpandProperty DistinguishedName");
    QStringList OU_DN_Names;
    for(auto &i : tmp)
    {
        OU_DN_Names << Clean_String(i);
    }
    return OU_DN_Names;
}

QStringList PSIntegration::List_All_Domain_Users()
{
    /*
     *
     *
     */

    QStringList tmp = Execute_Command("Get-ADUser -Filter * | Select-Object -ExpandProperty Name");
    QStringList AD_Users;
    for(auto &i : tmp)
    {
        AD_Users << Clean_String(i);
    }
    return AD_Users;
}

QString PSIntegration::Clean_String(QString str)
{
    /*
     *
     *
     *
     */

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

QString PSIntegration::Execute(QString param)
{
    /*
     *
     *
     *
     */

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

QString PSIntegration::User_Exists(QString SamName)
{
    /*
     *
     *
     */

    QString var = Clean_String(Execute("$test = (Get-ADUser -Filter {SamAccountName -like " + QString("\"") + SamName + QString("\"") + "}); if($null -ne $test) {return 'Yes'} elseif($null -eq $test) {return 'No'}"));
    if(var == "Yes")
    {
        return QString("Yes");
    }
    else if(var == "No")
    {
        return QString("No");
    }
}

QString PSIntegration::List_User_Identifier(QString name)
{
    QStringList cleaned_upns;
    QStringList cleaned_domains;
    QStringList UPNs = List_All_UPNs();
    QStringList Domains = List_All_Forests();
    for(auto &i : UPNs)
    {
        cleaned_upns << Clean_String(i);
    }
    for(auto &i : Domains)
    {
        cleaned_domains << Clean_String(i);
    }

    if(cleaned_upns.count() > 0)
    {
        return Clean_String(Execute("$temp = (Get-ADUser -Filter {Name -like " + QString("\"") + name + QString("\"") + " } -Properties UserPrincipalName).UserPrincipalName; $garbage, $upn = $temp.Split('@'); return $upn "));
    }
    else if(cleaned_upns.count() == 0)
    {
        return cleaned_domains.first(); // Needs to be fixed to work in multi-domain environments.
    }
}

QString PSIntegration::List_User_DN(QString name)
{
    return Clean_String(Execute("(Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "} -Properties DistinguishedName).DistinguishedName"));
}

QString PSIntegration::List_User_OU_DN(QString name)
{
    return Clean_String(Execute("$temp = (Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "}); $t = $temp.DistinguishedName; $garbage, $OU = $t.split(',', 2); return $OU"));
}

QString PSIntegration::List_User_OU_CN(QString name)
{
    return Clean_String(Execute("$temp = (Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "}); $t = $temp.DistinguishedName; $garbage, $OU = $t.split(',', 2); $clean, $junk = $OU.split(','); return $clean")).remove(0, 3);
}

QString PSIntegration::List_SamAccountName(QString name)
{
    return Clean_String(Execute("(Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "} -Properties SamAccountName).SamAccountName"));
}

QString PSIntegration::List_Mail(QString name)
{
    /*
     *
     *
     *
     */

    return Clean_String(Execute("(Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "} -Properties Mail).Mail"));
}

QString PSIntegration::List_Name(QString name)
{
    return Clean_String(name);
}

QString PSIntegration::List_ActiveSP_length()
{
    return active_SP_MinLength;
}

QString PSIntegration::List_ActiveSP_Complexity()
{
    return active_SP_Complexity;
}




void PSIntegration::List_Password_Policy(QString name)
{
    QString probe = Clean_String(Execute("$var = (Get-Module -ListAvailable -Name " + QString("\"") + "ActiveDirectory" + QString("\"") + "); if($var -ne $null){return " + QString("\"") + "Domain" + QString("\"") +"}; $var"));
    if(probe == "Domain")
    {

        probe = Clean_String(Execute("$var = (Get-ADFineGrainedPasswordPolicy -Filter * -ErrorAction SilentlyContinue); if($var -ne $null){return " + QString("\"") + "Found Grain" + QString("\"") + "}; $var"));
        if(probe == "Found Grain")
        {
            QStringList tmp = Execute_Command("Get-ADFineGrainedPasswordPolicy -Filter * | Select-Object -ExpandProperty AppliesTo");
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
                 QStringList tmp = Execute_Command("Get-ADFineGrainedPasswordPolicy -Filter * | Select-Object -ExpandProperty Name");
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

void PSIntegration::Set_FGPP_active(QString MinLength, QString Complexity)
{
    active_SP_MinLength = MinLength;
    active_SP_Complexity = Complexity;
}

void PSIntegration::Set_DDPP_active(QString MinLength, QString Complexity)
{
    active_SP_MinLength = MinLength;
    active_SP_Complexity = Complexity;
}

void PSIntegration::Set_APP_active(QString MinLength, QString Complexity)
{
    active_SP_MinLength = MinLength;
    active_SP_Complexity = Complexity;
}







































