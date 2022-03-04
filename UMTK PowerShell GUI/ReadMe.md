    #The license is GPL and is available to you here. 
    There is absolutely no warranty associated with this software. 
    Usage of this software expressly  releases me from any and all liability.
    
    
    The Powershell version works but was sidelined for the C++ version. As a consequence 
    of this the PS version is missing quite a few things that the C++ version has.
    
    
    
    
    ##The project - UMTK

    The User Maintenance Tool Kit (UMTK) was created to standardize how new user accounts are created within 
    Windows Active Directory. The following components are considered by many (though not all) to be standard 
    and should generally always be filled in. 

    A) First name
    B) Last name
    C) Display name
    D) Email attribute
    E) UPN
    F) SamAccountName
    G) ProxyAddress should at least have SMTP:primary@primaryemail.com
    
    The project also pulls from the default domain security policy to identify your password requirements.
    If the user is in a group that's associated with a Fine Grain Password policy it will prioritize that 
    grained policy for that user. By default the project forces you to use a template user with the idea 
    being that this template will have the appropriate group associations. 
    
    Once the account is created it will generate a PDF with all of the information the end user or their 
    supervisor will need to access the account. You can modify any of the fields that you need to though 
    you shouldn't really have to.
    

    
    
    
    
    
    ##Domain User Creation usage:
    
    1) You need to run the program as an administrator so right + Click and then select run as administrator.
    2) Click the ** Create Domain User button **.
    3) Select from the combo box the template user you want to copy.
    4) Type the fullname of the user into the text box.
    5) Click the ** Generate button **.
    6) Ensure that the password meets the requirement as laid out in the password text box.
    7) Click the ** Create button **.
    8) Validate that the information in the popup is accurate. If it is click ** OK **. If it's not, click ** Cancel **.
    9) Take the generated PDF and deliver it to the appropriate people.
    
    optional) If you need to give the user a different display name, or set the primary proxy to be something other 
    than the generated email, or create a secondary proxy click the ** Advanced Options ** button. 
    Just fill in whichever (or all) of the fields you need.
    
    The last step in the process along with the PDF creation is that the software will see if the 
    Azure AD Sync modules are installed. If the server can execute an ADSync it will run the ADSync. 
    
    
    If the server does not have the ADSync powershell modules it will reccomend that you run one if it 
    applies to you on the appropriate server. It gives the following command:
    
    `Import-Module ADSync; Start-ADSyncSyncCycle -PolicyType Delta` 
   
    as the recomended sync to run as opposed to an initial (full).
    
    It's a really straightforward process and it can prevent a lot of issues for end users as well as for IT staff. 
    You should always check your work and make sure things look right. The software will validate and prevent people 
    from:
    
    1) Creating an account that already exists (same username or same first and last name).
    2) Password that doesn't meet the standards.
    3) Any of the fields are missing except for the optional fields.
    
    It will present an error message and tell you what to do to fix it.
    
    
    
    
    
    
    
    
    ##Create Local User usage:
    
    1) You need to run the program as an administrator so right + Click and the nselect run as administrator
    2) Click the ** Create Local User **  button.
    3) Input the username.
    4) Input the users fullname. 
    5) Either check the ** Administrators ** box or not.
    6) Click the ** Create ** button.
    7) Give the generated PDF to the appropriate person.
    
    
    
    
    
    
    
    
    ## Disable User usage (Only intended for (and tested with) domain accounts)
    
    The idea here is to disable the user, remove the user from any groups it belong to with the single exception
    being the primary group (domain users, authenticated users etc..). It will then add a note to the description
    of the user account with something like Disabled: 02/01/2022 15:57:03 to make it easier for you to track.
    
    It also generates a PDF with helpful information such as groups before they used to be in.
    
    1) You need to run the program as an administrator so right + Click and then select run as administrator.
    2) Click the ** Disable User Account **  button.
    3) Select the user to disable from the combobox.
    4) Click the ** Disable ** button.
    5) Supply the appropriate user with the generated PDF or retain it for your historical purposes.
