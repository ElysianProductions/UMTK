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
    QTableView *company_table;

    // Company widget items
    QPushButton *c_insert_button;
    QLineEdit *c_ou_edit;
    QLineEdit *c_company_edit;
    QLineEdit *c_prefix_edit;
    QComboBox *c_sam_combo;
    QCheckBox *c_enable_button;
    //

    //PDF Widgets
    QLineEdit *user_creation_text_edit;
    QLineEdit *user_disable_text_edit;
    QLineEdit *image_path_edit;
    QComboBox *company_logo_position;
    //


    //Disable Settings widgets
    QLineEdit *folder_redirection_edit;
    QLineEdit *profile_storage_edit;

    //
    void setCompanyName(const QString &company);
    void setOUCNName(const QString &cn);
    void setUserPrefix(const QString &prefix);
    void setSamSetting(const int &samsetting);
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

     QString database_name;
     QString _oucn;
     QString _company;
     QString _prefix;

     int _samsetting;


     QSqlQueryModel *model;
     QSqlDatabase get_database();

     bool initalize_database(const QString &db_path);

     void refresh_model();
     void setSingleEnvSamStyle(const int &option);
     void setMultiCompanyStatus(const bool &status);
     void setNewUserText();
     void setDisableUserText();
     void setImagePath();
     void setLogoPosition(const int &index);
     void setProfileCleanupSettings(const bool &state);
     void setProfileRedirectionPath();
     void setProfileStoragePath();

     QSettings *MultiCompanySettings;
     QSettings *SamGenerationSettings;
     QSettings *PDFSettings;
     QSettings *DisableSettings;

};

#endif // CONFIGURATIONWIDGET_H
