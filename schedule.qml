import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3

import QtQuick.Controls 1.4 as QMLOld

import QtQuick.Controls.Styles.Flat 1.0 as Flat



Item {

    anchors.fill: parent

    QMLOld.Calendar
    {
        y: 0
        x: 50 //SHOULD BE REPLACE WITH SOME SCREEN VALUE

        width: parent.width - x*2
        height: parent.width - x*2 //special logic for always vertical Application

        id: calendar

        //?property

        onSelectedDateChanged:
        {
            chosenDate.text = calendar.selectedDate

            loadModelByDate(chosenDate.text)
        }
    }
    Text{
        y: calendar.height
        id: chosenDate
        x: 50
    }

    ListModel {
        id: listModel
        Component.onCompleted: {
            loadModel()
        }
    }

    function loadModelByDate(date)
    {
        listModel.clear()
        var aimList =  localBase.getAimsByDate(date)

        for (var i = 0; i < aimList.length; ++i)
        {
            var aimId = aimList[i][0]
            var aimName = aimList[i][1]
            var timeAndDate //= aimList[i][2]

            var timePart = aimList[i][2]
            var datePart = aimList[i][3]

            timeAndDate = datePart;

            if (timePart.length > 0)
                timeAndDate += "T" + timePart;

            var comment = aimList[i][4]
            var tag = aimList[i][5]
            var assignTo = aimList[i][6]
            var priority = aimList[i][7]

            var progress = aimList[i][8]
            var progressText = aimList[i][9]
            var parentAim = aimList[i][10]
            //var childAim = aimList[i][11]

            var repeatable = aimList[i][11]
            var privacy = aimList[i][12]


            listModel.append({"aimId":aimId,"name":aimName,"timeAndDate":timeAndDate,"comment":comment,"tag":tag,
                             "assignTo":assignTo,"priority":priority,
                             "progress":progress,"progressText":progressText,
                             "parentAim":parentAim,
                             "repeatable":repeatable,"privacy":privacy})
        }
    }

    function loadModel()
    {
        listModel.clear()
        var aimList =  localBase.getAims()

        for (var i = 0; i < aimList.length; ++i)
        {
            var aimId = aimList[i][0]
            var aimName = aimList[i][1]
            var timeAndDate //= aimList[i][2]

            var timePart = aimList[i][2]
            var datePart = aimList[i][3]

            timeAndDate = datePart;

            if (timePart.length > 0)
                timeAndDate += "T" + timePart;

            var comment = aimList[i][4]
            var tag = aimList[i][5]
            var assignTo = aimList[i][6]
            var priority = aimList[i][7]

            var progress = aimList[i][8]
            var progressText = aimList[i][9]
            var parentAim = aimList[i][10]

            var repeatable = aimList[i][11]
            var privacy = aimList[i][12]


            listModel.append({"aimId":aimId,"name":aimName,"timeAndDate":timeAndDate,"comment":comment,"tag":tag,
                             "assignTo":assignTo,"priority":priority,
                             "progress":progress,"progressText":progressText,
                             "parentAim":parentAim,
                             "repeatable":repeatable,"privacy":privacy})
        }
    }

    ListView
    {
        id:aimList
        clip: true
        width: parent.width
        y: calendar.height + calendar.y + 10
        height: parent.height - (calendar.height + calendar.y + 20)

        model: listModel

        Behavior on y {
                    NumberAnimation{ duration: 200 }
                }

        onContentYChanged: {


                   if (desktopClient == false) //android or ios
                   {
                       if( contentY < -100 ) {
                           searchBox.visible = true
                           settingsBox.visible = true
                           searchTimer.running = true
                       }
                       if (contentY > 300)
                       {
                            searchBox.visible = false
                            settingsBox.visible = false
                       }
                   }
                   else
                   {
                       //desktop
                   }


               }

        delegate: anotherDeligate//listDelegate

        highlight: highlightBar//Rectangle { color: "lightsteelblue"; radius: 5 }
        focus:  true


        ScrollBar.vertical: ScrollBar {}
    }

    Component {
        id: highlightBar
        Rectangle {
            id: highlightBarRect
            width: 200; height: 50
            color: userSettings.getColor("ListHightlight") //"#FFFF88"
            y: aimList.currentItem.y;
            Behavior on y { SpringAnimation { spring: 2; damping: 0.3 } } //read about it
        }
    }


    Component {
        id: anotherDeligate


        Item {
            id: wrapper //rename on refact
            width: aimList.width
            height: 110  //means need to know fonts also

            Column {
                Row
                {
                    height: 30 //must look over here
                    Text { id: aimNameText; color:userSettings.getColor("Text");   text: '<b>' + name + '</b>' ; font.pointSize: 20}
                    rightPadding:  (aimList.width - aimNameText.width)/2 - 10
                    leftPadding:  (aimList.width - aimNameText.width)/2 - 10
                }
                RowLayout
                {
                    height: 25
                    width: aimList.width

                    Text { color:userSettings.getColor("Text");text: 'Tag: ' + tag;  font.pointSize: 15}
                    //spacing: 50 //later can make it just like left\right padding
                    Text { rightPadding: 25; anchors.right: parent.right; color:userSettings.getColor("Text");text: 'Moment: ' + timeAndDate;  font.pointSize: 15}
                }
                RowLayout
                {
                    height: 15
                    width: aimList.width

                    Text {  color:userSettings.getColor("Text");text: 'AssignTo: ' + assignTo;  font.pointSize: 10}
                    //spacing: 50 //later can make it just like left\right padding
                    Text { rightPadding: 25; anchors.right: parent.right; color:userSettings.getColor("Text");text: 'Priority: ' + priority;  font.pointSize: 10}
                }
                Row
                {
                    height: 25
                    Text { color:userSettings.getColor("Text");text: 'Comment: ' + comment; font.pointSize: 10}
                }
            }
            states: State { // indent the item if it is the current item
                name: "Current"
                when: wrapper.ListView.isCurrentItem
                PropertyChanges { target: wrapper; x: 20 }
            }
            transitions: Transition {
                NumberAnimation { properties: "x"; duration: 200 }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: wrapper.ListView.view.currentIndex = index

                onPressAndHold: {
                    wrapper.ListView.view.currentIndex = index
                    aimMenu.x = mouse.x
                    aimMenu.y = parent.y + mouse.y //works on item
                    aimMenu.open()
                }
            }
        }
    }

}


