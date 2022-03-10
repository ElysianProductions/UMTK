import QtQuick 2.5
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3


Window {
    id: start_page
    width: 640
    height: 480
    visible: true
    title: qsTr("UMTK: User Maintenance Tool Kit")
    color: "light blue"

    Loader
    {
        id: load_page
    }


    ColumnLayout
    {
        id: start_page_layout
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter


        Button
        {
            id: domain_account
            text: "Create domain user"
            Layout.preferredWidth: 150
            Layout.preferredHeight:  40
            onClicked:
            {
                start_page.close();
                load_page.source = "DomainAccountWidget.qml"
            }

        }

        Button
        {
            id: local_account
            text: "Create local user"
            Layout.preferredWidth: 150
            Layout.preferredHeight:  40
            onClicked:
            {
                start_page.close();
                load_page.source = "LocalAccountWidget.qml"
            }

        }

        Button
        {
            id: disable_account
            text: "Disable domain account"
            Layout.preferredWidth: 150
            Layout.preferredHeight: 40
            onClicked:
            {
                start_page.close();
                load_page.source = "DisableAccountWidget.qml"
            }

        }

        Button
        {
            id: advanced_settings
            text: "Advanced settings"
            Layout.preferredWidth: 150
            Layout.preferredHeight: 40
            visible: false
            onClicked:
            {
                start_page.close();
                load_page.source = "AdvancedSettingsWidget.qml"
            }

        }


    }


}
