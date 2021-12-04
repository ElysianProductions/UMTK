#ifndef PSINTEGRATION_H
#define PSINTEGRATION_H

#include <QString>
#include <QProcess>
#include <QRegularExpressionMatch>
#include <QRegularExpression>

class PSIntegration
{
public:
    PSIntegration();


    void List_Password_Policy(QString name); // Detect the appropriate policy based on the template user. - done
    void Set_FGPP_active(QString MinLength, QString Complexity); // Configure the fine grain password policy as active for this specific user.
    void Set_DDPP_active(QString MinLength, QString Complexity); // Configure the default domain policy as active for this specific user.
    void Set_APP_active(QString MinLength, QString Complexity); // Configure an arbitrary password policy with min length = 10 and complexity = true


    bool Validate_Password(QString pword, QString MinPasswordLength, QString ComplexityEnabled); // Validate whether or not the password meets the requirements of the policy.


    QString User_Exists(QString SamName); // Validate whether or not a user with <SamAccountName> already exists - Done
    QString List_User_Identifier(QString name); // List the UPN - Done
    QString List_User_DN(QString name); // User distinguished name - Done
    QString List_User_OU_DN(QString name); // User Organizational unit distinguished name - Done
    QString List_User_OU_CN(QString name); // User Organizational unit clean name - done
    QString Execute(QString param); // Launch an elevated ps session and execute a command that returns a QString - Done
    QString Clean_String(QString str); // Cleanup a string and strip all occurrences of /r, /n - Done
    QString List_SamAccountName(QString name); // return the SamAccountName of the user - done
    QString List_Mail(QString name); // Return the Mail attribute. - done
    QString List_Name(QString name); // Return cleaned up supplied name. - done
    QString List_ActiveSP_Complexity(); // List the active security policy complexity status for this specific user.
    QString List_ActiveSP_length(); // List the active security policy minimum length for this specific user.


    QStringList List_All_OU_DNs(); // Return the Distinguished name of all Organizational units. // Done
    QStringList List_All_OU_CNs(); // Return the clean name of all Organizational units // Done
    QStringList List_All_Group_CNs(QString SamName); // Return the clean name of all groups - Done
    QStringList List_All_Group_DNs(QString SamName); // Get all group distinguished names - Done
    QStringList List_User_Group_CNs(QString SamName); // Return clean names of all groups for a specific user - done
    QStringList List_User_Group_DNs(QString SamName); // Return distinguished names of all groups for a specific user.
    QStringList List_All_UPNs(); // List all available UPN Suffixes - Done
    QStringList List_All_Forests(); // List all of the AD forest names - Done
    QStringList List_All_Domain_Users(); // List the names of all domain users
    QStringList Execute_Command(QString param); // Launch an elevated ps session and execute a command that returns a QStringList - Done

private:

    QString DDPP_ComplexityEnabled; // Default Domain Password Policy ComplexityEnabled
    QString DDPP_MinPasswordLength; // Default Domain Password Policy MinPasswordLength

    QStringList FGPP_Names;                 // Fine Grained Password Policy names
    QString FGPP_ComplexityEnabled;        // Fine Grained Password Policy ComplexityEnabled
    QString FGPP_MinPasswordLength;       // Fine Grained Password Policy MinPasswordLength
    QStringList FGPP_AppliesTo;          // Fine Grained Password Policy AppliesTo
    QString FGPP_DistinguishedName;     // Fine Grained Password Policy DistinguishedName
    int FGPP_MatchGrainPolicy;         // Fine Grained Password Policy index positon of matching policy name in FGPP_Names

    QString APP_MinPasswordLength;   //Arbitrary Password Policy MinPasswordLength
    QString APP_ComplexityEnabled; // Arbitrary Password Policy ComplexityEnabled


    QString active_SP_MinLength; // This is the active policy always. It pulls from one of the above variables.
    QString active_SP_Complexity; // This is the active policy always. It pulls from one of the above variables.

};

#endif // PSINTEGRATION_H
