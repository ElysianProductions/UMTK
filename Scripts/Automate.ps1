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
    # USAGE EXAMPLE: Get-USerIdentifier -Identity "Employee Name"
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
       $GroupsHash[$element] = Get-ADGroup $element | Select Name
   }
   return $GroupsHash
}

Function Get-PasswordPolicyType
{
    if(Get-Module -ListAvailable -Name "ActiveDirectory")
    {
        $test = (Get-ADFineGrainedPasswordPolicy -Filter * -ErrorAction SilentlyContinue)
        if($test -eq $null)
        {
            return "Default"
        }
        elseif($test -ne $null)
        {
            return  "Default \ Fine Grained"
        }
    }
    else
    {
        return "Arbitrary"
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
            if($Complexity -eq 1 -and $password.Length -ge $Length)
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
            # If secure return 1
            # If not secure return 0
            # Pretty straightforward forcing 1 Capital letter, one lower case letter, 1 number, 1 symbol and it has an 8 character minimum requirement
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
    
    # TODO: 
    # See issue described and outline here in the repo - https://github.com/ElysianProductions/UMTK/issues/32

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
    # Example usage - Automate-FillForms -Identity "Aaron Johnson" -Template (Get-OrginizationalUnit -Identity "Example User")
}

Export-ModuleMember -Function Automate-FillForms