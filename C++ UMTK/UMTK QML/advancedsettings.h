#ifndef ADVANCEDSETTINGS_H
#define ADVANCEDSETTINGS_H

#include <QObject>
#include <QString>
#include <QFile>




class AdvancedSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString get_ou_dn READ get_ou_dn WRITE setOUDName NOTIFY _DNameChanged) // Property 1.
    Q_PROPERTY(QString get_company_name READ get_company_name WRITE setCompanyName NOTIFY _CompanyNameChanged) // Property 2.
    Q_PROPERTY(QString get_user_prefix READ get_user_prefix WRITE setUserPrefix NOTIFY _UserPrefixChanged) // Property 3.

public:
    AdvancedSettings();

    QString get_ou_dn();         // OU distinguished name, pulls from the QML component dn_name_edit. See Q_PROPERTY labeled Property 1.
    QString get_company_name(); // Company name, pulls from the QML component company_name_edit. See Q_PROPERTY labeled Property 2.
    QString get_user_prefix(); // User prefix, pulls from the QML component user_prefix_edit. See Q_PROPERTY labeled Property 3.

    void setOUDName(const QString &ou_dn);
    void setCompanyName(const QString &c_name);
    void setUserPrefix(const QString &u_prefix);
    void create_database(QString path);

    bool dbExists(QString path);


protected:

public slots:
    void insert_data(const QString &ou, const QString &company, const QString &user);

signals:
    void _DNameChanged();
    void _CompanyNameChanged();
    void _UserPrefixChanged();

private:
    QString user_prefix;
    QString company_name;
    QString distinguished_name;

};

#endif // ADVANCEDSETTINGS_H
