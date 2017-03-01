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

    property string highlightAimColor : "#98FB98" //to settings please

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
            onCheckedChanged:
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
            x: parent.width - width
            id: searchByTag
            text: "tag"

            onCheckedChanged:
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
        y: 100
        width: 230
        height: 525
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

            //DateTimeChoose{}
            //old way - and its better and must be applied on refact, but now we should do faster way :
        Item {
            ColumnLayout
            {
            CheckBox
            {
                text: "Aim list"
            }
            CheckBox
            {
                text: "Time and date"
            }
            CheckBox
            {
                text: "Category"
            }
            CheckBox
            {
                text: "Repeatable"
            }
            CheckBox
            {
                text: "Privacy"
            }
            CheckBox
            {
                text: "Assign to"
            }
            CheckBox
            {
                text: "Parent"
            }
            CheckBox
            {
                text: "Child"
            }
            CheckBox
            {
                text: "Progress"
            }
            RowLayout
            {
                Rectangle
                {
                    id: colorTester
                    height: 35
                    width: 100 //calculate better
                    color: "#98FB98"
                    border.color: "black"
                    border.width: 1
                }
                Button
                {
                    text: "Highlight color"
                    onClicked:
                    {
                        colorDialog.color = colorTester.color
                        colorDialog.open()
                    }
                }
            }
            RowLayout
            {
                Button
                {
                    text:"Cancel"
                    onClicked:
                    {
                        viewSettingsPopup.close()
                    }
                }
                Button
                {
                    text:"Save settings"
                    onClicked:
                    {
                        aimViewWindow.highlightAimColor = colorTester.color
                        viewSettingsPopup.close()
                    }
                }//leave here only save button
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
    Component {
        id: anotherDeligate
        Item {
            id: wrapper
            width: aimList.width
            height: 55      //calculate on option
            Column {
                Text { text: 'Name: ' + name }
                Text { text: 'Category: ' + category }
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

            listModel.append({"name":aimName,"list":aimList,"timeAndDate":timeAndDate,"category":category,
                             "repeateable":repeatable,"privacy":privacy,"assignTo":assignTo,"parent":parentAim,
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
                listModel.append({"name":aimName,"list":aimList,"timeAndDate":timeAndDate,"category":category,
                                 "repeateable":repeatable,"privacy":privacy,"assignTo":assignTo,"parent":parentAim,
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
                listModel.append({"name":aimName,"list":aimList,"timeAndDate":timeAndDate,"category":category,
                                 "repeateable":repeatable,"privacy":privacy,"assignTo":assignTo,"parent":parentAim,
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
