import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3

//import QtQuick.Controls 1.4 //calendar

Item {
    id: aimAddWindow
    width: 480
    height: 800

    property int elementHeight: 45
    property int widthOffset: 70

    property int yOffset: 50 //be aware of low screens try everywhere

    TextField
    {
        id: aimName

        y:yOffset+elementHeight*0
        width: parent.width-widthOffset*2
        x: widthOffset

        placeholderText: "Aim name"
    }
    ComboBox
    {
        id: listName

        y:yOffset+elementHeight*1
        width: parent.width-widthOffset*2
        x: widthOffset

        model : ["Insert into list:","list2","list3"]
    }
    TextField
    {
        id: timeAndDate

        y:yOffset+elementHeight*2
        width: parent.width-widthOffset*2-50 - 5//also small offset
        x: widthOffset

        placeholderText: "Time and\or date"
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

    ComboBox
    {
        id: categoryName

        y:yOffset+elementHeight*3
        width: parent.width-widthOffset*2
        x: widthOffset

        model: ["Choose category:","tag1.tag2","t1.t2.t3.t4"] //if too long let stay only last 80 chars
    }
    Tumbler {
        id:repeatable

        y:yOffset+elementHeight*4
        height: elementHeight*3 - 5
        x: widthOffset

        property string fullText: ""

        property int tumblerColShift: 10

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
    ComboBox
    {
        id: privacy

        y:yOffset+elementHeight*7
        width: parent.width-widthOffset*2
        x: widthOffset

        model : ["Choose privacy:","private","shared","group:groupname"]
    }
    ComboBox
    {
        id: assignTo

        y:yOffset+elementHeight*8
        width: parent.width-widthOffset*2
        x: widthOffset

        model : ["Assign to:","myself","friend1","friend2"]
    }
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
    }
    ComboBox
    {
        id: progress

        y:yOffset+elementHeight*11
        width: parent.width-widthOffset*2
        x: widthOffset

        model: ["Progress: ","created","in progress","done"] //make few scale to %
    }
    //ITEM here to be able add links ass needed

    //Make a scroll area maximum of 3 here!! later
    /*
    onContentYChanged: {
               if( contentY < -100 ) {
                   searchField.visible = true;
               } */
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
    Button
    {
        id: addAimButton
        text: "Create aim"
        y: yOffset+elementHeight*12 + linkRepeater.model * elementHeight
        x: parent.width - width - widthOffset

        onClicked: {
            var insertedIndex = localBase.addAim(aimName.text,listName.currentText,timeAndDate.text,categoryName.currentText,
                                                 repeatable.fullText,privacy.currentText,assignTo.currentText,
                                                 parentAim.currentText,childAimsList.currentText,progress.currentText);

            //missing links here

            afterAddDialog.open()
        }
    }

    Dialog {
        id: afterAddDialog
        title: "Do you wish to add another aim?"
        standardButtons: Dialog.Yes | Dialog.No

        onYes:
        {

            aimName.text=""
            listName.currentIndex=0
            timeAndDate.text=""
            categoryName.currentIndex=0
            //reset repeatable
            privacy.currentIndex=0
            assignTo.currentIndex=0
            parentAim.currentIndex=0
            childAimsList.currentIndex=0
            progress.currentIndex=0
        }
        onNo:
        {
            //must jump to the aims view

            //use wellknown connection mechanics
        }
    }


    Popup {
        id: dateTimePopup //make also time popup with tumbler
        x: 100
        y: 100
        width: 220
        height: 450
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

            //DateTimeChoose{}
            //old way - and its better and must be applied on refact, but now we should do faster way :
        Item {
            Tumbler
            {
                y: 5
                id: dateTumbler
                TumblerColumn {
                    id: dayChoose
                    model: 31 //auto update on month choose
                }
                TumblerColumn {
                    id: monthChoose
                    model: ["January","Febuary","March","April","May"]

                    onCurrentIndexChanged:
                    {
                        //so should chande model

                        //and save the index - if it possible - if not shouse last one, for example 31 jan switches to 28 feb then

                        if (currentIndex == 0)
                            dayChoose.model = 31        //should do it more complex
                        if (currentIndex == 1)
                            dayChoose.model = 28
                        if (currentIndex == 2)
                            dayChoose.model = 30
                    }
                }
                TumblerColumn {
                    id: yearChoose
                    model: ["2017","2018","2019","2020","2021","2022","2023"]
                }

            }
            //switch off if we don't need date

            Switch
            {
                y: dateTumbler.height + 5
                id: useDate
                checked: true
            }

            Tumbler
            {
                id: timeTumbler

                y: dateTumbler.height + 5 + useDate.height + 5

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

            Switch
            {
                y: timeTumbler.y + timeTumbler.height + 5
                id: useTime
                checked: true
            }

            Button
            {
                y: useTime.y + useTime.height
                text: "Choose"

                onClicked: {
                    console.log("done") //output values
                    dateTimePopup.close()
                }
            }
        }
        }



}

