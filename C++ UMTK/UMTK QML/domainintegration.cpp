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
        given_name = da_fname().split(" ").first(); // create the given name
        sur_name = da_fname().split(" ").last(); // create the last name
        if(da_displayname().length() <= 0)
        {
            setDADisplayName(da_fname()); // If the optional display name is empty set default to first name
        }
        setDAUName(SamAccountName = given_name.at(0).toUpper() + sur_name.toLower()); // Set the SamAccountName
        setDAEmail(UserPrincipalName = SamAccountName + "@" + List_User_Identifier(Clean_String(da_template()))); // Set the email address

        user_group_dns = List_User_Group_DNs(List_SamAccountName(Clean_String(da_template()))); // Get the DistinguishedNames from the template users groups
        user_group_cns = List_User_Group_CNs(List_SamAccountName(Clean_String(da_template()))); // Get the clean names for the template users groups


        ou_distinguished_name = List_User_OU_DN(Clean_String(da_template())); // Get the distinguished name for the template users OU
        ou_clean_name = List_User_OU_CN(Clean_String(da_template())); // Get the clean name for the template users ou

        QString tmp_upn = List_User_Identifier(Clean_String(da_template())); // Store a temp copy of the template users upn in a string
        for(auto i = 0; i < all_upns.count(); ++i)
        {
            if(tmp_upn == all_upns.at(i))
            {
                setUPNComboIndex(i); // Match the upn so we can set the index position in the UPN ComboBox automatically
            }
        }


        for(auto i = 0; i < ou_names.count(); ++i)
        {
            if(ou_clean_name == ou_names.at(i))
            {
                setOUComboIndex(i); // MAtch the users CN OU so we can set the index position in the OU ComboBox automatically
            }
        }

        List_Password_Policy(Clean_String(da_template())); // Determine which password policy is used
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

void DomainIntegration::Dump_User_Form(QString data, QUrl image_path, QString name)
{
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

void DomainIntegration::Move_ADUser_Orgranizational_Unit(QString User_DN, QString Template_OU_Distinguished)
{
    Execute("Move-ADObject -Identity " + QString("\"") + User_DN + QString("\"") + " -TargetPath " + QString("\"") + Template_OU_Distinguished + QString("\""));
}

void DomainIntegration::Edit_User_Status(QString name)
{
    /* https://docs.microsoft.com/en-us/powershell/module/activedirectory/disable-adaccount?view=windowsserver2022-ps
     *
     *
     */
    QString identity = List_SamAccountName(name);
    QString result = Execute("Disable-ADAccount -Identity " + identity);
}

void DomainIntegration::Edit_Disable_Description(QString name)
{
    Execute("$date = Get-Date; $var = " + QString("\"") + "Disabled: " + QString("\"") + " + $date; Set-ADUser -Identity ((Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "} -Properties SamAccountName).SamAccountName) -Description $var");
}

bool DomainIntegration::Employee_Name_Exists(QStringList names, QString new_name)
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

bool DomainIntegration::Validate_Password(QString pword, QString MinPasswordLength, QString ComplexityEnabled)
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

bool DomainIntegration::Get_Azure_Status()
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

bool DomainIntegration::Validate_User_Status(QString template_name)
{

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

QString DomainIntegration::List_User_DN(QString name)
{
    return Clean_String(Execute("(Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "} -Properties DistinguishedName).DistinguishedName"));
}

QString DomainIntegration::create_domain_account()
{
    QString error;
    bool errorDetected = false;
    if(User_Exists(da_uname()) == "Yes")
    {
        error = error + "\nWARNING: SamAccountName " + da_uname() + " already exists.\n";
        errorDetected = true;
    }
    if(da_password().length() <= 0 || da_password().length() < da_lengthpolicy().toInt())
    {
        errorDetected = true;
        error = error + "\nWARNING: Password " + da_password() + " does not meet the minimum length requirements.\n Your password must be at least " + da_lengthpolicy() + " characters long.\n";
    }
    if(da_complexitypolicy() == "True" && Validate_Password(da_password(), da_lengthpolicy(), da_complexitypolicy()) == false)
    {
        errorDetected = true;
        error = error + "\nWARNING: Password " + da_password() + " does not meet the complexity requirements.\n Your password must have at least one of the following: \nOne upper case letter"
                                                                                          "\n One lower case letter. \nOne number\nOne Special character\n";
    }
    if(Employee_Name_Exists(all_users, full_name))
    {
        error = error + "\nWARNING: Name " + da_fname() + " already exists, consider using a middle name or initials.\n";
        errorDetected = true;
    }
    if(UserPrincipalName.length() <= 0)
    {
        error = error + "\nWARNING: Your UPN appears to be empty. Please try again.\n";
        errorDetected = true;
    }
    if(Validate_User_Status(da_template()) == false)
    {
        error = error + "\nWARNING: The template user " + da_template() + " is disabled. This means it likely has no groups.\nPlease confirm that the user has the groups needed before creating this user.\n";
        errorDetected = true;
    }
    if(errorDetected == true)
    {
        return error;
    }

    if(User_Exists(da_uname()) == "No" && da_password().length() >= da_lengthpolicy().toInt() && Validate_Password(da_password(), da_lengthpolicy(), da_complexitypolicy()) == true
            && Employee_Name_Exists(all_users, full_name) == false && UserPrincipalName.length() > 0)
    {
        if(da_pproxy().length() > 0 && da_sproxy().length() > 0 && da_displayname().length() > 0)
        {
            QString p = "$p = " + QString("\"") + da_password() + QString("\"") + "; $sec = $p | ConvertTo-SecureString -AsPlainText -Force; ";

            Execute(p + "New-ADUser -Name " + "\"" + da_fname() + "\"" + " -GivenName " + "\"" + given_name + "\"" +
                        " -Surname " + "\"" + sur_name + "\"" + " -AccountPassword $sec -UserPrincipalName " + "\"" + UserPrincipalName + "\"" +
                        " -DisplayName " + "\"" + da_displayname() + "\"" + " -EmailAddress " + "\"" + da_email() + "\"" + " -SamAccountName " +
                        "\"" + SamAccountName + "\"" + " -Enabled 1; exit");


            Execute("$tmp = (Get-ADUser -Filter {Name -like \"" + da_template() + "\"}); "
                    "$groups = (Get-ADUser $tmp -Properties MemberOf).MemberOf; $usr = \"" + SamAccountName + "\"; "
                    "Foreach ($group in $groups) {Add-ADGroupMember -Identity (Get-ADGroup $group).name -Members $usr}; exit ");

            Execute("Set-ADUser -Identity \"" + SamAccountName + "\" -Add @{Proxyaddresses = " + "\"SMTP:" + da_pproxy() + "\"}");
            Execute("Set-ADUser -Identity \"" + SamAccountName + "\" -Add @{Proxyaddresses = " + "\"smtp:" + da_sproxy() + "\"}");

            user_dn = List_User_DN(da_fname());
            Move_ADUser_Orgranizational_Unit(user_dn, ou_distinguished_name);

            QString azure = Run_Azure_Sync(Get_Azure_Status());

            Dump_User_Form("<html> <h1> <center> The following information pertains to the new user request that you have submitted: </center> </h1> <br><br><br> <body> <strong> Employee name: </strong> " + da_fname() +
                                "<br> <strong> Username: </strong> " + SamAccountName + " <br> <strong> Email address: </strong> " + da_email() +
                                "<br> <strong> Password: </strong> " + da_password() + " <br> <strong> Groups: </strong> " + user_group_cns.join(" , ") +
                                "<br> <strong> Template user provided: </strong> " + da_template() +
                                "</body> </html>", QUrl(""), da_fname()
                               );
            return QString("SUCCESS - The following user has been created and a PDF named " + SamAccountName + ".pdf has been generated and saved on your desktop.\nPresent it via encrypted email to the end user.\n\n\nEmployee name: " + da_fname() +
                           "\nUsername: " + SamAccountName + "\nEmail address: " + da_email() + "\nDisplay name: " + da_displayname() + "\nOrganizational unit: " + ou_clean_name +
                           "\nUser Principal Name: " + UserPrincipalName + "\nGroups: " + user_group_cns.join(" , ")+ "\nPassword: " + da_password() + "\n\n" + azure);
        }
        if(da_pproxy().length() > 0 && da_sproxy().length() > 0 && da_displayname().length() <= 0)
        {
            QString p = "$p = " + QString("\"") + da_password() + QString("\"") + "; $sec = $p | ConvertTo-SecureString -AsPlainText -Force; ";

            Execute(p + "New-ADUser -Name " + "\"" + da_fname() + "\"" + " -GivenName " + "\"" + given_name + "\"" +
                        " -Surname " + "\"" + sur_name + "\"" + " -AccountPassword $sec -UserPrincipalName " + "\"" + UserPrincipalName + "\"" +
                        " -DisplayName " + "\"" + da_fname() + "\"" + " -EmailAddress " + "\"" + da_email() + "\"" + " -SamAccountName " +
                        "\"" + SamAccountName + "\"" + " -Enabled 1; exit");


            Execute("$tmp = (Get-ADUser -Filter {Name -like \"" + da_template() + "\"}); "
                    "$groups = (Get-ADUser $tmp -Properties MemberOf).MemberOf; $usr = \"" + SamAccountName + "\"; "
                    "Foreach ($group in $groups) {Add-ADGroupMember -Identity (Get-ADGroup $group).name -Members $usr}; exit ");

            Execute("Set-ADUser -Identity \"" + SamAccountName + "\" -Add @{Proxyaddresses = " + "\"SMTP:" + da_pproxy() + "\"}");
            Execute("Set-ADUser -Identity \"" + SamAccountName + "\" -Add @{Proxyaddresses = " + "\"smtp:" + da_sproxy() + "\"}");

            user_dn = List_User_DN(da_fname());
            Move_ADUser_Orgranizational_Unit(user_dn, ou_distinguished_name);

            QString azure = Run_Azure_Sync(Get_Azure_Status());

            Dump_User_Form("<html> <h1> <center> The following information pertains to the new user request that you have submitted: </center> </h1> <br><br><br> <body> <strong> Employee name: </strong> " + da_fname() +
                                "<br> <strong> Username: </strong> " + SamAccountName + " <br> <strong> Email address: </strong> " + da_email() +
                                "<br> <strong> Password: </strong> " + da_password() + " <br> <strong> Groups: </strong> " + user_group_cns.join(" , ") +
                                "<br> <strong> Template user provided: </strong> " + da_template() +
                                "</body> </html>", QUrl(""), da_fname()
                               );
            return QString("SUCCESS - The following user has been created and a PDF named " + SamAccountName + ".pdf has been generated and saved on your desktop.\nPresent it via encrypted email to the end user.\n\n\nEmployee name: " + da_fname() +
                           "\nUsername: " + SamAccountName + "\nEmail address: " + da_email() + "\nDisplay name: " + da_fname() + "\nOrganizational unit: " + ou_clean_name +
                           "\nUser Principal Name: " + UserPrincipalName + "\nGroups: " + user_group_cns.join(" , ")+ "\nPassword: " + da_password() + "\n\n" + azure);
        }
        if(da_pproxy().length() > 0 && da_sproxy().length() <= 0 && da_displayname().length() > 0)
        {
            QString p = "$p = " + QString("\"") + da_password() + QString("\"") + "; $sec = $p | ConvertTo-SecureString -AsPlainText -Force; ";

            Execute(p + "New-ADUser -Name " + "\"" + da_fname() + "\"" + " -GivenName " + "\"" + given_name + "\"" +
                        " -Surname " + "\"" + sur_name + "\"" + " -AccountPassword $sec -UserPrincipalName " + "\"" + UserPrincipalName + "\"" +
                        " -DisplayName " + "\"" + da_displayname() + "\"" + " -EmailAddress " + "\"" + da_email() + "\"" + " -SamAccountName " +
                        "\"" + SamAccountName + "\"" + " -Enabled 1; exit");


            Execute("$tmp = (Get-ADUser -Filter {Name -like \"" + da_template() + "\"}); "
                    "$groups = (Get-ADUser $tmp -Properties MemberOf).MemberOf; $usr = \"" + SamAccountName + "\"; "
                    "Foreach ($group in $groups) {Add-ADGroupMember -Identity (Get-ADGroup $group).name -Members $usr}; exit ");

            Execute("Set-ADUser -Identity \"" + SamAccountName + "\" -Add @{Proxyaddresses = " + "\"SMTP:" + da_pproxy() + "\"}");


            user_dn = List_User_DN(da_fname());
            Move_ADUser_Orgranizational_Unit(user_dn, ou_distinguished_name);

            QString azure = Run_Azure_Sync(Get_Azure_Status());

            Dump_User_Form("<html> <h1> <center> The following information pertains to the new user request that you have submitted: </center> </h1> <br><br><br> <body> <strong> Employee name: </strong> " + da_fname() +
                                "<br> <strong> Username: </strong> " + SamAccountName + " <br> <strong> Email address: </strong> " + da_email() +
                                "<br> <strong> Password: </strong> " + da_password() + " <br> <strong> Groups: </strong> " + user_group_cns.join(" , ") +
                                "<br> <strong> Template user provided: </strong> " + da_template() +
                                "</body> </html>", QUrl(""), da_fname()
                               );
            return QString("SUCCESS - The following user has been created and a PDF named " + SamAccountName + ".pdf has been generated and saved on your desktop.\nPresent it via encrypted email to the end user.\n\n\nEmployee name: " + da_fname() +
                           "\nUsername: " + SamAccountName + "\nEmail address: " + da_email() + "\nDisplay name: " + da_displayname() + "\nOrganizational unit: " + ou_clean_name +
                           "\nUser Principal Name: " + UserPrincipalName + "\nGroups: " + user_group_cns.join(" , ")+ "\nPassword: " + da_password() + "\n\n" + azure);
        }
        if(da_pproxy().length() <= 0 && da_sproxy().length() > 0 && da_displayname().length() > 0)
        {
            QString p = "$p = " + QString("\"") + da_password() + QString("\"") + "; $sec = $p | ConvertTo-SecureString -AsPlainText -Force; ";

            Execute(p + "New-ADUser -Name " + "\"" + da_fname() + "\"" + " -GivenName " + "\"" + given_name + "\"" +
                        " -Surname " + "\"" + sur_name + "\"" + " -AccountPassword $sec -UserPrincipalName " + "\"" + UserPrincipalName + "\"" +
                        " -DisplayName " + "\"" + da_displayname() + "\"" + " -EmailAddress " + "\"" + da_email() + "\"" + " -SamAccountName " +
                        "\"" + SamAccountName + "\"" + " -Enabled 1; exit");


            Execute("$tmp = (Get-ADUser -Filter {Name -like \"" + da_template() + "\"}); "
                    "$groups = (Get-ADUser $tmp -Properties MemberOf).MemberOf; $usr = \"" + SamAccountName + "\"; "
                    "Foreach ($group in $groups) {Add-ADGroupMember -Identity (Get-ADGroup $group).name -Members $usr}; exit ");

            Execute("Set-ADUser -Identity \"" + SamAccountName + "\" -Add @{Proxyaddresses = " + "\"SMTP:" + da_email() + "\"}");
            Execute("Set-ADUser -Identity \"" + SamAccountName + "\" -Add @{Proxyaddresses = " + "\"smtp:" + da_sproxy() + "\"}");

            user_dn = List_User_DN(da_fname());
            Move_ADUser_Orgranizational_Unit(user_dn, ou_distinguished_name);

            QString azure = Run_Azure_Sync(Get_Azure_Status());

            Dump_User_Form("<html> <h1> <center> The following information pertains to the new user request that you have submitted: </center> </h1> <br><br><br> <body> <strong> Employee name: </strong> " + da_fname() +
                                "<br> <strong> Username: </strong> " + SamAccountName + " <br> <strong> Email address: </strong> " + da_email() +
                                "<br> <strong> Password: </strong> " + da_password() + " <br> <strong> Groups: </strong> " + user_group_cns.join(" , ") +
                                "<br> <strong> Template user provided: </strong> " + da_template() +
                                "</body> </html>", QUrl(""), da_fname()
                               );
            return QString("SUCCESS - The following user has been created and a PDF named " + SamAccountName + ".pdf has been generated and saved on your desktop.\nPresent it via encrypted email to the end user.\n\n\nEmployee name: " + da_fname() +
                           "\nUsername: " + SamAccountName + "\nEmail address: " + da_email() + "\nDisplay name: " + da_displayname() + "\nOrganizational unit: " + ou_clean_name +
                           "\nUser Principal Name: " + UserPrincipalName + "\nGroups: " + user_group_cns.join(" , ")+ "\nPassword: " + da_password() + "\n\n" + azure);
        }
        if(da_pproxy().length() <= 0 && da_sproxy().length() <= 0 && da_displayname().length() > 0)
        {
            QString p = "$p = " + QString("\"") + da_password() + QString("\"") + "; $sec = $p | ConvertTo-SecureString -AsPlainText -Force; ";

            Execute(p + "New-ADUser -Name " + "\"" + da_fname() + "\"" + " -GivenName " + "\"" + given_name + "\"" +
                        " -Surname " + "\"" + sur_name + "\"" + " -AccountPassword $sec -UserPrincipalName " + "\"" + UserPrincipalName + "\"" +
                        " -DisplayName " + "\"" + da_displayname() + "\"" + " -EmailAddress " + "\"" + da_email() + "\"" + " -SamAccountName " +
                        "\"" + SamAccountName + "\"" + " -Enabled 1; exit");


            Execute("$tmp = (Get-ADUser -Filter {Name -like \"" + da_template() + "\"}); "
                    "$groups = (Get-ADUser $tmp -Properties MemberOf).MemberOf; $usr = \"" + SamAccountName + "\"; "
                    "Foreach ($group in $groups) {Add-ADGroupMember -Identity (Get-ADGroup $group).name -Members $usr}; exit ");

            Execute("Set-ADUser -Identity \"" + SamAccountName + "\" -Add @{Proxyaddresses = " + "\"SMTP:" + da_email() + "\"}");

            user_dn = List_User_DN(da_fname());
            Move_ADUser_Orgranizational_Unit(user_dn, ou_distinguished_name);

            QString azure = Run_Azure_Sync(Get_Azure_Status());

            Dump_User_Form("<html> <h1> <center> The following information pertains to the new user request that you have submitted: </center> </h1> <br><br><br> <body> <strong> Employee name: </strong> " + da_fname() +
                                "<br> <strong> Username: </strong> " + SamAccountName + " <br> <strong> Email address: </strong> " + da_email() +
                                "<br> <strong> Password: </strong> " + da_password() + " <br> <strong> Groups: </strong> " + user_group_cns.join(" , ") +
                                "<br> <strong> Template user provided: </strong> " + da_template() +
                                "</body> </html>", QUrl(""), da_fname()
                               );
            return QString("SUCCESS - The following user has been created and a PDF named " + SamAccountName + ".pdf has been generated and saved on your desktop.\nPresent it via encrypted email to the end user.\n\n\nEmployee name: " + da_fname() +
                           "\nUsername: " + SamAccountName + "\nEmail address: " + da_email() + "\nDisplay name: " + da_displayname() + "\nOrganizational unit: " + ou_clean_name +
                           "\nUser Principal Name: " + UserPrincipalName + "\nGroups: " + user_group_cns.join(" , ")+ "\nPassword: " + da_password() + "\n\n" + azure);
        }
        if(da_pproxy().length() <= 0 && da_sproxy().length() <= 0 && da_displayname().length() <= 0)
        {
            QString p = "$p = " + QString("\"") + da_password() + QString("\"") + "; $sec = $p | ConvertTo-SecureString -AsPlainText -Force; ";

            Execute(p + "New-ADUser -Name " + "\"" + da_fname() + "\"" + " -GivenName " + "\"" + given_name + "\"" +
                        " -Surname " + "\"" + sur_name + "\"" + " -AccountPassword $sec -UserPrincipalName " + "\"" + UserPrincipalName + "\"" +
                        " -DisplayName " + "\"" + da_fname() + "\"" + " -EmailAddress " + "\"" + da_email() + "\"" + " -SamAccountName " +
                        "\"" + SamAccountName + "\"" + " -Enabled 1; exit");


            Execute("$tmp = (Get-ADUser -Filter {Name -like \"" + da_template() + "\"}); "
                    "$groups = (Get-ADUser $tmp -Properties MemberOf).MemberOf; $usr = \"" + SamAccountName + "\"; "
                    "Foreach ($group in $groups) {Add-ADGroupMember -Identity (Get-ADGroup $group).name -Members $usr}; exit ");

            Execute("Set-ADUser -Identity \"" + SamAccountName + "\" -Add @{Proxyaddresses = " + "\"SMTP:" + da_email() + "\"}");

            user_dn = List_User_DN(da_fname());
            Move_ADUser_Orgranizational_Unit(user_dn, ou_distinguished_name);

            QString azure = Run_Azure_Sync(Get_Azure_Status());

            Dump_User_Form("<html> <h1> <center> The following information pertains to the new user request that you have submitted: </center> </h1> <br><br><br> <body> <strong> Employee name: </strong> " + da_fname() +
                                "<br> <strong> Username: </strong> " + SamAccountName + " <br> <strong> Email address: </strong> " + da_email() +
                                "<br> <strong> Password: </strong> " + da_password() + " <br> <strong> Groups: </strong> " + user_group_cns.join(" , ") +
                                "<br> <strong> Template user provided: </strong> " + da_template() +
                                "</body> </html>", QUrl(""), da_fname()
                               );
            return QString("SUCCESS - The following user has been created and a PDF named " + SamAccountName + ".pdf \nhas been generated and saved on your desktop.\nPresent it via encrypted email to the end user.\n\n\nEmployee name: " + da_fname() +
                           "\nUsername: " + SamAccountName + "\nEmail address: " + da_email() + "\nDisplay name: " + da_fname() + "\nOrganizational unit: " + ou_clean_name +
                           "\nUser Principal Name: " + UserPrincipalName + "\nGroups: " + user_group_cns.join(" , ")+ "\nPassword: " + da_password() + "\n\n" + azure);
        }


        //
    }

}

QString DomainIntegration::Run_Azure_Sync(bool var)
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

QString DomainIntegration::User_Exists(QString SamName)
{
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

QString DomainIntegration::List_Mail(QString name)
{
    return Clean_String(Execute("(Get-ADUser -Filter {Name -Like " + QString("\"") + name + QString("\"") + "} -Properties Mail).Mail"));
}

QString DomainIntegration::disable_domain_account()
{
    QStringList groups = List_User_Group_CNs(List_SamAccountName(da_template())); // Get the CNs for all groups the user is a member of.
    Execute("$user = " + QString("\"") + da_template() + QString("\"") + "; Get-ADPrincipalGroupMembership $user | Foreach {Remove-ADGroupMember $_ -Members $user -Confirm:$false}"); // Strip the user from all groups.
    Edit_Disable_Description(da_template()); // Append the desctipion of the account to say DISABLED: date & time
    Edit_User_Status(da_template()); // Disable the user account.
    Dump_User_Form("<html> <h1> <center> The following information pertains to the disable user request that you have submitted: </center> </h1> <br><br><br> <body> <strong> Employee name: </strong> " + da_template() +
                            "<br> <strong> Username: </strong> " + List_SamAccountName(da_template()) + " <br> <strong> Email address: </strong> " + List_Mail(da_template()) +
                            "<br> <strong> Groups: </strong> " + groups.join(" , ") +
                            "</body> </html>", QUrl(""), da_template()
                    );

    return QString("The user " + da_template() + " has been disabled.");
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
