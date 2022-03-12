#ifndef CONFIGURATIONWIDGET_H
#define CONFIGURATIONWIDGET_H

#include <QObject>
#include <QListWidget>
#include <QListView>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QButtonGroup>
#include <QSpacerItem>
#include <QComboBox>
#include <QTableView>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QHeaderView>

#include <QDebug>

class ConfigurationWidget : public QObject
{
    Q_OBJECT
public:
    ConfigurationWidget();
    ~ConfigurationWidget();
    QWidget* get_widget();

    QStackedWidget *menus_widget;
    QListWidget *menu_options_widget;
    QPushButton *close_button;
    QLineEdit *user_creation_text_edit;
    QLineEdit *user_disable_text_edit;
    QLineEdit *image_path_edit;
    QTableView *company_table;

    void setCompanyName(const QString &company);
    void setOUCNName(const QString &cn);
    void setUserPRefix(const QString &prefix);



    QString ou_name();
    QString company_name();
    QString prefix_name();

protected:



signals:

    void _OUChanged();
    void _CompanyChanged();
    void _PrefixChanged();

public slots:
    void swap_menu(QListWidgetItem *selected, QListWidgetItem *previous);

private:
     QWidget* get_menu_widget(QPushButton *close_button);
     QWidget* get_company_custimization_widget();
     QWidget* get_pdf_custimization_widget(QLineEdit *user_creation_text_edit, QLineEdit *user_disable_text_edit, QLineEdit *image_path_edit);
     QWidget* get_generation_custimization_widget();

     QString _oucn;
     QString _company;
     QString _prefix;

     QSqlDatabase db;
     QSqlQuery *query;
     QSqlQueryModel *model;

};

#endif // CONFIGURATIONWIDGET_H
