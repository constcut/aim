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


    property bool desktopClient: true ///set somehow from cpp

    //fuck off user colors for a while
    Material.accent: userSettings.getColor("Accent");
    Material.primary: userSettings.getColor("Primary");
    //Material.foreground: userSettings.getColor("Foreground");
    //Material.background: userSettings.getColor("Background");
    Material.theme: Material.Dark

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
                text: "view"
                onClicked: mainLoader.source = "aimView.qml"
            }

            ToolButton
            {
                text: "sched"
                onClicked:  mainLoader.source = "schedule.qml"
            }

            ToolButton
            {
                text: "tree"
                onClicked:  mainLoader.source = "aimTree.qml"
            }

            ToolButton
            {
                text: "tags"
                onClicked:  mainLoader.source = "tagTree.qml"
            }

            ToolButton
            {
                text: "add"
                onClicked:  drawerRight.open()
            }

            //http://doc.qt.io/qt-5/qml-qtquick-controls2-toolbar.html
        }
    }

    Loader {
        y: toolBar.height + 5
        anchors.centerIn: parent
        id: mainLoader
        source: "aimView.qml" //hello by default
    }

    Connections
    {
        target: mainLoader.item
        onRequestOpenAddAim: {

            addLoader.item.loadForAddNew()
            drawerRight.open()
        }
        onRequestOpenEditAim: {
            //LOAD ITEM IN DRAWER

            //console.log("going to edit things " + aimId)

            addLoader.item.loadAimForEdit(aimId)

            drawerRight.open()

        }
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
           id: drawerRight
           width: 0.66 * window.width
           height: window.height

            edge: Qt.RightEdge

           Loader {
               y: 0
               anchors.centerIn: parent
               anchors.fill:  parent
               id: addLoader
               source: "aimAdd.qml"
           }

           Connections
           {
               target: addLoader.item
               onRequestOpenViewAims: {
                   drawerRight.close()
                   mainLoader.item.loadModel()

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
