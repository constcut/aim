import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3

import QtQuick.Controls.Styles.Flat 1.0 as Flat

Item {

    id: aimListItem

     Component.onCompleted: {
        loadViewSettings()
     }

    property string highlightAimColor : userSettings.getColor("ListHightlight") //to settings please

    property int contentMoveToSearch : -100
    property int contentMoveToHideSearch : 300

    property int listElementHeight : 110
    property int firstSectionHeight : 30
    property int secondSectionHeight : 25
    property int thirdSectionHeight : 15
    property int forthSectionHeight : 25

    property int fontSizeSmall : 10
    property int fontSizeMiddle : 15
    property int fontSizeBig : 20

    property int highlightWidth : 200
    property int highlightHeight : 50

    property int popupXOffset : 50
    property int popupYOffset : 50

    property int popupWidth : 230
    property int popupHeight : 560

    property int microOffset : 10


    ListView
    {
        id:aimList
        clip: true
        width: parent.width
        //y: 0
        height: parent.height

        model: listModel

        Behavior on y {
                    NumberAnimation{ duration: 200 }
                }

        onContentYChanged: {


                   if (desktopClient == false) //android or ios
                   {
                       if( contentY < contentMoveToSearch ) {
                           searchBox.visible = true
                           settingsBox.visible = true
                           searchTimer.running = true
                       }
                       if (contentY > contentMoveToHideSearch)
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

    function loadViewSettings()
    {
       var settingsList =  userSettings.getViewAimSettings()

        aimListItem.timeAndDateShow = settingsList[0]
        aimListItem.commentShow = settingsList[1]
        aimListItem.tagShow = settingsList[2]
        aimListItem.assignToShow = settingsList[3]
        aimListItem.priorityShow = settingsList[4]
        aimListItem.progressShow = settingsList[5]
        aimListItem.progressTextShow = settingsList[6]
        aimListItem.parentAimShow = settingsList[7]
        aimListItem.childAimShow = settingsList[8]

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
            height: listElementHeight  //means need to know fonts also

            Column {
                Row
                {
                    height: firstSectionHeight //must look over here
                    Text { id: aimNameText; color:userSettings.getColor("Text");   text: '<b>' + name + '</b>' ; font.pointSize: fontSizeBig}
                    rightPadding:  (aimList.width - aimNameText.width)/2 - microOffset
                    leftPadding:  (aimList.width - aimNameText.width)/2 - microOffset
                }
                RowLayout
                {
                    height: secondSectionHeight
                    width: aimList.width

                    Text { color:userSettings.getColor("Text");text: 'Tag: ' + tag; visible: aimListItem.tagShow && tag.length > 0   ; font.pointSize: fontSizeMiddle}
                    //spacing: 50 //later can make it just like left\right padding
                    Text { rightPadding: fontSizeMiddle; anchors.right: parent.right; color:userSettings.getColor("Text");text: 'Moment: ' + timeAndDate; visible: aimListItem.timeAndDateShow && timeAndDate.length > 0   ; font.pointSize: fontSizeMiddle}
                }
                RowLayout
                {
                    height: thirdSectionHeight
                    width: aimList.width

                    Text {  color:userSettings.getColor("Text");text: 'AssignTo: ' + assignTo; visible: aimListItem.assignToShow && assignTo.length > 0 && assignTo != "Assign to:"  ; font.pointSize: fontSizeSmall}
                    //spacing: 50 //later can make it just like left\right padding
                    Text { anchors.right: parent.right; color:userSettings.getColor("Text");text: 'Priority: ' + priority; visible: aimListItem.priorityShow && priority.length > 0; font.pointSize: fontSizeSmall}
                }
                Row
                {
                    height: forthSectionHeight
                    Text { color:userSettings.getColor("Text");text: 'Comment: ' + comment; visible: aimListItem.commentShow  && comment.length > 0 ; font.pointSize: fontSizeSmall}

                    Text { color:userSettings.getColor("Text");text: 'Parent: ' + parentName; visible: aimListItem.parentAimShow  && parentName.length > 0 ; font.pointSize: fontSizeSmall}
                }
            }
            states: State { // indent the item if it is the current item
                name: "Current"
                when: wrapper.ListView.isCurrentItem
                PropertyChanges { target: wrapper; x: fontSizeBig }
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


    MessageDialog {
        id: deleteConfirmDialog
        title: "Confirm"
        text: "Are you sure want to delete this aim?"

        standardButtons: StandardButton.Ok | StandardButton.Cancel
        //something not enough good with this style
        //long russian names became short

        onAccepted:   {
            var index = aimList.currentIndex
            var aimId = listModel.get(index).aimId
            localBase.deleteAim(aimId)
            loadModel()
        }
    }

    Menu {
        id: aimMenu

        MenuItem {
            text: "View"

            onTriggered:
            {
                var index = aimList.currentIndex
                var aimId = listModel.get(index).aimId
                aimListItem.parent.requestOpenSingleAim(aimId) //aimViewWindow
            }
        }


        MenuItem {
            text: "Edit"

            onTriggered:
            {
               // console.log("edit triggered")
                var index = aimList.currentIndex
                var aimId = listModel.get(index).aimId
                aimListItem.parent.requestOpenEditAim(aimId)
            }
        }
        MenuItem {
            text: "Delete"

            onTriggered:
            {
                console.log("delete triggered")

                //aimList.currentIndex
                deleteConfirmDialog.open()

            }
        }
        MenuItem {
            text: "Send"

            onTriggered:
            {
                console.log("send triggered")
            }
        }
        MenuItem {
            text: "Add child"

            onTriggered:
            {
                console.log("add child triggered")
            }
        }
    }

    Component {
        id: highlightBar
        Rectangle {
            id: highlightBarRect
            width: highlightWidth; height: highlightHeight
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

            var parentName = ""
            var parentLine = localBase.getSingleAim(parentAim)
            if (parentLine.length > 1)
                parentName = parentLine[1]


            var repeatable = aimList[i][11]
            var privacy = aimList[i][12]

            //quick shower
            if (repeatable.length > 0)
                timeAndDate += " : " + repeatable


            listModel.append({"aimId":aimId,"name":aimName,"timeAndDate":timeAndDate,"comment":comment,"tag":tag,
                             "assignTo":assignTo,"priority":priority,
                             "progress":progress,"progressText":progressText,
                             "parentAim":parentAim,
                             "repeatable":repeatable,"privacy":privacy,"parentName":parentName})
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

            var parentName = ""
            var parentLine = localBase.getSingleAim(parentAim)
            if (parentLine.length > 1)
                parentName = parentLine[1]

            var repeatable = aimList[i][11]
            var privacy = aimList[i][12]


            listModel.append({"aimId":aimId,"name":aimName,"timeAndDate":timeAndDate,"comment":comment,"tag":tag,
                             "assignTo":assignTo,"priority":priority,
                             "progress":progress,"progressText":progressText,
                             "parentName":parentName,
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

            var parentName = ""
                     var parentLine = localBase.getSingleAim(parentAim)
             if (parentLine.length > 1)
                     parentName = parentLine[1]

            var repeatable = aimList[i][11]
            var privacy = aimList[i][12]

            //quick shower
            if (repeatable.length > 0)
                timeAndDate += " : " + repeatable


            //also could run as set of filters making another list - first filter name, then tag etc
            if (aimName.search(regExpName) !== -1)
                listModel.append({"aimId":aimId,"name":aimName,"timeAndDate":timeAndDate,"comment":comment,"tag":tag,
                                 "assignTo":assignTo,"priority":priority,
                                 "progress":progress,"progressText":progressText,
                                 "parentAim":parentAim,
                                 "repeatable":repeatable,"privacy":privacy,"parentName":parentName})
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

            var parentName = ""
            var parentLine = localBase.getSingleAim(parentAim)
             if (parentLine.length > 1)
                     parentName = parentLine[1]

            var repeatable = aimList[i][11]
            var privacy = aimList[i][12]
            //also could run as set of filters making another list - first filter name, then tag etc

            //quick shower
            if (repeatable.length > 0)
                timeAndDate += " : " + repeatable

            if (tag.search(regExpTag) !== -1)
            //if (tag == searchTag)
                listModel.append({"aimId":aimId,"name":aimName,"timeAndDate":timeAndDate,"comment":comment,"tag":tag,
                                      "assignTo":assignTo,"priority":priority,
                                      "progress":progress,"progressText":progressText,
                                      "parentAim":parentAim,
                                      "repeatable":repeatable,"privacy":privacy,"parentName":parentName})
        }
    }


    ListModel {
        id: listModel
        Component.onCompleted: {
            //loadModel()
        }
    }

    //OTHER MOVED THINGS

    function requestSettingsPopup()
    {
        viewSettingsPopup.open()
    }

    Popup {
        id: viewSettingsPopup //make also time popup with tumbler
        x: popupXOffset
        y: popupYOffset
        width: popupWidth
        height: popupHeight //ensure manual sizing
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


}
