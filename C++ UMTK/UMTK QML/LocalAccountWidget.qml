import QtQuick 2.5
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import com.elysianproductions.psintegration 1.0

Window {
    id: local_account_page
    width: 640
    height: 480
    visible: true
    title: qsTr("UMTK: Create local account")
    color: "light blue"


    Loader
    {
        id: load_page
    }

    PSIntegration
    {
        id: psintegration
        la_username: la_username_input.text
        la_fullname: la_name_input.text
        la_password: la_password_input.text
        is_admin: la_is_admin_button.checkState
    }


    ColumnLayout
    {
        id: local_page_layout
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter


        TextField
        {
            id: la_username_input
            placeholderText: "Input username"
            Layout.preferredWidth: 200
            Layout.preferredHeight:  40
        }

        TextField
        {
            id: la_name_input
            placeholderText: "Input full name"
            Layout.preferredWidth: 200
            Layout.preferredHeight:  40
        }

        TextField
        {
            id: la_password_input
            placeholderText: "Input password"
            Layout.preferredWidth: 200
            Layout.preferredHeight:  40
            echoMode: TextInput.Password
        }

        CheckBox
        {
            id: la_is_admin_button
            text: "Administrator"
        }


        Button
        {
            id: la_ca_button
            signal requestLocalAccountCreation(string name, string username, string password, bool ischecked)
            objectName: "la_ca_button"
            text: "Create"
            Layout.preferredWidth: 200
            Layout.preferredHeight:  40
            onClicked:
            {
                la_message_box.visible = true
                la_message_box.text = psintegration.createLocalAccount();

            }

        }

        Button
        {
            id: la_cancel_button
            text: "Cancel"
            Layout.preferredWidth: 200
            Layout.preferredHeight:  40
            onClicked:
            {
                local_account_page.close();
                load_page.source = "main.qml"
            }
        }


        Label
        {
            id: la_message_box
            Layout.preferredWidth: 200
            Layout.preferredHeight:  40
            visible: false
        }

    }


}
