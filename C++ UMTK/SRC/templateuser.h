#ifndef TEMPLATEUSER_H
#define TEMPLATEUSER_H
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QDebug>
#include <QRegularExpression>

#include "psintegration.h"

class TemplateUser : public PSIntegration
{
public:
    TemplateUser();

    QString get_template_user_dn();
    QString get_name();
    QString get_samaccount_name();
    QString get_userprincipal_name();
    QString get_mail();
    QString get_active_password_policy();
    QStringList get_proxy_addresses();
    QStringList get_groups();
    QStringList get_GroupDNs();          // Return all group distinguished names
    QString get_ActiveSP_length();      // Return the active security policy minimum password length
    QString get_ActiveSP_Complexity(); // Return the active security policy complexity
    QString get_OrganizationalUnitDN();
    QString get_OrganizationalUnitCN();

    void set_template_user_dn(QString str);
    void set_name(QString str);
    void set_samaccount_name(QString str);
    void set_userprincipal_name(QStringList UPNs, QStringList Domains, QString name);
    void set_mail(QString str);
    void set_groups(QString str);
    void detect_password_policy(QString name);
    void set_OrganizationalUnitDN(QString name);
    void set_OrganizationalUnitCN(QString name);
private:

    QStringList execute_command(QString param);
    QString execute(QString param);
    QString clean_string(QString str);

    void set_DDPP_active(QString MinLength, QString Complexity);   // Set Default Domain Password Policy as active.
    void set_FGPP_active(QString MinLength, QString Complexity);  // Set Fine Grain Password Policy as active.
    void set_APP_active(QString MinLength, QString Complexity);  // Set Arbitrary Password Policy as active.

    QString user_distinguished_name;
    QString Name;
    QString SamAccountName;
    QString UserPrincipalName;
    QString OU_distinguished_name;
    QString OU_Clean_Name;
    QString Mail;
    QStringList Groups;
    QStringList GroupsDistinguished;

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

#endif // TEMPLATEUSER_H
