#include <QObject>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QQuickWindow>
#include <QQuickView>
#include <QUrl>
#include "psintegration.h"
#include "domainintegration.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<PSIntegration>("com.elysianproductions.psintegration", 1, 0, "PSIntegration");
    qmlRegisterType<DomainIntegration>("com.elysianproductions.domainintegration", 1, 0, "DomainIntegration");


    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/UMTKQML/main.qml"_qs);


    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
