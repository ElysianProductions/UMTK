#ifndef MAINWIDGET_H
#define MAINWIDGET_H


#include <QObject>
#include <QVBoxLayout>
#include <QPushButton>

class MainWidget : public QObject
{
    Q_OBJECT
public:
    MainWidget();
    QWidget* get_widget();

    QPushButton *server_button;
    QPushButton *local_button;
    QPushButton *edit_user_button;
    QPushButton *disable_user_button;



private:
     QWidget* initalize_widget(QPushButton *server_button, QPushButton *local_button, QPushButton *edit_user_button, QPushButton *disable_user_button);
};

#endif // MAINWIDGET_H
