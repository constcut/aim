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
    id: runningAimWindow
    width: 480
    height: 800

    property int elementHeight: 45 //apply to every one
    property int widthOffset: 20 //70

    property int yOffset: 50 //be aware of low screens try everywhere

    property string currentAimId : ""

    ComboBox
    {
        id: listTypeSelect

        y:yOffset+elementHeight*1-45
        width: parent.width-widthOffset*2
        x: widthOffset

        model : ["All aims","This moment(+5min?)","Delayed","In future","Running","Last stopped"]

        onCurrentIndexChanged: {
            //reload aim select
            if (currentIndex==0)
            {
                //all aims
                aimSelect.model = localBase.getAimsNames()
            }
            else if (currentIndex == 1)
            {
               //this moment
                aimSelect.model = localBase.getCurrentMomementAimsNames()
            }
            else if (currentIndex == 2)
            {
                //Delayed
                aimSelect.model = localBase.getDelayedAimsNames()
            }
            else if (currentIndex == 3)
            {
                //Future
                aimSelect.model = localBase.getFutureAimsNames()
            }
            else if (currentIndex == 4)
            {
                //Running
                aimSelect.model = runningAims.getRunningAimsNames()
            }
            else if (currentIndex == 5)
            {
                //Last stoped
                aimSelect.model = runningAims.getStoppedAimsNames()
            }
        }
    }

    ComboBox
    {
        id : aimSelect

        y :yOffset+elementHeight*3-45
        width : parent.width-widthOffset*2
        x : widthOffset

        model : localBase.getAimsNames()

        onCurrentIndexChanged: {
            if (listTypeSelect.currentIndex == 0) //ALL
                if (currentIndex >=1)
                {
                    var aimTable = localBase.getAims()
                    var aimLine = aimTable[currentIndex-1]
                    var aimId = aimLine[0]
                    currentAimId = aimId;
                }
                else
                    currentAimId = ""

            if (listTypeSelect.currentIndex == 1) //CurrentMoment
                {
                    var aimTable = localBase.getCurrentMomementAims()
                    var aimLine = aimTable[currentIndex]
                    var aimId = aimLine[0]
                    currentAimId = aimId;
                }

            //REFACT those parts to as example +
            if (listTypeSelect.currentIndex == 2) //Delayed
                {
                    var aimTable = localBase.getDelayedAims()
                    var aimLine = aimTable[currentIndex]
                    var aimId = aimLine[0]
                    currentAimId = aimId;
                }

            if (listTypeSelect.currentIndex == 3) //Future
                {
                    var aimTable = localBase.getFutureAims()
                    var aimLine = aimTable[currentIndex]
                    var aimId = aimLine[0]
                    currentAimId = aimId;
                }

            if (listTypeSelect.currentIndex == 4) //Running
                {
                    var aimTable = runningAims.getRunningAims()
                    var aimLine = aimTable[currentIndex]
                    var aimId = aimLine[0]
                    currentAimId = aimId;
                }

            if (listTypeSelect.currentIndex == 5) //Stopped
                {
                    var aimTable = runningAims.getStoppedAims()
                    var aimLine = aimTable[currentIndex]
                    var aimId = aimLine[0]
                    currentAimId = aimId;
                }

            //AND in the end perform actions on qml scene
            if (currentAimId.length > 0)
            {
                if (runningAims.isRunning(currentAimId))
                {
                    stopButton.enabled = true
                    startButton.enabled = false
                }
                else
                {
                    startButton.enabled = true
                    stopButton.enabled = false
                }

                secondsCount.text = runningAims.getSecondsPassed(currentAimId)
            }
        }
    }

    Button
    {
        id:startButton
        y :yOffset+elementHeight*5-45
        x : widthOffset
        text : "Start"

        onClicked: {
            stopButton.enabled = true
            startButton.enabled = false

            if (currentAimId.length > 0)
                runningAims.start(currentAimId)
        }
    }

    Button
    {
        id:stopButton
        y: yOffset+elementHeight*5-45
        x : parent.width - widthOffset - width
        text: "Stop"
        enabled: false

        onClicked: {
            stopButton.enabled = false
            startButton.enabled = true

            if (currentAimId.length > 0)
                runningAims.stop(currentAimId)
        }
    }

    Text
    {
        id:secondsCount
        x: widthOffset + (parent.width-width-widthOffset)/2 //WOULD LIKE to see it in center :)
        y: yOffset+elementHeight*7-45
        font.pointSize: 50
        text: "14:31"
    }

    Timer{
            id: refreshTimer
            running: true; interval: 500; repeat: true
            onTriggered: {

                if (currentAimId.length > 0)
                    secondsCount.text = runningAims.getSecondsPassed(currentAimId)
            }
        }

    TextArea
    {
        id: logOfActivity

        y: yOffset+elementHeight*10-45
        width: parent.width-widthOffset*2
        height: elementHeight*3

        x: widthOffset

        placeholderText: "Simple log of activity,\ncan be replaced with ListView"
    }

    TextArea
    {
        id: summaryText

        y: yOffset+elementHeight*14-45
        width: parent.width-widthOffset*2
        height: elementHeight*3

        x: widthOffset

        placeholderText: "Some summory about this aim, like total hours, times runned\nand in the end - time line!"
    }

}
