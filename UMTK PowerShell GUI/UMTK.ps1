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

    $pass_eval = Validate-Password -password $_password # Should == 1
    $user_leng = Validate-UserLength -uname $_username # Should == 1
    $user_exists = Validate-DomainUser -username $_username # Should == 0
    $email_eval = Validate-Email -emailaddress $_email # Should == 1
    
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
            # $distinguished_ous = (Get-ADOrganizationalUnit -Filter * | Select-Object -ExpandProperty Distinguishedname) can be uncommented if the experimental shit works.
            $tuser = (Get-ADUser -Filter {samAccountName -like $_username} | Select-Object -ExpandProperty DistinguishedName) 
            
            if($_cleaned_ous -eq 1)
            {
                Move-ADObject -Identity $tuser -TargetPath $distinguished_ous[$_ou]
            }
            elsesif($_cleaned_ous -eq 0)
            {
                Move-ADObject -Identity $tuser -TargetPath $distinguished_ous[$_ou]
            }
            
            
            $message_label.ForeColor = "Green"
            $message_label.Text =  "Success: The user account for " + $_fullname + " has been created and can be found in: " + $distinguished_ous[$_ou] 

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
            $lmessage_label.Text = "The user Account for " + $lusername + " has been created, a file has been created and can be found at " + $path_var + "<username.html> please take this file and present it to the user. Once transfered, delete this file."
        } 
        elseif($admin_button.Checked -eq $false)
        {
            $path_var = Validate-Path 
            Dump-UserForm -username $lusername -password $lpassword -path $path_var -is_domain 0 -local_administrator 0
            $lmessage_label.ForeColor = "Green"
            $lmessage_label.Text = "The user Account for " + $lusername + " has been created, a file has been created and can be found at " + $path_var + "<username.html>  please take this file and present it to the user. Once transfered, delete this file."
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
    if($test.Name -ne $null)
    {
        return 1    
    }
    elseif($test.Name -eq $null)
    {    
        return 0
    }
    # This was frustrating for me, I wanted to redirect/supress the error differently but I couldn't https://github.com/PowerShell/PowerShell/issues/11133     
    # It's messed up because the purpose of the function is to check to see if the local user exists. The problem is that if the local user does not exist
    # it throws a UserNotFoundException. It still returns correctly and allows you to carry on but that's not the point. So, instead of fighting I just 
    # added -ErrorAction SilentlyContinue which does supress it from view.
}

Function Validate-DomainUser
{
    param (
        [parameter(Mandatory = $true)]
        [string]$username
    )
    $test = (Get-ADUser -Filter {SamAccountName -like $username})
    if($test -ne $null)
    {
        return 1
    }
    elseif($test -eq $null)
    {
        return 0
    }
    # Very straightforward boolean check:
    #    If the username (SamAccountName) supplied is found in AD 
    #        Return 1 for true 
    #    If the username (SamAccountName) supplied is not found in AD
    #        Return 0 for false
    # If this function returns a 1 then a user with that name already exists and the user should not be created.
    # If this function returns a 0 then the user does not alread yexist and can be created.
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
    # If secure return 1
    # If not secure return 0
    # Pretty straightforward forcing 1 Capital letter, one lower case letter, 1 number, 1 symbol and it has an 8 character minimum requirement
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
    # This may seem a little strange to folks but:
    #    If the length of $uname is -lt 3 return false (as in not valid)
    #    If the length of $uname is -gt 3 return true (as in valid)
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
    # See https://docs.microsoft.com/en-us/dotnet/api/system.net.mail.mailaddress?view=net-5.0
}

Function Validate-Path 
{
   $t = (Get-ItemProperty -Path 'HKCU:\Software\Microsoft\Windows\CurrentVersion\Explorer\User Shell Folders\' -Name Desktop | Select-Object Desktop -ErrorAction SilentlyContinue)
   if($t.Desktop -ne $null)
   {     
       $str = $t.Desktop.toString() + "\"
       return $str
   }
   else
   {
       $str = $ENV:USERPROFILE + "\Downloads\"
       return $str
   }
   # This one had originally been part of the Dump-UserForm message but I moved it into it's own unique function. 
   # When called it attempts to locate the listed registry key and find the Desktop dword value. This value should, 
   # if it exists, contain the folder redirection path of the user. If it does not exist (null) we just shift the path
   # to the users downloads folder using the USERPROFILE environment variable.
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
    
    # Well this one was fun. I needed a way to turn this "OU=Administrators,OU=Elysium,DC=Elysium,DC=local" into this Elysium - Administrators
    # It really needs to be built on and improved but it does the job it was made to do which is identify DC's with multi-client OUs that have 
    # different UPNs tied to them.
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
        $wstring = "
                    <h1><center>Below this line is a print out of the information that you provided to us, it's our acknowledgement that we not only received but carried out the work that you requested.</center></h1>                    
                    <body><h5>Employee Name: " + $employee + "</h5><h5>Username: " + $username + "</h5><h5>Email Address: " + $email + "</h5><h5>Password: " + $password + "</h5><br>The user was placed into the following groups using the template user " + "(" + $template + ") that you provided " + $membership + "
                    We placed the user into the same Orginizational Unit as the template user that you provided (" + $template + ") " + $OU + ".<br><br><center>If there are issues with users not receiving the appropriate access or required items to complete their
                    day to day jobs please provide us with a thorough list containing what they need. Please also understand that this is what the template user is intended to be used for. Using another user account with all of the appropriate access 
                    levels prevents mistakes that cause productivity issues!</center> 
                   "
        $path = $path + $username + ".html"
        $wstring | Out-File -FilePath $path
    }
    elseif($is_domain -eq 0)
    {
       if($local_administrator -eq 1)
       {
           $wstring = "
                        <h1><center>Below this line is a print out of the information that you provided to us. It's our acknowledgement that we not only received but carried out the work that you requested.</center></h1>                    
                        <body><h5>Username: " + $username + "</h5><h5>Password: " + $password + "</h5><br>The user was given local administrator access per your request; to the local computer (individual mahcine) that you specified.
                      "
           $path = $path + $username + ".html"
           $wstring | Out-File -FilePath $path
       }
       elseif($local_administrator -eq 0)
       {
           $wstring = "
                        <h1><center>Below this line is a print out of the information that you provided to us. It's our acknowledgement that we not only received but also carried out the work that you requested.</center></h1>                    
                        <body><h5>Username: " + $username + "</h5><h5>Password: " + $password + "</h5> This user has been created and granted access to the local (individual machine) that you specified.
                      "
            $path = $path + $username + ".html"
            $wstring | Out-File -FilePath $path
       }
    }
}

Function AreProxies-Hidden
{
    if($proxies_clicked -eq 1)
    {
        Hide-AddProxies
    }
    elseif($proxies_clicked -eq 0)
    {
       Show-AddProxies
    }
}

Function Show-AddProxies
{
    $proxies_clicked = 1
    $primary_proxy_label.Visible = $true
    $primary_proxy_input.Visible = $true
    $secondary_proxy_label.Visible = $true
    $secondary_proxy_input.Visible = $true
}

Function Show-AddDisplayname
{
    $displayname_clicked = 1
    $displayname_label.Visible = $true
    $displayname_input.Visible = $true
}

Function Is-DisplaynameNull 
{
    if($displayname_input.Text.Length -eq 0)
    {   
        return 1
    }
    elseif($displayname_input.Text.Length -ge 1)
    {
        return 0
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

Function DomainUser 
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

    $menu_bar = New-Object System.Windows.Forms.MenuStrip
    $menu_tool_strip = New-Object System.Windows.Forms.ToolStrip
    $Domain_Form.MainMenuStrip = $menu_bar
    
    # FILE MENU ITEM
    $file_action = New-Object System.Windows.Forms.ToolStripMenuItem
    $file_action.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $file_action.Text = "File"
    # save command 
    $save_command = New-Object System.Windows.Forms.ToolStripButton
    $save_command.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $save_command.Text = "Save Command"

    # Close program 
    $close_program = New-Object System.Windows.Forms.ToolStripButton
    $close_program.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $close_program.Text = "Close Program"
    
    # EDIT MENU ITEM
    $edit_action = New-Object System.Windows.Forms.ToolStripMenuItem
    $edit_action.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $edit_action.Text = "Edit"

    # SETTINGS MENU ITEM
    $settings_action = New-Object System.Windows.Forms.ToolStripMenuItem
    $settings_Action.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $settings_action.Text = "Settings"
    # Set additional proxy 
    $add_proxies = New-Object System.Windows.Forms.ToolStripButton
    $add_proxies.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $add_proxies.Text = "Set Aditional proxy"
    $proxies_clicked = 0
    $add_proxies.Add_Click({AreProxies-Hidden})

    # set display name
    $add_display_name = New-Object System.Windows.Forms.ToolStripButton
    $add_display_name.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $add_display_name.Text = "Set Display Name"
    $displayname_clicked = 0
    $add_display_name.Add_Click({ if($displayname_clicked -eq 0) {Show-AddDisplayname} elseif($displayname_clicked -eq 1) {Hide-AddDisplayname} })

    # Help function
    $save_me = New-Object System.Windows.Forms.ToolStripButton
    $save_me.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $save_me.Text = "Help"    

    # Add items to menu bar
    $menu_bar.Items.Add($file_action)
    $menu_bar.Items.Add($edit_action)
    $menu_bar.Items.Add($settings_action)

    # Add submenu items
    $file_action.DropDownItems.Add($save_command)
    $file_action.DropDownItems.Add($close_program)

    $settings_action.DropDownItems.Add($add_proxies)
    $settings_action.DropDownItems.Add($add_display_name)

    # Add menu to form
    $Domain_form.Controls.Add($menu_tool_strip)
    $Domain_Form.Controls.Add($menu_bar)

    $users_combo = New-Object Windows.Forms.ComboBox 
    $users_combo.size = New-Object System.Drawing.Size(350, 150)
    $users_combo.location = New-Object System.Drawing.Size(205, 110)
    $users_combo.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $pull_users = Get-ADUser -Filter * | Select-Object -ExpandProperty Name
    Foreach($usr in $pull_users)
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
    $Domain_Form.Controls.Add($special_combo)

    $ou_combo = New-Object Windows.Forms.ComboBox 
    $ou_combo.size = New-Object System.Drawing.Size(350, 150)
    $ou_combo.location = New-Object System.Drawing.Size(205, 75)
    $ou_combo.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $clean_ous = Get-ADOrganizationalUnit -Filter * | Select-Object -ExpandProperty Name
    $distinguished_ous = Get-ADOrganizationalUnit -Filter * | Select-Object -ExpandProperty distinguishedName
    #TEST CODE - EXPERIMENTAL
    if($upn_lookup.Count -gt 2) # 2
    {
        $are_ous_cleaned = 0 
        Foreach($ou in $distinguished_ous)
        {
            $adjusted = Validate-DistinguishedNames -ou $ou -index 0
            $ou_combo.Items.Add($adjusted)       
        }
    }
    elseif($upn_lookup.Count -le 2) # 2
    {    
         $are_ous_cleaned = 1
         Foreach($ou in $clean_ous)
        {
            $ou_combo.Items.Add($ou)
        }
    }
    #TEST CODE - EXPERIMENTAL
    $Domain_Form.Controls.Add($ou_combo)

    $ou_combo_label = New-Object Windows.Forms.Label
    $ou_combo_label.size = New-Object System.Drawing.Size(220, 35)
    $ou_combo_label.location = New-Object System.Drawing.Size(0, 75)
    $ou_combo_label.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $ou_combo_label.text = "Orginizational unit:"
    $Domain_Form.Controls.Add($ou_combo_label)


    $special_combo_label = New-Object Windows.Forms.Label
    $special_combo_label.size = New-Object System.Drawing.Size(200, 35)
    $special_combo_label.location = New-Object System.Drawing.Size(0, 145)
    $special_combo_label.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $special_combo_label.text = "Doamin or UPN:"
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
    $Domain_Form.Controls.Add($username_label)

    $username_input = New-Object Windows.Forms.TextBox
    $username_input.size = New-Object System.Drawing.Size(350, 75)
    $username_input.location = New-Object System.Drawing.Size(205, 215)
    $username_input.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $Domain_Form.Controls.Add($username_input)

    $password_label = New-Object Windows.Forms.Label
    $password_label.size = New-Object System.Drawing.Size(200, 35)
    $password_label.location = New-Object System.Drawing.Size(0, 250)
    $password_label.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $password_label.text = "Password:"
    $Domain_Form.Controls.Add($password_label)

    $password_input = New-Object Windows.Forms.TextBox
    $password_input.size = New-Object System.Drawing.Size(350, 75)
    $password_input.location = New-Object System.Drawing.Size(205, 250)
    $password_input.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $Domain_Form.Controls.Add($password_input)

    $email_label = New-Object Windows.Forms.Label
    $email_label.size = New-Object System.Drawing.Size(200, 35)
    $email_label.location = New-Object System.Drawing.Size(0, 285)
    $email_label.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $email_label.text = "Email address:"
    $Domain_Form.Controls.Add($email_label)

    $email_input = New-Object Windows.Forms.TextBox
    $email_input.size = New-Object System.Drawing.Size(350, 75)
    $email_input.location = New-Object System.Drawing.Size(205, 285)
    $email_input.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
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
    $create_button.location = New-Object System.Drawing.Size(205, 415)
    $create_button.Add_Click({CreateDomainUser -_ou $ou_combo.SelectedIndex -_template $users_combo.Text -_upn $special_combo.Text -_fullname $employee_name_input.Text -_username $username_input.Text -_password $password_input.Text -_email $email_input.Text})
    $Domain_Form.Controls.Add($create_button)

    $close_button = New-Object Windows.Forms.Button
    $close_button.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $close_button.Text = "Close"
    $close_button.size = New-Object System.Drawing.Size(120, 30)
    $close_button.location = New-Object System.Drawing.Size(435, 415)
    $close_button.Add_Click({$Domain_Form.Add_FormClosing({$_.Cancel=$false});$Domain_Form.Close()})   
    $Domain_Form.Controls.Add($close_button)

    $message_label = New-Object Windows.Forms.Label
    $message_label.size = New-Object System.Drawing.Size(700, 180)
    $message_label.location = New-Object System.Drawing.Size(0, 445) # was 410
    $message_label.Font = New-Object System.Drawing.Font("Courier",12,[System.Drawing.FontStyle]::Regular)
    $Domain_Form.Controls.Add($message_label)

    $Domain_Form.Add_Shown({$Domain_Form.Activate()})
    [void] $Domain_Form.ShowDialog()
}

Function LocalUser 
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

Function EditUser 
{
    Write-Host "Edit User"
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
    $duser_button.Add_Click({DomainUser})
    $UMTK_Form.Controls.Add($duser_button)

    $luser_button = New-Object System.Windows.Forms.Button 
    $luser_button.Size = New-Object System.Drawing.Size(120, 30)
    $luser_button.Location = New-Object System.Drawing.Size(230, 280)
    $luser_button.Text = "Local User"
    $luser_button.Add_Click({LocalUser})
    $UMTK_Form.Controls.Add($luser_button)

    $euser_button = New-Object System.Windows.Forms.Button 
    $euser_button.Size = New-Object System.Drawing.Size(120, 30)
    $euser_button.Location = New-Object System.Drawing.Size(230, 310)
    $euser_button.Text = "Edit User"
    $euser_button.Add_Click({EditUser})
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