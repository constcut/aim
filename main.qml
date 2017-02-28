import QtQuick 2.4
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3


ApplicationWindow {
    id: window
    title: qsTr("Aim")
    width: 480
    height: 800
    visible: true

    Component.onCompleted: {
    }

   header:  ToolBar { //or header
            id: toolBar
        RowLayout
        {
           // anchors.fill: parent
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
