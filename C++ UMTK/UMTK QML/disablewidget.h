#ifndef DISABLEWIDGET_H
#define DISABLEWIDGET_H

#include <QObject>
#include <QComboBox>
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>
#include "psintegration.h"


class DisableWidget : public QObject, PSIntegration
{
    Q_OBJECT
public:
    DisableWidget();

    QWidget* get_widget();


    QComboBox *user_selection;
    QPushButton *disable_button;
    QPushButton *cancel_button;
    QTextBrowser *informational;

private:


    QWidget* initalize_widget(QComboBox *user_selection, QPushButton *disable_button, QPushButton *cancel_button, QTextBrowser *informational);
};

#endif // DISABLEWIDGET_H
