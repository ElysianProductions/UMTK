#ifndef PSINTEGRATION_H
#define PSINTEGRATION_H

#include <QObject>
#include <QProcess>
#include <QString>
#include <QStringList>


#include <QDebug>

class PSIntegration : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString la_username READ la_username WRITE setLAUsername NOTIFY la_userNChanged) // Property 1.
    Q_PROPERTY(QString la_fullname READ la_fullname WRITE setLAFullname NOTIFY la_userFNChanged) // Property 2.
    Q_PROPERTY(QString la_password READ la_password WRITE setLAPassword NOTIFY la_userPChanged) // Property 3.
    Q_PROPERTY(bool is_admin READ is_admin WRITE setLAAdmin NOTIFY la_AdminStatusChanged) // Property 4.


public:
    PSIntegration();

    void setLAUsername(const QString &laun); // Set local account username.
    void setLAFullname(const QString &lafn); // Set local account fullname.
    void setLAPassword(const QString &lapass); // Set local account fullname.
    void setLAAdmin(const bool &laadmin); // Local acocunt as administrator.


    QString la_username();   // See QProperty labeled Property 1.
    QString la_fullname();  // See Qproperty labeled Property 2.
    QString la_password(); // See QProperty labeled Property 3.
    bool is_admin(); // See QProperty labeled Property 4.

protected:


public slots:
    QString createLocalAccount(); // create local account slot.
    //void createDomainAccount(); // create domain account slot.
   // void disableDomainAccount(); // disable domain slot.

signals:
    void la_userNChanged();  // Local Account Username Text changed.
    void la_userFNChanged(); // Local Account Full name Text changed.
    void la_userPChanged();  // Local Account Password Text changed.
    void la_AdminStatusChanged();   // Local Account Administrator check box status.

private:
    QString la_uname; // local account username.
    QString la_fname; // local account full name.
    QString la_pass; // local account password;
    bool la_admin; // Local acocunt administrator.

    QString Execute(QString param); //


};

#endif // PSINTEGRATION_H
