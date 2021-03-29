<#
	Standardized New User Creations: A powershell script to standardize the process of creaitng new active directory user accounts.
    Copyright (C) 2021  Aaron Johnson

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
#>

function Menu
{
	$choice = Read-Host "Select from one of the following options by typing the number: `n`t1) View Licensing information`n`t2) Create a user`n`tChoice "
	$validate = 0
	while($validate -eq 0)
	{
		if($choice -eq 1)
		{
		    $validate = 1
			Display-Licensing
		}
		if($choice -eq 2)
		{
			$validate = 1
			Create-User
		}
	}
}


function Display-Licensing
{
	$validate = 0 
	while($validate -eq  0)
	{
	    $choice = Read-Host "SNUC  Copyright (C) 2021  Aaron Johnson `n This program comes with ABSOLUTELY NO WARRANTY; for details type 'show w'. `n This is free software, and you are welcome to redistribute it `n under certain conditions; type 'show c' for details.`n To cotninue on, type 'continue' "
	    if($choice.toLower() -eq "show c")
	    {
		    clear
            Write-Host " See the License (GPLv3 License.txt) contained in the root directory of this program. `n "
			           " When you downloaded this you were provided with a license. If you downloaded this `n"
					   " from anywhere other than https://github.com/ElysianProductions/SNUC you must ensure you are given a license. `n  "
					   " My master repository contains a license in the root directory as well as the sub-directory which includes this script. `n "
					   " The file is again labeled GPLv3 License.txt If for some reason you lose or delete that license it can be found here:`n https://www.gnu.org/licenses/gpl-3.0.en.html "
					 
	    }
	    if($choice.toLower() -eq "show w")
	    {
            clear   
		    $warranty = "      15. Disclaimer of Warranty. `n"
                             " THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW. EXCEPT WHEN OTHERWISE STATED IN WRITING`n "
						     " THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM 'AS IS' WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR`n "
						     " IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE`n "
						     " ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU. SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME`n  "
						     " THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION."
	    }
	    if($choice.toLower() -eq "continue")
	    { 
			$validate = 1
	    }	
	}
    clear
	Create-User
}


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



function Add-ProxyAddress 
{
	param
	(
	    [string]$Username, 
		[string]$Proxy,
        [string]$Type 
	)
	
    if($type.toLower() -eq "primary")
    {
        $tmp = "SMTP: "
        $Proxy = $tmp + $Proxy
        Set-ADUser -Identity $Username -Add @{Proxyaddresses = $Proxy} 
    }
    if($type.toLower() -eq "secondary")
    {
        $tmp = "smtp: "
        $Proxy = $tmp + $Proxy
        Set-ADUser -Identity $Username -Add @{Proxyaddresses = $Proxy} 
    }
    # Usage example = Add-ProxyAddress -Username $user_name -Proxy $proxy -Type "secondary"
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

    $_ou = (Get-ADOrganizationalUnit -Filter * | select-object -ExpandProperty Name)
    # $_ou = (Get-OUNames -List "Names")
	<#
        The purpose of this is simply to store the 'clean' easily readable OU names.
    #>

    $_ou_distinguished = (Get-ADOrganizationalUnit -Filter * | Select-Object -ExpandProperty distinguishedName)
    # $_ou_distinguished = (Get-OUNames -List "Distinguished")
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
            $tmp = Read-Host "You selected " $_upn_suffixes[$suffix_choice] " if this is right, type 'y\n' "
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
            $tmp = Read-Host "You selected "  $_cloud_client[$cloud_choice]  " if this is right, type 'y\n' "
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
		
	
	    $answer = Read-Host "If the user needs a different display name type 'y\n' "
        if($answer.ToLower() -eq 'y')
        {
            $display_name = Read-Host "Input the users display name "
			$validate = 0
            while($validate -eq 0)
            {
                $choice = Read-Host "You entered" + $display_name + " is this correct, y/n "
                if($choice -eq "y")
                {
                    $validate = 1
                }
                if($choice -eq "n")
                {
                    $display_name = Read-Host "Input the users new display name "
                }
            }
        }
        else
		{
            $display_name = $full_name
		}			
        
    

	    $_primary_proxy = "SMTP:" + $email_address
	    $multi_proxy = Read-Host "By default the primary proxy has already been generated. If the user requires another (or different) proxy type 'y\n' "
	    if($multi_proxy.toLower() -eq "y")
	    {
	       #$tmp = Read-Host "Input the secondary proxy address without typing 'smtp:' "
           #$_secondary_proxy = 	"smtp:" + $tmp
           $prox_val = 0
           $prox_choice = ''
           while($prox_val -eq 0)
           {
               $prox_choice = Read-Host "Please select from one of the following values`n`t1) Primary SMTP`n`t2) Secondary SMTP`n`t3) Finished`nChoice "
               if($prox_choice -eq 1)
               {
                   $tmp = Read-Host "Please enter the full email"
                   $_primary_proxy = "SMTP:" + $tmp
               }
               if($prox_choice -eq 2)
               {
                   $tmp = Read-Host "Please enter the full email"
                   $_secondary_proxy = "smtp:" + $tmp
               }
               if($prox_choice -eq 3)
               {
                   $prox_val = 1
               }
            }		   
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
            $user_name = $_cloud_prefix[$cloud_choice] + $user_name
        }
		<#
		    Correcting the SamAccountName based on cloud usage.
		#>
		
		
        $_given_name, $_sur_name = $full_name.Split(' ')

        Write-Host "`n----------------------------Confirm User----------------------------`n"
        Write-Host "Employee Name: " $full_name
        Write-Host "UPN: " $_upn
        Write-Host "sAMAccountName: " $user_name
        Write-Host "Display name: " $display_name
        Write-Host "Email Address: " $email_address
        Write-Host "Primary Proxy Address: " $_primary_proxy
		if($_secondary_proxy.length -ge 1)
		{
			Write-Host "Secondary Proxy Address: " $_secondary_proxy
		}
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

            Set-ADUser -Identity $user_name -Add @{Proxyaddresses = $_primary_proxy}
			if($_secondary_proxy.length -ge 1)
			{
			    Set-ADUser -Identity $user_name -Add @{Proxyaddresses = $_secondary_proxy}	
			}
        }    
    }
	
	Shift-OU -Username (Get-ADUser -Filter {SamAccountName -like $user_name}) -OU_Array (Get-ADOrganizationalUnit -Filter * | Select-Object -expandproperty Name) -OU_Path (Get-ADOrganizationalUnit -Filter * | Select-Object -ExpandProperty distinguishedName)
    # Shift-OU -Username (Get-ADUser -Filter {SamAccountName -like $user_name}) -OU_Array (Get-OUNames -List "Name" ) -OU_Path (Get-OUNames -List "Distinguished")

}
Menu 