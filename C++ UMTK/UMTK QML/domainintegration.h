#ifndef DOMAININTEGRATION_H
#define DOMAININTEGRATION_H


#include <QObject>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QUrl>
#include <QTextDocument>
#include <QPrinter>
#include <QDebug>

class DomainIntegration : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString da_template READ da_template WRITE setDAUserTemplate NOTIFY da_TemplateUserChanged) // Property 1.
    Q_PROPERTY(QString da_fname READ da_fname WRITE setDAFName NOTIFY da_FNameChanged) // Property 2.
    Q_PROPERTY(QString da_uname READ da_uname WRITE setDAUName NOTIFY da_UNameChanged) // Property 3.
    Q_PROPERTY(QString da_email READ da_email WRITE setDAEmail NOTIFY da_EmailChanged) // Property 4.
    Q_PROPERTY(QString da_displayname READ da_displayname WRITE setDADisplayName NOTIFY da_DisplayNameChanged) // Property 5.
    Q_PROPERTY(QString da_password READ da_password WRITE setDAPassword NOTIFY da_PasswordChanged) // Property 6.
    Q_PROPERTY(QString da_pproxy READ da_pproxy WRITE setDAPrimaryProxy NOTIFY da_PrimaryProxyChanged) // Property 7.
    Q_PROPERTY(QString da_sproxy READ da_sproxy WRITE setDASecondaryProxy NOTIFY da_SecondaryProxyChanged) // Property 8.
    Q_PROPERTY(QString da_complexitypolicy READ da_complexitypolicy WRITE setDAComplexityPolicy NOTIFY da_ComplexityChanged) // Property 14.
    Q_PROPERTY(QString da_lengthpolicy READ da_lengthpolicy WRITE setDALengthPolicy NOTIFY da_LengthPolicyChanged) // Property 15.
    Q_PROPERTY(QStringList da_allusers READ da_allusers WRITE setDAAllUsers NOTIFY da_AllUsersChanged) // Property 9.
    Q_PROPERTY(QStringList da_allupns READ da_allupns WRITE setDAAllUPNs NOTIFY da_AllUPNsChanged) // Property 10.
    Q_PROPERTY(QStringList da_alloucns READ da_alloucns WRITE setDAAllOUCns NOTIFY da_AllOUCNsChanged) // Property 11.
    Q_PROPERTY(int da_ouselect READ da_ouselect WRITE setOUComboIndex NOTIFY da_OUComboIndexChanged) // Property 12.
    Q_PROPERTY(int da_upnselect READ da_upnselect WRITE setUPNComboIndex NOTIFY da_UPNComboIndexChanged) // Property 13.

public:
    DomainIntegration();

    void setDAUserTemplate(const QString &templateuser); // See Q_PROPERTY labeled Property 1.
    void setDAFName(const QString &dafname); // See Q_PROPERTY labeled Property 2.
    void setDAUName(const QString &dauname); // See Q_PROPERTY labeled Property 3.
    void setDAEmail(const QString &daemail); // See Q_PROPERTY labeled Property 4.
    void setDADisplayName(const QString &dadpname); // See Q_PROPERTY labeled Property 5.
    void setDAPassword(const QString &dapassword); // See Q_PROPERTY labeled Property 6.
    void setDAPrimaryProxy(const QString &dapproxy); // See Q_PROPERTY labeled Property 7.
    void setDASecondaryProxy(const QString &dasproxy); // See Q_PROPERTY labeled Property 8.
    void setDAAllUsers(const QStringList &daallusers); // See Q_PROPERTY labeled Property 9.
    void setDAAllUPNs(const QStringList &daallupns); // See Q_PROPERTY labeled Property 10.
    void setDAAllOUCns(const QStringList &daalloucns); // See Q_PROPERTY labeled Property 11.
    void setOUComboIndex(const int &daouselection); // See Q_PROPERTY labeled Property 12.
    void setUPNComboIndex(const int &daupnselection); // See Q_PROPERTY labeled Property 13.
    void setDAComplexityPolicy(const QString &dacomplexity); // See Q_PROPERTY labeled Property 14.
    void setDALengthPolicy(const QString &dalengthpolicy); // See Q_PROPERTY labeled Property 15.
    void setUserDN();
    void Edit_Disable_Description(QString name);
    void Edit_User_Status(QString name);

    QString da_template(); // See Q_PROPERTY labeled Property 1.
    QString da_fname(); // See Q_PROPERTY labeled Property 2.
    QString da_uname(); // See Q_PROPERTY labeled Property 3.
    QString da_email(); // See Q_PROPERTY labeled property 4.
    QString da_displayname(); // See Q_PROPERTY labeled property 5.
    QString da_password(); // See Q_PROPERTY labeled property 6.
    QString da_pproxy(); // See Q_PROPERTY labeled Property 7.
    QString da_sproxy(); // See Q_PROPERTY labeled Property 8.
    QString da_complexitypolicy(); // See Q_PROPERTY labeled Property 14.
    QString da_lengthpolicy(); // See Q_PROPERTY labeled Property 15.
    QString Execute(QString param);


    QStringList da_allusers(); // See Q_PROPERTY labeled Property 9.
    QStringList da_allupns(); // See Q_PROPERTY labeled Property 10.
    QStringList da_alloucns(); // See Q_PROPERTY labeled Property 11.
    QStringList Execute_Commands(QString param);
    QStringList get_all_users();

    int da_ouselect(); // See Q_PROPERTY labeled Property 12.
    int da_upnselect(); // See Q_PROPERTY labeled Property 13.

protected:

public slots:
    void automate();
    QString create_domain_account();
    QString disable_domain_account();

signals:
    void da_TemplateUserChanged(); // User selection in combobox changed.
    void da_FNameChanged(); // Fullname in textedit changed.
    void da_UNameChanged(); // Username in textedit changed.
    void da_EmailChanged(); // Email in textedit changed.
    void da_DisplayNameChanged(); // Display name in textedit changed.
    void da_PasswordChanged(); // Password in textedit changed.
    void da_PrimaryProxyChanged(); // Primary SMTP proxy address in textedit changed.
    void da_SecondaryProxyChanged(); // Secondary SMTP proxy address in textedit changed.
    void da_AllUsersChanged(); // Used to prefill the ComboBox with all domain users found.
    void da_AllUPNsChanged(); // Used to prefill the ComboBox with all UPNs found.
    void da_AllOUCNsChanged();// Used to prefil the ComboBox with all OU CNs found in the domain.
    void da_OUComboIndexChanged(); // Ueed to change index to match the template users OU.
    void da_UPNComboIndexChanged(); // Used to change the index to match the template users upn.
    void da_ComplexityChanged(); // Used to set the place holder text for the password policy complexity status.
    void da_LengthPolicyChanged(); // Use to set the place holder text for the password policy minimum length.


private:
    QString template_user; // Template user to copy select from the combo box and set by setDAUserTemplate.
    QString full_name; //Full name of new employee.
    QString given_name; // The first name of the user.
    QString sur_name; // The last name of the user.
    QString username; // Username of new employee.
    QString display_name; // Display name of new employee (by default set to the full name).
    QString email_address; // The email of the new employee (byt default full_name + @ + domain/upn.
    QString password; // Password of the new employee.
    QString primary_proxy; // Primary proxy address of the new employee (by default the generated/edited email address).
    QString secondary_proxy; // Secondarty proxy address of the new employee.
    QString SamAccountName; // User SamAccountName.
    QString UserPrincipalName; // UserPrincipalName.
    QString ou_distinguished_name; // The new users OU DN.
    QString ou_clean_name; // The news users OU CN.
    QString user_dn; // The new uers current DistinguishedName to be used when we move the user to its new OU.
    QString DDPP_ComplexityEnabled; // Default Domain Password Policy ComplexityEnabled
    QString DDPP_MinPasswordLength; // Default Domain Password Policy MinPasswordLength
    QString FGPP_ComplexityEnabled;        // Fine Grained Password Policy ComplexityEnabled
    QString FGPP_MinPasswordLength;       // Fine Grained Password Policy MinPasswordLength
    QString FGPP_DistinguishedName;     // Fine Grained Password Policy DistinguishedName
    QString APP_MinPasswordLength;   //Arbitrary Password Policy MinPasswordLength
    QString APP_ComplexityEnabled; // Arbitrary Password Policy ComplexityEnabled
    QString active_SP_MinLength; // This is the active policy always. It pulls from one of the above variables.
    QString active_SP_Complexity; // This is the active policy always. It pulls from one of the above variables.
    QString Clean_String(QString str);
    QString List_User_Identifier(QString name);
    QString List_SamAccountName(QString name);
    QString List_User_OU_CN(QString name);
    QString List_User_OU_DN(QString name);
    QString User_Exists(QString SamName);
    QString Run_Azure_Sync(bool var);
    QString List_ActiveSP_Complexity();
    QString List_ActiveSP_length();
    QString List_User_DN(QString name);
    QString List_Mail(QString name);


    void List_Password_Policy(QString name);
    void load_domain_information();
    void Set_FGPP_active(QString MinLength, QString Complexity);
    void Set_DDPP_active(QString MinLength, QString Complexity);
    void Set_APP_active(QString MinLength, QString Complexity);
    void Move_ADUser_Orgranizational_Unit(QString User_DN, QString Template_OU_Distinguished);
    void Dump_User_Form(QString data, QUrl image_path, QString name);

    bool Employee_Name_Exists(QStringList names, QString new_name);
    bool Validate_Password(QString pword, QString MinPasswordLength, QString ComplexityEnabled);
    bool Get_Azure_Status();
    bool Validate_User_Status(QString template_name);

    QStringList all_forests; // All forests found in the domain.
    QStringList all_upns; // All UPNs found in the domain.
    QStringList ou_names; // All cleaned organizational names in the domain.
    QStringList ou_dn_names; // All distinguished names of all the organizational units found in the domain.
    QStringList user_group_cns; // All cleaned group names of the template user.
    QStringList user_group_dns; // All distinguished names of the template users groups.
    QStringList all_group_dns; // All group distinguished names in the domain.
    QStringList all_group_cns; // All clean group names in the domain.
    QStringList all_users; // All users found in the domain.
    QStringList FGPP_Names;                 // Fine Grained Password Policy names
    QStringList FGPP_AppliesTo;          // Fine Grained Password Policy AppliesTo
    QStringList List_All_Forests();
    QStringList List_All_UPNs();
    QStringList List_All_OU_CNs();
    QStringList List_All_OU_DNs();
    QStringList List_All_Domain_Users();
    QStringList List_All_Group_CNs();
    QStringList List_ALL_Group_DNs();
    QStringList List_User_Group_CNs(QString SamName);
    QStringList List_User_Group_DNs(QString SamName);





    int FGPP_MatchGrainPolicy;         // Fine Grained Password Policy index positon of matching policy name in FGPP_Names
    int ou_cn_selection; // The CN name to be the index of the ou cn combo box.
    int upn_selection; // The upn to be the index of the upn combo box
};

#endif // DOMAININTEGRATION_H
