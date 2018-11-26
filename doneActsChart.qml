import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4 as QMLOld
import QtQuick.Controls.Styles.Flat 1.0 as Flat

import wwav.es.app 1.0

//HOW we can improve it: 1 make tool tip to show exact time under cursor
// 2 : make possible click and recognize the aim (double to open)
// 3 : zoom

//Ok and also if parent and child running at same time at done sched must be no repeats - means calculate that time only once

Item {
    id:doneChartItem

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

            //var seconds = localBase.getDoneActionsLength(chosenDate.text)
            //summaryText.text = "Total spent " + localBase.secondsTranslate(seconds); // seconds + " : "
            ///Ok we skip the thing above, but we need later to calculate it
            //and make it in better way, if aim got a child and both are running -
            //summ mustn't be calculated twice!

            //We need another function, that would return QVariantList
            //And each line is QStringList
            //And fields are all we had in the database

            var list = localBase.getAllDoneActionsList(chosenDate.text)
            doneActsChart.setSource(list)

            //But we must insure, that we wouldn't pass out the borders
            var firstRecordX = doneActsChart.getFirstRecordX()

            if (firstRecordX > screenGlobal.adaptXSize(553)) //dude 553 is very local one
                flick.contentX = screenGlobal.adaptXSize(553)
            else
                flick.contentX = firstRecordX

            //and below we push it into our QML painted item

            //WE WOULD HAVE DIFFERENT recalculations of size here
            //summaryText.height = screenGlobal.adaptYSize( parent.width * (lists.length+4) / 16 );
            //flick.contentHeight = screenGlobal.adaptYSize(  parent.width *  (lists.length+4) / 16 );
        }

        Component.onCompleted: {
            calendar.selectedDateChanged()
        }
    }

    Button{
        text: "-"
        y: calendar.height - height + screenGlobal.adaptXSize( 5 )
        x: calendar.width + widthOffset
        onClicked: {
            if (text == "-"){
                chosenDate.visible = calendar.visible = false
                y = screenGlobal.adaptYSize( 10 )
                flick.y = screenGlobal.adaptYSize( 50 )
                text = "+"
            }else if (text == "+"){
                chosenDate.visible = calendar.visible = true
                y = calendar.height - height + screenGlobal.adaptXSize( 5 )
                flick.y = calendar.height + screenGlobal.adaptYSize( 30 )
                text = "-"
            }
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

        width: parent.width - screenGlobal.adaptYSize( 10 )
        height: parent.height - y - screenGlobal.adaptYSize( 30 )

        contentWidth: parent.width * 2.1
        contentHeight: doneChartItem.height //android dislike- screenGlobal.adaptYSize( 30 )

        ScrollBar.horizontal: ScrollBar { id: hbar; active: vbar.active }
        ScrollBar.vertical: ScrollBar { id: vbar; active: hbar.active }

        DoneActsChart{
            id: doneActsChart
            width: doneChartItem.width * 2.1 //to insure we got all
            height: doneChartItem.height //later update it on load
        }

        //onContentXChanged: console.log(contentX)
    }

}


