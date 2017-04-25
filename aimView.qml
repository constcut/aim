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

    property int elementHeight: 45
    property int widthOffset: 70
    property int yOffset: 50 //be aware of low screens try everywhere

    property string highlightAimColor : userSettings.getColor("ListHightlight") //to settings please

    Item
    {
      y: yOffset
      x: widthOffset

      width: parent.width - widthOffset*2

      id: searchBox
      visible: false //false

      property int fullHeight: elementHeight*2


       TextField
       {
           id: aimNameSearch
           placeholderText: "Input here aim name"
           visible: true
           width: parent.width

           onTextChanged:
           {
               searchTimer.restart()
               if (text.length > 0)
               {
                   searchModel(text)
               }
               else
               {
                   loadModel()
               }
           }
       }
       ComboBox
       {
           id: aimTagSearch
           model: ["Select tag: ", "tag1","tag2","tag3","tag4"]
           visible: false
           width: parent.width

           onActiveFocusChanged:
           {
               if (activeFocus)
                   searchTimer.stop()
               else
                   searchTimer.start()
           }

           onActivated:
           {
            searchTimer.start()

               if (currentIndex==0)
                   loadModel()
               else
                   searchModelTag(currentText)
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
                if (checked)
                {
                    aimTagSearch.visible = false
                    aimNameSearch.visible = true
                    searchTimer.restart()
                }
            }
       }

       RadioButton
       {
            y: elementHeight
            x: parent.width/2 - width/2
            id: searchByType
            text: "type"   //assigned to me, done, overdated etc

            onCheckedChanged: //cover under function
            {
                if (checked)
                {
                    aimNameSearch.visible = false
                    aimTagSearch.visible = true
                    searchTimer.restart()
                }
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
                if (checked)
                {
                    aimNameSearch.visible = false
                    aimTagSearch.visible = true
                    searchTimer.restart()
                }
            }
       }
    } //SEARCH BOX END
    Item
    {
        //settings box
        y: yOffset
        x: searchBox.width + widthOffset + 15

        width: widthOffset-10

        id: settingsBox
        visible: false //false

        RoundButton
        {
            text:  "\u2699"
            font.pixelSize: 20
            onClicked:
            {
                viewSettingsPopup.open()
            }

            ToolTip.visible: hovered
            ToolTip.text: qsTr("Open settings")
        }
    }

    Popup {
        id: viewSettingsPopup //make also time popup with tumbler
        x: 100
        y: 50
        width: 230
        height: 560 //ensure manual sizing
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        Component.onCompleted: //onOpened:
        {
            var settingList = userSettings.getViewAimSettings()

            aimListField.checked = settingList[0]
            timeAndDateField.checked = settingList[1]
            categoryField.checked = settingList[2]
            repeatableField.checked = settingList[3]
            privacyField.checked = settingList[4]
            assignToField.checked = settingList[5]
            parentField.checked  = settingList[6]
            childField.checked  = settingList[7]
            progressField.checked  = settingList[8]
        }

        Item {
            ColumnLayout
            {
            CheckBox
            {
                id: aimListField
                text: "Aim list"
            }
            CheckBox
            {
                id: timeAndDateField
                text: "Time and date"
            }
            CheckBox
            {
                id: categoryField
                text: "Category"
            }
            CheckBox
            {
                id: repeatableField
                text: "Repeatable"
            }
            CheckBox
            {
                id: privacyField
                text: "Privacy"
            }
            CheckBox
            {
                id: assignToField
                text: "Assign to"
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
            CheckBox
            {
                id: progressField
                text: "Progress"
            }
            Button
            {
                text:"Save settings"
                onClicked:
                {
                    var viewAimList = [aimListField.checked,timeAndDateField.checked,
                            categoryField.checked,repeatableField.checked,
                            privacyField.checked,assignToField.checked,parentField.checked,
                            childField.checked,progressField.checked]

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
            id: searchTimer; running: false; interval: 10000; repeat: false
            onTriggered: {

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


    ListView
    {
        id:aimList
        clip: true
        width: parent.width
        y: searchBox.visible ? searchBox.fullHeight + yOffset : yOffset
        height: parent.height - yOffset*4 //if put someting down make it *4

        model: listModel

        Behavior on y {
                    NumberAnimation{ duration: 200 }
                }

        onContentYChanged: {
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

        delegate: anotherDeligate//listDelegate

        highlight: highlightBar//Rectangle { color: "lightsteelblue"; radius: 5 }
        focus:  true


        ScrollBar.vertical: ScrollBar {}
    }


    //update width and so on

    function loadViewSettings()
    {
       var settingsList =  userSettings.getViewAimSettings()

        aimViewWindow.aimListShow = settingsList[0]
        aimViewWindow.timeAndDateShow = settingsList[1]
        aimViewWindow.categoryShow = settingsList[2]
        aimViewWindow.repeatableShow = settingsList[3]
        aimViewWindow.privacyShow = settingsList[4]
        aimViewWindow.assignToShow = settingsList[5]
        aimViewWindow.parentAimShow = settingsList[6]
        aimViewWindow.childAimsShow = settingsList[7]
        aimViewWindow.progressShow = settingsList[8]
    }

    property bool aimListShow : true
    property bool timeAndDateShow: true
    property bool categoryShow: true
    property bool repeatableShow: true
    property bool privacyShow: true
    property bool assignToShow: true
    property bool parentAimShow: true
    property bool childAimsShow: true
    property bool progressShow: true

    Component {
        id: anotherDeligate


        Item {
            id: wrapper //rename on refact
            width: aimList.width
            height: 110  //means need to know fonts also


            Column {
                Row
                {
                    height: 25 //must look over here
                    Text { text: '<i>Name:</i> <b>' + name + '</b>' }
                    Text { text: ' Moment: ' + timeAndDate; visible: aimViewWindow.timeAndDateShow }
                    Text { text: ' Repeat: ' + repeatable; visible: aimViewWindow.repeatableShow }
                }
                Row
                {
                    height: 25
                    Text { text: 'Category: ' + categoryValue; visible: aimViewWindow.categoryShow }
                    Text { text: ' List: ' + aimListName; visible: aimViewWindow.repeatableShow }
                    Text { text: ' Progress: ' + progress; visible: aimViewWindow.progressShow }
                }
                Row
                {
                    height: 25
                    Text { text: ' Privacy: ' + privacy; visible: aimViewWindow.privacyShow}
                    Text { text: ' AssignTo: ' + assignTo; visible: aimViewWindow.assignToShow}
                }
                Row
                {
                    height: 25
                    Text { text: 'Parent: ' + parentAim; visible: aimViewWindow.parentAimShow}
                    Text { text: ' Children: ' + child; visible: aimViewWindow.childAimsShow}
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
        }
        MenuItem {
            text: "Delete"
        }
        MenuItem {
            text: "Send"
        }
    }

    Component {
        id: highlightBar
        Rectangle {
            id: highlightBarRect
            width: 200; height: 50
            color: highlightAimColor //"#FFFF88"
            y: aimList.currentItem.y;
            Behavior on y { SpringAnimation { spring: 2; damping: 0.1 } } //read about it
        }
    }

    function loadModel()
    {
        listModel.clear()
        var aimList =  localBase.getAims()

        for (var i = 0; i < aimList.length; ++i)
        {
            //aimId is 0


            var aimName = aimList[i][1]
            var aimListName = aimList[i][2]
            var timeAndDate = aimList[i][3]
            var category = aimList[i][4]
            var repeatable = aimList[i][5]
            var privacy = aimList[i][6]
            var assignTo = aimList[i][7]
            var parentAim = aimList[i][8]
            var childAim = aimList[i][9]
            var progress = aimList[i][10]

            //var linkList = localBase.getAimLinks(aimName)

            listModel.append({"name":aimName,"aimListName":aimList,"timeAndDate":timeAndDate,"categoryValue":category,
                             "repeatable":repeatable,"privacy":privacy,"assignTo":assignTo,"parentAim":parentAim,
                             "child":childAim,"progress":progress})
        }
    }

    function searchModel(searchNames)
    {
        listModel.clear()
        var aimList =  localBase.getAims()

        var regExpName = new RegExp(searchNames)

        for (var i = 0; i < aimList.length; ++i)
        {
            var aimName = aimList[i][1]
            var aimListName = aimList[i][2]
            var timeAndDate = aimList[i][3]
            var category = aimList[i][4]
            var repeatable = aimList[i][5]
            var privacy = aimList[i][6]
            var assignTo = aimList[i][7]
            var parentAim = aimList[i][8]
            var childAim = aimList[i][9]
            var progress = aimList[i][10]

            //also could run as set of filters making another list - first filter name, then tag etc
            if (aimName.search(regExpName) !== -1)
                listModel.append({"name":aimName,"aimListName":aimList,"timeAndDate":timeAndDate,"categoryValue":category,
                                 "repeateable":repeatable,"privacy":privacy,"assignTo":assignTo,"parentAim":parentAim,
                                 "child":childAim,"progress":progress})
        }
    }
    //unite + extend
    function searchModelTag(searchTag)
    {
        listModel.clear()
        var aimList =  localBase.getAims()

        for (var i = 0; i < aimList.length; ++i)
        {
            var aimName = aimList[i][1]
            var aimListName = aimList[i][2]
            var timeAndDate = aimList[i][3]
            var category = aimList[i][4]
            var repeatable = aimList[i][5]
            var privacy = aimList[i][6]
            var assignTo = aimList[i][7]
            var parentAim = aimList[i][8]
            var childAim = aimList[i][9]
            var progress = aimList[i][10]

            //also could run as set of filters making another list - first filter name, then tag etc
            if (category == searchTag)
                listModel.append({"name":aimName,"aimListName":aimList,"timeAndDate":timeAndDate,"categoryValue":category,
                                 "repeateable":repeatable,"privacy":privacy,"assignTo":assignTo,"parentAim":parentAim,
                                 "child":childAim,"progress":progress})
        }
    }


    ListModel {
        id: listModel
        Component.onCompleted: {
            loadModel()
        }
    }

}
