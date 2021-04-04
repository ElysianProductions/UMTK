#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QObject>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QWidget>
#include <QGridLayout>
#include <QString>
#include <QStringList>
#include <QProcess>


class ServerWidget : public QObject
{
    Q_OBJECT
public:
    ServerWidget();
    QWidget* create_widget();

    QComboBox *upn_combo;
    QComboBox *ou_combo;
    QComboBox *cloud_combobox;
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

    QStringList commands;
    QStringList cloud_clients;
    QStringList cloud_prefixes;
    QStringList ad_users;
    QStringList ou_clean_names;
    QStringList ou_distinguished_names;
    QStringList ad_group_membership;
    QStringList ad_upn_sufffixes;
    QStringList ad_domain_names;

private:
    QStringList execute_command(QString param);
    QWidget* initalize_widget(QComboBox *upn_combo, QComboBox *ou_combo, QComboBox *cloud_combobox, QComboBox *template_user_combo, QComboBox *domain_name_combo, QLineEdit *employee_name_edit, QLineEdit *user_edit, QLineEdit *password_edit, QLineEdit *email_edit, QLineEdit *display_name_edit, QLineEdit *primary_proxy_edit, QLineEdit *secondary_proxy_edit, QPushButton *create_button, QPushButton *delete_button);




};

#endif // SERVERWIDGET_H
