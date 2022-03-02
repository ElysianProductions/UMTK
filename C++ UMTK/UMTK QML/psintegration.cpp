#include "psintegration.h"

PSIntegration::PSIntegration()
{

}

void PSIntegration::setLAUsername(const QString &laun)
{
    if(laun != la_uname)
    {
        la_uname = laun;
        emit la_userNChanged();
    }
}

void PSIntegration::setLAFullname(const QString &lafn)
{
    if(lafn != la_fname)
    {
        la_fname = lafn;
        emit la_userFNChanged();
    }
}

void PSIntegration::setLAPassword(const QString &lapass)
{
    if(lapass != la_pass)
    {
        la_pass = lapass;
        emit la_userPChanged();
    }
}

void PSIntegration::setLAAdmin(const bool &laadmin)
{
    if(laadmin != la_admin)
    {
        la_admin = laadmin;
        emit la_AdminStatusChanged();
    }
}

QString PSIntegration::createLocalAccount()
{
    if(la_username().length() < 2 || la_fullname().length() < 2 || !la_fullname().contains(" ") || la_password().length() <= 0)
    {
        QString error;
        if(la_username().length() < 2)
        {
            error = "ERROR - The Username " + la_username() + " must be at least 2 characters.\n";
        }

        if(la_fullname().length() < 2)
        {
            error = error + "ERROR - The Fullname " + la_fullname() + " must be more than 2 characters long.\n";
        }
        if(!la_fullname().contains(" "))
        {
             error = error + "ERROR - The Fullname " + la_fullname() + " must contain at least one space.\n";
        }
        if(la_password().length() <= 0)
        {
             error = error + "ERROR - The password cannot be empty.\n";
        }
        return error;
    }
    else if(la_username().length() > 2 && la_fullname().length() > 2 && la_fullname().contains(" ") && la_password().length() > 0)
    {
        if(is_admin())
        {
            QString tmp = "net user " + la_username() + " " + la_password() + " /add";
            Execute(tmp);
            QString tmp_two = "net localgroup Administrators " + la_username() + " /add";
            Execute(tmp_two);
            return "Username: " + la_username() + "\nFull name: " + la_fullname() + "\nPassword: " + la_password() + "\nAdministrator: True";

        }
        else if(!is_admin())
        {
            QString tmp = "net user " + la_username() + " " + la_password() + " /add";
            Execute(tmp);
            return "Username: " + la_username() + "\nFull name: " + la_fullname() + "\nPassword: " + la_password() + "\nAdministrator: False";
        }
    }
}

QString PSIntegration::la_username()
{
    return la_uname;
}

QString PSIntegration::la_fullname()
{
    return la_fname;
}

QString PSIntegration::la_password()
{
    return la_pass;
}


QString PSIntegration::Execute(QString param)
{
    QProcess *process = new QProcess();
    QByteArray success;
    //QByteArray errors;
    QStringList params;
    params = QStringList({"-Command", QString("Start-Process -NoNewWindow -Verb runAs powershell; "), param});
    process->start("powershell", params);
    process->waitForFinished(-1);
    success.append(process->readAllStandardOutput());
    //errors.append(process->readAllStandardError());
    //qDebug() << errors;
    process->terminate();
    QString data = QString(success);
    return data;
}


bool PSIntegration::is_admin()
{
    return la_admin;
}
