import QtQuick 2.5
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import com.elysianproductions.domainintegration 1.0

Window {
    id: domain_account_page
    width: 1024
    height: 768
    visible: true
    title: qsTr("UMTK: Create domain account")
    color: "light blue"

    DomainIntegration
    {
        id: domain_integration
    }

    Loader
    {
        id: load_page
    }


    ColumnLayout
    {
        id: domain_page_layout
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter


        ComboBox
        {
            id: da_template_combo
            Layout.preferredWidth: 400
            Layout.preferredHeight:  40
        }

        TextField
        {
            id: da_name_edit
            placeholderText: "Enter full name"
            Layout.preferredWidth: 400
            Layout.preferredHeight:  40
        }

        TextField
        {
            id: da_username_edit
            placeholderText: "Enter username"
            Layout.preferredWidth: 400
            Layout.preferredHeight:  40
            visible: false
        }

        TextField
        {
            id: da_emailaddress_edit
            placeholderText: "Enter email"
            Layout.preferredWidth: 400
            Layout.preferredHeight:  40
            visible: false
        }


        TextField
        {
            id: da_displayname_edit
            placeholderText: "Enter display name"
            Layout.preferredWidth: 400
            Layout.preferredHeight:  40
            visible: false
        }

        TextField
        {
            id: da_primaryproxy_edit
            placeholderText: "Enter primary proxy address"
            Layout.preferredWidth: 400
            Layout.preferredHeight:  40
            visible: false
        }

        TextField
        {
            id: da_secondaryproxy_edit
            placeholderText: "Enter secondary proxy"
            Layout.preferredWidth: 400
            Layout.preferredHeight:  40
            visible: false
        }

        TextField
        {
            id: da_password_edit
            placeholderText: "Enter password"
            Layout.preferredWidth: 400
            Layout.preferredHeight:  40
            visible: false
            echoMode: TextInput.Password
        }

        Button
        {
            id: da_generate_button
            text: "Generate"
            Layout.preferredWidth: 400
            Layout.preferredHeight:  40
            onClicked:
            {
                // Add automate function
                da_username_edit.visible = true;
                da_password_edit.visible = true;
            }
        }

        Button
        {
            id: da_cancel_button
            text: "Cancel"
            Layout.preferredWidth: 400
            Layout.preferredHeight:  40
            onClicked:
            {
                domain_account_page.close();
                load_page.source = "main.qml"
            }
        }


        Button
        {
            id: da_advanced_button
            text: "Advanced options"
            Layout.preferredWidth: 400
            Layout.preferredHeight:  40
            onClicked:
            {
                if(da_displayname_edit.visible && da_primaryproxy_edit.visible && da_secondaryproxy_edit.visible)
                {
                    da_displayname_edit.visible = false;
                    da_primaryproxy_edit.visible = false;
                    da_secondaryproxy_edit.visible = false;
                }
                else if(!da_displayname_edit.visible && !da_primaryproxy_edit.visible && !da_secondaryproxy_edit.visible)
                {
                    da_displayname_edit.visible = true;
                    da_primaryproxy_edit.visible = true;
                    da_secondaryproxy_edit.visible = true;
                }
            }
        }

        TextArea
        {
            id: da_viewport
            readOnly: true
            Layout.preferredWidth: 400
            Layout.preferredHeight:  200
        }

    }


}
