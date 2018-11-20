import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3

import QtQuick.Controls 1.4 as QMLOld

import QtQuick.Controls.Styles.Flat 1.0 as Flat

Item {
    id:actionsItem

    anchors.fill: parent

       property int fontSizeMiddle : screenGlobal.adaptYSize(15)

    property int widthOffset : screenGlobal.adaptXSize( 50 )
    property int microOffset : screenGlobal.adaptXSize( 10 )

    signal requestOpenAddAim()
    signal requestOpenEditAim(var aimId)
    signal requestOpenSingleAim(var aimId)

    QMLOld.Calendar{
        y: 0
        x: widthOffset //SHOULD BE REPLACE WITH SOME SCREEN VALUE

        width: parent.width - widthOffset*2
        height: parent.width - widthOffset*2 //special logic for always vertical Application

        id: calendar

        onSelectedDateChanged:{
            chosenDate.text = calendar.selectedDate
            var seconds = localBase.getDoneActionsLength(chosenDate.text)
            summaryText.text = "Total spent " + localBase.secondsTranslate(seconds); // seconds + " : "

            var lists = localBase.getDoneActionsList(chosenDate.text)
            for (var i = 0; i < lists.length; ++i){
                var singleLine = lists[i]
                summaryText.text += "\n" + singleLine;
            }

            summaryText.height = screenGlobal.adaptYSize( parent.width * lists.length / 16 );
            flick.contentHeight = screenGlobal.adaptYSize(  parent.width *  lists.length / 16 );

            //nice to calculate contentWidth also ok later
        }

        Component.onCompleted: {
            calendar.selectedDateChanged()
        }
    }
    Text{
        y: calendar.height
        id: chosenDate
        x: widthOffset
    }



    Flickable {
        id: flick

        x: screenGlobal.adaptYSize( 5 )
        y: calendar.height + screenGlobal.adaptYSize( 30 )

        width: parent.width - widthOffset/2
        height: parent.height - y - screenGlobal.adaptYSize( 30 )

        contentWidth: parent.width * 1.2
        contentHeight: screenGlobal.adaptYSize(parent.height - y) //android dislike- screenGlobal.adaptYSize( 30 )

        ScrollBar.horizontal: ScrollBar { id: hbar; active: vbar.active }
        ScrollBar.vertical: ScrollBar { id: vbar; active: hbar.active }
        //good if we would calculate the width real needed but yet is ok too
        TextArea{
            textFormat: "RichText"
            id: summaryText
            width: parent.width * 2
            height: screenGlobal.adaptYSize( actionsItem.height )
            readOnly:  true

            font.pixelSize: fontSizeMiddle

            onLinkActivated:{
                console.log("Aim link:" + link)
                actionsItem.requestOpenSingleAim(link)
            }
        }
    }

}


