import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3

import QtQuick.Controls.Styles.Flat 1.0 as Flat

Item {

    id: aimViewWindow
    width: 480
    height: 800

    signal requestOpenAddAim()

    property int elementHeight: 45
    property int widthOffset: 70
    property int yOffset: 50 //be aware of low screens try everywhere

    property string highlightAimColor : userSettings.getColor("ListHightlight") //to settings please

    property bool desktopClient: Qt.platform.os != "android" && Qt.platform.os != "ios"

    Item
    {
      y: yOffset
      x: 10 //widthOffset

      width: parent.width - widthOffset*2

      id: searchBox
      visible: desktopClient //false

      property int fullHeight: elementHeight*2



       TextField
       {
           id: aimNameSearch
           placeholderText: "Input here search request"
           visible: true
           width: parent.width

           onTextChanged:
           {
               searchTimer.restart()

               if (searchByName.checked)
               {
                   if (text.length > 0)
                       searchModel(text)
                   else
                       loadModel()
               }

               if (searchByTag.checked)
               {
                   if (text.length > 0)
                       searchModelTag(text)
                   else
                       loadModel()
               }
           }
       }

       RadioButton
       {
            y: elementHeight
            x: 0
            id: searchByName
            checked: true
            text: "name"
            onCheckedChanged: //cover under function
            {
                //only for android
                //if (checked) searchTimer.restart()
            }
       }

       RadioButton
       {
            y: elementHeight
            x: parent.width - width
            id: searchByTag
            text: "tag"

            onCheckedChanged: //cover under function
            {
                //only for android
                //if (checked) searchTimer.restart()
            }
       }
    } //SEARCH BOX END
    Item
    {
        //settings box
        y: yOffset - 20
        x: searchBox.width + widthOffset + 15

        width: widthOffset-10

        id: settingsBox
        visible: desktopClient //false

        RoundButton
        {
            text:  "*" // "\u2699"
            font.pixelSize: 20
            onClicked:
            {
                viewSettingsPopup.open()
            }

            ToolTip.visible: hovered
            ToolTip.text: qsTr("Open settings")
        }
    }

    //please note it yet not hided like another button does, so it would have to make some tricks on android around it, but its simple
    Item
    {
        //settings box
        y: yOffset + 50
        x: searchBox.width + widthOffset + 15

        width: widthOffset-10

        id: addBox
        visible: desktopClient //false

        RoundButton
        {
            text:  "+"
            font.pixelSize: 20
            onClicked:
            {
                aimViewWindow.requestOpenAddAim()
            }

            ToolTip.visible: hovered
            ToolTip.text: qsTr("Add aim")
        }
    }



    Popup {
        id: viewSettingsPopup //make also time popup with tumbler
        x: 50
        y: 50
        width: 230
        height: 560 //ensure manual sizing
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        Component.onCompleted: //onOpened:
        {
            var settingList = userSettings.getViewAimSettings()

            timeAndDateField.checked = settingList[0]
            commentField.checked = settingList[1]
            tagField.checked = settingList[2]
            assignToField.checked = settingList[3]
            priorityField.checked = settingList[4]
            progressField.checked = settingList[5]
            progressTextField.checked  = settingList[6]
            parentField.checked  = settingList[7]
            childField.checked  = settingList[8]
        }

        Item {
            ColumnLayout
            {
            CheckBox
            {
                id: timeAndDateField
                text: "Time and date"
            }
            CheckBox
            {
                id: commentField
                text: "Comment"
            }
            CheckBox
            {
                id: tagField
                text: "Tag"
            }
            CheckBox
            {
                id: assignToField
                text: "Assign to"
            }
            CheckBox
            {
                id: priorityField
                text: "Priority"
            }
            CheckBox
            {
                id: progressField
                text: "Progress"
            }
            CheckBox
            {
                id: progressTextField
                text: "Progress text"
            }
            CheckBox
            {
                id: parentField
                text: "Parent"
            }
            CheckBox
            {
                id: childField
                text: "Child"
            }
            Button
            {
                text:"Save settings"
                onClicked:
                {
                    var viewAimList = [timeAndDateField.checked,commentField.checked,
                            tagField.checked,assignToField.checked,
                            priorityField.checked,progressField.checked,progressTextField.checked,
                            parentField.checked,childField.checked]

                    userSettings.setViewAimSettings(viewAimList)

                    loadViewSettings()

                    viewSettingsPopup.close()

                }
            }
            }
        }

    }

        ColorDialog {
            id: colorDialog
            title: "Please choose a color"
            onAccepted: {
                colorTester.color = colorDialog.color
            }
        }

    Timer{

        //should avoid it for desktop

            id: searchTimer; running: false; interval: 10000; repeat: false
            onTriggered: {


                if (desktopClient == false)
                {
                    if (listModel.count == 0)
                    {
                        running = true
                    }
                    else
                    {
                        searchBox.visible = false
                        settingsBox.visible = false
                    }
                }
            }
        }


    Rectangle
    {
         y: searchBox.visible ? searchBox.fullHeight + yOffset +10: yOffset+10
         x: 0
         width: parent.width
         height:10

         border.color: "black"
         color: "gray"
    }

    ListView
    {
        id:aimList
        clip: true
        width: parent.width
        y: searchBox.visible ? searchBox.fullHeight + yOffset+25 : yOffset+25
        height: parent.height - yOffset*4 //if put someting down make it *4

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


    //update width and so on

    function loadViewSettings()
    {
       var settingsList =  userSettings.getViewAimSettings()

        aimViewWindow.timeAndDateShow = settingsList[0]
        aimViewWindow.commentShow = settingsList[1]
        aimViewWindow.tagShow = settingsList[2]
        aimViewWindow.assignToShow = settingsList[3]
        aimViewWindow.priorityShow = settingsList[4]
        aimViewWindow.progressShow = settingsList[5]
        aimViewWindow.progressTextShow = settingsList[6]
        aimViewWindow.parentAimShow = settingsList[7]
        aimViewWindow.childAimShow = settingsList[8]

        //aimViewWindow.repeatableShow = settingsList[9] //not yet
        //aimViewWindow.privacyShow = settingsList[10] //not yet
    }

    property bool timeAndDateShow: true
    property bool commentShow: true
    property bool tagShow: true
    property bool assignToShow: true
    property bool priorityShow: true
    property bool progressShow:  true
    property bool progressTextShow: true
    property bool parentAimShow: true
    property bool childAimShow: true

    property bool repeatableShow: false //not yet
    property bool privacyShow: false //not yet

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

                    Text { color:userSettings.getColor("Text");text: 'Tag: ' + tag; visible: aimViewWindow.tagShow && tag.length > 0   ; font.pointSize: 15}
                    //spacing: 50 //later can make it just like left\right padding
                    Text { rightPadding: 25; anchors.right: parent.right; color:userSettings.getColor("Text");text: 'Moment: ' + timeAndDate; visible: aimViewWindow.timeAndDateShow && timeAndDate.length > 0   ; font.pointSize: 15}
                }
                RowLayout
                {
                    height: 15
                    width: aimList.width

                    Text {  color:userSettings.getColor("Text");text: 'AssignTo: ' + assignTo; visible: aimViewWindow.assignToShow && assignTo.length > 0 && assignTo != "Assign to:"  ; font.pointSize: 10}
                    //spacing: 50 //later can make it just like left\right padding
                    Text { rightPadding: 25; anchors.right: parent.right; color:userSettings.getColor("Text");text: 'Priority: ' + priority; visible: aimViewWindow.priorityShow && priority.length > 0; font.pointSize: 10}
                }
                Row
                {
                    height: 25
                    Text { color:userSettings.getColor("Text");text: 'Comment: ' + comment; visible: aimViewWindow.commentShow  && comment.length > 0 ; font.pointSize: 10}
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

    Menu {
        id: aimMenu

        MenuItem {
            text: "Edit"

            onTriggered:
            {
                console.log("edit triggered")
            }
        }
        MenuItem {
            text: "Delete"

            onTriggered:
            {
                console.log("delete triggered")

                //aimList.currentIndex

                var index = aimList.currentIndex
                var aimId = listModel.get(index).aimId

                localBase.deleteAim(aimId)

                loadModel()

            }
        }
        MenuItem {
            text: "Send"

            onTriggered:
            {
                console.log("send triggered")
            }
        }
    }

    Component {
        id: highlightBar
        Rectangle {
            id: highlightBarRect
            width: 200; height: 50
            color: highlightAimColor //"#FFFF88"
            y: aimList.currentItem.y;
            Behavior on y { SpringAnimation { spring: 2; damping: 0.3 } } //read about it
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
            var timeAndDate = aimList[i][2]
            var comment = aimList[i][3]
            var tag = aimList[i][4]
            var assignTo = aimList[i][5]
            var priority = aimList[i][6]

            var progress = aimList[i][7]
            var progressText = aimList[i][8]
            var parentAim = aimList[i][9]
            var childAim = aimList[i][10]

            var repeatable = aimList[i][11]
            var privacy = aimList[i][12]


            listModel.append({"aimId":aimId,"name":aimName,"timeAndDate":timeAndDate,"comment":comment,"tag":tag,
                             "assignTo":assignTo,"priority":priority,
                             "progress":progress,"progressText":progressText,
                             "parentAim":parentAim,"childAim":childAim,
                             "repeatable":repeatable,"privacy":privacy})
        }
    }

    function searchModel(searchNames)
    {
        listModel.clear()
        var aimList =  localBase.getAims()

        var regExpName = new RegExp(searchNames)

        for (var i = 0; i < aimList.length; ++i)
        {
            var aimId = aimList[i][0]
            var aimName = aimList[i][1]
            var timeAndDate = aimList[i][2]
            var comment = aimList[i][3]
            var tag = aimList[i][4]
            var assignTo = aimList[i][5]
            var priority = aimList[i][6]

            var progress = aimList[i][7]
            var progressText = aimList[i][8]
            var parentAim = aimList[i][9]
            var childAim = aimList[i][10]

            var repeatable = aimList[i][11]
            var privacy = aimList[i][12]


            //also could run as set of filters making another list - first filter name, then tag etc
            if (aimName.search(regExpName) !== -1)
                listModel.append({"aimId":aimId,"name":aimName,"timeAndDate":timeAndDate,"comment":comment,"tag":tag,
                                 "assignTo":assignTo,"priority":priority,
                                 "progress":progress,"progressText":progressText,
                                 "parentAim":parentAim,"childAim":childAim,
                                 "repeatable":repeatable,"privacy":privacy})
        }
    }
    //unite + extend
    function searchModelTag(searchTag)
    {
        listModel.clear()
        var aimList =  localBase.getAims()

         var regExpTag= new RegExp(searchTag)

        for (var i = 0; i < aimList.length; ++i)
        {
            var aimId = aimList[i][0]
            var aimName = aimList[i][1]
            var timeAndDate = aimList[i][2]
            var comment = aimList[i][3]
            var tag = aimList[i][4]
            var assignTo = aimList[i][5]
            var priority = aimList[i][6]

            var progress = aimList[i][7]
            var progressText = aimList[i][8]
            var parentAim = aimList[i][9]
            var childAim = aimList[i][10]

            var repeatable = aimList[i][11]
            var privacy = aimList[i][12]

            //also could run as set of filters making another list - first filter name, then tag etc

            if (tag.search(regExpTag) !== -1)
            //if (tag == searchTag)
                listModel.append({"aimId":aimId,"name":aimName,"timeAndDate":timeAndDate,"comment":comment,"tag":tag,
                                      "assignTo":assignTo,"priority":priority,
                                      "progress":progress,"progressText":progressText,
                                      "parentAim":parentAim,"childAim":childAim,
                                      "repeatable":repeatable,"privacy":privacy})
        }
    }


    ListModel {
        id: listModel
        Component.onCompleted: {
            loadModel()
        }
    }

}
