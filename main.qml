import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3

import QtQuick.Controls.Material 2.1

ApplicationWindow {
    id: window
    title: qsTr("Aim")
    width: 480
    height: 800
    visible: true

    Material.accent: Material.Green
    //Material.theme: Material.Dark

    Component.onCompleted: {
    }

   header:  ToolBar { //or header
            id: toolBar
        RowLayout
        {
           // anchors.fill: parent

            //old way will remove soon
            /*
            ToolButton
            {
                text: "Add aim"
                onClicked: mainLoader.source = "aimAdd.qml"
            }
            ToolButton
            {
                text: "View aims"
                onClicked: mainLoader.source = "aimView.qml"
            }
            */

            //vertical position not so well - need to absolute it

            ToolButton
            {
                text: "Aims"
                onClicked: mainLoader.source = "aims.qml"
            }
            ToolButton
            {
                text: "User settings"
                onClicked:  drawerLeft.open()
            }
            ToolButton
            {
                text: "Chats"
                onClicked:  drawerDown.open()
            }

            ToolButton
            {
                text: "Dark"
                onClicked: window.Material.theme = Material.Dark
            }
            ToolButton
            {
                text: "Light"
                onClicked: window.Material.theme = Material.Light
            }

            //http://doc.qt.io/qt-5/qml-qtquick-controls2-toolbar.html
        }
    }

    Loader {
        y: toolBar.height + 5
        anchors.centerIn: parent
        id: mainLoader
        source: "aims.qml" //hello by default
    }

    Drawer {
           id: drawerLeft
           width: 0.66 * window.width
           height: window.height

           ColumnLayout
           {
           Button
           {
               text: "Check button"
           }
           TextEdit
           {
               text: "Sometext"
           }
           }
       }


    Drawer {
           id: drawerDown
           width: window.width
           height: 0.5*window.height

           edge: Qt.BottomEdge

           ColumnLayout
           {
           Button
           {
               text: "Check button1"
           }
           TextEdit
           {
               text: "Sometext1"
           }
           }
       }

}
