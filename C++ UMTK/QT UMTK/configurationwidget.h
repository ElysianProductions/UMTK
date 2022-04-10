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
#include <QSettings>

#include <QDebug>
#include <QStandardItemModel>
#include <QTreeView>
#include "psintegration.h"

class ConfigurationWidget : public PSIntegration
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

    // Company widget items
    QPushButton *c_insert_button;
    QLineEdit *c_ou_edit;
    QLineEdit *c_company_edit;
    QLineEdit *c_prefix_edit;
    QComboBox *c_sam_combo;
    QCheckBox *c_enable_button;
    //

    void setCompanyName(const QString &company);
    void setOUCNName(const QString &cn);
    void setUserPrefix(const QString &prefix);
    void setSamSetting(const int &samsetting);
    void setMultiCompanyStatus(const bool &status);

    void runQuery(const QString &OU, const QString &company, const QString &prefix, const int &sam_selection);


    QString ou_name();
    QString company_name();
    QString prefix_name();

    int sam_setting();

protected:



signals:

    void _OUChanged();
    void _CompanyChanged();
    void _PrefixChanged();
    void _SamSettingChanged();
    void _QueryRan();

public slots:
    void swap_menu(QListWidgetItem *selected, QListWidgetItem *previous);
    void setupQuery();

private:
     QWidget* get_menu_widget(QPushButton *close_button);
     QWidget* get_company_custimization_widget(QPushButton *c_insert_button, QLineEdit *c_ou_edit, QLineEdit *c_company_edit, QLineEdit *c_prefix_edit, QComboBox *c_sam_combo);
     QWidget* get_pdf_custimization_widget(QLineEdit *user_creation_text_edit, QLineEdit *user_disable_text_edit, QLineEdit *image_path_edit);
     QWidget* get_generation_custimization_widget();
     QWidget* get_disable_custimation_widget();
     QWidget* getHelpWidget();


     QString _oucn;
     QString _company;
     QString _prefix;

     int _samsetting;


     QSqlQueryModel *model;

     bool initalize_database(const QString &db_path);
     QSqlDatabase get_database();
     void refresh_model();
     QString database_name;


     QSettings *MultiCompanySettings;
};

#endif // CONFIGURATIONWIDGET_H
