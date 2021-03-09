#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QTextBrowser>
#include <QStringList>
#include <QProcess>
#include <QGridLayout>
#include <QRegExp>
#include <QDebug>

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget();

    QWidget* initalize_widget();

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
    QPushButton *create_button;
    QPushButton *delete_button;

    QStringList commands;
    QStringList cloud_clients;
    QStringList cloud_suffixes;




    QStringList ad_users;
    QStringList ou_clean_names;
    QStringList ou_distinguished_names;
    QStringList ad_group_membership;
    QStringList ad_upn_sufffixes;
    QStringList ad_domain_names;

private:
    QStringList execute_command(QString param);
    QWidget* create_widget(QComboBox *upn_combo, QComboBox *ou_combo, QComboBox *cloud_combobox, QComboBox *template_user_combo, QComboBox *domain_name_combo, QLineEdit *employee_name_edit, QLineEdit *user_edit, QLineEdit *password_edit, QLineEdit *email_edit, QLineEdit *display_name_edit, QPushButton *create_button, QPushButton *delete_button);
};

#endif // MAINWIDGET_H
