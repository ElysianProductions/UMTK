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
    $file_action.Text = "File"
    # save command 
    $save_command = New-Object System.Windows.Forms.ToolStripButton
    $save_command.Text = "Save Command"
    # save to file
    $save_command_file = New-Object System.Windows.Forms.ToolStripButton
    $save_command_file.Text = "Save to File" 
    # Close program 
    $close_program = New-Object System.Windows.Forms.ToolStripButton
    $close_program.Text = "Close Program"
    
    # EDIT MENU ITEM
    $edit_action = New-Object System.Windows.Forms.ToolStripMenuItem
    $edit_action.Text = "Edit"
    

    # SETTINGS MENU ITEM
    $settings_action = New-Object System.Windows.Forms.ToolStripMenuItem
    $settings_action.Text = "Settings"
    # Set additional proxy 
    $add_proxies= New-Object System.Windows.Forms.ToolStripButton
    $add_proxies.Text = "Set Aditional proxy"

    # set display name
    $add_display_name = New-Object System.Windows.Forms.ToolStripButton
    $add_display_name.Text = "Set Display Name"

    # HELP MENU ITEM
    $help_action = New-Object System.Windows.Forms.ToolStripMenuItem
    $help_action.Text = "Help"

    # Help function
    $save_me = New-Object System.Windows.Forms.ToolStripButton
    $save_me.Text = "Help"    

    # Add items to menu bar
    $menu_bar.Items.Add($file_action)
    $menu_bar.Items.Add($edit_action)
    $menu_bar.Items.Add($settings_action)
    $menu_bar.Items.Add($help_action)

    # Add submenu items
    $file_action.DropDownItems.Add($save_command)
    $file_action.DropDownItems.Add($save_command_file)
    $file_action.DropDownItems.Add($close_program)

    $settings_action.DropDownItems.Add($add_proxies)
    $settings_action.DropDownItems.Add($add_display_name)

    $help_action.DropDownItems.Add($save_me)

    # Add menu to form
    $Domain_form.Controls.Add($menu_tool_strip)
    $Domain_Form.Controls.Add($menu_bar)

    $ou_combo = New-Object Windows.Forms.ComboBox 
    $ou_combo.size = New-Object System.Drawing.Size(350, 150)
    $ou_combo.location = New-Object System.Drawing.Size(150, 70)
    $clean_ous = Get-ADOrganizationalUnit -Filter * | Select-Object -ExpandProperty Name
    Foreach($ou in $clean_ous)
    {
        $ou_combo.Items.Add($ou)
    }
    
    #
    $Domain_Form.Controls.Add($ou_combo)

   
    $users_combo = New-Object Windows.Forms.ComboBox 
    $users_combo.size = New-Object System.Drawing.Size(350, 150)
    $users_combo.location = New-Object System.Drawing.Size(150, 100)
    $pull_users = Get-ADUser -Filter * | Select-Object -ExpandProperty Name
    Foreach($usr in $pull_users)
    {
        $users_combo.Items.Add($usr)
    }
    
    #
    $Domain_Form.Controls.Add($users_combo)


     #special combo box
    $special_combo = New-Object Windows.Forms.ComboBox 
    $special_combo.size = New-Object System.Drawing.Size(350, 150)
    $special_combo.location = New-Object System.Drawing.Size(150, 130)
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


    $employee_name_label = New-Object Windows.Forms.Label
    $employee_name_label.size = New-Object System.Drawing.Size(150, 50)
    $employee_name_label.location = New-Object System.Drawing.Size(0, 160)
    $employee_name_label.text = "Employee name:"
    $Domain_Form.Controls.Add($employee_name_label)


    $employee_name_input = New-Object Windows.Forms.TextBox
    $employee_name_input.size = New-Object System.Drawing.Size(350, 75)
    $employee_name_input.location = New-Object System.Drawing.Size(150, 160)
    $Domain_Form.Controls.Add($employee_name_input)


    $username_label = New-Object Windows.Forms.Label
    $username_label.size = New-Object System.Drawing.Size(150, 50)
    $username_label.location = New-Object System.Drawing.Size(0, 210)
    $username_label.text = "Username:"
    $Domain_Form.Controls.Add($username_label)


    $username_input = New-Object Windows.Forms.TextBox
    $username_input.size = New-Object System.Drawing.Size(350, 75)
    $username_input.location = New-Object System.Drawing.Size(150, 210)
    $Domain_Form.Controls.Add($username_input)


    $password_label = New-Object Windows.Forms.Label
    $password_label.size = New-Object System.Drawing.Size(150, 50)
    $password_label.location = New-Object System.Drawing.Size(0, 260)
    $password_label.text = "Password:"
    $Domain_Form.Controls.Add($password_label)


    $password_input = New-Object Windows.Forms.TextBox
    $password_input.size = New-Object System.Drawing.Size(350, 75)
    $password_input.location = New-Object System.Drawing.Size(150, 260)
    $Domain_Form.Controls.Add($password_input)

    $email_label = New-Object Windows.Forms.Label
    $email_label.size = New-Object System.Drawing.Size(150, 50)
    $email_label.location = New-Object System.Drawing.Size(0, 310)
    $email_label.text = "Email address:"
    $Domain_Form.Controls.Add($email_label)


    $email_input = New-Object Windows.Forms.TextBox
    $email_input.size = New-Object System.Drawing.Size(350, 75)
    $email_input.location = New-Object System.Drawing.Size(150, 310)
    $Domain_Form.Controls.Add($email_input)


    $create_button = New-Object Windows.Forms.Button
    $create_button.Text = "Create"
    $create_button.size = New-Object System.Drawing.Size(120, 30)
    $create_button.location = New-Object System.Drawing.Size(150, 360)
    $create_button.Add_Click({CreateDomainUser -_ou $ou_combo.SelectedIndex -_template $users_combo.Text -_upn $special_combo.Text -_fullname $employee_name_input.Text -_username $username_input.Text -_password $password_input.Text -_email $email_input.Text})
    $Domain_Form.Controls.Add($create_button)


    $close_button = New-Object Windows.Forms.Button
    $close_button.Text = "Close"
    $close_button.size = New-Object System.Drawing.Size(120, 30)
    $close_button.location = New-Object System.Drawing.Size(310, 360)
    $close_button.Add_Click({$Domain_Form.Add_FormClosing({$_.Cancel=$false});$Domain_Form.Close()})   
    $Domain_Form.Controls.Add($close_button)

    $message_label = New-Object Windows.Forms.Label
    $message_label.size = New-Object System.Drawing.Size(350, 50)
    $message_label.location = New-Object System.Drawing.Size(150, 410)
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
        [string]$_email,
        [string]$_displayname,
        [array[]]$_proxies
    )
    if($_ou -gt 0 -and $_template.Count -gt 0 -and $_upn.Count -gt 0 -and $_fullname.Count -gt 0 -and $_username -gt 0 -and $_password.Count -gt 0 -and $_email -gt 0) 
    {
        if($_displayname.Count -le 0)
        {
            $_displayname = $_fullname
        }
        if($_proxies.Count -gt 0)
        {
            if($_proxies.Count -eq 1)
            {
                $primary_proxy = $_proxies[1]
            }
            elseif($domain_proxies.Count -gt 1)
            {
                $primary_proxy = $_proxies[1]
                $secondary_proxy = $_proxies[$_proxies.Count - 1]
            }
        }
        elseif($_proxies -le 0)
        {
            $primary_proxy = "SMTP:" + $_email
        }

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