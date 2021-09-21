[System.Reflection.Assembly]::LoadWithPartialName("System.Windows.Forms")


Function DomainUser 
{
    [void] [System.Reflection.Assembly]::LoadWithPartialName("System.Drawing")
    [void] [System.Reflection.Assembly]::LoadWithPartialName("System.Windows.Forms")

    $user_screens = [System.Windows.Forms.Screen]::AllScreens
    foreach($screen in $user_screens)
    {
        $screen_name = $screen.DeviceName
        $width = $screen.Bounds.Width
        $height = $screen.Bounds.Height
        $primary = $screen.Primary
    }

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
    $distinguished_ous = Get-ADOrganizationalUnit -Filter * | Select-Object -ExpandProperty Distinguishedname
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
    $create_button.Add_Click({CreateDomainUser -domain_ou_selection $ou_combo.Text -domain_template_user $users_combo.Text -domain_upn $special_combo.Text -domain_employee_name $employee_name_input.Text -domain_user_name $username_input.Text -domain_user_password $password_input.Text -domain_email_address $email_input.Text})
    $Domain_Form.Controls.Add($create_button)


    $close_button = New-Object Windows.Forms.Button
    $close_button.Text = "Close"
    $close_button.size = New-Object System.Drawing.Size(120, 30)
    $close_button.location = New-Object System.Drawing.Size(310, 360)
    $Domain_Form.Controls.Add($close_button)


    $Domain_Form.Add_Shown({$Domain_Form.Activate()})
    [void] $Domain_Form.ShowDialog()
}


Function CreateDomainUser
{
    param (
        [string]$domain_ou_selection,
        [string]$domain_template_user,
        [string]$domain_upn,
        [string]$domain_employee_name,
        [string]$domain_user_name, 
        [string]$domain_user_password,
        [string]$domain_email_address,
        [string]$domain_display_name,
        [array[]]$domain_proxies
    )

    if($domain_ou_selection.Count -le 0 -or $domain_template_user.Count -le 0 -or
       $domain_upn.Count -le 0 -or $domain_employee_name.Count -le 0 -or $domain_user_name -le 0 -or 
       $domain_user_password.Count -le 0 -or $domain_email_address -le 0) 
    {
        if($domain_display_name.Count -le 0)
        {
            $domain_display_name = $domain_employee_name
        }
        if($domain_proxies.Count -gt 0)
        {
            if($domain_proxies.Count -eq 1)
            {
                $primary_proxy = $domain_proxies[1]
            }
            elseif($domain_proxies.Count -gt 1)
            {
                $primary_proxy = $domain_proxies[1]
                $secondary_proxy = $domain_proxies[$domain_proxies.Count - 1]
            }
        }
        elseif($domain_proxies -le 0)
        {
            $primary_proxy = "SMTP:" + $domain_email_address
        }

         $_given, $_surname = $domain_employee_name.Split(' ')
         $tmp_pass = $domain_user_password | ConvertTo-SecureString -AsPlainText -Force
         New-ADUser -Name $domain_employee_name -GivenName $_given -Surname $_surname -AccountPassword $tmp_pass -UserPrincipalName $domain_upn -DisplayName $domain_display_name  -EmailAddress $domain_email_address -SamAccountName $domain_user_name  -Enabled 1

        
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

    $user_screens = [System.Windows.Forms.Screen]::AllScreens
    foreach($screen in $user_screens)
    {
        $screen_name = $screen.DeviceName
        $width = $screen.Bounds.Width
        $height = $screen.Bounds.Height
        $primary = $screen.Primary
    }

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


    $UMTK_Form.Add_Shown({$UMTK_Form.Activate()})
    [void] $UMTK_Form.ShowDialog()
}

Main

# This is straightforward, it will be a lightweight easily modifiable script for Windows only.
# It will follow the same layout as the C++ GUI does to the letter. The purpose of creating 
# this is to ensure that those comfortable with PS can use and tailor to their needs. The 
# overal intent of the C++ program is to prove an easily usable, portable system for the 
# addition, removal, and or modification of a user account on any system. It will essentially 
# be the communcation utility which executes the same or very similar commands in the hard coded
# native scripted utilities. The reason these are going to be GUI based is because people want things 
# to be as easy as possible while preventing as many mistakes as possible.