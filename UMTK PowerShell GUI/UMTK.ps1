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


    $employee_name_label = New-Object Windows.Forms.Label
    $employee_name_label.size = New-Object System.Drawing.Size(150, 50)
    $employee_name_label.location = New-Object System.Drawing.Size(0, 50)
    $employee_name_label.text = "Employee name:"
    $Domain_Form.Controls.Add($employee_name_label)


    $employee_name_input = New-Object Windows.Forms.TextBox
    $employee_name_input.size = New-Object System.Drawing.Size(180, 50)
    $employee_name_input.location = New-Object System.Drawing.Size(150, 50)
    $Domain_Form.Controls.Add($employee_name_input)


    $username_label = New-Object Windows.Forms.Label
    $username_label.size = New-Object System.Drawing.Size(150, 50)
    $username_label.location = New-Object System.Drawing.Size(0, 100)
    $username_label.text = "Username:"
    $Domain_Form.Controls.Add($username_label)


    $username_input = New-Object Windows.Forms.TextBox
    $username_input.size = New-Object System.Drawing.Size(50, 50)
    $username_input.location = New-Object System.Drawing.Size(150, 100)
    $Domain_Form.Controls.Add($username_input)


    $password_label = New-Object Windows.Forms.Label
    $password_label.size = New-Object System.Drawing.Size(150, 50)
    $password_label.location = New-Object System.Drawing.Size(0, 150)
    $password_label.text = "Password:"
    $Domain_Form.Controls.Add($password_label)


    $password_input = New-Object Windows.Forms.TextBox
    $password_input.size = New-Object System.Drawing.Size(50, 50)
    $password_input.location = New-Object System.Drawing.Size(200, 150)
    $Domain_Form.Controls.Add($password_input)

    $email_label = New-Object Windows.Forms.Label
    $email_label.size = New-Object System.Drawing.Size(150, 50)
    $email_label.location = New-Object System.Drawing.Size(0, 350)
    $email_label.text = "Email address:"
    $Domain_Form.Controls.Add($email_label)


    $email_input = New-Object Windows.Forms.TextBox
    $email_input.size = New-Object System.Drawing.Size(350, 75)
    $email_input.location = New-Object System.Drawing.Size(0, 350)
    $Domain_Form.Controls.Add($email_input)



    $Domain_Form.Add_Shown({$Domain_Form.Activate()})
    [void] $Domain_Form.ShowDialog()
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