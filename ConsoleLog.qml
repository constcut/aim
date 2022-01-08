import QtQuick 2.0
import wwav.es.app 1.0

Item {
    anchors.fill: parent

    //Later should make it nicer, scrollable :)

    //also can open as window - means sepparated (not on android) - for easier debugging

    id:item
        ConsoleLog
        {
            id:consoleLog

            y: 100

            width: parent.width
            height: parent.height

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    consoleLog.update()
                }
            }
            //DOUBLE click may also make and input - that would lead to console loging too
        }

        Timer {
            id: refTimer
            interval: 1000
            onTriggered: {
                consoleLog.update()
            }
            repeat: true
            running: false
        }

        Component.onCompleted:{
            console.log("Loaded console main item")
            refTimer.running = true
        }

}
