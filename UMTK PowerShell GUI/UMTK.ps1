[System.Reflection.Assembly]::LoadWithPartialName("System.Windows.Forms")

Function CreateDomainUser
{
    param (
        [int]$_ou,
        [string]$_template,
        [string]$_upn,
        [string]$_fullname,
        [string]$_username, 
        [string]$_password,
        [string]$_email,
        [int]$_cleaned_ous
    )
    if($primary_proxy_input.Text.Length -eq 0)
    {
        $pprox = "SMTP:" + $_email
    }
    elseif($primary_proxy_input.Text.Length -ge 4)
    {
        $pprox = "SMTP:" + $primary_proxy_input.Text
    }
    if($secondary_proxy_input.Text.Length -ge 4)
    {
        $secprox = "smtp:" + $secondary_proxy_input.Text
    }
    if($displayname_input.Text.Length -eq 0)
    {
        $_displayname = $_fullname
    }
    elseif($displayname_input.Text.Length -ge 3)
    {
        $_displayname = $displayname_input.Text
    }
    Add-Type -AssemblyName System.Web 
    $good = 0  
    switch(Get-PasswordPolicyType)
    {
        "Default" 
        {
               $default_security = (Get-ADDefaultDomainPasswordPolicy)
               while($good -eq 0)
               {
                   if((Validate-PasswordRedux -password $password_input.Text -Case 1 -Length $default_security.MinPasswordLength -Complexity $default_security.ComplexityEnabled) -eq 0)
                   {
                       $_pass = [System.Web.Security.Membership]::GeneratePassword($default_security.MinPasswordLength,1)
                       $good = Validate-PasswordRedux -password $_pass -Case 1 -Length $default_security.MinPasswordLength -Complexity $default_security.ComplexityEnabled
                       if($good -eq 1)
                       {
                           $pass_eval = 1
                       }
                   }
                   else
                   {
                       $pass_eval = 1
                       $good = 1
                   }
               }
        }
        "Default \ Fine Grained" 
        {
            $policy_group, $policy_name = (Match-UserToFineGrainedPasswordPolicy -Identity $_template)
            if($policy_name.Length -eq 0)
            {
               $default_security = (Get-ADDefaultDomainPasswordPolicy)
               while($good -eq 0)
               {
                   if((Validate-PasswordRedux -password $password_input.Text -Case 1 -Length $default_security.MinPasswordLength -Complexity $default_security.ComplexityEnabled) -eq 0)
                   {
                       $_pass = [System.Web.Security.Membership]::GeneratePassword($default_security.MinPasswordLength,1)
                       $good = Validate-PasswordRedux -password $_pass -Case 1 -Length $default_security.MinPasswordLength -Complexity $default_security.ComplexityEnabled
                       if($good -eq 1)
                       {
                           $pass_eval = 1
                       }
                   }
                   else
                   {
                       $pass_eval = 1
                       $good = 1
                   }

               }
            }
            elseif($policy_name.Length -gt 1)
            {
                $Grained = (Get-ADFineGrainedPasswordPolicy -Filter {Name -like $policy_name})
                while($good -eq 0)
                {

                     if((Validate-PasswordRedux -password $password_input.Text -Case 1 -Length $Grained.MinPasswordLength -Complexity $Grained.ComplexityEnabled) -eq 0)
                   {
                       $_pass = [System.Web.Security.Membership]::GeneratePassword($Grained.MinPasswordLength,1)
                       $good = Validate-PasswordRedux -password $_pass -Case 1 -Length $Grained.MinPasswordLength -Complexity $Grained.ComplexityEnabled
                       if($good -eq 1)
                       {
                           $pass_eval = 1
                       }
                   }
                   else
                   {
                       $pass_eval = 1
                       $good = 1
                   }
                }
            }
        }
        "Arbitrary" 
        {
            while($good -eq 0)
            {
                if((Validate-Password -password $password_input.Text) -eq 0)
                {
                    $_pass = [System.Web.Security.Membership]::GeneratePassword(8,1)
                    $good = Validate-PasswordRedux -password $_pass -Case 2
                    if($good -eq 1)
                    {
                        $pasS_eval = 1
                    }
                    else
                    {
                        $good = 1
                        $pass_eval = 1
                    }
                }
            }
        }
    }
    $user_leng = Validate-UserLength -uname $_username
    $user_exists = Validate-DomainUser -username $_username
    $email_eval = Validate-Email -emailaddress $_email
    if($_fullname.Split(' ').Count -eq 1)
    {
        # Fail with message letting user know that First and Last names are required..
    }
    elseif($_fullname.Split(' ').Count -eq 2)
    {
        $_given, $_surname = $_fullname.Split(' ')
    }
    elseif($_fullname.Split(' ').Count -ge 3)
    {
        $_given, $_middle, $_surname = $_fullname.Split(' ')
    }
    if($_ou -gt 0 -and $_template.Length -gt 0 -and $_upn.Count -gt 0 -and $_fullname.Length -gt 0 -and $_username.Length -gt 0 -and $_password.Length -gt 0 -and $_email.Length -gt 0)
    {
        if($pass_eval -eq 1 -and $user_leng -eq 1 -and $user_exists -eq 0 -and $email_eval -eq 1)
        {
            $userpname = $_username + "@" + $_upn
            $tmp_pass = $_password | ConvertTo-SecureString -AsPlainText -Force

            if($_middle.Length -eq 0)
            {
                New-ADUser -Name $_fullname -GivenName $_given -Surname $_surname -AccountPassword $tmp_pass -UserPrincipalName $userpname -displayName $_displayname  -EmailAddress $_email -SamAccountName $_username  -Enabled 1
            }
            elseif($_middle.Length -gt 0)
            {
                New-ADUser -Name $_fullname -GivenName $_given -middleName $_middle -Surname $_surname -AccountPassword $tmp_pass -UserPrincipalName $userpname -displayName $_displayname  -EmailAddress $_email -SamAccountName $_username  -Enabled 1
            }
            $g = (Get-ADUser -Filter {name -like $_template})
            $groups = (Get-ADUser $g -Properties MemberOf).MemberOF
            Foreach($group in $groups)
            {
                Add-ADGroupMember -Identity (Get-ADGroup $group).name -Members $_username
            }        
            if($pprox.Length -ge 1)
            {
                Set-ADUser -Identity $_username -Add @{Proxyaddresses = $pprox}
            }
            if($secprox.Length -gt 1)
            {
                Set-ADUser -Identity $_username -Add @{Proxyaddresses = $secprox}
            }
            $tuser = (Get-ADUser -Filter {samAccountName -like $_username} | Select-Object -ExpandProperty DistinguishedName) 
            if($_cleaned_ous -eq 1)
            {
                Move-ADObject -Identity $tuser -TargetPath $distinguished_ous[$_ou]
            }
            elseif($_cleaned_ous -eq 0)
            {
                Move-ADObject -Identity $tuser -TargetPath $distinguished_ous[$_ou]
            }
            $_tc_ = (Write-GroupsCleanly -Identity $_template)
            $path_var = Validate-Path
            $final_form = (Dump-UserForm -username $_username -password $_password -path $path_var -is_domain 1 -employee $_fullname -email $_email -UPN $_upn -OU $clean_ous[$_ou] -membership ($_tc_ -join ", ") -template $_template -local_administrator 0)
            if((Get-AzureStatus) -eq 1)
            {
                Start-ADSyncSyncCycle -PolicyType "Delta"
                $message_label.ForeColor = "Green"
                Print-ToPdf -Content $final_form 
                $message_label.Text = "The user account for " + $_username + " has been created. A file was created on your desktop ( " + $_username + ".html ) give this file to the user, then delete it. An Azure AD Sync has also been successfully executed."
            }
            elseif((Get-AzureStatus) -eq 0)
            {
                $message_label.ForeColor = "Green"
                Print-ToPdf -Content $final_form 
                $message_label.Text = "The user account for " + $_username + " has been created. A file was created on your desktop ( " + $_username + ".html ) give this file to the user, then delete it. `n`nWARNING: The Azure AD Sync module was not found. Logon the appropriate server and execute the following commands in an administrative powershell: `nImport-Module ADSync`nStart-ADSyncSyncCycle -PolicyType Delta "

            }
        }
        elseif($pass_eval -eq 0)
        {
            $message_label.ForeColor = "Red"
            $message_label.Text =  "Failure:" + $_password + " is either null or not secure. You must have at minimum 8 characters. You must include 1 Capital letter, 1 lower case letter, 1 number, 1 special character\symbol."
        }
        elseif($user_leng -eq 0)
        {
            $message_label.ForeColor = "Red"
            $message_label.Text =  "FAILURE:" + $_username + " is either null or to short. Please make sure it has at least 3 characters."
        }
        elseif($user_exists -eq 1)
        {
            $message_label.ForeColor = "Red"
            $message_label.Text =  "An account with the username: " + $_username + " already exists, please try again..."
        }
        elseif($email_eval -eq 0)
        {
            $message_label.ForeColor = "Red"
            $message_label.Text =  "FAILURE: You have used an invalid email format. It must be username@domain.tld - Not: " + $_email
        }
    }
}
Function CreateLocalUser
{
     param (
        [int]$is_admin,
        [string]$lusername,
        [string]$lpassword
    )
    $pass_eval = Validate-Password -password $lpassword
    $user_exists = Validate-LocalUser -username $lusername
    $uleng = Validate-UserLength -uname $lusername
    if($pass_eval -eq 1 -and $user_exists -eq 0 -and $uleng -eq 1)
    {
        NET USER $lusername $lpassword /add 
        if($admin_button.Checked -eq $true)
        {
            NET localgroup Administrators $lusername /add
            $path_var = Validate-Path 
            Dump-UserForm -username $lusername -password $lpassword -path $path_var -is_domain 0 -local_administrator 1
            $lmessage_label.ForeColor = "Green"
            $lmessage_label.Text = "The user Account for " + $lusername + " has been created, a file has been created and can be found at " + $path_var + $lusername + ".html please take this file and present it to the user. Once transfered, delete this file."
        } 
        elseif($admin_button.Checked -eq $false)
        {
            $path_var = Validate-Path 
            Dump-UserForm -username $lusername -password $lpassword -path $path_var -is_domain 0 -local_administrator 0
            $lmessage_label.ForeColor = "Green"
            $lmessage_label.Text = "The user Account for " + $lusername + " has been created, a file has been created and can be found at " + $path_var + $lusername + ".html  please take this file and present it to the user. Once transfered, delete this file."
        }
    }
    else
    {
        if($uleng -eq 0)
        {
            $lmessage_label.ForeColor = "Red"
            $lmessage_label.Text = "FAILURE: Either the username feild is empty or it's 2 characters or less. Make sure the username is at least three characters longs"
        }

        if($pass_eval -eq 0)
        {
            $lmessage_label.ForeColor = "Red"
            $lmessage_label.Text = "FAILURE: Either the password " + $lpassword + " is less than 8 characters or it's not secure. Please add 1 uppercase, 1 lowercase character, a number and at least one symbol. Also, make sure it's 8 or more characters."
        }

        if($user_exists -eq 1)
        {
            $lmessage_label.ForeColor = "Red"
            $lmessage_label.Text = "The user Account for " + $lusername + " already exists, please try again."
        }
    }
}
Function Validate-LocalUser 
{
    param (
        [parameter(Mandatory = $true)]
        [string]$username
    )

    $test = (Get-LocalUser -Name $username -ErrorAction SilentlyContinue | Select-Object Name )
    if($null -ne $test.Name)
    {
        return 1    
    }
    elseif($null -eq $test.Name)
    {    
        return 0
    }
}
Function Validate-DomainUser
{
    param (
        [parameter(Mandatory = $true)]
        [string]$username
    )
    $test = (Get-ADUser -Filter {SamAccountName -like $username})
    if($null -ne $test)
    {
        return 1
    }
    elseif($null -eq $test)
    {
        return 0
    }
}
Function Validate-Password
{
    param (
        [parameter (Mandatory = $true)]
        [string]$password
    )
    $pattern = "(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*[^A-Za-z0-9])(?=.{8,})" 
    if($password -cmatch $pattern)
    {
        return 1
    }
    else
    {
        return 0
    }
}
Function Validate-PasswordRedux
{
    param (
        [parameter (Mandatory = $true)]
        [string]$password,
        [int]$Case,
        [int]$Length,
        [int]$Complexity
    )
    switch($Case)
    {
        1
        {
            if($Complexity -eq 0 -and $password.Length -ge $Length)
            {
                $pattern = "(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*[^A-Za-z0-9])"
                if($password -cmatch $pattern)
                {
                    return 1
                }
                else
                {
                    return 0
                }
            }
            elseif($Complexity -eq 1 -and $password.Length -ge $Length)
            {
                $pattern = "(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*[^A-Za-z0-9])"
                if($password -cmatch $pattern)
                {
                    return 1
                }
                else
                {
                    return 0
                }
            }
            else
            {
                return 0
            }
        }

        2
        {
            $pattern = "(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*[^A-Za-z0-9])(?=.{8,})" 
            if($password -cmatch $pattern)
            {
                return 1
            }
            else
            {
                return 0
            }
        }
    }
}
Function Validate-UserLength 
{
    param (
        [parameter (Mandatory = $true)]
        [string]$uname
    )
    if($uname.Length -lt 3)
    {
        return 0
    }
    elseif($uname.Length -gt 3)
    {
        return 1
    }
}
Function Validate-Email
{
    param (
        [parameter (Mandatory = $true)]
        [string]$emailaddress
    )
    $pattern = "^([\w-\.]+)@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.)|(([\w-]+\.)+))([a-zA-Z]{2,4}|[0-9]{1,3})(\]?)$"
    $validate = [mailaddress]$emailaddress 
    if($validate.Address -cmatch $pattern)
    {
        return 1
    }
    else
    {
        return 0
    }
}
Function Validate-Path 
{
   $t = (Get-ItemProperty -Path 'HKCU:\Software\Microsoft\Windows\CurrentVersion\Explorer\User Shell Folders\' -Name Desktop | Select-Object Desktop -ErrorAction SilentlyContinue)
   if($null -ne $t.Desktop)
   {     
       $str = $t.Desktop.toString() + "\"
       return $str
   }
   else
   {
       $str = $ENV:USERPROFILE + "\Downloads\"
       return $str
   }
}
Function Validate-DistinguishedNames
{
    param (
        [parameter (Mandatory = $true)]
        [int]$index,
        [string]$ou
    )
    $a,$b, $c = $ou.Split(',')
    $a = $a.substring(2) -replace '[\W]', '' 
    $b = $b.substring(2) -replace '[\W]', ''
    $hacked = $b + " - " + $a 
    return $hacked
}
Function Validate-MultiEnvUserNames
{
    param (
        [parameter (Mandatory = $true)]
        [int]$Index,
        [string]$User
    )
    $dirty = Get-OrginizationalUnit -Identity $User
    $comma_count = ($dirty.ToCharArray() | Where-Object {$_ -eq ','} | Measure-Object).Count + 1

    if($comma_count -gt 1)
    {
        [System.Collections.ArrayList]$a = $dirty.split(',')
       
        $a.RemoveAt($a.Count - 1)
        $a.RemoveAt($a.Count - 1)
        $var = $a -join "->"
        
    }
    return $var + " - " + $User
}
Function Get-UserIdentifier
{
    param (
        [parameter (Mandatory = $true)]
        [string]$Identity
    )
    $upn = Get-ADForest | Select-Object -ExpandProperty UPNSuffixes
    $domain = Get-ADForest | Select-Object -ExpandProperty Domains
    if($upn.Count -eq 0)
    {
        $domain = Get-ADForest | Select-Object -ExpandProperty Domains
        return $domain
    }
    elseif($upn.Count -ge 1)
    {
        $temp = (Get-ADUser -Filter {Name -like $Identity} -Properties UserPrincipalName).UserPrincipalName 
        $garbage, $upn = $temp.Split('@')
        return $upn
    }
}
Function Get-OrginizationalUnit 
{
    param (
        [parameter (Mandatory = $true)]
        $Identity
    )

    $temp = (Get-ADUser -Filter {Name -like $Identity})
    $t = $temp.DistinguishedName
    $garbage, $OU = $t.split(',', 2)
    return $OU
}
Function Get-UserGroups 
{
   param (
       [parameter (Mandatory = $true)]
       [string]$Identity
   )
   $Groups = (Get-ADUser -Filter {Name -like $Identity} -Properties MemberOf).MemberOf
   $GroupsHash = @{}
   Foreach($element in $Groups)
   {
       $GroupsHash[$element] = Get-ADGroup $element | Select-Object Name
   }
   return $GroupsHash
}
Function Get-PasswordPolicyType
{
    if(Get-Module -ListAvailable -Name "ActiveDirectory")
    {
        $test = (Get-ADFineGrainedPasswordPolicy -Filter * -ErrorAction SilentlyContinue)
        if($null -eq $test)
        {
            return "Default"
        }
        elseif($null -ne $test)
        {
            return  "Default \ Fine Grained"
        }
    }
    else
    {
        return "Arbitrary"
    }
}
Function Get-AzureStatus
{
    if(Get-Module -ListAvailable -Name "ADSync")
    {
        return 1
    }
    else
    {
        return 0 
    }
}
Function Get-FineGrainedPasswordPolicies 
{
    $Grains = @{}
    $Grains = (Get-ADFineGrainedPasswordPolicy -Filter * | Select-Object AppliesTo, DistinguishedName, Name, ComplexityEnabled, MinPasswordLength)
    return $Grains
}
Function Match-UserToFineGrainedPasswordPolicy
{
     param (
         [parameter (Mandatory = $true)]
         [string]$Identity
    )
    $Groups = (Get-UserGroups -Identity $Identity)
    $Grains = (Get-FineGrainedPasswordPolicies)

    foreach($key in $Grains)
    {
      
       foreach($element in $Groups)
       {
           $var = $element.Values
       }
       $applied_to = $key.AppliesTo
       $name = $key.Name
       $good, $bad = $applied_to.split(',', 2)
       $good = $good.substring(3)
       if($var -match $good)
       {
           return $good, $name 
       }
    }
}
Function Automate-FillForms 
{
    param (
        [parameter (Mandatory = $true)]
        $Identity,
        $Template
    )
    $n_one, $n_two = $Identity.Split(' ')
    $n_one = $n_one.Substring(0, $n_one.Length - ($n_one.Length - 1))
    $_autouser = $n_one.toUpper() + $n_two.toLower()
    
    $_dou_path = (Get-OrginizationalUnit -Identity $Template)
    $t = (Get-OrginizationalUnit -Identity $Template)
    $_cou_name, $gou_name = $t.split(',')
    $_cou_name = $_cou_name.substring(3)  

    $upn = Get-ADForest | Select-Object -ExpandProperty UPNSuffixes
    $identifier = Get-UserIdentifier -Identity $Template
    if($upn.Length -ge 1)
    {
        $_autoemail = $_autouser + "@" + $identifier
    }
    elseif($upn.Length -eq 0)
    {
        $cleaned, $garbage = $identifier.split('.')
        $_autoemail = $_autouser + "@" + $cleaned.ToLower() + ".com" 
    }
    Add-Type -AssemblyName System.Web 
    $good = 0  
    switch(Get-PasswordPolicyType)
    {
        "Default" 
        {
               $default_security = (Get-ADDefaultDomainPasswordPolicy)
               while($good -eq 0)
               {
                   $_pass = [System.Web.Security.Membership]::GeneratePassword($default_security.MinPasswordLength,1)
                   $good = Validate-PasswordRedux -password $_pass -Case 1 -Length $default_security.MinPasswordLength -Complexity $default_security.ComplexityEnabled
               }
        }
        "Default \ Fine Grained" 
        {
            $policy_group, $policy_name = (Match-UserToFineGrainedPasswordPolicy -Identity $Template)
            if($policy_name.Length -eq 0)
            {
               $default_security = (Get-ADDefaultDomainPasswordPolicy)
               while($good -eq 0)
               {
                   $_pass = [System.Web.Security.Membership]::GeneratePassword($default_security.MinPasswordLength,1)
                   $good = Validate-PasswordRedux -password $_pass -Case 1 -Length $default_security.MinPasswordLength -Complexity $default_security.ComplexityEnabled
               }
            }
            elseif($policy_name.Length -gt 1)
            {
                $Grained = (Get-ADFineGrainedPasswordPolicy -Filter {Name -like $policy_name})
                while($good -eq 0)
                {
                    $_pass = [System.Web.Security.Membership]::GeneratePassword($Grained.MinPasswordLength,1)
                    $good = Validate-PasswordRedux -password $_pass -Case 1 -Length $Grained.MinPasswordLength -Complexity $Grained.ComplexityEnabled
                }
            }
        }
        "Arbitrary" 
        {
            while($good -eq 0)
            {
                $_pass = [System.Web.Security.Membership]::GeneratePassword(8,1)
                $good = Validate-PasswordRedux -password $_pass -Case 2
            }
        }
    }
    return $_autouser, $_autoemail, $_pass, $_dou_path, $_cou_name, $identifier
}
Function Automate-Generate
{
   $_genuser, $_genemail, $_genpass, $_gendou_path, $_gencou_name, $_gendomain = Automate-FillForms -Identity $employee_name_input.Text -Template $users_combo.Text
 
   $username_input.Text = $_genuser
   $email_input.Text = $_genemail
   $password_input.Text = $_genpass
   $ou_combo.Text = $_gencou_name
   $special_combo.Text = $_gendomain

   $special_combo.Visible = $true 
   $special_combo_label.Visible = $true 
   $ou_combo.Visible = $true 
   $ou_combo_label.Visible = $true
   $username_input.Visible = $true 
   $username_label.Visible = $true 
   $password_label.Visible = $true 
   $password_input.Visible = $true 
   $email_input.Visible = $true 
   $email_label.Visible = $true 
   $create_button.Visible = $true
   $advanced_button.Visible = $true
}
Function Dump-UserForm
{
    param (
        [string]$employee,
        [string]$username,
        [string]$email,
        [string]$UPN,
        [string]$OU,
        [string]$membership,
        [string]$template,
        [string]$password,
        [int]$is_domain,
        [int]$local_administrator,
        [string]$path
    )
    if($is_domain -eq 1)
    {
        $wstring = "Employee Name: " + $employee + "`nUsername: " + $username + "`nEmail Address: " + $email + "`nPassword: " + $password + "`nGroup Membership: " + $membership + "`nOrganizational Unit: " + $OU + ""
        return $wstring
    }
    elseif($is_domain -eq 0)
    {
       if($local_administrator -eq 1)
       {
           $wstring = "Username: " + $username + "`nPassword: " + $password + "`nThe user was given local administrator access per your request; to the local computer (individual mahcine) that you specified."
            return $wstring
       }
       elseif($local_administrator -eq 0)
       {
           $wstring = "Username: " + $username + "`nPassword: " + $password + "`nThis user has been created and granted access to the local (individual machine) that you specified."
            return $wstring
       }
    }
}
Function Write-GroupsCleanly
{
    param (
        [parameter (Mandatory = $true)]
        [string]$Identity
    )
    $tmp = @()
    $tmp = ((Get-ADUser -Filter {Name -like $Identity} -Properties MemberOf).MemberOf)
    $break_down = @()
    foreach($element in $tmp)
    {
        $break_down += Get-ADGroup $element | Select-Object Name

    }

    return $break_down.Name
}
Function Print-ToPdf
{
    param (
        [parameter (Mandatory = $true)]
        [string]$Content
    )
    $Content | Out-Printer -Name "Microsoft Print to PDF"
}
Function AdvancedComponents
{
    if($primary_proxy_label.Visible -eq $false -and $primary_proxy_input.Visible -eq $false -and $displayname_label.Visible -eq $false -and $displayname_input.Visible -eq $false)
    {
        $primary_proxy_label.Visible = $true
        $primary_proxy_input.Visible = $true
        $secondary_proxy_label.Visible = $true
        $secondary_proxy_input.Visible = $true
        $displayname_label.Visible = $true
        $displayname_input.Visible = $true
    }
    elseif($primary_proxy_label.Visible -eq $true -and $primary_proxy_input.Visible -eq $true -and $displayname_label.Visible -eq $true -and $displayname_input.Visible -eq $true)
    {
        $primary_proxy_label.Visible = $false
        $primary_proxy_input.Visible = $false
        $secondary_proxy_label.Visible = $false
        $secondary_proxy_input.Visible = $false
        $displayname_label.Visible = $false
        $displayname_input.Visible = $false
    }
}
Function Split-FullName
{
    param (
        [parameter (Mandatory = $true)]
        [string]$Name
    )
    $temp_first = ""
    $temp_middle = ""
    $temp_last = ""
    if($Name.Contains(' '))
    {
        if($Name.Split(' ').Count -eq 1)
        {
            return $Name
        }
        elseif($Name.Split(' ').Count -eq 2)
        {
            $temp_first, $temp_last = $Name.Split(' ')
            return $temp_first, $temp_last
        }
        elseif($Name.Split(' ').Count -eq 3)
        {
            $temp_first, $temp_middle, $temp_last = $Name.Split(' ')
            $temp_middle = $temp_middle.Substring(0, $temp_middle.Length - ($temp_middle.Length - 1))
            return $temp_first, $temp_middle, $temp_last
        }
        elseif($Name.Split(' ').Count-gt 3)
        {
             return $Name 
        }

    }
    elseif(-not $Name.Contains(" "))
    {
        return $Name 
    }
}

Function EditUserWidget-ReloadUserList
{
    $pull_users = (Get-ADUser -Filter * | Select-Object Name, GivenNAme, SurName | Sort-Object SurName, GivenName)
    $eusers_combo.Items.Clear()
    Foreach($usr in $pull_users.Name)
    {
        $eusers_combo.Items.Add($usr)
    }
}

# Experimental 
Function EditUser-SelectField
{

    if($eusers_combo.Text.Length -gt 0 -and $efield_combo.Text.Length -gt 0)
    {   
        $edisable_button.Visible = $false
        switch($efield_combo.Text)
        {
            "Orginizational Unit"
            {
                $var = Get-OrginizationalUnit -Identity $eusers_combo.Text
                $emessage_label.Text = $eusers_combo.Text  + " is in the OU:`n" + (Get-OrginizationalUnit -Identity $eusers_combo.Text)
            }
            "Domain\UPN"
            {
                $emessage_label.Text = $eusers_combo.Text + " is currently using the following identifier:`n" + (Get-UserIdentifier -Identity $eusers_combo.Text)
            }
            "Employee Name"
            {
                $ecomponent_input.Visible = $true
                $echange_button.Visible = $true
                $ecomponent_label.Text = "Input name"
                $isDone = 0
                $emessage_label.Text = "The employees name is " + $eusers_combo.Text
                $_fullname = $eusers_combo.Text
                $echange_button.Add_Click({
                    if($ecomponent_input.Text.Split(' ').Count -le 0)
                    {
                        $emessage_label.ForeColor = "Red"
                        $emessage_label.Text = "The employees name is still " + $eusers_combo.Text + " no changes have been made. You must input the users full name..."
                    }
                    elseif($ecomponent_input.Text.Split(' ').Count -eq 2)
                    {
                        $_fullname = $ecomponent_input.Text 
                        $_given, $_surname = $_fullname.Split(' ')
                        Set-ADUser -Identity ((Get-ADUser -Filter {Name -Like $eusers_combo.Text} -Properties SamAccountName).SamAccountName) -GivenName $_given -Surname $_surname -OtherName $_middlename
                        Rename-ADObject -Identity ((Get-ADUser -Filter {Name -Like $eusers_combo.Text} -Properties DistinguishedName).DistinguishedName) -NewName $_fullname
                        $emessage_label.ForeColor = "Green"
                        $emessage_label.Text = "The users name has been updated to " + $_fullname + "`nYou should ensure that you update the SamAccountName and UPN."
                        $echange_button.Visible = $false
                        $ecomponent_input.Visible = $false
                        $ecomponent_label.Text = ""
                        $ecomponent_input.Text = ""
                        $erefresh_button.Visible = $true

                    }
                    elseif($ecomponent_input.Text.Split(' ').Count -eq 3)
                    {
                        $_fullname = $ecomponent_input.Text
                        $_given, $_middlename, $_surname = $_fullname.Split(' ')
                        Set-ADUser -Identity ((Get-ADUser -Filter {Name -Like $eusers_combo.Text} -Properties SamAccountName).SamAccountName) -GivenName $_given -Surname $_surname -OtherName $_middlename
                        Rename-ADObject -Identity ((Get-ADUser -Filter {Name -Like $eusers_combo.Text} -Properties DistinguishedName).DistinguishedName) -NewName $_fullname
                        $emessage_label.ForeColor = "Green"
                        $emessage_label.Text = "The users name has been updated to " + $_fullname + "`nYou should ensure that you update the SamAccountName and UPN."
                        $echange_button.Visible = $false
                        $ecomponent_input.Visible = $false
                        $ecomponent_label.Text = ""
                        $ecomponent_input.Text = ""
                        $erefresh_button.Visible = $true
                    }
                    elseif($ecomponent_input.Text.Split(' ').Count -gt 3)
                    {
                        $isDone = 0
                        $ecomponent_input.Text = ""
                        $emessage_label.ForeColor = "Red"
                        $emessage_label.Text = "The employees name is still " + $eusers_combo.Text + " no changes have been made. You have done something terribly wrong.."
                    }
                })
            }
            "Username"
            {
                $emessage_label.Text = "The users SamAccountName is " + (Get-ADUser -Filter {Name -Like $eusers_combo.Text } -Properties SamAccountName).SamAccountName + 
                "`nThe users UserPrincipalName is " + (Get-ADUser -Filter {Name -Like $eusers_combo.Text } -Properties UserPrincipalName).UserPrincipalName
                $ecomponent_label.Text = "Insert username"
                $ecomponent_input.Visible = $true 
                $edisable_button.Visible = $false
                $erefresh_button.Visible = $false
                $echange_button.Visible = $true
                $echange_button.Add_Click({
                    if($ecomponent_input.Text.Length -le 0)
                    {
                        # do nothing
                    }
                    elseif($ecomponent_input.Text.Length -ge 1)
                    {
                        $len = Validate-UserLength -uname $ecomponent_input.Text
                        if($len -eq 1)
                        {
                            $exists = Validate-DomainUser -username $ecomponent_input.Text
                            if($exists -eq 0)
                            {
                                
                                $upn = (Get-UserIdentifier -Identity $eusers_combo.Text)
                                $UserPrincipalName = $ecomponent_input.Text + "@" + $upn
                                Set-ADUser -Identity ((Get-ADUser -Filter {Name -Like $eusers_combo.Text} -Properties SamAccountName).SamAccountName) -SamAccountName $ecomponent_input.Text -UserPrincipalName $UserPrincipalName
                                $emessage_label.ForeColor = "Green"
                                $emessage_label.Text = $eusers_combo.Text + "'s SamAccountName is now " + $ecomponent_input.Text + "`n and the UserPrincipalName is " + $UserPrincipalName
                                $erefresh_button.Visible = $true
                                $ecomponent_input.Text = ""
                                $ecomponent_input.Visible = $false 
                                $ecomponent_label.Visible = $false
                            }    
                            elseif($exists -eq 1)
                            {
                                $emessage_label.ForeColor = "Red"
                                $emessage_label.Text = "WARNING: The username " + $ecomponent_input.Text + " already exists. Please use something else."
                            }
                        }
                        elseif($len -eq 0)
                        {
                            $emessage_label.ForeColor = "Red"
                            $emessage_label.Text = "WARNING: The username " + $ecomponent_input.Text + " is to short. Try again."
                        }
                    }    
                })

            }
            "Password"
            {
                $ecomponent_label.Text = "Insert password"
                $ecomponent_input.Visible = $true 
                $edisable_button.Visible = $false
                $erefresh_button.Visible = $false
                $echange_button.Visible = $true
                Add-Type -AssemblyName System.Web 
                $good = 0
                $_pass = ""
                switch(Get-PasswordPolicyType)
                {
                    "Default"
                    {
                        $emessage_label.Text = "Password must meet the following requirements:`n" + "Complexity: " + ((Get-ADDefaultDomainPasswordPolicy).ComplexityEnabled) +
                            "`nMinimum Password Length: " + ((Get-ADDefaultDomainPasswordPolicy).MinPasswordLength)
                        $echange_button.Add_Click({
                            if($ecomponent_input.Text.Length -gt 1)
                            {
                                $default_security = (Get-ADDefaultDomainPasswordPolicy)
                                $good = Validate-PasswordRedux -password $ecomponent_input.Text -Case 1 -Length $default_security.MinPasswordLength -Complexity $default_security.ComplexityEnabled
                                if($good -eq 1)
                                {
                                    Set-ADAccountPassword -Identity ((Get-ADUser -Filter {Name -Like $eusers_combo.Text} -Properties SamAccountName).SamAccountName) -Reset -NewPassword (ConvertTo-SecureString -AsPlainText $ecomponent_input.Text -Force)
                                    $emessage_label.ForeColor = "Green"
                                    $emessage_label.Text = $eusers_combo.Text + "'s password is now`n" + $ecomponent_input.Text
                                    $echange_button.Visible = $false
                                    $ecomponent_input.Visible = $false
                                    $ecomponent_label.Text = ""
                                    $ecomponent_input.Text = ""
                                    $erefresh_button.Visible = $true
                                }
                                elseif($good -eq 0)
                                {
                                    $emessage_label.ForeColor = "Red"
                                    $emessage_label.Text = "Password did not meet the following requirements:`n" + "Complexity: " + ((Get-ADDefaultDomainPasswordPolicy).ComplexityEnabled) +
                                        "`nMinimum Password Length: " + ((Get-ADDefaultDomainPasswordPolicy).MinPasswordLength)
                                }
                            }
                            elseif($ecomponent_input.Text.Length -le 0)
                            {
                                $emessage_label.ForeColor = "Red"
                                $emessage_label.Text = "You need to input something, this box cannot be empty."
                            }
                        })
                    }
                    "Default \ Fine Grained"
                    {
                        $policy_group, $policy_name = (Match-UserToFineGrainedPasswordPolicy -Identity $eusers_combo.Text)
                        if($policy_name.Length -eq 0)
                        {
                            $emessage_label.Text = "Password must meet the following requirements:`n" + "Complexity: " + ((Get-ADDefaultDomainPasswordPolicy).ComplexityEnabled) +
                                "`nMinimum Password Length: " + ((Get-ADDefaultDomainPasswordPolicy).MinPasswordLength)
                            $default_security = (Get-ADDefaultDomainPasswordPolicy)
                            $echange_button.Add_Click({
                                if($ecomponent_input.Text.Length -gt 1)
                                {
                                    $good = Validate-PasswordRedux -password $ecomponent_input.Text -Case 1 -Length $default_security.MinPasswordLength -Complexity $default_security.ComplexityEnabled
                                    if($good -eq 1)
                                    {
                                        Set-ADAccountPassword -Identity ((Get-ADUser -Filter {Name -Like $eusers_combo.Text} -Properties SamAccountName).SamAccountName) -Reset -NewPassword (ConvertTo-SecureString -AsPlainText $ecomponent_input.Text -Force)
                                        $emessage_label.ForeColor = "Green"
                                        $emessage_label.Text = $eusers_combo.Text + "'s password is now`n" + $ecomponent_input.Text
                                        $echange_button.Visible = $false
                                        $ecomponent_input.Visible = $false
                                        $ecomponent_label.Text = ""
                                        $ecomponent_input.Text = ""
                                        $erefresh_button.Visible = $true
                                    }
                                    elseif($good -eq 0)
                                    {
                                        $emessage_label.ForeColor = "Red"
                                        $emessage_label.Text = "Password did not meet the following requirements:`n" + "Complexity: " + ((Get-ADDefaultDomainPasswordPolicy).ComplexityEnabled) +
                                            "`nMinimum Password Length: " + ((Get-ADDefaultDomainPasswordPolicy).MinPasswordLength)
                                    }
                                }
                                elseif($ecomponent_input.Text.Length -le 1)
                                {
                                    $emessage_label.ForeColor = "Red"
                                    $emessage_label.Text = "This field cannot be empty. Please input a new password..."
                                }
                            })
                        }
                        elseif($policy_name.Length -ge 1)
                        {
                            $Grained = (Get-ADFineGrainedPasswordPolicy -Filter {Name -like $policy_name})
                            $emessage_label.Text = "Password must meet the following requirements:`n" + "Complexity: " + ($Grained.ComplexityEnabled) +
                                "`nMinimum Password Length: " + ($Grained.MinPasswordLength)
                            $echange_button.Add_Click({
                                if($ecomponent_input.Text.Length -gt 1)
                                {
                                    $good = Validate-PasswordRedux -password ($ecomponent_input.Text) -Case 1 -Length $Grained.MinPasswordLength -Complexity $Grained.ComplexityEnabled   
                                    if($good -eq 1)
                                    {
                                        Set-ADAccountPassword -Identity ((Get-ADUser -Filter {Name -Like $eusers_combo.Text} -Properties SamAccountName).SamAccountName) -Reset -NewPassword (ConvertTo-SecureString -AsPlainText $ecomponent_input.Text -Force)
                                        $emessage_label.ForeColor = "Green"
                                        $emessage_label.Text = $eusers_combo.Text + "'s password is now`n" + $ecomponent_input.Text
                                        $echange_button.Visible = $false
                                        $ecomponent_input.Visible = $false
                                        $ecomponent_label.Text = ""
                                        $ecomponent_input.Text = ""
                                        $erefresh_button.Visible = $true
                                    }
                                    elseif($good -eq 0)
                                    {
                                        $emessage_label.ForeColor = "Red"
                                        $emessage_label.Text = "Password did not meet the following requirements:`n" + "Complexity: " + ($Grained.ComplexityEnabled) +
                                            "`nMinimum Password Length: " + ($Grained.MinPasswordLength)
                                    }
                                }
                                elseif($ecomponent_input.Text.Length -le 1)
                                {
                                    $emessage_label.ForeColor = "Red"
                                    $emessage_label.Text = "You need to input something, this box cannot be empty."
                                }
                            }) 
                        }
                    }
                    "Arbitrary"
                    {
                        $emessage_label.Text = "Password must meet the following requirements:`nComplexity Enabled: True`nMinimum Password Length: 8"
                        $echange_button.Add_Click({
                            if($ecomponent_input.Text.Length -gt 1)
                            {
                                $good = Validate-PasswordRedux -password $_pass -Case 2
                                if($good -eq 1)
                                {
                                    Set-ADAccountPassword -Identity ((Get-ADUser -Filter {Name -Like $eusers_combo.Text} -Properties SamAccountName).SamAccountName) -Reset -NewPassword (ConvertTo-SecureString -AsPlainText $ecomponent_input.Text -Force)
                                    $emessage_label.ForeColor = "Green"
                                    $emessage_label.Text = $eusers_combo.Text + "'s password is now`n" + $ecomponent_input.Text
                                    $echange_button.Visible = $false
                                    $ecomponent_input.Visible = $false
                                    $ecomponent_label.Text = ""
                                    $ecomponent_input.Text = ""
                                    $erefresh_button.Visible = $true
                                }
                                elseif($good -eq 0)
                                {
                                    $emessage_label.Text = "Password did not meet the following requirements:`nComplexity Enabled: True`nMinimum Password Length: 8"
                                }
                            }
                            elseif($ecomponent_input.Text.Length -le 1)
                            {
                                $emessage_label.ForeColor = "Red"
                                $emessage_label.Text = "You need to input something, this box cannot be empty."
                            }
                        })
                    }
                }
            }
            "Email Address"
            {
                $emessage_label.Text = $eusers_combo.Text + "'s email address is " + (Get-ADUser -Filter {Name -Like $eusers_combo.Text } -Properties Mail).Mail
            }
            "Primary Proxy"
            {
                $var = (Get-ADUser -Filter {Name -like $eusers_combo.Text} -Properties Proxyaddresses).Proxyaddresses
                $found = $var | Select-String -Pattern "SMTP:" -CaseSensitive -SimpleMatch
                $emessage_label.Text = $eusers_combo.Text + "'s primary proxy address is " + $found
            }
            "Secondary Proxy"
            {
                $var = (Get-ADUser -Filter {Name -like $eusers_combo.Text} -Properties Proxyaddresses).Proxyaddresses
                $found = $var | Select-String -Pattern "smtp:" -CaseSensitive -SimpleMatch
                $emessage_label.Text = $eusers_combo.Text + "'s secondary proxy address is " + $found
            }
            "Display Name"
            {
                $emessage_label.Text = $eusers_combo.Text + "'s display name is " + (Get-ADUser -Filter {Name -like $eusers_combo.Text} -Properties displayName).displayName
            }  
        }
    }
}

# Experimental

Function DomainUserWidget
{
    [void] [System.Reflection.Assembly]::LoadWithPartialName("System.Drawing")
    [void] [System.Reflection.Assembly]::LoadWithPartialName("System.Windows.Forms")

    $Domain_Form = New-Object System.Windows.Forms.Form
    $Domain_Form.Text = "UMTK: Domain User Creation"
    $Domain_Form.Size = New-Object System.Drawing.Size(800, 800)
    $Domain_Form.minimumSize = New-Object System.Drawing.Size(800,800) 
    $Domain_Form.maximumSize = New-Object System.Drawing.Size(800,800) 
    $Domain_Form.FormBorderStyle = 'Fixed3D'
    $Domain_Form.StartPosition = 'CenterScreen'

    $users_combo = New-Object Windows.Forms.ComboBox 
    $users_combo.size = New-Object System.Drawing.Size(350, 150)
    $users_combo.location = New-Object System.Drawing.Size(205, 110)
    $users_combo.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $pull_users = (Get-ADUser -Filter * | Select-Object Name, GivenNAme, SurName | Sort-Object SurName, GivenName) # Select-Object -ExpandProperty Name

    Foreach($usr in $pull_users.Name)
    {
        $users_combo.Items.Add($usr)
    }
    $Domain_Form.Controls.Add($users_combo)

    $users_combo_label = New-Object Windows.Forms.Label
    $users_combo_label.size = New-Object System.Drawing.Size(200, 35)
    $users_combo_label.location = New-Object System.Drawing.Size(0, 110)
    $users_combo_label.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $users_combo_label.text = "Template user:"
    $Domain_Form.Controls.Add($users_combo_label)

    $special_combo = New-Object Windows.Forms.ComboBox 
    $special_combo.size = New-Object System.Drawing.Size(350, 150)
    $special_combo.location = New-Object System.Drawing.Size(205, 145)
    $special_combo.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $domain_lookup = Get-ADForest | Select-Object -ExpandProperty Domains
    $upn_lookup = Get-ADForest | Select-Object -ExpandProperty UPNSuffixes
    if($upn_lookup.Count -gt 0)
    {
        Foreach($upn in $upn_lookup)
        {
            $special_combo.Items.Add($upn)
        }
    }
    else
    {
        Foreach($domain in $domain_lookup)
        {
            $special_combo.Items.Add($domain)
        }
    }
    $special_combo.Visible = $false 
    $Domain_Form.Controls.Add($special_combo)

    $ou_combo = New-Object Windows.Forms.ComboBox 
    $ou_combo.size = New-Object System.Drawing.Size(350, 150)
    $ou_combo.location = New-Object System.Drawing.Size(205, 75)
    $ou_combo.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $clean_ous = Get-ADOrganizationalUnit -Filter * | Select-Object -ExpandProperty Name
    $distinguished_ous = Get-ADOrganizationalUnit -Filter * | Select-Object -ExpandProperty distinguishedName
    #$multi_env_users = Get-ADUser -Filter * | Select-Object -ExpandProperty Name
    if($upn_lookup.Count -gt 2)
    {
        Foreach($ou in $distinguished_ous)
        {
            $adjusted = Validate-DistinguishedNames -ou $ou -index 0
            $ou_combo.Items.Add($adjusted)       
        }
        #$users_combo.Items.Clear()
        #$users_combo.Size = New-Object System.Drawing.Size(500, 150)
        #Foreach($usr in $multi_env_users)
        #{
        #    $adjusted_users = Validate-MultiEnvUserNames -User $usr -Index 0
        #    $users_combo.Items.Add($adjusted_users)
        #}

    }
    elseif($upn_lookup.Count -le 2)
    {    
         Foreach($ou in $clean_ous)
        {
            $ou_combo.Items.Add($ou)
        }
    }
    $ou_combo.Visible = $false
    $Domain_Form.Controls.Add($ou_combo)

    $ou_combo_label = New-Object Windows.Forms.Label
    $ou_combo_label.size = New-Object System.Drawing.Size(220, 35)
    $ou_combo_label.location = New-Object System.Drawing.Size(0, 75)
    $ou_combo_label.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $ou_combo_label.text = "Organizational unit:"
    $ou_combo_label.Visible = $false 
    $Domain_Form.Controls.Add($ou_combo_label)

    $special_combo_label = New-Object Windows.Forms.Label
    $special_combo_label.size = New-Object System.Drawing.Size(200, 35)
    $special_combo_label.location = New-Object System.Drawing.Size(0, 145)
    $special_combo_label.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $special_combo_label.text = "Domain or UPN:"
    $special_combo_label.Visible = $false
    $Domain_Form.Controls.Add($special_combo_label)

    $employee_name_label = New-Object Windows.Forms.Label
    $employee_name_label.size = New-Object System.Drawing.Size(200, 35)
    $employee_name_label.location = New-Object System.Drawing.Size(0, 180)
    $employee_name_label.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $employee_name_label.text = "Employee name:"
    $Domain_Form.Controls.Add($employee_name_label)

    $employee_name_input = New-Object Windows.Forms.TextBox
    $employee_name_input.size = New-Object System.Drawing.Size(350, 75)
    $employee_name_input.location = New-Object System.Drawing.Size(205, 180)
    $employee_name_input.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $Domain_Form.Controls.Add($employee_name_input)

    $username_label = New-Object Windows.Forms.Label
    $username_label.size = New-Object System.Drawing.Size(200, 35)
    $username_label.location = New-Object System.Drawing.Size(0, 215)
    $username_label.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $username_label.text = "Username:"
    $username_label.Visible = $false
    $Domain_Form.Controls.Add($username_label)

    $username_input = New-Object Windows.Forms.TextBox
    $username_input.size = New-Object System.Drawing.Size(350, 75)
    $username_input.location = New-Object System.Drawing.Size(205, 215)
    $username_input.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $username_input.Visible = $false 
    $Domain_Form.Controls.Add($username_input)

    $password_label = New-Object Windows.Forms.Label
    $password_label.size = New-Object System.Drawing.Size(200, 35)
    $password_label.location = New-Object System.Drawing.Size(0, 250)
    $password_label.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $password_label.text = "Password:"
    $password_label.Visible = $false
    $Domain_Form.Controls.Add($password_label)

    $password_input = New-Object Windows.Forms.TextBox
    $password_input.size = New-Object System.Drawing.Size(350, 75)
    $password_input.location = New-Object System.Drawing.Size(205, 250)
    $password_input.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $password_input.Visible = $false
    $Domain_Form.Controls.Add($password_input)

    $email_label = New-Object Windows.Forms.Label
    $email_label.size = New-Object System.Drawing.Size(200, 35)
    $email_label.location = New-Object System.Drawing.Size(0, 285)
    $email_label.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $email_label.text = "Email address:"
    $email_label.Visible = $false
    $Domain_Form.Controls.Add($email_label)

    $email_input = New-Object Windows.Forms.TextBox
    $email_input.size = New-Object System.Drawing.Size(350, 75)
    $email_input.location = New-Object System.Drawing.Size(205, 285)
    $email_input.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $email_input.Visible = $false
    $Domain_Form.Controls.Add($email_input)

    $primary_proxy_label = New-Object Windows.Forms.Label
    $primary_proxy_label.size = New-Object System.Drawing.Size(200, 35)
    $primary_proxy_label.location = New-Object System.Drawing.Size(0, 320)
    $primary_proxy_label.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $primary_proxy_label.text = "Primary Proxy:"
    $primary_proxy_label.Visible = $false
    $Domain_Form.Controls.Add($primary_proxy_label)

    $primary_proxy_input = New-Object Windows.Forms.TextBox
    $primary_proxy_input.size = New-Object System.Drawing.Size(350, 75)
    $primary_proxy_input.location = New-Object System.Drawing.Size(205, 320)
    $primary_proxy_input.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $primary_proxy_input.Visible = $false
    $Domain_Form.Controls.Add($primary_proxy_input)

    $advanced_button = New-Object Windows.Forms.Button
    $advanced_button.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $advanced_button.Text = "Advanced"
    $advanced_button.size = New-Object System.Drawing.Size(145, 30)
    $advanced_button.location = New-Object System.Drawing.Size(50, 420)
    $advanced_button.Add_Click({AdvancedComponents})
    $advanced_button.Visible = $false
    $Domain_Form.Controls.Add($advanced_button)

    $secondary_proxy_label = New-Object Windows.Forms.Label
    $secondary_proxy_label.size = New-Object System.Drawing.Size(204, 35)
    $secondary_proxy_label.location = New-Object System.Drawing.Size(0, 355)
    $secondary_proxy_label.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $secondary_proxy_label.text = "Secondary Proxy:"
    $secondary_proxy_label.Visible = $false
    $Domain_Form.Controls.Add($secondary_proxy_label)

    $secondary_proxy_input = New-Object Windows.Forms.TextBox
    $secondary_proxy_input.size = New-Object System.Drawing.Size(350, 75)
    $secondary_proxy_input.location = New-Object System.Drawing.Size(205, 355)
    $secondary_proxy_input.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $secondary_proxy_input.Visible = $false
    $Domain_Form.Controls.Add($secondary_proxy_input)

    $displayname_label = New-Object Windows.Forms.Label
    $displayname_label.size = New-Object System.Drawing.Size(200, 35)
    $displayname_label.location = New-Object System.Drawing.Size(0, 390)
    $displayname_label.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $displayname_label.text = "Display name:"
    $displayname_label.Visible = $false
    $Domain_Form.Controls.Add($displayname_label)

    $displayname_input = New-Object Windows.Forms.TextBox
    $displayname_input.size = New-Object System.Drawing.Size(350, 75)
    $displayname_input.location = New-Object System.Drawing.Size(205, 390)
    $displayname_input.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $displayname_input.Visible = $false
    $Domain_Form.Controls.Add($displayname_input)

    $create_button = New-Object Windows.Forms.Button
    $create_button.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $create_button.Text = "Create"
    $create_button.size = New-Object System.Drawing.Size(120, 30)
    $create_button.location = New-Object System.Drawing.Size(195, 420) #415
    $create_button.Add_Click({CreateDomainUser -_ou $ou_combo.SelectedIndex -_template $users_combo.Text -_upn $special_combo.Text -_fullname $employee_name_input.Text -_username $username_input.Text -_password $password_input.Text -_email $email_input.Text})
    $create_button.Visible = $false 
    $Domain_Form.Controls.Add($create_button)

    $close_button = New-Object Windows.Forms.Button
    $close_button.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $close_button.Text = "Close"
    $close_button.size = New-Object System.Drawing.Size(120, 30)
    $close_button.location = New-Object System.Drawing.Size(435, 420) # 415
    $close_button.Add_Click({$Domain_Form.Add_FormClosing({$_.Cancel=$false});$Domain_Form.Close()})   
    $Domain_Form.Controls.Add($close_button)

    $generate_button = New-Object Windows.Forms.Button
    $generate_button.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $generate_button.Text = "Generate"
    $generate_button.size = New-Object System.Drawing.Size(120, 30)
    $generate_button.location = New-Object System.Drawing.Size(315, 420) #415
    $generate_button.Add_Click({Automate-Generate})   
    $Domain_Form.Controls.Add($generate_button)

    $message_label = New-Object Windows.Forms.Label
    $message_label.size = New-Object System.Drawing.Size(700, 300)
    $message_label.location = New-Object System.Drawing.Size(0, 450) # was 410
    $message_label.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $Domain_Form.Controls.Add($message_label)

    $Domain_Form.Add_Shown({$Domain_Form.Activate()})
    [void] $Domain_Form.ShowDialog()
}
Function LocalUserWidget 
{
    [void] [System.Reflection.Assembly]::LoadWithPartialName("System.Drawing")
    [void] [System.Reflection.Assembly]::LoadWithPartialName("System.Windows.Forms")

    $Luser_Form = New-Object System.Windows.Forms.Form
    $Luser_Form.Text = "UMTK: Local User Creation"
    $Luser_Form.Size = New-Object System.Drawing.Size(600, 600)
    $Luser_Form.minimumSize = New-Object System.Drawing.Size(600,600) 
    $Luser_Form.maximumSize = New-Object System.Drawing.Size(600,600) 
    $Luser_Form.FormBorderStyle = 'Fixed3D'
    $Luser_Form.StartPosition = 'CenterScreen'
    
    $lusername_label = New-Object Windows.Forms.Label
    $lusername_label.size = New-Object System.Drawing.Size(150, 35)
    $lusername_label.location = New-Object System.Drawing.Size(0, 90)
    $lusername_label.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $lusername_label.text = "Username:"
    $Luser_Form.Controls.Add($lusername_label)

    $lusername_input = New-Object Windows.Forms.TextBox
    $lusername_input.size = New-Object System.Drawing.Size(350, 75)
    $lusername_input.location = New-Object System.Drawing.Size(150, 90)
    $lusername_input.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $Luser_Form.Controls.Add($lusername_input)

    $lpassword_label = New-Object Windows.Forms.Label
    $lpassword_label.size = New-Object System.Drawing.Size(150, 35)
    $lpassword_label.location = New-Object System.Drawing.Size(0, 125)
    $lpassword_label.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $lpassword_label.text = "Password:"
    $Luser_Form.Controls.Add($lpassword_label)

    $lpassword_input = New-Object Windows.Forms.TextBox
    $lpassword_input.size = New-Object System.Drawing.Size(350, 75)
    $lpassword_input.location = New-Object System.Drawing.Size(150, 125)
    $lpassword_input.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $Luser_Form.Controls.Add($lpassword_input)

    $admin_button = New-Object Windows.Forms.CheckBox
    $admin_button.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $admin_button.Text = "Admin"
    $admin_button.size = New-Object System.Drawing.Size(120, 30)
    $admin_button.location = New-Object System.Drawing.Size(150, 160)
    $Luser_Form.Controls.Add($admin_button)

    $lcreate_button = New-Object Windows.Forms.Button
    $lcreate_button.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $lcreate_button.Text = "Create"
    $lcreate_button.size = New-Object System.Drawing.Size(120, 30)
    $lcreate_button.location = New-Object System.Drawing.Size(150, 190)
    $lcreate_button.Add_Click({CreateLocalUser -lfullname $lemployee_name_input.Text -lusername $lusername_input.Text -lpassword $lpassword_input.Text})   
    $Luser_Form.Controls.Add($lcreate_button)

    $lclose_button = New-Object Windows.Forms.Button
    $lclose_button.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $lclose_button.Text = "Close"
    $lclose_button.size = New-Object System.Drawing.Size(120, 30)
    $lclose_button.location = New-Object System.Drawing.Size(380, 190)
    $lclose_button.Add_Click({$Luser_Form.Add_FormClosing({$_.Cancel=$false});$Luser_Form.Close()})   
    $Luser_Form.Controls.Add($lclose_button)

    $lmessage_label = New-Object Windows.Forms.Label
    $lmessage_label.size = New-Object System.Drawing.Size(600, 200)
    $lmessage_label.location = New-Object System.Drawing.Size(0, 220)
    $lmessage_label.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $lUser_Form.Controls.Add($lmessage_label)

    $Luser_Form.Add_Shown({$Luser_Form.Activate()})
    [void] $Luser_Form.ShowDialog()
}
Function EditUserWidget 
{
    [void] [System.Reflection.Assembly]::LoadWithPartialName("System.Drawing")
    [void] [System.Reflection.Assembly]::LoadWithPartialName("System.Windows.Forms")

    $Euser_Form = New-Object System.Windows.Forms.Form
    $Euser_Form.Text = "UMTK: Domain User Modifcation"
    $Euser_Form.Size = New-Object System.Drawing.Size(800, 800)
    $Euser_Form.minimumSize = New-Object System.Drawing.Size(800,800) 
    $Euser_Form.maximumSize = New-Object System.Drawing.Size(800,800) 
    $Euser_Form.FormBorderStyle = 'Fixed3D'
    $Euser_Form.StartPosition = 'CenterScreen'

    $eusers_combo = New-Object Windows.Forms.ComboBox 
    $eusers_combo.size = New-Object System.Drawing.Size(350, 150)
    $eusers_combo.location = New-Object System.Drawing.Size(205, 145)
    $eusers_combo.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $pull_users = (Get-ADUser -Filter * | Select-Object Name, GivenNAme, SurName | Sort-Object SurName, GivenName)

    Foreach($usr in $pull_users.Name)
    {
        $eusers_combo.Items.Add($usr)
    }
    $Euser_Form.Controls.Add($eusers_combo)

    $eusers_label = New-Object Windows.Forms.Label
    $eusers_label.size = New-Object System.Drawing.Size(200, 35)
    $eusers_label.location = New-Object System.Drawing.Size(0, 145)
    $eusers_label.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $eusers_label.text = "Select user:"
    $Euser_Form.Controls.Add($eusers_label)

    $efields = @("Orginizational Unit", "Domain\UPN", "Employee Name", "Username", "Password", "Email Address" ,"Primary Proxy" ,"Secondary Proxy", "Display Name")
    $efield_combo = New-Object Windows.Forms.ComboBox 
    $efield_combo.size = New-Object System.Drawing.Size(350, 150)
    $efield_combo.location = New-Object System.Drawing.Size(205, 90)
    $efield_combo.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    Foreach($var in $efields)
    {
        $efield_combo.Items.Add($var)
    }
    $Euser_Form.Controls.Add($efield_combo)

    $efield_label = New-Object Windows.Forms.Label
    $efield_label.size = New-Object System.Drawing.Size(220, 35)
    $efield_label.location = New-Object System.Drawing.Size(0, 90)
    $efield_label.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $efield_label.text = "Select Field:"
    $efield_label.Visible = $true 
    $Euser_Form.Controls.Add($efield_label)

    $eclose_button = New-Object Windows.Forms.Button
    $eclose_button.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $eclose_button.Text = "Close"
    $eclose_button.size = New-Object System.Drawing.Size(120, 35)
    $eclose_button.location = New-Object System.Drawing.Size(665, 5) # 415
    $eclose_button.Add_Click({$Euser_Form.Add_FormClosing({$_.Cancel=$false});$Euser_Form.Close()})   
    $Euser_Form.Controls.Add($eclose_button)

    $eload_button = New-Object Windows.Forms.Button
    $eload_button.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $eload_button.Text = "Load"
    $eload_button.size = New-Object System.Drawing.Size(120, 35)
    $eload_button.location = New-Object System.Drawing.Size(5, 5) #415
    $eload_button.Add_Click({EditUser-SelectField})   
    $Euser_Form.Controls.Add($eload_button)

    $emessage_label = New-Object Windows.Forms.Label
    $emessage_label.size = New-Object System.Drawing.Size(700, 300)
    $emessage_label.location = New-Object System.Drawing.Size(0, 450) # was 410
    $emessage_label.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $Euser_Form.Controls.Add($emessage_label)

    # NEW COMPONENTS
    $ecomponent_input = New-Object Windows.Forms.TextBox
    $ecomponent_input.size = New-Object System.Drawing.Size(350, 75)
    $ecomponent_input.location = New-Object System.Drawing.Size(205, 190)
    $ecomponent_input.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $ecomponent_input.Visible = $false
    $Euser_Form.Controls.Add($ecomponent_input)
     
    $ecomponent_label = New-Object Windows.Forms.Label
    $ecomponent_label.size = New-Object System.Drawing.Size(700, 300)
    $ecomponent_label.location = New-Object System.Drawing.Size(0, 190) # was 410
    $ecomponent_label.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $Euser_Form.Controls.Add($ecomponent_label)

    $echange_button = New-Object Windows.Forms.Button
    $echange_button.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $echange_button.Text = "Change"
    $echange_button.size = New-Object System.Drawing.Size(120, 35)
    $echange_button.location = New-Object System.Drawing.Size(130, 5) #415
    $echange_button.Visible = $false
    $Euser_Form.Controls.Add($echange_button)

    $erefresh_button = New-Object Windows.Forms.Button
    $erefresh_button.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $erefresh_button.Text = "Refresh AD"
    $erefresh_button.size = New-Object System.Drawing.Size(150, 35)
    $erefresh_button.location = New-Object System.Drawing.Size(255, 5) #415
    $erefresh_button.Visible = $false
    $erefresh_button.Add_Click({EditUserWidget-ReloadUserList})
    $Euser_Form.Controls.Add($erefresh_button)

    $edisable_button = New-Object Windows.Forms.Button
    $edisable_button.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $edisable_button.Text = "Disable"
    $edisable_button.size = New-Object System.Drawing.Size(120, 35)
    $edisable_button.location = New-Object System.Drawing.Size(540, 5) #415
    $edisable_button.Visible = $true   
    $Euser_Form.Controls.Add($edisable_button)

    # NEW COMPONENTS


    $Euser_Form.Add_Shown({$Euser_Form.Activate()})
    [void] $Euser_Form.ShowDialog()
}
Function Main 
{
    [void] [System.Reflection.Assembly]::LoadWithPartialName("System.Drawing")
    [void] [System.Reflection.Assembly]::LoadWithPartialName("System.Windows.Forms")

    $UMTK_Form = New-Object System.Windows.Forms.Form
    $UMTK_Form.Text = "User Maintenance Tool Kit"
    $UMTK_Form.Size = New-Object System.Drawing.Size(600, 600)
    $UMTK_Form.minimumSize = New-Object System.Drawing.Size(600,600) 
    $UMTK_Form.maximumSize = New-Object System.Drawing.Size(600,600) 
    $UMTK_Form.FormBorderStyle = 'Fixed3D'
    $UMTK_Form.StartPosition = 'CenterScreen'

    $duser_button = New-Object System.Windows.Forms.Button 
    $duser_button.Size = New-Object System.Drawing.Size(120, 30)
    $duser_button.Location = New-Object System.Drawing.Size(230, 250)
    $duser_button.Text = "Domain User"
    $duser_button.Add_Click({DomainUserWidget})
    $UMTK_Form.Controls.Add($duser_button)

    $luser_button = New-Object System.Windows.Forms.Button 
    $luser_button.Size = New-Object System.Drawing.Size(120, 30)
    $luser_button.Location = New-Object System.Drawing.Size(230, 280)
    $luser_button.Text = "Local User"
    $luser_button.Add_Click({LocalUserWidget})
    $UMTK_Form.Controls.Add($luser_button)

    $euser_button = New-Object System.Windows.Forms.Button 
    $euser_button.Size = New-Object System.Drawing.Size(120, 30)
    $euser_button.Location = New-Object System.Drawing.Size(230, 310)
    $euser_button.Text = "Edit User"
    $euser_button.Add_Click({EditUserWidget})
    $UMTK_Form.Controls.Add($euser_button)

    $exit_button = New-Object System.Windows.Forms.Button 
    $exit_button.Size = New-Object System.Drawing.Size(120, 30)
    $exit_button.Location = New-Object System.Drawing.Size(230, 340)
    $exit_button.Text = "Exit"
    $exit_button.Add_Click({$UMTK_Form.Add_FormClosing({$_.Cancel=$false});$UMTK_Form.Close()})   
    $UMTK_Form.Controls.Add($exit_button)

    $UMTK_Form.Add_Shown({$UMTK_Form.Activate()})
    [void] $UMTK_Form.ShowDialog()
}

Main