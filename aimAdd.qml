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
    id: aimAddWindow
    width: 480
    height: 800

    signal requestOpenViewAims()
    //and other signals could be also here

    property int elementHeight: 45 //apply to every one
    property int widthOffset: 20 //70

    property int yOffset: 50 //be aware of low screens try everywhere

    TextField
    {
        id: aimName

        y:yOffset+elementHeight*0
        width: parent.width-widthOffset*2
        x: widthOffset

        placeholderText: "Aim name"
    }

    TextField
    {
        id: timeAndDate

        y:yOffset+elementHeight*2
        width: parent.width-widthOffset*2-50 - 5//also small offset
        x: widthOffset

        placeholderText: "Time / date"
        //format { hh:mm[:ss] dd.MM[.yyyy] } //can have also buttons for dialogs
    }
    Button
    {
        id: timeAndDatePick
        y:yOffset+elementHeight*2
        x: widthOffset  + timeAndDate.width + 5
        width: 50
        text: "Pick"

        onClicked:
        {
                dateTimePopup.open()
        }
    }


    TextArea
    {
        id: commentText

        y: yOffset+elementHeight*4
        width: parent.width-widthOffset*2
        height: elementHeight*2

        x: widthOffset

        placeholderText: "Place for comment\nMany lines"
    }


    TextField
    {
        id: tags

        y:yOffset+elementHeight*7
        width: parent.width-widthOffset*2
        x: widthOffset

        placeholderText: "#tags"
    }

    //in future make repatable by period
    /*
    Tumbler {
        id:repeatable

        y:yOffset+elementHeight*4
        height: elementHeight*3 - 5
        x: widthOffset

        property string fullText: ""

        property int tumblerColShift: 10

        //style: "Flat"

        TumblerColumn { //looks like better to replace with: http://doc.qt.io/qt-5/qml-qtquick-controls2-tumbler.html
            id: monthRepeat
            model: 13 //take attention o code below
            width: (parent.width-widthOffset*2)/5 - repeatable.tumblerColShift
        }
        TumblerColumn {
            id: dayRepeat
            model: 8
            width: (parent.width-widthOffset*2)/5 - repeatable.tumblerColShift
        }
        TumblerColumn {
            id:hourRepeat
            model: 24
            width: (parent.width-widthOffset*2)/5 - repeatable.tumblerColShift
        }
        TumblerColumn {
            id: minuteRepeat
            model: 60
            width: (parent.width-widthOffset*2)/5 - repeatable.tumblerColShift
        }
        TumblerColumn {
            id: secondRepeat
            model: 60
            width: (parent.width-widthOffset*2)/5 - repeatable.tumblerColShift
        }
    }
    */

    //later
    /*
    ComboBox
    {
        id: privacy

        y:yOffset+elementHeight*7
        width: parent.width-widthOffset*2
        x: widthOffset

        model : ["Choose privacy:","private","shared","group:groupname"]
    }*/
    ComboBox
    {
        id: assignTo

        y:yOffset+elementHeight*9
        width: parent.width-widthOffset*2
        x: widthOffset

        model : ["Assign to:","myself","friend1","friend2"]
    }


    ComboBox
    {
        id: priority

        y:yOffset+elementHeight*11
        width: parent.width-widthOffset*2
        x: widthOffset

        model : ["Priority:","0-none","25-low","50-mid","75-high","100-critical"]
    }

    //MUST BE LIST WHERE CAN BE SELECTED MANY
    /*
    ComboBox
    {
        id: parentAim

        y:yOffset+elementHeight*9
        width: parent.width-widthOffset*2
        x: widthOffset

        //currentText: "Parent aim"

        model: ["Parent aim:","aim1","aim2","aim3"]
    }
    ComboBox
    {
        id: childAimsList
        //should make with another element that could select few

        y:yOffset+elementHeight*10
        width: parent.width-widthOffset*2
        x: widthOffset

        //currentText: "Child aims"

        model: ["Child aim:","aim1","aim2","aim3"]
    }*/


    ///well not on start right now i guess
    /*
    ComboBox
    {
        id: progress

        y:yOffset+elementHeight*11
        width: parent.width-widthOffset*2
        x: widthOffset

        model: ["Progress: ","created","in progress","done"] //make few scale to %
    }
    */
    //ITEM here to be able add links ass needed

    //Make a scroll area maximum of 3 here!! later
    /*
    onContentYChanged: {
               if( contentY < -100 ) {
                   searchField.visible = true;
               } */


    //yes and links we dont need also right now
    /*
    Repeater
    {
        id: linkRepeater
        //y:5+elementHeight*12
        //width: parent.width-widthOffset*2
        //x: widthOffset

        model: 0

        TextField
        {
            id: linkField1 //how to access it ?

            x: widthOffset
            y: elementHeight*index + yOffset+elementHeight*12
            height: elementHeight-5
            width: parent.width-widthOffset*2
            placeholderText: "Link"
        }
        //good to have remove button also
    }
    Button
    {
        id: addLinkButton
        text: "Add link"
        y: yOffset+elementHeight*12 + linkRepeater.model * elementHeight
        x: widthOffset

        onClicked:
        {
            if (linkRepeater.model < 4)
            linkRepeater.model = linkRepeater.model + 1
        }
    }
    */
    Button
    {
        id: addAimButton
        text: "Create aim"
        y: yOffset+elementHeight*13 //+ linkRepeater.model * elementHeight
        x: parent.width - width - widthOffset

        onClicked: {

            var assignToText = assignTo.currentText
            if (assignToText == "Assign to:")
                assignToText = ""

            var priorityText = priority.currentText
            if (priorityText == "Priority:")
                priorityText = ""


            var insertedIndex =
            localBase.addAim(aimName.text,timeAndDate.text, commentText.text, tags.text,
                                                 assignToText, priorityText);

            aimName.text = timeAndDate.text = commentText.text = tags.text = ""
            assignTo.currentIndex = -1
            priority.currentIndex = -1

            aimAddWindow.requestOpenViewAims()
        }
    }


    Text
    {
        id: notifyTODO
        text: "Missing: parent, child list, period."
        y: yOffset+elementHeight*15 //+ linkRepeater.model * elementHeight
        x: parent.width - width - widthOffset

    }


    //TODO:
    ///SO NOW ONLY MISSING CONFIGURATION AND SCROLLABLE IF TOO MANY ELEMENTS

    Dialog {
        id: afterAddDialog
        title: "Request"
        Text
        {
            text:  "Do you wish to add another aim?"
        }

        standardButtons: Dialog.Yes | Dialog.No

        onYes:
        {
            aimName.text=""
            listName.currentIndex = -1 //not sure
            timeAndDate.text=""
            categoryName.currentIndex  = -1
            //reset repeatable
            privacy.currentIndex=0
            assignTo.currentIndex=0
            parentAim.currentIndex=0
            childAimsList.currentIndex=0
            progress.currentIndex=0
        }
        onNo:
        {
            aimAddWindow.requestOpenViewAims()
        }
    }


    Popup {
        id: dateTimePopup //make also time popup with tumbler
        x: 10
        y: 100
        width: 360
        height: 600
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

            //DateTimeChoose{}
            //old way - and its better and must be applied on refact, but now we should do faster way :
        Item {

            id: popupItem

            QmlOld.Calendar
            {
                y: 5
                x: 5 //SHOULD BE REPLACE WITH SOME SCREEN VALUE

                width:  260
                height: 260 //special logic for always vertical Application

                id: calendar
            }

            function formatText(count, modelData) {
                var data = count === 12 ? modelData + 1 : modelData;
                return data.toString().length < 2 ? "0" + data : data;
            }

            FontMetrics {
                id: fontMetrics
            }

            Component {
                  id: delegateTumbler

                  Label {
                      text: popupItem.formatText(Tumbler.tumbler.count, modelData)
                      opacity: 1.0 - Math.abs(Tumbler.displacement) / (Tumbler.tumbler.visibleItemCount / 2)
                      horizontalAlignment: Text.AlignHCenter
                      verticalAlignment: Text.AlignVCenter
                      font.pixelSize: fontMetrics.font.pixelSize * 1.25
                  }
              }

            Row {
                id: timeTumbler

                y: calendar.height + 10
                x: 5



                Tumbler {
                  id: hoursTumbler
                  model: 24
                  delegate: delegateTumbler

                  width:  120
                }

                Tumbler {
                  id: minutesTumbler
                  model: 60
                  delegate: delegateTumbler

                  width:  120
                }

                //ON CHOOSE TURN ON SWITCH
            }

            /*
            Tumbler
            {
                id: timeTumbler

                y: calendar.height + 10
                x: 5

                width:  260

                TumblerColumn {
                    id: hourChoose
                    model: 24
                }
                TumblerColumn {
                    id: minuteChoose
                    model: 60
                }
                TumblerColumn {
                    id: secondChoose
                    model: 60
                }
            }
            */

            Switch
            {
                y: timeTumbler.y + timeTumbler.height + 5
                id: useTime
                checked: false
            }

            Button
            {
                y: useTime.y + useTime.height
                text: "Choose"

                x: 260-width-10

                onClicked: {
                    console.log("done") //output values

                    timeAndDate.text = calendar.selectedDate

                    dateTimePopup.close()
                }
            }
        }
        }



}

