[System.Reflection.Assembly]::LoadWithPartialName("System.Windows.Forms")


Function DomainUser 
{
    [void] [System.Reflection.Assembly]::LoadWithPartialName("System.Drawing")
    [void] [System.Reflection.Assembly]::LoadWithPartialName("System.Windows.Forms")

    $Domain_Form = New-Object System.Windows.Forms.Form
    $Domain_Form.Text = "UMTK: Domain User Creation"
    $Domain_Form.Size = New-Object System.Drawing.Size(600, 600)
    $Domain_Form.minimumSize = New-Object System.Drawing.Size(600,600) 
    $Domain_Form.maximumSize = New-Object System.Drawing.Size(600,600) 
    $Domain_Form.FormBorderStyle = 'Fixed3D'
    $Domain_Form.StartPosition = 'CenterScreen'

    $menu_bar = New-Object System.Windows.Forms.MenuStrip
    $menu_tool_strip = New-Object System.Windows.Forms.ToolStrip
    $Domain_Form.MainMenuStrip = $menu_bar
    
    # FILE MENU ITEM
    $file_action = New-Object System.Windows.Forms.ToolStripMenuItem
    $file_action.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $file_action.Text = "File"
    # save command 
    $save_command = New-Object System.Windows.Forms.ToolStripButton
    $save_command.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $save_command.Text = "Save Command"

    # Close program 
    $close_program = New-Object System.Windows.Forms.ToolStripButton
    $close_program.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $close_program.Text = "Close Program"
    
    # EDIT MENU ITEM
    $edit_action = New-Object System.Windows.Forms.ToolStripMenuItem
    $edit_action.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $edit_action.Text = "Edit"
    

    # SETTINGS MENU ITEM
    $settings_action = New-Object System.Windows.Forms.ToolStripMenuItem
    $settings_Action.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $settings_action.Text = "Settings"
    # Set additional proxy 
    $add_proxies = New-Object System.Windows.Forms.ToolStripButton
    $add_proxies.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $add_proxies.Text = "Set Aditional proxy"
    $proxies_clicked = 0
    $add_proxies.Add_Click({AreProxies-Hidden})

    # set display name
    $add_display_name = New-Object System.Windows.Forms.ToolStripButton
    $add_display_name.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $add_display_name.Text = "Set Display Name"
    $displayname_clicked = 0
    $add_display_name.Add_Click({ if($displayname_clicked -eq 0) {Show-AddDisplayname} elseif($displayname_clicked -eq 1) {Hide-AddDisplayname} })

    # HELP MENU ITEM
    $help_action = New-Object System.Windows.Forms.ToolStripMenuItem
    $help_Action.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $help_action.Text = "Help"

    # Help function
    $save_me = New-Object System.Windows.Forms.ToolStripButton
    $save_me.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $save_me.Text = "Help"    

    # Add items to menu bar
    $menu_bar.Items.Add($file_action)
    $menu_bar.Items.Add($edit_action)
    $menu_bar.Items.Add($settings_action)
    $menu_bar.Items.Add($help_action)

    # Add submenu items
    $file_action.DropDownItems.Add($save_command)
    $file_action.DropDownItems.Add($close_program)

    $settings_action.DropDownItems.Add($add_proxies)
    $settings_action.DropDownItems.Add($add_display_name)

    $help_action.DropDownItems.Add($save_me)

    # Add menu to form
    $Domain_form.Controls.Add($menu_tool_strip)
    $Domain_Form.Controls.Add($menu_bar)

    $ou_combo = New-Object Windows.Forms.ComboBox 
    $ou_combo.size = New-Object System.Drawing.Size(350, 150)
    $ou_combo.location = New-Object System.Drawing.Size(150, 55)
    $ou_combo.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $clean_ous = Get-ADOrganizationalUnit -Filter * | Select-Object -ExpandProperty Name
    Foreach($ou in $clean_ous)
    {
        $ou_combo.Items.Add($ou)
    }
    $Domain_Form.Controls.Add($ou_combo)


    $ou_combo_label = New-Object Windows.Forms.Label
    $ou_combo_label.size = New-Object System.Drawing.Size(150, 35)
    $ou_combo_label.location = New-Object System.Drawing.Size(0, 55)
    $ou_combo_label.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $ou_combo_label.text = "OU:"
    $Domain_Form.Controls.Add($ou_combo_label)

   
    $users_combo = New-Object Windows.Forms.ComboBox 
    $users_combo.size = New-Object System.Drawing.Size(350, 150)
    $users_combo.location = New-Object System.Drawing.Size(150, 90)
    $users_combo.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $pull_users = Get-ADUser -Filter * | Select-Object -ExpandProperty Name
    Foreach($usr in $pull_users)
    {
        $users_combo.Items.Add($usr)
    }
    $Domain_Form.Controls.Add($users_combo)


    $users_combo_label = New-Object Windows.Forms.Label
    $users_combo_label.size = New-Object System.Drawing.Size(150, 35)
    $users_combo_label.location = New-Object System.Drawing.Size(0, 90)
    $users_combo_label.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $users_combo_label.text = "Template:"
    $Domain_Form.Controls.Add($users_combo_label)



    $special_combo = New-Object Windows.Forms.ComboBox 
    $special_combo.size = New-Object System.Drawing.Size(350, 150)
    $special_combo.location = New-Object System.Drawing.Size(150, 125)
    $special_combo.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
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


    $special_combo_label = New-Object Windows.Forms.Label
    $special_combo_label.size = New-Object System.Drawing.Size(150, 35)
    $special_combo_label.location = New-Object System.Drawing.Size(0, 125)
    $special_combo_label.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $special_combo_label.text = "Doamin\UPN:"
    $Domain_Form.Controls.Add($special_combo_label)


    $employee_name_label = New-Object Windows.Forms.Label
    $employee_name_label.size = New-Object System.Drawing.Size(150, 35)
    $employee_name_label.location = New-Object System.Drawing.Size(0, 160)
    $employee_name_label.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $employee_name_label.text = "Employee name:"
    $Domain_Form.Controls.Add($employee_name_label)


    $employee_name_input = New-Object Windows.Forms.TextBox
    $employee_name_input.size = New-Object System.Drawing.Size(350, 75)
    $employee_name_input.location = New-Object System.Drawing.Size(150, 160)
    $employee_name_input.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $Domain_Form.Controls.Add($employee_name_input)


    $username_label = New-Object Windows.Forms.Label
    $username_label.size = New-Object System.Drawing.Size(150, 35)
    $username_label.location = New-Object System.Drawing.Size(0, 195)
    $username_label.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $username_label.text = "Username:"
    $Domain_Form.Controls.Add($username_label)


    $username_input = New-Object Windows.Forms.TextBox
    $username_input.size = New-Object System.Drawing.Size(350, 75)
    $username_input.location = New-Object System.Drawing.Size(150, 195)
    $username_input.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $Domain_Form.Controls.Add($username_input)


    $password_label = New-Object Windows.Forms.Label
    $password_label.size = New-Object System.Drawing.Size(150, 35)
    $password_label.location = New-Object System.Drawing.Size(0, 230)
    $password_label.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $password_label.text = "Password:"
    $Domain_Form.Controls.Add($password_label)


    $password_input = New-Object Windows.Forms.TextBox
    $password_input.size = New-Object System.Drawing.Size(350, 75)
    $password_input.location = New-Object System.Drawing.Size(150, 230)
    $password_input.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $Domain_Form.Controls.Add($password_input)

    $email_label = New-Object Windows.Forms.Label
    $email_label.size = New-Object System.Drawing.Size(150, 35)
    $email_label.location = New-Object System.Drawing.Size(0, 265)
    $email_label.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $email_label.text = "Email address:"
    $Domain_Form.Controls.Add($email_label)


    $email_input = New-Object Windows.Forms.TextBox
    $email_input.size = New-Object System.Drawing.Size(350, 75)
    $email_input.location = New-Object System.Drawing.Size(150, 265)
    $email_input.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $Domain_Form.Controls.Add($email_input)


    $primary_proxy_label = New-Object Windows.Forms.Label
    $primary_proxy_label.size = New-Object System.Drawing.Size(150, 35)
    $primary_proxy_label.location = New-Object System.Drawing.Size(0, 300)
    $primary_proxy_label.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $primary_proxy_label.text = "Primary Proxy:"
    $primary_proxy_label.Visible = $false
    $Domain_Form.Controls.Add($primary_proxy_label)



    $primary_proxy_input = New-Object Windows.Forms.TextBox
    $primary_proxy_input.size = New-Object System.Drawing.Size(350, 75)
    $primary_proxy_input.location = New-Object System.Drawing.Size(150, 300)
    $primary_proxy_input.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $primary_proxy_input.Visible = $false
    $Domain_Form.Controls.Add($primary_proxy_input)


    $secondary_proxy_label = New-Object Windows.Forms.Label
    $secondary_proxy_label.size = New-Object System.Drawing.Size(150, 35)
    $secondary_proxy_label.location = New-Object System.Drawing.Size(0, 335)
    $secondary_proxy_label.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $secondary_proxy_label.text = "Secondary Proxy:"
    $secondary_proxy_label.Visible = $false
    $Domain_Form.Controls.Add($secondary_proxy_label)



    $secondary_proxy_input = New-Object Windows.Forms.TextBox
    $secondary_proxy_input.size = New-Object System.Drawing.Size(350, 75)
    $secondary_proxy_input.location = New-Object System.Drawing.Size(150, 335)
    $secondary_proxy_input.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $secondary_proxy_input.Visible = $false
    $Domain_Form.Controls.Add($secondary_proxy_input)
    

    $displayname_label = New-Object Windows.Forms.Label
    $displayname_label.size = New-Object System.Drawing.Size(150, 35)
    $displayname_label.location = New-Object System.Drawing.Size(0, 365)
    $displayname_label.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $displayname_label.text = "Display name:"
    $displayname_label.Visible = $false
    $Domain_Form.Controls.Add($displayname_label)


    $displayname_input = New-Object Windows.Forms.TextBox
    $displayname_input.size = New-Object System.Drawing.Size(350, 75)
    $displayname_input.location = New-Object System.Drawing.Size(150, 365)
    $displayname_input.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $displayname_input.Visible = $false
    $Domain_Form.Controls.Add($displayname_input)


    $create_button = New-Object Windows.Forms.Button
    $create_button.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $create_button.Text = "Create"
    $create_button.size = New-Object System.Drawing.Size(120, 30)
    $create_button.location = New-Object System.Drawing.Size(150, 395)
    $create_button.Add_Click({CreateDomainUser -_ou $ou_combo.SelectedIndex -_template $users_combo.Text -_upn $special_combo.Text -_fullname $employee_name_input.Text -_username $username_input.Text -_password $password_input.Text -_email $email_input.Text})
    $Domain_Form.Controls.Add($create_button)


    $close_button = New-Object Windows.Forms.Button
    $close_button.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $close_button.Text = "Close"
    $close_button.size = New-Object System.Drawing.Size(120, 30)
    $close_button.location = New-Object System.Drawing.Size(310, 395)
    $close_button.Add_Click({$Domain_Form.Add_FormClosing({$_.Cancel=$false});$Domain_Form.Close()})   
    $Domain_Form.Controls.Add($close_button)

    $message_label = New-Object Windows.Forms.Label
    $message_label.size = New-Object System.Drawing.Size(350, 50)
    $message_label.location = New-Object System.Drawing.Size(150, 510) # was 410
    $message_label.Font = New-Object System.Drawing.Font("Courier",8,[System.Drawing.FontStyle]::Regular)
    $Domain_Form.Controls.Add($message_label)

    $Domain_Form.Add_Shown({$Domain_Form.Activate()})
    [void] $Domain_Form.ShowDialog()
}


Function CreateDomainUser
{
    param (
        [int]$_ou,
        [string]$_template,
        [string]$_upn,
        [string]$_fullname,
        [string]$_username, 
        [string]$_password,
        [string]$_email
    )

    if($primary_proxy_input.Text.Length -eq 0)
    {
        $pprox = "SMTP:" + $_email
    }
    elseif($primary_proxy_input.Text.Length -ge 1)
    {
        $pprox = "SMTP:" + $primary_proxy_input.Text
    }
    if($secondary_proxy_input.Text.Length -ge 1)
    {
        $secprox = "smtp:" + $secondary_proxy_input.Text
    }

    if($displayname_input.Text.Length -eq 0)
    {
        $_displayname = $_fullname
    }
    elseif($displayname_input.Text.Length -ge 1)
    {
        $_displayname = $displayname_input.Text
    }

    if($_ou -gt 0 -and $_template.Count -gt 0 -and $_upn.Count -gt 0 -and $_fullname.Count -gt 0 -and $_username -gt 0 -and $_password.Count -gt 0 -and $_email -gt 0) 
    {
         $user_exists = DoesUser-Exist $_username
         if($user_exists -eq 1)
         {
             $message_label.ForeColor = "Red"
             $message_label.Text =  "An account with the username: " + $_username + " already exists, please try again..."
         }
         elseif($user_exists -eq 0)
         { 
             $userpname = $_username + "@" + $_upn
             $_given, $_surname = $_fullname.Split(' ')
             $tmp_pass = $_password | ConvertTo-SecureString -AsPlainText -Force
             New-ADUser -Name $_fullname -GivenName $_given -Surname $_surname -AccountPassword $tmp_pass -UserPrincipalName $userpname -displayName $_displayname  -EmailAddress $_email -SamAccountName $_username  -Enabled 1
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
             $distinguished_ous = (Get-ADOrganizationalUnit -Filter * | Select-Object -ExpandProperty Distinguishedname)
             $tuser = (Get-ADUser -Filter {samAccountName -like $_username} | Select-Object -ExpandProperty DistinguishedName) 
             Move-ADObject -Identity $tuser -TargetPath $distinguished_ous[$_ou]

             $user_exists = DoesUser-Exist $_username
             if($user_exists -eq 0)
             {
                $message_label.ForeColor = "Red"
                $message_label.Text = "Something very terrible has happened. Gross, what have you done?!"
             }
             elseif($user_exists -eq 1)
             {
                $message_label.ForeColor = "Green"
                $message_label.Text =  "The user account for " + $_fullname + " has been created..."
             }
         }
    }
}


Function DoesUser-Exist
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

Function Hide-AddProxies
{
    $displayname_clicked = 0
    $displayname_label.Visible = $false
    $displayname_input.Visible = $false
}


Function Show-AddDisplayname
{
    $displayname_clicked = 1
    $displayname_label.Visible = $true
    $displayname_input.Visible = $true
}


Function Hide-AddDisplayname 
{

}


Function Is-DisplaynameNull 
{
    if($displayname_input.Text.Length -eq 0)
    {
        Write-Host "1"
        return 1
    }
    elseif($displayname_input.Text.Length -ge 1)
    {
        Write-Host "0"
        return 0
    }
}


Function LocalUser 
{
    Write-Host "Local User"
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