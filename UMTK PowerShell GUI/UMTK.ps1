[System.Reflection.Assembly]::LoadWithPartialName("System.Windows.Forms")


Function DomainUser 
{
    Write-Host "Domain User"
}


Function LocalUser 
{
    Write-Host "Local User"
}

Function EditUser 
{

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
    $luser_button.Location = New-Object System.Drawing.Size(230, 290)
    $luser_button.Text = "Local User"
    $luser_button.Add_Click({LocalUser})
    $UMTK_Form.Controls.Add($luser_button)



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