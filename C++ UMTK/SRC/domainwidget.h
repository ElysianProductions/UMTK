#ifndef DOMAINWIDGET_H
#define DOMAINWIDGET_H

#include <QObject>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QWidget>
#include <QGridLayout>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QTextBrowser>


#include "psintegration.h"

class DomainWidget : public QObject, PSIntegration
{
    Q_OBJECT

public:
    DomainWidget();
    QWidget *get_widget();



    QComboBox *upn_combo;
    QComboBox *ou_combo;
    QComboBox *template_user_combo;
    QComboBox *domain_name_combo;
    QLineEdit *employee_name_edit;
    QLineEdit *user_edit;
    QLineEdit *password_edit;
    QLineEdit *email_edit;
    QLineEdit *display_name_edit;
    QLineEdit *primary_proxy_edit;
    QLineEdit *secondary_proxy_edit;
    QPushButton *create_button;
    QPushButton *cancel_button;
    QPushButton *generate_button;
    QPushButton *advanced_button;
    QTextBrowser *informational;

    QStringList commands;


    void load_domain_information();

    QStringList AD_Users;
    QStringList Domain_UPNS;
    QStringList OU_Names;
    QStringList OU_DN_Names;
    QStringList Domain_Name;

private:

    QWidget* initalize_widget(QComboBox *upn_combo, QComboBox *ou_combo, QComboBox *template_user_combo,
                              QComboBox *domain_name_combo, QLineEdit *employee_name_edit, QLineEdit *user_edit,
                              QLineEdit *password_edit, QLineEdit *email_edit, QLineEdit *display_name_edit,
                              QLineEdit *primary_proxy_edit, QLineEdit *secondary_proxy_edit, QPushButton *create_button,
                              QPushButton *cancel_button, QPushButton *generate_button, QTextBrowser *informational, QPushButton *advanced_button);
};

#endif // DOMAINWIDGET_H
