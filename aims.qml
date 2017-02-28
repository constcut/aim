import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2 //avoid it too
import QtQuick.Extras 1.4  //avoid it please
import QtQuick.Layouts 1.3 //isn't there 2?

Item {

    id: aimViewWindow
    width: 480
    height: 800

    property int elementHeight: 45
    property int widthOffset: 70
    property int yOffset: 50 //be aware of low screens try everywhere


    PageIndicator {
        id: pageIndicator
        interactive: true
        count: view.count
        currentIndex: view.currentIndex

        y: yOffset

        //anchors.bottom: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
    }

    SwipeView {
        id: view

        y: yOffset + yOffset/2
        x: 0

        width: parent.width
        height: parent.height


        currentIndex: 1
        //anchors.fill: parent

        Item {
            id: firstPage

            Button {
                text: "Text1"
            }
        }
        Item {
            id: secondPage

            Button {
                text: "Text2"
            }
        }
        Item {
            id: thirdPage

            Button {
                text: "Text3"
            }
        }
    }



}
