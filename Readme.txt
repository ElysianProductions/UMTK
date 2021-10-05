What is this project?

The purpose of this project is to create user accounts (primarilly) within AD easier, faster and more effective. It's being written to force a standard set of default parameters:
    Required:
        1) The email field (mail attribute) must be filled out. 
        2) The user must have a primary SMTP (proxyAddress attribute) added.
        3) The user must not be made without selecting a template user first.
        4) A report must be generated which illustrates the users account creation (all of their logon information for the machine\domain) that can be given to them.
        5) Force the technician to use a secure password (more than 8 characters, following nist guidelines)
    Optional:
        1) The user has the ability to added a secondary smtp (proxyAddress attribute) at creation.
        2) The user has the ability to change the display name (displayName attribute) at creation.
        3) A report is generated to put the onus of correct creation on the technician who creates the account.
        
Why is this even a project?

Mostly because I want to stop people that I deal with on a daily basis from creating accounts wrong. Is this really the best way? Probably not, but it's part of how I'm doing it. Things like adding smtp\email making various migraitons easier. Forcing the technician to choose a template acocunt forces the end users supervisor to really provide people with the right information or it falls on them if it's done wrong. Generating forms to distribute to the users makes it easier for them to see the information; and forms for the technician can be given to his or her boss if they repeatedly do it wrong. 

When you work for an entity that has a specific way of doing things if it's not done correctly those things can wreak havoc. 


When will it be compelete? 

That's hard to say, right now the C++ and PowerShell versions both create the Domain accounts, add the addresses etc.. They need to be polished and made to function better. There are a variety of features that will come over time too.
