#ifndef PSINTEGRATION_H
#define PSINTEGRATION_H

#include <QString>
#include <QProcess>
#include <QRegularExpressionMatch>
#include <QRegularExpression>
#include <QTextStream>
#include <QUrl>
#include <QPdfWriter>
#include <QPainter>
#include <QTextDocument>
#include <QPrinter>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSettings>

class PSIntegration : public QObject
{
    Q_OBJECT
public:
    PSIntegration();


    void set_db_lists();
    void List_Password_Policy(QString name); // Detect the appropriate policy based on the template user. - done
    void Set_FGPP_active(QString MinLength, QString Complexity); // Configure the fine grain password policy as active for this specific user.
    void Set_DDPP_active(QString MinLength, QString Complexity); // Configure the default domain policy as active for this specific user.
    void Set_APP_active(QString MinLength, QString Complexity); // Configure an arbitrary password policy with min length = 10 and complexity = true
    void Move_ADUser_Orgranizational_Unit(QString User_CN, QString Template_OU_Distinguished); // Move the specified user to the specified OU.
    void Dump_User_Form(QString data, QUrl image_path, QString name); // Take the contents of the data and draw it on a pdf. If chosen you can supply an image to paint as well.
    //void Set_URL_Image_Path(QString url); // Set the path to the image
    void Edit_Name(QString name, QString first_name, QString middle_name, QString last_name);
    void Edit_Password(QString name, QString password);
    void Edit_Display_Name(QString name, QString display_name);
    void Edit_Proxy_Addresses(QString name, QStringList proxies);
    void Edit_Email_Address(QString name, QString email);
    void Edit_UserPrincipalName(QString name, QString upn);
    void Edit_UserSamAccountName(QString name, QString SamAccount);
    void Edit_Group_Memebership(QString name, QStringList GroupsToAdd, QStringList GroupsToRemove);
    void Edit_User_Status(QString name); // Disable the user account
    void Edit_User_Strip_All_Groups(); // Remove user from all memberships
    void Edit_Disable_Description(QString name);

    bool Validate_Password(QString pword, QString MinPasswordLength, QString ComplexityEnabled); // Validate whether or not the password meets the requirements of the policy.
    bool Get_Azure_Status(); // Determine whether or not the Azure module is on the server.
    bool Employee_Name_Exists(QStringList names, QString new_name); // Validate whether or not a person with the given and surnames already exists.
    bool Validate_User_Status(QString template_name); // Confirm whether or not the template user is disabled.
    bool Validate_Email_Address(QString email_address); // Validate whether or not their is a duplicate email address.


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
    QString Run_Azure_Sync(bool var); // Execute an Azure AD Sync if possible.
    QString List_URL_Image_Path(); // dump the path to the url image.
    QString List_User_UserPrincipalName(QString name); // List the UserPrincipalName property of the specified user.
    QString List_User_DisplayName(QString name); // List the displayName property of the specified user.

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



void setAllADUsers(const QStringList &list); // Set all_users StringList.
void setAllADUPNs(const QStringList &list); // Set all_upns StringList
void setAllADForests(const QStringList &list); // Set all_forests StringList
void setAllOUNames(const QStringList &list); // Set all_ou_names StringList;
void setAllOUDNs(const QStringList &list); // Set all_ou_distinguished StringList;
void mapOUToCompany(); // Create ou_to_company by querying OU, Company in UMTK.db
void mapCompanyToPrefix(); // Create company_to_prefix by querying Company, Prefix in UMTK.db
void mapUserToCompany(); // create a map for user to company for multi company support
void mapUserToOU(); // create a map for user to ou for multi company support
void mapCompanyToSam(); // Create a map for company to sam for multi company support
void reMapConnections();

/*void setLogoPath(const QString &path);
void setCustomCreationText(const QString &path);
void setCustomDisableText(const QString &path);
void setCustomLogoPosition(const QString &path);
*/

QStringList getAllADUsers(); // Return all_users StringList
QStringList getAllADUPNs(); // Return all_upns StringList
QStringList getAllADForests(); // Return all_forests StringList
QStringList getAllOUNames(); // Return all_ou_names StringList
QStringList getAllOUDNs(); // Return all_ou_distingusihed StringList



QString stripCompanyName(QString employee); // Remove X characters from the start of the string to the hypen + 1
QString getEmployeeName(QString name); // Pass the full name of your user, function returns Clean_String(name).
QString getEmployeeIdentifier(QString name); // Pass the full name of your user.
QString validateSamOption(QString template_name, QString new_user); // Pass the template users name to check company options, pass the new users name to then configure the SamAccountName.

bool multi_company_enabled;
bool getMultiCompanyStatus();

protected:
    QString image;


signals:

    void _ADUsersChanged();
    void _UPNsChanged();
    void _ForestsChanged();
    void _OUNamesChanged();
    void _OUDistinguishedNamesChanged();


public slots:


private:

    bool initalize_database(const QString &db_path); // Initialize the UMTK.db
    QSqlDatabase get_database(); // Get the connection.

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


    QMap<QString, QString> ou_to_company; //QMap of OU to Company pairs
    QMap<QString, QString> company_to_prefix; // QMap of Company to Prefix pairs.
    QMap<QString, QString> user_to_company; // QMap of User to Company pairs.
    QMap<QString, QString> user_to_ou; // QMap of User to OU pairs.
    QMap<QString, int> company_to_sam; // QMap of Company to Sam pairs.

    QStringList ou_list;
    QStringList company_names_list;
    QStringList user_prefix_list;
    QStringList sam_setting_list;
    QString database_name;

    QStringList all_users;
    QStringList all_ou_distinguished;
    QStringList all_ou_names;
    QStringList all_upns;
    QStringList all_forests;

};

#endif // PSINTEGRATION_H
