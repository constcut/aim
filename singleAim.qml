import QtQuick 2.8
import QtQuick.Controls 2.4
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
//import QtQuick.Extras 1.4
import QtQuick.Layouts 1.11
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

            var parentAimId = aimLine[10]
            var parentName = ""
            var parentLine = localBase.getSingleAim(parentAimId)
            if (parentLine.length > 1)
                parentName = parentLine[1]

            var dateAndTime = aimLine[3]
            if (aimLine[2].length > 0)
                dateAndTime += "T" + aimLine[2].length
            if (aimLine[11].length > 0)
                dateAndTime += " : " + aimLine[11];

            aimName.text = "<style>a:link { color:#11BB33; }</style><big><b>" + aimLine[1] + "</b></big><small>" + "    : <a href=\"" + parentAimId + "\">" + parentName + "</a>  " + "</small>"; //IN FUTURE WE CAL ALSO CREATE LINKS FOR DATES
            //MAYBE GIVE THE FULL SEQUENCE OF PARENTS?
            //on review and attention check or just refactoring - make this idea as settings option, it could be nice
            aimMoment.text = dateAndTime
            if (aimLine[5].length > 1)
                aimMoment.text += "  tag: " +aimLine[5]

            //section 2

            childList.model = localBase.getChildAimsNames(aimId);
            childInfoText.text = "Has " + childList.model.length + " child";

            if (childList.model.length === 0)
                openChildButton.enabled = false;
            else
                openChildButton.enabled = true;

            //YET unused, but later should be:
            //aimAssignedTo.text = aimLine[6]
            //aimPriority.text = aimLine[7]
            //viewParentButton.visible = false

            aimSummory.text = localBase.getActivitySummary(aimId)

            progressSlider.value = aimLine[8]
            progressText.text = aimLine[9]
            //aimComment.text = aimLine[4] //move to tree commenting

            if (runningAims.isRunning(aimId)){
                aimRunningState.text = runningAims.getSecondsPassed(aimId) + " seconds";
                stopStartButton.text = "Stop";
            }
            else{
                aimRunningState.text = "Aim is stopped"
                stopStartButton.text = "Start";
            }



        }
    }


    Flickable {
        id: flickName
        x: screenGlobal.adaptXSize( 5 )
        y: screenGlobal.adaptYSize( 10 )
        width: parent.width - screenGlobal.adaptYSize( 10 )
        height: elementHeight*2
        contentWidth: parent.width * 5
        contentHeight: elementHeight*2
        ScrollBar.horizontal: ScrollBar { active: true }
        TextArea{
            textFormat: "RichText"
            id: aimName
            width: parent.width * 5
            height: elementHeight*2
            readOnly:  true
            font.pixelSize: bigFontSize
            onLinkActivated:{
                singleAimWindow.requestOpenSingleAim(link)
            }
        }
        Text {
                id: aimMoment
                text: "moment" //better with period
                font.pixelSize: middleFontSize
                y: aimName.height
            }
    }

    Rectangle {
         y: flickName.y + elementHeight*3
         x: 0
         width: parent.width
         height: screenGlobal.adaptYSize(10)

         border.color: "black"
         color: "gray"
    }

    RowLayout{
        y: flickName.y + elementHeight*3 +screenGlobal.adaptYSize(20)
        x: screenGlobal.adaptXSize( 10 )
        Text {
            id: childInfoText
            text : "Child list"
        }
        ComboBox{
            id: childList
            font.pixelSize: smallFontSize
            Layout.preferredWidth: parent.width/2 - screenGlobal.adaptXSize( 45 )
        }
        Button{
            id:openChildButton
            text: "Open child"
            font.pixelSize: smallFontSize
            onClicked:{
                var childIndex = childList.currentIndex
                var childLines = localBase.getChildAims(aimId)
                var requestedLine = childLines[childIndex]
                singleAimWindow.requestOpenSingleAim(requestedLine[0])
            }
        }
        Button{
            id:newChildButton
            text:"New child"
            font.pixelSize: smallFontSize
            onClicked:{
                singleAimWindow.requestOpenAddAim() //it must be possible
                //to presend parent parameter for the add window
            }
        }
    }

    Rectangle {
         y: flickName.y + elementHeight*4 +screenGlobal.adaptYSize(30)
         x: 0
         width: parent.width
         height: screenGlobal.adaptYSize(10)

         border.color: "black"
         color: "gray"
         id: borderBeforeProgress
    }


    ScrollView {
         id: progressView
         y: borderBeforeProgress.y + borderBeforeProgress.height*2
         width: parent.width // /2
         height: elementHeight*3
         x: screenGlobal.adaptXSize(10)
        TextArea{
            id: progressText
            placeholderText: "Progress description"
            font.pixelSize: smallFontSize
        }
    }
    /*TextArea { //sorry yet we haven,t found place for comment
        y: borderBeforeProgress.y + borderBeforeProgress.height*2
        id: aimComment
        placeholderText: "Comment"
        font.pixelSize: smallFontSize
        width: parent.width/2
        x: parent.width/2
        height: elementHeight*3
        visible: false
    }*/
    Slider{
        id: progressSlider
        y: progressView.y + progressView.height + screenGlobal.adaptYSize(10)
        width: parent.width - saveProgressButton.width  - screenGlobal.adaptXSize(20)
        from: 1
        to: 100
        ToolTip {
              parent: progressSlider.handle
              visible: progressSlider.pressed
              text: progressSlider.value.toFixed(1) + "%"
          }
    }
    Button{
        id: saveProgressButton
        y: progressView.y + progressView.height + screenGlobal.adaptYSize(10)
        x: parent.width - width - screenGlobal.adaptXSize(10)
        text : "Save progress"
        font.pixelSize: smallFontSize //its normal size verywhere
        onClicked: {
            //here also should lay comment
            localBase.updateAimProgress(aimId,progressSlider.value,progressText.text)
        }
    }

    Button {
        y: screenGlobal.adaptYSize(10) + elementHeight*2
        x: parent.width - width - screenGlobal.adaptXSize(10)
        font.pixelSize: smallFontSize //its normal size verywhere
        text : "Edit"

        onClicked: {
            singleAimWindow.requestOpenEditAim(aimId)
        }
    }

    Rectangle {
         y: saveProgressButton.y + saveProgressButton.height + screenGlobal.adaptYSize(10)
         x: 0
         width: parent.width
         height: screenGlobal.adaptYSize(10)

         border.color: "black"
         color: "gray"
         id: borderBeforeSummary
    }

    Text {
        id: aimSummory
        //placeholderText:  "Summory"
        font.pixelSize: smallFontSize
        y: borderBeforeSummary.y + borderBeforeSummary.height*2
        width: 3*parent.width/4
        height: elementHeight*1.5
        x: screenGlobal.adaptXSize(10)
    }
    Text {
        id: aimRunningState
        text: "Running"
        x: 3*parent.width/4
        font.pixelSize: smallFontSize
        y: borderBeforeSummary.y + borderBeforeSummary.height*2
    }

    Button {
        y:  aimRunningState.y + aimRunningState.height + screenGlobal.adaptYSize(10) //screenGlobal.adaptYSize(30)
        x: parent.width - width - screenGlobal.adaptXSize(10)
        font.pixelSize: smallFontSize //its normal size verywhere
        id: stopStartButton
        text: "stop|start"
        onClicked: {
            if (text == "Stop"){
                runningAims.stop(aimId)
                stopStartButton.text = "Start";
            } else
            if (text == "Start"){
                runningAims.start(aimId)
                stopStartButton.text = "Stop";
            }
        }
    }
    Timer{
            id: refreshTimer
            running: true; interval: 500; repeat: true
            onTriggered: {
                if (aimId.length > 0){
                    if (runningAims.isRunning(aimId)){
                        aimRunningState.text = "Aim running " + runningAims.getSecondsPassed(aimId) + " seconds";
                        stopStartButton.text = "Stop";
                    }
                    else{
                        aimRunningState.text = "Aim is stopped"
                        stopStartButton.text = "Start";
                    }
                }
             }
        }


    Rectangle {
         y: aimSummory.y + aimSummory.height + screenGlobal.adaptYSize(10)
         x: 0
         width: parent.width
         height: screenGlobal.adaptYSize(10)

         border.color: "black"
         color: "gray"
         id: borderBeforeLinks
    }




    /*Text {
        id: aimName
        text: "Name"
        font.pixelSize: bigFontSize
        y: elementHeight
    }*/
    /*
    Text {
        id: aimTag
        text: "Tag"
        font.pixelSize: middleFontSize
        y: elementHeight*3
    }*/

    /*
    Text {
        id: aimAssignedTo
        text: "AssignedTo"
        font.pixelSize: middleFontSize

        y: elementHeight*4
    }
    Text {
        id: aimPriority
        text: "Priority"
        font.pixelSize: middleFontSize

        y: elementHeight*5
    }
    Text {
        id: aimParent
        text: "Parent"
        font.pixelSize: middleFontSize

        y: elementHeight*6
    }*/
    //COOL TO MAKE ALSO CHILD LIST!

    //transform to textarea





}
