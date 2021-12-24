#ifndef EDITWIDGET_H
#define EDITWIDGET_H

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

class EditWidget : public QObject, PSIntegration
{
    Q_OBJECT

public:
    EditWidget();

    QWidget *get_widget();

    QComboBox *upn_combo;
    QComboBox *ou_combo;
    QComboBox *user_combo;
    QComboBox *forest_combo;
    QComboBox *current_groups_combo;
    QLineEdit *username_edit;
    QLineEdit *password_edit;
    QLineEdit *email_edit;
    QLineEdit *displayname_edit;
    QLineEdit *primary_proxy_edit;
    QLineEdit *secondary_proxy_edit;
    QLineEdit *givenname_edit;
    QLineEdit *surname_edit;
    QPushButton *edit_button;
    QPushButton *cancel_button;
    QPushButton *load_button;
    QTextBrowser *informational;

private:
    QWidget* initalize_widget(QComboBox *upn_combo, QComboBox *ou_combo, QComboBox *user_combo,
                              QComboBox *forest_combo, QLineEdit *username_edit,QLineEdit *password_edit,
                              QLineEdit *email_edit, QLineEdit *displayname_edit, QLineEdit *primary_proxy_edit,
                              QLineEdit *secondary_proxy_edit, QPushButton *edit_button, QLineEdit *givenname_edit,
                              QLineEdit *surname_edit, QComboBox *current_groups_combo, QPushButton *cancel_button,
                              QPushButton *load_button, QTextBrowser *informational);
};

#endif // EDITWIDGET_H
