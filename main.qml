import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3

import QtQuick.Controls.Material 2.1

ApplicationWindow {
    id: window
    title: qsTr("Aim")
    width: 480 //screenGlobal.getScreenWidth();
    height: 800 //screenGlobal.getScreenHeight();
    visible: true

    Material.accent: userSettings.getColor("Accent");
    Material.primary: userSettings.getColor("Primary");
    Material.foreground: userSettings.getColor("Foreground");
    Material.background: userSettings.getColor("Background");
    //Theme - for a while not supported because then we had to change everything

    //Material elevation

    Component.onCompleted: {
        console.log (window.Material.foreground)
    }

   header:  ToolBar
   {
            id: toolBar
        RowLayout
        {
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
