#include "psintegration.h"

PSIntegration::PSIntegration()
{
    //image = "";
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
    process->waitForFinished(-1);
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
       cleaned_groups << Clean_String(i);
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
        cleaned_groups << Clean_String(i);
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

    //QStringList tmp = Execute_Command("Get-ADUser -Filter * | Select-Object -ExpandProperty Name");
    QStringList tmp = Execute_Command("$tmp = (Get-ADUser -Filter * | Select-Object Name, GivenName, SurName | Sort-Object SurName, GivenName); return $tmp.Name");
    QStringList AD_Users;
    for(auto &i : tmp)
    {
        //AD_Users << prepend_company(i, List_User_OU_CN(i));
        AD_Users << Clean_String(i);
    }
    return AD_Users;
}

QStringList PSIntegration::getAllADUsers()
{
    return all_users;
}

QStringList PSIntegration::getAllADUPNs()
{
    return all_upns;
}

QStringList PSIntegration::getAllADForests()
{
    return all_forests;
}

QStringList PSIntegration::getAllOUNames()
{
    return all_ou_names;
}

QStringList PSIntegration::getAllOUDNs()
{
    return all_ou_distinguished;
}

QSqlDatabase PSIntegration::get_database()
{
    return QSqlDatabase::database(database_name);
}

QString PSIntegration::Clean_String(QString str)
{
    /*
     * Strips any carriage returns
     * and new lines if they exist.
     * When pulling them from PS they usually exist.
     * So this functions needs to get called a lot.
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
    /* Implement QProcess with Lambda and forgoe usage of waitForFinished(-1) to prevent GUI hard locks and improve general performance.
     * Also need to implement a process bar to give some indication on time duration to not frighten people.
     * Could even use https://www.qt.io/blog/2017/08/25/a-new-qprocessstartdetached which is great but since I went from 5.2.x to 6.2.x I never knew this was implemented.
     * Running detached and using the same session would also work.
     */

    QProcess *process = new QProcess();
    QByteArray success;
    QByteArray errors;
    QStringList params;
    params = QStringList({"-Command", QString("Start-Process -NoNewWindow -Verb runAs powershell; "), param});
    process->start("powershell", params);
    process->waitForFinished(-1);
    success.append(process->readAllStandardOutput());
    errors.append(process->readAllStandardError());
    //qDebug() << errors;
    process->terminate();
    QString data = QString(success);
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

QString PSIntegration::Run_Azure_Sync(bool var)
{
    if(var)
    {
        Execute("Start-ADSyncSyncCycle -PolicyType " + QString("\"") + "Delta" + QString("\""));
        return QString("A sucessful AD Sync has been executed.");
    }
    else if(!var)
    {
        return QString("\nThe ADSync module is not installed here. If this client uses Azure AD Sync please get on the server with the AD Sync module and execute the following command: \n Import-Module ADSync; Start-ADSyncSyncCycle -PolicyType "+ QString("\"") + "Delta" + QString("\""));
    }
}

QString PSIntegration::List_URL_Image_Path()
{
    return image;
}

QString PSIntegration::List_User_UserPrincipalName(QString name)
{
    return Clean_String(Execute("(Get-ADUser -Filter {Name -Like "  + QString("\"") + name + QString("\"") + "} | Select-Object -ExpandProperty UserPrincipalName)"));
}

QString PSIntegration::List_User_DisplayName(QString name)
{
    return Clean_String(Execute("(Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "} -Properties displayName).displayName"));
}

QString PSIntegration::stripCompanyName(QString employee)
{
    /* Create a pattern to find the first `-`.
     * Match it, if it's found return a substring of employee.
     * The substring contains everything after the first instance of `-` + 1.
     * The employee string before handed off to this function looks like so, `Company name - Employee name`.
     * The string returned by this function looks like ` Employee name`.
     */
    QRegularExpression re("(^[^-]*-)");
    QRegularExpressionMatch match = re.match(employee);
    if(match.hasMatch())
    {
        employee = employee.remove(0, match.capturedLength() + 1);
        return employee;
    }
    else if(!match.hasMatch())
    {
        return employee;
    }
}

QString PSIntegration::getEmployeeIdentifier(QString name)
{

    /* Iterate through all UPNs, "Clean" each one and add it to the StringList.
     * Iterate through any and all forests, "Clean" each one and add it to the StringList.
     * If the UPN count is greater than 0, search the users name, pull the appropriate UPN, split it within PS, return PS result and then return the string.
     * If the UPN count is equal to zero just return the forest name. Right now it does NOT support multi forests.
     *
     */
    QStringList cleaned_upns;
    QStringList cleaned_domains;
    QStringList UPNs = getAllADUPNs();
    QStringList Domains = getAllADForests();
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

QString PSIntegration::getEmployeeName(QString name)
{
    /*
     * Return the "cleaned" employee name.
     * See Clean_String to find out what "Cleaned" means.
     */
    return Clean_String(name);
}

QString PSIntegration::validateSamOption(QString template_name, QString new_user)
{
    if(List_All_UPNs().count() > 1)
    {
        QRegularExpression re("(^[^-]*-)");
        QRegularExpressionMatch match = re.match(template_name);
        int k = 0;
        if(match.hasMatch())
        {
            template_name = template_name.remove(match.capturedLength() - 2 , template_name.length());
            QMap<QString, int>::const_iterator i = company_to_sam.constBegin();
            while(i != company_to_sam.constEnd())
            {
                if(i.key() == template_name)
                {
                    k = i.value();
                }
                ++i;
            }
            if(k > 0)
             {
                 switch(k)
                     {
                         case 1: { QStringList Names = new_user.split(" "); return Names.first(); }
                         case 2: { QStringList Names = new_user.split(" "); return Names.last(); }
                         case 3: { QStringList Names = new_user.split(" "); return Names.first().at(0).toUpper() + Names.last().toLower(); }
                         case 4: { QStringList Names = new_user.split(" "); return Names.last() + Names.first().at(0).toUpper(); }
                         case 5: { QStringList Names = new_user.split(" "); return Names.first() + Names.last(); }
                         case 6: { QStringList Names = new_user.split(" "); return Names.last() + Names.first(); }
                     }
             }
             else if(k <= 0)
             {
                 QStringList Names = new_user.split(" ");
                 return Names.first().at(0).toUpper() + Names.last().toLower();
             }
        }


        else if(!match.hasMatch())
        {
            QStringList Names = new_user.split(" ");
            return Names.first().at(0).toUpper() + Names.last().toLower();
        }
    }
    else if(List_All_UPNs().count() <= 1)
    {
        QSettings *GenerationSettings = new QSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Elysian Productions\\UMTK-Classic\\Generation Settings\\SamAccount Settings\\", QSettings::Registry64Format);
        int k = GenerationSettings->value("SamStyle").toInt();
        if(k > 0)
        {
            switch(k)
            {
                case 1: { QStringList Names = new_user.split(" "); return Names.first(); }
                case 2: { QStringList Names = new_user.split(" "); return Names.last(); }
                case 3: { QStringList Names = new_user.split(" "); return Names.first().at(0).toUpper() + Names.last().toLower(); }
                case 4: { QStringList Names = new_user.split(" "); return Names.last() + Names.first().at(0).toUpper(); }
                case 5: { QStringList Names = new_user.split(" "); return Names.first() + Names.last(); }
                case 6: { QStringList Names = new_user.split(" "); return Names.last() + Names.first(); }
            }
        }
        else if(k <= 0)
        {
            QStringList Names = new_user.split(" ");
            return Names.first().at(0).toUpper() + Names.last().toLower();
        }
    }
}

bool PSIntegration::initalize_database(const QString &db_path)
{
    database_name = db_path;
    auto db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(database_name);
    bool is_open = db.open();
    return is_open;
}

bool PSIntegration::Employee_Name_Exists(QStringList names, QString new_name)
{
    QStringList tmp;
    for(auto &i : names)
    {
        tmp << Clean_String(i);
    }

    int test = 0;
    for(auto i = 0; i < tmp.count(); ++i)
    {
        if(tmp.at(i) == new_name)
        {
            test = 1;
        }
    }
    if(test == 0)
    {
        return false;
    }
    else if(test == 1)
    {
        return true;
    }
}

bool PSIntegration::Validate_Password(QString pword, QString MinPasswordLength, QString ComplexityEnabled)
{
    if(pword.length() >= MinPasswordLength.toInt())
    {
        if(ComplexityEnabled == "True")
        {
            QRegularExpression re;
            QRegularExpressionMatch match;
            re.setPattern("(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*[^A-Za-z0-9])");
            match = re.match(pword);
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
            match = re.match(pword);
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
    if(pword.length() < MinPasswordLength.toInt())
    {
        return false;
    }
}

bool PSIntegration::Get_Azure_Status()
{
    QString status = Clean_String(Execute("if(Get-Module -ListAvailable -Name " + QString("\"") + "ADSync" + QString("\"") + ") {return 1} else {return 0}"));
    if(status.toInt() == 1)
    {
        return true;
    }
    else if(status.toInt() == 0)
    {
        return false;
    }
}

bool PSIntegration::Validate_User_Status(QString template_name)
{
    /*
     *
     */

    QString var = Clean_String(Execute("$temp = (Get-ADUser -Filter {Name -Like " + QString("\"") + template_name + QString("\"") + "} -Properties Enabled).Enabled; return $temp"));
    if(var == "True")
    {
        return true;
    }
    else if(var == "False")
    {
        return false;
    }
}

bool PSIntegration::Validate_Email_Address(QString email_address)
{
    return false;
}

bool PSIntegration::getMultiCompanyStatus()
{
    return multi_company_enabled;
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

void PSIntegration::Move_ADUser_Orgranizational_Unit(QString User_CN, QString Template_OU_Distinguished)
{
    Execute("Move-ADObject -Identity " + QString("\"") + User_CN + QString("\"") + " -TargetPath " + QString("\"") + Template_OU_Distinguished + QString("\""));
}

void PSIntegration::Dump_User_Form(QString data, QUrl image_path, QString name)
{
    /* I'm going to take the same HTML approach as I did in the PowerShell version.
     * The string will be built with the HTML tags. For this reason the provided image
     * needs to be in the QUrl format
     */
    if(image_path.isEmpty())
    {
        // ignore variaible and use just the contents of data.
        QString path;

        path = Clean_String(Execute("$t = (Get-ItemProperty -Path 'HKCU:\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\User Shell Folders\\' -Name Desktop | Select-Object Desktop -ErrorAction SilentlyContinue); "
                       "if($null -ne $t.Desktop) {$str = $t.Desktop.toString() + " + QString("\"") + "\\" + QString("\"") + "; return $str } else {$str = $ENV:USERPROFILE + " + QString("\"") + "\\Downloads\\; return $str" + QString("\"") + "}"
                    ));
        QTextDocument document;
        document.setHtml(data);
        QPrinter printer(QPrinter::PrinterResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setPageSize(QPageSize::A4);
        printer.setOutputFileName(path + name + ".pdf");
        printer.setPageMargins(QMarginsF(15, 15, 15, 15));

        document.print(&printer);
    }
    else if(!image_path.isEmpty())
    {
        // Use the path and include it in the string.
    }
}

/*void PSIntegration::Set_URL_Image_Path(QString path)
{
    image = path;
}
void PSIntegration::setLogoPath(const QString &path)
{
    image = path;
}*/





void PSIntegration::Edit_Name(QString name, QString first_name, QString middle_name, QString last_name)
{
    if(first_name.length() > 0 || last_name.length() > 0 || middle_name.length() > 0)
    {
        if(first_name.length() > 0 && middle_name.length() > 0 && last_name.length() > 0)
        {
            Execute("Set-ADUser -Identity ((Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + " } -Properties SamAccountName).SamAccountName) -GivenName " + QString("\"") + first_name +
                   QString("\"") + " -Surname " + QString("\"") + last_name + QString("\"") + " -OtherName " + QString("\"") + middle_name + QString("\""));

            Execute("Rename-ADObject -Identity ((Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "} -Properties DistinguishedName).DistinguishedName) -NewName " + QString("\"") + first_name + " " + middle_name + " " + last_name + QString("\""));

            Edit_UserSamAccountName(first_name + " " + last_name, first_name.at(0).toUpper() + last_name.toLower());
            Edit_UserPrincipalName(first_name + " " + last_name, first_name.at(0).toUpper() + last_name.toLower() + "@" + List_User_Identifier(first_name + " " + last_name));
            Edit_Display_Name(first_name + " " + last_name, first_name + " " + last_name);
            Edit_Email_Address(first_name + " " + last_name, first_name.at(0).toUpper() + last_name.toLower() + "@" + List_User_Identifier(first_name + " " + last_name));
        }
        if(first_name.length() <= 0 && middle_name.length() > 0 && last_name.length() > 0)
        {
            Execute("Set-ADUser -Identity ((Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + " } -Properties SamAccountName).SamAccountName) -OtherName " + QString("\"") + middle_name +
                   QString("\"") + " -Surname " + QString("\"") + last_name + QString("\""));

            Execute("Rename-ADObject -Identity ((Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "} -Properties DistinguishedName).DistinguishedName) -NewName " +
                    QString("\"") + name.split(" ").first() + " " + middle_name + " " + last_name + QString("\""));

            Edit_UserSamAccountName(first_name + " " + last_name, first_name.at(0).toUpper() + last_name.toLower());
            Edit_UserPrincipalName(first_name + " " + last_name, first_name.at(0).toUpper() + last_name.toLower() + "@" + List_User_Identifier(first_name + " " + last_name));
            Edit_Display_Name(first_name + " " + last_name, first_name + " " + last_name);
            Edit_Email_Address(first_name + " " + last_name, first_name.at(0).toUpper() + last_name.toLower() + "@" + List_User_Identifier(first_name + " " + last_name));
        }
        if(first_name.length() <= 0 && middle_name.length() <= 0 && last_name.length() > 0)
        {
            Execute("Set-ADUser -Identity ((Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + " } -Properties SamAccountName).SamAccountName) -Surname " +
                    QString("\"") + last_name + QString("\""));

            Execute("Rename-ADObject -Identity ((Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "} -Properties DistinguishedName).DistinguishedName) -NewName " + QString("\"") + name.split(" ").first() + " " + last_name + QString("\""));

            Edit_UserSamAccountName(name.split(" ").first() + " " + last_name, name.split(" ").first().at(0).toUpper() + last_name.toLower());
            Edit_UserPrincipalName(name.split(" ").first() + " " + last_name, name.split(" ").first().at(0).toUpper() + last_name.toLower() + "@" + List_User_Identifier(name.split(" ").first()  + " " + last_name));
            Edit_Display_Name(name.split(" ").first() + " " + last_name, name.split(" ").first() + " " + last_name);
            Edit_Email_Address(name.split(" ").first() + " " + last_name, name.split(" ").first().at(0).toUpper() + last_name.toLower() + "@" + List_User_Identifier(name.split(" ").first()  + " " + last_name));
        }
        if(first_name.length() <= 0 && middle_name.length() > 0 && last_name.length() <= 0)
        {
            Execute("Set-ADUser -Identity ((Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + " } -Properties SamAccountName).SamAccountName) -OtherName " + QString("\"") + middle_name + QString("\""));

            Execute("Rename-ADObject -Identity ((Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "} -Properties DistinguishedName).DistinguishedName) -NewName "
                    + QString("\"") + name.split(" ").first() + " " + middle_name + " " + name.split(" ").last() + QString("\""));

            Edit_UserSamAccountName(name.split(" ").first() + " " + name.split(" ").last(), first_name.at(0).toUpper() + name.split(" ").last().toLower());
            Edit_UserPrincipalName(name.split(" ").first() + " " + name.split(" ").last(), first_name.at(0).toUpper() + name.split(" ").last().toLower() + "@" + List_User_Identifier(name.split(" ").first() + " " + middle_name + " " + name.split(" ").last()));
            Edit_Display_Name(name.split(" ").first() + " " + name.split(" ").last(), name.split(" ").first() + " " + name.split(" ").last());
            Edit_Email_Address(name.split(" ").first() + " " + name.split(" ").last(), name.split(" ").at(0).toUpper() + name.split(" ").last().toLower() + "@" + List_User_Identifier(name.split(" ").first() + " " + middle_name + " " + name.split(" ").last()));
        }
        if(first_name.length() > 0 && middle_name.length() <= 0 && last_name.length() > 0)
        {

            Execute("Set-ADUser -Identity ((Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + " } -Properties SamAccountName).SamAccountName) -GivenName " + QString("\"") + first_name +
                   QString("\"") + " -Surname " + QString("\"") + last_name + QString("\""));

            Execute("Rename-ADObject -Identity ((Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "} -Properties DistinguishedName).DistinguishedName) -NewName " + QString("\"") + first_name + " " + last_name + QString("\""));

            Edit_UserSamAccountName(first_name + " " + last_name, first_name.at(0).toUpper() + last_name.toLower());
            Edit_UserPrincipalName(first_name + " " + last_name, first_name.at(0).toUpper() + last_name.toLower() + "@" + List_User_Identifier(first_name + " " + last_name));
            Edit_Display_Name(first_name + " " + last_name, first_name + " " + last_name);
            Edit_Email_Address(first_name + " " + last_name, first_name.at(0).toUpper() + last_name.toLower() + "@" + List_User_Identifier(first_name + " " + last_name));
        }
        if(first_name.length() > 0 && middle_name.length() > 0 && last_name.length() <= 0)
        {
            Execute("Set-ADUser -Identity ((Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + " } -Properties SamAccountName).SamAccountName) -GivenName " + QString("\"") + first_name +
                   QString("\"") + " -OtherName " + QString("\"") + middle_name + QString("\""));

            Execute("Rename-ADObject -Identity ((Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "} -Properties DistinguishedName).DistinguishedName) -NewName " +
                    QString("\"") + first_name + " " + middle_name + " " + name.split(" ").last() + QString("\""));

            Edit_UserSamAccountName(first_name + " " + name.split(" ").last(), first_name.at(0).toUpper() + name.split(" ").last().toLower());
            Edit_UserPrincipalName(first_name + " " + name.split(" ").last(), first_name.at(0).toUpper() + name.split(" ").last().toLower() + "@" + List_User_Identifier(first_name + " " + middle_name + " " + name.split(" ").last()));
            Edit_Display_Name(first_name + " " + name.split(" ").last(), first_name + " " + name.split(" ").last());
            Edit_Email_Address(first_name + " " + name.split(" ").last(), first_name.at(0).toUpper() + name.split(" ").last().toLower() + "@" + List_User_Identifier(first_name + " " + middle_name + " " + name.split(" ").last()));
        }
    }
    else
    {

    }
}

void PSIntegration::Edit_UserSamAccountName(QString name, QString SamAccount)
{
    Execute("Set-ADUser -Identity ((Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "} -Properties SamAccountName).SamAccountName) -SamAccountName " + QString("\"") + SamAccount + QString("\""));
}

void PSIntegration::Edit_Password(QString name, QString password)
{

}

void PSIntegration::Edit_Display_Name(QString name, QString display_name)
{
    Execute("Set-ADUser -Identity ((Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "} -Properties SamAccountName).SamAccountName) -displayName " + QString("\"") + display_name + QString("\""));
}

void PSIntegration::Edit_Proxy_Addresses(QString name, QStringList proxies)
{

}

void PSIntegration::Edit_Email_Address(QString name, QString email)
{
    Execute("Set-ADUser -Identity ((Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "} -Properties SamAccountName).SamAccountName) -EmailAddress " + QString("\"") + email + QString("\""));
}

void PSIntegration::Edit_UserPrincipalName(QString name, QString upn)
{
    Execute("Set-ADUser -Identity ((Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "} -Properties SamAccountName).SamAccountName) -UserPrincipalName " + QString("\"") + upn + QString("\""));
}

void PSIntegration::Edit_Group_Memebership(QString name, QStringList GroupsToAdd, QStringList GroupsToRemove)
{

}

void PSIntegration::Edit_User_Status(QString name)
{
    /* https://docs.microsoft.com/en-us/powershell/module/activedirectory/disable-adaccount?view=windowsserver2022-ps
     *
     *
     */
    QString identity = List_SamAccountName(name);
    QString result = Execute("Disable-ADAccount -Identity " + identity);
}

void PSIntegration::Edit_Disable_Description(QString name)
{
    Execute("$date = Get-Date; $var = " + QString("\"") + "Disabled: " + QString("\"") + " + $date; Set-ADUser -Identity ((Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "} -Properties SamAccountName).SamAccountName) -Description $var");
}

void PSIntegration::set_db_lists()
{
    if(initalize_database("C:\\Program Files (x86)\\UMTK-Classic\\Database\\UMTK.db"))
    {
        QSqlQuery *query = new QSqlQuery(get_database());
        query->prepare("SELECT OU, Company, Prefix, SamAccount FROM Clients");
        query->exec();
        while(query->next())
        {
            ou_list << query->value(0).toString();
            company_names_list << query->value(1).toString();
            user_prefix_list << query->value(2).toString();
            sam_setting_list << query->value(3).toString();
        }
    }
}

void PSIntegration::mapOUToCompany()
{
    QSqlQuery *query = new QSqlQuery(get_database());
    query->prepare("SELECT OU, Company FROM Clients");
    query->exec();
    while(query->next())
    {
        ou_to_company.insert(query->value(0).toString(), query->value(1).toString());
    }
}

void PSIntegration::mapCompanyToPrefix()
{
    QSqlQuery *query = new QSqlQuery(get_database());
    query->prepare("SELECT Company, Prefix FROM Clients");
    query->exec();
    while(query->next())
    {
        company_to_prefix.insert(query->value(0).toString(), query->value(1).toString());
    }
}

void PSIntegration::mapUserToCompany()
{
    int k = 0;
    QStringList tmp;
    while(k < getAllADUsers().count())
    {
        if(user_to_ou.contains(getAllADUsers().at(k)))
        {
            QMap<QString, QString>::iterator i = user_to_ou.find(getAllADUsers().at(k));
            if(ou_to_company.contains(i.value()))
            {
                QMap<QString, QString>::iterator j = ou_to_company.find(i.value());
                tmp << j.value() + " - " + i.key();
            }
        }
        ++k;
    }
    tmp.sort();
    setAllADUsers(tmp);
}

void PSIntegration::mapCompanyToSam()
{
    QSqlQuery *query = new QSqlQuery(get_database());
    query->prepare("SELECT DISTINCT Company, SamAccount FROM Clients;");
    query->exec();
    while(query->next())
    {
        company_to_sam.insert(query->value(0).toString(), query->value(1).toInt());
    }
}

void PSIntegration::mapUserToOU()
{
    QStringList list = getAllADUsers();

    for(auto i = 0; i < list.count(); ++i)
    {
        user_to_ou.insert(list.at(i), Clean_String(Execute("$temp = (Get-ADUser -Filter {Name -Like " + QString("\"") + list.at(i) + QString("\"") + "}); $t = $temp.DistinguishedName; $garbage, $OU = $t.split(',', 2); $clean, $junk = $OU.split(','); return $clean")).remove(0, 3));
    }
}

void PSIntegration::setAllADUsers(const QStringList &list)
{
    QStringList tmp;
    for(auto &i : list)
    {
        tmp << Clean_String(i);
    }
    all_users = tmp;
    //Q_EMIT _ADUsersChanged();
}

void PSIntegration::setAllADUPNs(const QStringList &list)
{
   QStringList tmp;
   for(auto &i : list)
   {
       tmp << Clean_String(i);
   }
   all_upns = tmp;
   //Q_EMIT _UPNsChanged();
}

void PSIntegration::setAllADForests(const QStringList &list)
{
    QStringList tmp;
    for(auto &i : list)
    {
        tmp << Clean_String(i);
    }
    all_forests = tmp;
    //Q_EMIT _ForestsChanged();
}

void PSIntegration::setAllOUNames(const QStringList &list)
{
    QStringList tmp;
    for(auto &i : list)
    {
        tmp << Clean_String(i);
    }
    all_ou_names = tmp;
    //Q_EMIT _OUNamesChanged();
}

void PSIntegration::setAllOUDNs(const QStringList &list)
{
    QStringList tmp;
    for(auto &i : list)
    {
        tmp << Clean_String(i);
    }
    all_ou_distinguished = tmp;
    //Q_EMIT _OUDistinguishedNamesChanged();
}

void PSIntegration::reMapConnections()
{
    mapOUToCompany();
    mapCompanyToPrefix();
    mapUserToOU();
    mapUserToCompany();
}

void PSIntegration::roboCopyProfile(const QString redirection_path, const QString &storage_path, const QString &username)
{
    if(redirection_path.length() > 0 && storage_path.length() > 0)
    {
        QDir dir(redirection_path + "\\" + username);
        if(dir.exists())
        {
            QProcess *process = new QProcess();
            QByteArray success;
            QByteArray errors;
            QStringList params;
            QString param = "Move-Item -Source " + redirection_path + username + " -Destination " + storage_path + username;
            qDebug() << param;
            params = QStringList({"-Command", param});
            process->startDetached("powershell", params);
        }
    }
}


//















