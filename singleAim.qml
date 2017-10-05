import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
//import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Extras 1.4 as QmlExtra
import QtQuick.Controls 1.4 as QmlOld
import QtQuick.Controls.Styles.Flat 1.0 as Flat

Item {

    id: singleAimWindow
    width: 480
    height: 800

    //signal to edit!

    signal requestOpenAddAim()
    signal requestOpenEditAim(var aimId)
    signal requestOpenSingleAim(var aimId)

    property int elementHeight: 45 //apply to every one
    property int widthOffset: 20 //70

    property int yOffset: 50 //be aware of low screens try everywhere

    property string aimId : ""

    Component.onCompleted:
    {
        //load single aim

        if (aimId.length > 0)
        {
            var aimLine = localBase.getSingleAim(aimId)

            aimName.text = aimLine[1]

            var dateAndTime = aimLine[3]
            if (aimLine[2].length > 0)
                dateAndTime += "T" + aimLine[2].length
            if (aimLine[11].length > 0)
                dateAndTime += " : " + aimLine[11];

            aimMoment.text = dateAndTime

            aimTag.text = aimLine[5]
            aimComment.text = aimLine[4]

            aimAssignedTo.text = aimLine[6]

            aimPriority.text = aimLine[7]
            aimParent.text = aimLine[10]
            //MAYBE NEED TO HAVE FIELD PARENT AIM? ALSO? seamize once hadd to solve possible rename issue

            aimSummory.text = localBase.getActivitySummary(aimId)

            progressSlider.value = aimLine[8]
            progressText.text = aimLine[9]

            if (runningAims.isRunning(aimId))
            {
                aimRunningState.text = "Aim running " + runningAims.getSecondsPassed(aimId) + " seconds";
                stopStartButton.text = "Stop";
            }
            else
            {
                aimRunningState.text = "Aim is stopped"
                stopStartButton.text = "Start";
            }


        }
    }

    Text {
        id: aimName
        text: "Name"
        font.pointSize: 35

        y: elementHeight
    }
    Text {
        id: aimMoment
        text: "moment" //better with period
        font.pointSize: 25

        y: elementHeight*2
    }

    Text {
        id: aimTag
        text: "Tag"
        font.pointSize: 25

        y: elementHeight*3
    }
    Text {
        id: aimAssignedTo
        text: "AssignedTo"
        font.pointSize: 25

        y: elementHeight*4
    }
    Text {
        id: aimPriority
        text: "Priority"
        font.pointSize: 25

        y: elementHeight*5
    }
    Text {
        id: aimParent
        text: "Parent"
        font.pointSize: 25

        y: elementHeight*6
    }
    //COOL TO MAKE ALSO CHILD LIST!

    Text {
        id: aimComment
        text: "Comment"
        font.pointSize: 15

        y: elementHeight*7
    }

    TextArea
    {
        id: progressText
        placeholderText: "Progress description"

        font.pointSize: 15

        y: elementHeight*9
    }
    Slider
    {
        id: progressSlider
        y: elementHeight*12
    }
    Button
    {
        y: elementHeight*12
        x: parent.width - width - 10
        text : "Save progress"

        onClicked: {
            localBase.updateAimProgress(aimId,progressSlider.value,progressText.text)
        }
    }

    Text {
        id: aimSummory
        text: "Summory"
        font.pointSize: 15

        y: elementHeight*13
    }

    Text {
        id: aimRunningState
        text: "Running"
        x: parent.width/2

        font.pointSize: 15

        y: elementHeight*13
    }

    Button {

        y: elementHeight*14
        x: parent.width/2

        id: stopStartButton
        text: "stop|start"
        onClicked: {
            if (text == "Stop")
            {
                runningAims.stop(aimId)
                stopStartButton.text = "Start";
            } else
            if (text == "Start")
            {
                runningAims.start(aimId)
                stopStartButton.text = "Stop";
            }
        }
    }

    Timer{
            id: refreshTimer
            running: true; interval: 500; repeat: true
            onTriggered: {

                if (aimId.length > 0)
                {
                    if (runningAims.isRunning(aimId))
                    {
                        aimRunningState.text = "Aim running " + runningAims.getSecondsPassed(aimId) + " seconds";
                        stopStartButton.text = "Stop";
                    }
                    else
                    {
                        aimRunningState.text = "Aim is stopped"
                        stopStartButton.text = "Start";
                    }
                }
             }
        }


    Button {

        y: elementHeight*15
        x: parent.width-width-10

        text : "Edit"

        onClicked: {
            singleAimWindow.requestOpenEditAim(aimId)
        }
    }

    Button {

        y: elementHeight*15

        text : "View parent"

        visible : text.length > 0

        onClicked: {
            var parentPossibleList = localBase.searchAimsByName(aimParent.text)
            var firstLine = parentPossibleList[0]
            singleAimWindow.requestOpenSingleAim(firstLine[0])
        }
    }

    Text {
        y: elementHeight*15
        text : "Child list needed here! Or even child tree"
    }


}
