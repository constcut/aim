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
    width: screenGlobal.getScreenWidth()//480
    height: screenGlobal.getScreenHeight()//800

    //signal to edit!

    signal requestOpenAddAim()
    signal requestOpenEditAim(var aimId)
    signal requestOpenSingleAim(var aimId)

    property int elementHeight: screenGlobal.adaptYSize(45) //apply to every one
    property int widthOffset: screenGlobal.adaptXSize(20) //70

    property int yOffset: screenGlobal.adaptYSize(50) //be aware of low screens try everywhere

    property int microOffset: screenGlobal.adaptXSize(10)

    property int bigFontSize : screenGlobal.adaptYSize(35)
    property int middleFontSize : screenGlobal.adaptYSize(25)
    property int smallFontSize : screenGlobal.adaptYSize(15)

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

            var parentAimId = aimLine[10]
            var parentName = ""
            viewParentButton.visible = false

            var parentLine = localBase.getSingleAim(parentAimId)
            if (parentLine.length > 1)
            {
                parentName = parentLine[1]
                viewParentButton.visible = true
            }


            aimParent.text = parentName
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


            childList.model = localBase.getChildAimsNames(aimId);
            childInfoText.text = "Has " + childList.model.length + " child";
        }
    }

    Text {
        id: aimName
        text: "Name"
        font.pointSize: bigFontSize

        y: elementHeight
    }
    Text {
        id: aimMoment
        text: "moment" //better with period
        font.pointSize: middleFontSize

        y: elementHeight*2
    }

    Text {
        id: aimTag
        text: "Tag"
        font.pointSize: middleFontSize

        y: elementHeight*3
    }
    Text {
        id: aimAssignedTo
        text: "AssignedTo"
        font.pointSize: middleFontSize

        y: elementHeight*4
    }
    Text {
        id: aimPriority
        text: "Priority"
        font.pointSize: middleFontSize

        y: elementHeight*5
    }
    Text {
        id: aimParent
        text: "Parent"
        font.pointSize: middleFontSize

        y: elementHeight*6
    }
    //COOL TO MAKE ALSO CHILD LIST!

    Text {
        id: aimComment
        text: "Comment"
        font.pointSize: smallFontSize

        y: elementHeight*7
    }

    TextArea
    {
        id: progressText
        placeholderText: "Progress description"

        font.pointSize: smallFontSize

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
        x: parent.width - width - microOffset
        text : "Save progress"

        onClicked: {
            localBase.updateAimProgress(aimId,progressSlider.value,progressText.text)
        }
    }

    Text {
        id: aimSummory
        text: "Summory"
        font.pointSize: smallFontSize

        y: elementHeight*13
    }

    Text {
        id: aimRunningState
        text: "Running"
        x: parent.width/2

        font.pointSize: smallFontSize

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
        x: parent.width-width-microOffset

        text : "Edit"

        onClicked: {
            singleAimWindow.requestOpenEditAim(aimId)
        }
    }

    Button {

        id: viewParentButton
        y: elementHeight*15

        text : "View parent"

        visible : text.length > 0

        onClicked: {

            if (aimParent.text.length > 0)
            {
                var parentPossibleList = localBase.searchAimsByName(aimParent.text)
                var firstLine = parentPossibleList[0]
                singleAimWindow.requestOpenSingleAim(firstLine[0])
            }
        }
    }

    Text {
        id: childInfoText
        y: elementHeight*4
        x: parent.width/2
        text : "Child list"
    }

    ComboBox
    {
        id: childList
        y:elementHeight*5
        x: parent.width/2
        //model : localBase.getAimsNames()
    }

    Button
    {
        y: elementHeight*6
        x: parent.width/2
        text: "Open child"

        onClicked:
        {
            var childIndex = childList.currentIndex
            var childLines = localBase.getChildAims(aimId)
            var requestedLine = childLines[childIndex]
            singleAimWindow.requestOpenSingleAim(requestedLine[0])
        }
    }


}
