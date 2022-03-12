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

class ConfigurationWidget : public QObject
{
    Q_OBJECT
public:
    ConfigurationWidget();
    QWidget* get_widget();

    QStackedWidget *menus_widget;
    QListWidget *menu_options_widget;
    QPushButton *close_button;
    QLineEdit *user_creation_text_edit;
    QLineEdit *user_disable_text_edit;
    QLineEdit *image_path_edit;

protected:

signals:


public slots:
    void swap_menu(QListWidgetItem *selected, QListWidgetItem *previous);

private:
     QWidget* get_menu_widget(QPushButton *close_button);
     QWidget* get_company_custimization_widget();
     QWidget* get_pdf_custimization_widget(QLineEdit *user_creation_text_edit, QLineEdit *user_disable_text_edit, QLineEdit *image_path_edit);
     QWidget* get_generation_custimization_widget();

};

#endif // CONFIGURATIONWIDGET_H
