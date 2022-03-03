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
        da_template: da_template_combo.currentText
        da_fname: da_name_edit.text
        da_uname: da_username_edit.text
        da_email: da_emailaddress_edit.text
        da_displayname: da_displayname_edit.text
        da_password: da_password_edit.text
        da_pproxy: da_primaryproxy_edit.text
        da_sproxy: da_secondaryproxy_edit.text
        da_upnselect: da_template_combo.currentIndex
        da_ouselect: da_template_combo.currentIndex

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
            id: da_upn_combo
            Layout.preferredWidth: 400
            Layout.preferredHeight:  40
            visible: false
            model: domain_integration.da_allupns

        }

        ComboBox
        {
            id: da_ou_combo
            Layout.preferredWidth: 400
            Layout.preferredHeight:  40
            visible: false
            model: domain_integration.da_alloucns
        }

        ComboBox
        {
            id: da_template_combo
            Layout.preferredWidth: 400
            Layout.preferredHeight:  40
            model: domain_integration.da_allusers
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

        }

        Button
        {
            id: da_generate_button
            text: "Generate"
            Layout.preferredWidth: 400
            Layout.preferredHeight:  40
            onClicked:
            {
                da_username_edit.visible = true;
                da_password_edit.visible = true;
                da_emailaddress_edit.visible = true;
                da_upn_combo.visible = true;
                da_ou_combo.visible = true;
                domain_integration.automate();
                da_username_edit.text = domain_integration.da_uname
                da_emailaddress_edit.text = domain_integration.da_email
                da_displayname_edit.text = domain_integration.da_displayname
                da_upn_combo.currentIndex = domain_integration.da_upnselect
                da_ou_combo.currentIndex = domain_integration.da_ouselect
                da_password_edit.placeholderText = "Min length: " + domain_integration.da_lengthpolicy + " Complexity on: " + domain_integration.da_complexitypolicy
            }
        }

        Button
        {
            id: da_create_button
            text: "Create"
            Layout.preferredWidth: 400
            Layout.preferredHeight: 40
            onClicked:
            {
                da_viewport.text = domain_integration.create_domain_account();
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
