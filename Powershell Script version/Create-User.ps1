<#
	This script was written by Aaron Johnson and is being released under the MIT License. It's free and open source and can be used by anyone for any purpose. 
	I only ask that you credit me as the original creator. With that said, see below for compatability.

	TESTED AND CONFIRMED WORKING ON THE FOLLOWING WITH NO ISSUES:
	    Windows Server 2019 
	    Windows Server 2016 
	    Windows Server 2012 Essentials
		Windows Server 2012 R2 Standard
	    Windows Small Business Server 2011
	
	TESTED ON:
		
	
	FAILS ON:
	    Windows Server 2008 - Any version 
		Windows SErver 2003 - Any version

#>



function Shift-OU 
{
      param (
        [string]$Username, #This represents the user name.
        [array[]]$OU_Array, # Just the cleaned up names. 
		[array[]]$OU_Path # The Distinguished names.
			
    )
    
    $ou_choice = 0 
    $validate = 0
    while($validate -eq 0)
    {
        Write-Host "Select one of the following values: "
        for($i = 0; $i -lt $_ou.count; $i++)
        {
            "`t " + $i + ": " + $_ou[$i]
        }
        $ou_choice = Read-Host " Select OU "
        $tmp = Read-Host "You selected " $OU_Array[$ou_choice] " if this is right, type 'y' "
        if($tmp.ToLower() -eq "y")
        {
            $validate = 1
			$loc = $OU_Path[$ou_choice]	
            Move-ADObject -Identity "$Username" -TargetPath "$loc"
        } 
    }
	
<#
    The following commented section of code is an example execution of this function.
	Note, you can clearly modify how you pull the data in; you're just restricted to 
	pushing that data into an array per the parameters of the function. With the 
	only exception to this rule being the username. It can easily be modified to 
	pull data from files. 
	
	Shift-OU -Username (Get-ADUser -Filter {SamAccountName -like "user_name"}) -OU_Array (Get-ADOrganizationalUnit -Filter * | Select-Object -expandproperty Name) -OU_Path (Get-ADOrganizationalUnit -Filter * | Select-Object -ExpandProperty distinguishedName)
#>
}


function Get-UPN
{
      Get-ADForest | Select-Object -ExpandProperty UPNSuffixes
      <# 
	       Separating everything out to individual functions just makes things easier.
		   
		   This is a simple one. It pulls all of your UPNs.
	   #>
}

function Get-DomainName 
{
    Get-ADDomain | Select-Object -ExpandProperty Forest
	<# 
	    Separating everything out to individual functions just makes things easier.
		   
		This is a simple one. It pulls your domain name.
	#>
}
  
function Get-OUNames 
{
    param
    (
        [string]$List
    )

    if($List.ToLower() -eq "name")
    {
        Get-ADOrganizationalUnit -Filter * | Select-Object -ExpandProperty Name   
    }
    if($List.ToLower() -eq "distinguished")
    {
        Get-ADOrganizationalUnit -Filter * | Select-Object -ExpandProperty distinguishedName
    }
	
	<# 
	    Separating everything out to individual functions just makes things easier.
		   
	    Feed this function one of two variables:
		-List "Name"
		-List "Distinguished"
		
		Both are used hereand respectively generate:
		<example name> 
		<Users>
		<My Users>
		
		<OU= example> 
		etc..
	#>

}






function Create-User
{	
    $_whoami = (Get-ItemProperty "HKCU:Software\Microsoft\Windows\CurrentVersion\Explorer\User Shell Folders") | 
        Select-Object -ExpandProperty Desktop
    <#
        This could be a problem in the future. It assumes that the administrator running this 
	    script is part of the folder redirection group. It also assumes that folder redirection exists. 
    
        We can easily adjust this based on the return value. Ie. Redirected profiles VS local.

        
        if(!(Test-Path $env:userprofile + "\Downloads\))
        {
            $_whoami = (Get-ItemProperty "HKCU:Software\Microsoft\Windows\CurrentVersion\Explorer\User Shell Folders") | 
                Select-Object -ExpandProperty Desktop
        }
        else 
		{ 
		    $_whoami = $env:userprofile + "\Downloads\
		}			
    #>

    # $_ou = (Get-ADOrganizationalUnit -Filter * | select-object -expandproperty Name)
    [array[]]$_ou = (Get-OUNames -List "Names")
	<#
        The purpose of this is simply to store the 'clean' easily readable OU names.
    #>

    # $_ou_distinguished = (Get-ADOrganizationalUnit -Filter * | Select-Object -ExpandProperty distinguishedName)
    [array[]]$_ou_distinguished = (Get-OUNames -List "Distinguished")
	<#
        This will pull in your distinguished name(s). 
	    Note: OU=Elysium Administrators,OU=Elysium,DC=Elysium,DC=local 
    
        We just pull it for each OU. There is no reason to hard code any default info 
        on the root OU or the DC. We could dump this into a CSV file along with the cleaned 
        up OU names to just read in. Depending on how large the DC is... 	
    #>

    # $_dname = (Get-ADDomain | Select-Object -expandproperty Forest)
	$_dname = (Get-DomainName)
    <#
        This could get interesting because it assumes a single forest. 
	    I need to test in a multi forest environment to get it going. 
	    The changes required to make it work in a multi forest environment 
	    are literally just dumping it into an array, asking the user which one 
	    they want to use and then pulling from the index. See how I handled the 
		upn issue below.
    #>
	

    [array[]]$_upn_suffixes = Get-UPN
    if($_upn_suffixes.count -ge 1)
    {
        $validate_suffix = 0
        while($validate_suffix -eq 0)
        {
            Write-Host "Select one of the following values: "
            for($i =0; $i -lt $_upn_suffixes.count; $i++)
            {
                "`t " + $i + ": " + $_upn_suffixes[$i]
            }
            $suffix_choice = Read-Host "Select UPN: "
            $tmp = Read-Host "You selected " $_upn_suffixes[$suffix_choice] " if this is right, type 'y' "
            if($tmp.ToLower() -eq "y")
            {
                $validate_suffix = 1
            }    
        }
    }
    
    $_dname = Get-DomainName

    if($_upn_suffixes.count -ge 1)
    {
        $upn_tmp = "@" + $_upn_suffixes[$suffix_choice]
    }
    else 
    {
        $upn_tmp = "@" + $_dname
    }

	<#
	    This has been tested and confirmed working. This is one of those ever growing 
		items that could be dumped into a file and just read into an array. Becausee 
		this is being written with an MSP in mind; if the MSP continues to expand 
		then this list will grow and could do so exponentially. 
	#>
	
	
	
	$_cloud_prefix = "TOH_", "DCWIB_", "RWM_", "Taxpert_", "TFCPA_", "DSA_"
    
	$_cloud_client = "Taylor oil & Heat", "Dutchess County Workforce Investment Board",
        "Rosenthal Wine Merchants", "Taxpert", "Tom Fucito CPA", "Dana Sperry CPA"

    $is_cloud = Read-Host "If this is a cloud user type 'y' "
    if($is_cloud -eq "y")
    {
        $validate_cloud = 0
        while($validate_cloud -eq 0)
        {
            Write-Host = "Select from one of the following values: "
            for($i = 0; $i -lt $_cloud_prefix.count; $i++)
            {
                "`t " + $i + ": " + $_cloud_client[$i]
            }
            $cloud_choice = Read-Host "Select cloud prefix "
            $tmp = Read-Host "You selected "  $_cloud_client[$cloud_choice]  " if this is right, type 'y' "
            if($tmp.ToLower() -eq "y")
            {
                $validate_cloud = 1
            }
        }
    }
	<#
        To make this non-pca specific these two objects need to live in a file. 
		
		Essentially you have two arrays. The first containing the full company name and the second containing the prefix 
		associated with that company. You can read the data from a file into an array give them the full comapny name. 
		
		After they select teh name you just read from a second file containing jsut the prefixes and you pull the 
		coresponding index value. It's easier to do it this way because it reduces screwups.
	#>
    
	
    $_is_done = 0
    while(!($_is_done))
    {
        $full_name = Read-Host "Input the users full name " 
        $user_name = Read-Host "Input the username you'd like to use "
        $email_address = Read-Host "Input the users full email address "
        $pfin = Read-Host "Password" -AsSecureString

	
	    $answer = Read-Host "If the user needs a different display name type 'y' "
        if($answer.ToLower() -eq 'y')
        {
            $display_name = Read-Host "Input the users display name "
        }
        else
		{
            $display_name = $full_name
		}			
        
    

	    $_proxy = "SMTP:" + $email_address
	    $multi_proxy = Read-Host "By default the primary proxy has already been generated. If the user requires another proxy type 'y' "
	    if($multi_proxy.toLower() -eq "y")
	    {
	       $tmp = Read-Host "Input the secondary proxy address without typing 'smtp:' "
           $_secondary_proxy = 	"smtp:" + $tmp    
	    }
        <#
	        We can do the proxy addresses with an array but I don't see it as critical. 
		    Either way, if they need a second addy this is where it happens. 
	    #>
     	
        $_upn = $user_name + $upn_tmp 
	    
		<#
	        Just concatenating the username to the upn that was selected earleir.
  	    #>
	
		
		if($is_cloud -eq "y" )
        {
            $username = $_cloud_prefix[$cloud_choice] + $username
        }
		<#
		    Correcting the SamAccountName based on cloud usage.
		#>
		
		
        $_given_name, $_sur_name = $full_name.Split(' ')

        Write-Host "`n----------------------------Confirm User----------------------------`n"
        Write-Host "Employee Name: " $full_name
        Write-Host "UPN: " $_upn
        Write-Host "SamAccountName: " $user_name
        Write-Host "Display name: " $display_name
        Write-Host "Email Address: " $email_address
        Write-Host "Proxy Address: " $_proxy
        Write-Host "`n----------------------------Confirm User----------------------------`n"
    
        
	    $all_set = Read-Host " If the above information is accurate type 'y', if not type 'n'"
        if($all_set.toLower() -eq "y")
        {
            $_is_done = 1
            New-ADUser -Name $full_name -GivenName $_given_name -Surname $_sur_name -AccountPassword $pfin -UserPrincipalName $_upn -DisplayName $display_name -EmailAddress $email_address -SamAccountName $user_name -Enabled 1
        
            $template_account = Read-Host "Specificy Template user "
            <#
		       I need to be able to validate whether or not the user being searched for exists. 
		 	   If the user being searched for does not exist I don't want to see, errors, exceptions 
			   or death of the script. It's simple enoug hto search and respond back with a "User does not exists" message.
		    #>
		
		    $usr = Get-ADUser -Filter {name -like $template_account}
            $groups = (Get-ADUser $usr -Properties MemberOf).MemberOf
            foreach($group in $groups)
            {
                Add-ADGroupMember -Identity (Get-ADGroup $group).name -Members $user_name
            }

            Set-ADUser -Identity $user_name -Add @{Proxyaddresses = $_proxy}
        }    
    }
	
	#Shift-OU -Username (Get-ADUser -Filter {SamAccountName -like $user_name}) -OU_Array (Get-ADOrganizationalUnit -Filter * | Select-Object -expandproperty Name) -OU_Path (Get-ADOrganizationalUnit -Filter * | Select-Object -ExpandProperty distinguishedName)
    Shift-OU -Username (Get-ADUser -Filter {SamAccountName -like $user_name}) -OU_Array (Get-OUNames -List "Name" ) -OU_Path (Get-OUNames -List "Distinguished")

}
Create-User