import QtQuick 2.4
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3




ApplicationWindow {
    title: qsTr("Aim")
    width: 480
    height: 800
    visible: true

    Component.onCompleted: {
        //datePopup.open()
    }

   footer:  ToolBar { //or header
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
        anchors.centerIn: parent
        id: mainLoader
        source: "aimView.qml" //hello by default
    }




}
