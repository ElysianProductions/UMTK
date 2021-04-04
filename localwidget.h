#ifndef LOCALWIDGET_H
#define LOCALWIDGET_H

#include <QObject>
#include <QString>
#include <QProcess>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QCheckBox>

class LocalWidget : public QObject
{
    Q_OBJECT
public:
    LocalWidget();

    QWidget* create_widget();

    QLineEdit *username_edit;
    QLineEdit *fullname_edit;
    QLineEdit *emailaddress_edit;
    QLineEdit *password_edit;

    QPushButton *create_button;
    QPushButton *cancel_button;

    QCheckBox *admin_checkbox;



private:
    QWidget* initalize_widget(QLineEdit *username_edit, QLineEdit *fullname_edit, QLineEdit *emailaddress_edit, QLineEdit *password_edit, QPushButton *create_button, QPushButton *cancel_buton, QCheckBox *admin_checkbox);

};

#endif // LOCALWIDGET_H
