import QtQuick 2.15
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import Qt.labs.qmlmodels 1.0
import com.elysianproductions.advancedsettings 1.0

Window {
    id: advanced_Settings_page
    width: 1024
    height: 768
    visible: true
    title: qsTr("UMTK: Advanced Configuration")
    color: "light blue"

    AdvancedSettings
    {
        id: advanced_settings
        get_ou_dn: dn_name_edit.text
        get_company_name: company_name_edit.text
        get_user_prefix: user_prefix_edit.text
    }

    Loader
    {
        id: load_page
    }

    ColumnLayout
    {
        id: settings_page_layout_one
        TableView
        {
            Layout.preferredWidth: 1000
            Layout.preferredHeight: 200
            columnSpacing: 1
            rowSpacing: 1
            clip: true
            visible: false

            model: TableModel
            {
                TableModelColumn {display: "DistinguishedName"}
                TableModelColumn {display: "CompanyName"}
                TableModelColumn {display: "UserPrefix"}

                rows: [
                    {
                        //"DistinguishedName": "OU=EP Users,OU=Elysian Productions,OU=Client OUs,DC=ElysianProductions,DC=local",
                        //"CompanyName": "Elysian Productions",
                        //"UserPrefix": "EP_"
                    },
                    {
                        //"DistinguishedName": "OU=EP Administrators,OU=Elysian Productions,OU=Client OUs,DC=ElysianProductions,DC=local",
                        //"CompanyName": "Elysian Productions",
                        //"UserPrefix": "EP_"
                    }

                ]

            }
            delegate: Rectangle
            {
                    implicitWidth: 500
                    implicitHeight: 50
                    border.width: 1

                    Text
                    {
                        text: display
                        //anchors.centerIn: parent
                    }
                }

        }

    }

    ColumnLayout
    {
        id: settings_page_layout_two
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter


        TextField
        {
            id: dn_name_edit
            placeholderText: "Enter OU distinguished name"
            Layout.preferredWidth: 500
            Layout.preferredHeight:  40
        }

        TextField
        {
            id: company_name_edit
            placeholderText: "Enter Company name"
            Layout.preferredWidth: 500
            Layout.preferredHeight:  40
            visible: true
        }

        TextField
        {
            id: user_prefix_edit
            placeholderText: "Enter User prefix"
            Layout.preferredWidth: 500
            Layout.preferredHeight:  40
            visible: true
        }

        Button
        {
            id: input_button
            text: "Input data"
            Layout.preferredWidth: 500
            Layout.preferredHeight:  40
            onClicked:
            {
                advanced_settings.insert_data(dn_name_edit.text, company_name_edit.text, user_prefix_edit.text);
            }
        }

        Button
        {
            id: cancel_button
            text: "Cancel"
            Layout.preferredWidth: 500
            Layout.preferredHeight: 40
            onClicked:
            {

            }
        }

    }

}
