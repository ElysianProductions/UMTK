import QtQuick 2.5
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import com.elysianproductions.psintegration 1.0

Window {
    id: disable_account_page
    width: 1024
    height: 768
    visible: true
    title: qsTr("UMTK: Disable user account")
    color: "light blue"

    Loader
    {
        id: load_page
    }


    ColumnLayout
    {
        id: disable_page_layout
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter


        ComboBox
        {
            id: da_template_combo
            Layout.preferredWidth: 400
            Layout.preferredHeight:  40
        }

        Button
        {
            id: la_create_button
            text: "Disable"
            Layout.preferredWidth: 400
            Layout.preferredHeight:  40


        }

        Button
        {
            id: la_cancel_button
            text: "Cancel"
            Layout.preferredWidth: 400
            Layout.preferredHeight:  40
            onClicked:
            {
                disable_account_page.close();
                load_page.source = "main.qml"
            }
        }

        TextArea
        {
            id: disable_account_viewport
            readOnly: true
            Layout.preferredWidth: 400
            Layout.preferredHeight:  200
        }


    }


}
