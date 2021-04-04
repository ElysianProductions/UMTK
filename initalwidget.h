#ifndef INITALWIDGET_H
#define INITALWIDGET_H

#include <QObject>
#include <QPushButton>
#include <QVBoxLayout>


class InitalWidget : public QObject
{
    Q_OBJECT
public:
    InitalWidget();

    QWidget* create_widget();


    QPushButton *server_button;
    QPushButton *local_button;
    QVBoxLayout *primary_layout;

public slots:



private:

    QWidget* initalize_widget(QPushButton *server_button, QPushButton *local_button, QVBoxLayout *layout);

};

#endif // INITALWIDGET_H
