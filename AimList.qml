import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.1
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.15



//import Qt.labs.handlers 1.0

//import QtQuick.Controls.Styles.Flat 1.0 as Flat

Item {

    id: aimListItem

     Component.onCompleted: {
        loadViewSettings()
     }

    property string highlightAimColor : userSettings.getColor("ListHightlight") //to settings please

    property int contentMoveToSearch : screenGlobal.adaptYSize(-100)
    property int contentMoveToHideSearch : screenGlobal.adaptYSize(300)

    property int listElementHeight : screenGlobal.adaptYSize(110)
    property int firstSectionHeight : screenGlobal.adaptYSize(30)
    property int secondSectionHeight : screenGlobal.adaptYSize(25)
    property int thirdSectionHeight : screenGlobal.adaptYSize(25)
    property int forthSectionHeight : screenGlobal.adaptYSize(25)

    property int fontSizeSmall : screenGlobal.adaptYSize(10) //here some names are swapped
    property int fontSizeMiddle : screenGlobal.adaptYSize(15)
    property int fontSizeBig : screenGlobal.adaptYSize(20)

    property int highlightWidth : screenGlobal.adaptXSize(200)
    property int highlightHeight : screenGlobal.adaptYSize(50)

    property int popupXOffset : screenGlobal.adaptXSize(1)
    property int popupYOffset : screenGlobal.adaptYSize(1)

    property int popupWidth : screenGlobal.adaptXSize(230)
    property int popupHeight : screenGlobal.adaptYSize(560)

    property int microOffset : screenGlobal.adaptXSize(10)


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
                    Text { id: aimNameText; color:userSettings.getColor("Text");   text: '<b>' + name + '</b>' ; font.pixelSize: fontSizeBig}
                    rightPadding:  (aimList.width - aimNameText.width)/2 - microOffset
                    leftPadding:  (aimList.width - aimNameText.width)/2 - microOffset
                }
                RowLayout
                {
                    height: secondSectionHeight
                    width: aimList.width
                    Text { color:userSettings.getColor("Text");text: 'Tag: ' + tag; visible: aimListItem.tagShow && tag.length > 0   ; font.pixelSize: fontSizeMiddle}
                    //+20 is for transition animation - check it better later please
                    Text { rightPadding: fontSizeMiddle+20; Layout.alignment: Qt.AlignRight; color:userSettings.getColor("Text");text: 'Moment: ' + timeAndDate; visible: aimListItem.timeAndDateShow && timeAndDate.length > 0   ; font.pixelSize: fontSizeMiddle}
                }
                RowLayout
                {
                    height: thirdSectionHeight
                    Text { color:userSettings.getColor("Text");text: 'Parent: ' + parentName; visible: aimListItem.parentAimShow  && parentName.length > 0 ; font.pixelSize: fontSizeSmall}
                    Text { color:userSettings.getColor("Text");text: 'Comment: ' + comment; visible: aimListItem.commentShow  && comment.length > 0 ; font.pixelSize: fontSizeSmall}
                }
                RowLayout
                {
                    height: forthSectionHeight
                    //width: aimList.width //it was on upper one

                    Text {  color:userSettings.getColor("Text");text: 'AssignTo: ' + assignTo; visible: aimListItem.assignToShow && assignTo.length > 0 && assignTo != "Assign to:"  ; font.pixelSize: fontSizeSmall}
                    //spacing: 50 //later can make it just like left\right padding
                    Text { Layout.alignment: Qt.AlignRight; color:userSettings.getColor("Text");text: 'Priority: ' + priority; visible: aimListItem.priorityShow && priority.length > 0; font.pixelSize: fontSizeSmall}
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

                onDoubleClicked: {
                    var index = aimList.currentIndex
                    var aimId = listModel.get(index).aimId
                    aimListItem.parent.requestOpenSingleAim(aimId) //aimViewWindow
                }

                onPressAndHold: {
                    wrapper.ListView.view.currentIndex = index
                    aimMenu.x = mouse.x
                    aimMenu.y = parent.y + mouse.y //works on item
                    aimMenu.open()
                }
            }

            TapHandler{ //failed to use because of index, and cannot get focuse while mouse area
                 onTapped:
                 {
                     if (tapCount == 3)
                     {
                         console.log("tap 3")

                         var index = aimList.currentIndex
                         var aimId = listModel.get(index).aimId
                         aimListItem.parent.requestOpenSingleAim(aimId) //aimViewWindow
                     }
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
            font.pixelSize: fontSizeBig
            onTriggered:{
                var index = aimList.currentIndex
                var aimId = listModel.get(index).aimId
                aimListItem.parent.requestOpenSingleAim(aimId) //aimViewWindow
            }
        }
        MenuItem {
            text: "Edit"
            font.pixelSize: fontSizeBig
            onTriggered:{
               // console.log("edit triggered")
                var index = aimList.currentIndex
                var aimId = listModel.get(index).aimId
                aimListItem.parent.requestOpenEditAim(aimId)
            }
        }
        MenuItem {
            text: "Delete"
            font.pixelSize: fontSizeBig
            onTriggered:{
                console.log("delete triggered")
                //aimList.currentIndex
                deleteConfirmDialog.open()
            }
        }
        MenuItem {
            text: "Send"
            onTriggered:{
                console.log("send triggered")
            }
        }
        MenuItem {
            text: "Add child"
            onTriggered:{
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
            Behavior on y { SpringAnimation { spring: 2*screenGlobal.adaptXSize(1); damping: 0.3*screenGlobal.adaptXSize(1) } } //read about it
        }
    }

    function loadModel()
    {
        listModel.clear()

        var aimList
        var sortingOrderType = userSettings.getSortingOrderType()

        if (sortingOrderType === 0)
            aimList  =  localBase.getAims()
        else if (sortingOrderType === 1)
            aimList  =  localBase.getAimsBackwards()
        else if (sortingOrderType === 2)
            ; //tree like mode
        else if (sortingOrderType === 3)
            aimList = localBase.getLastActsAims()
        else if (sortingOrderType === 4)
            aimList = localBase.getFutureAims() //no only got future aims, later can then have no date aims, and in the end delayed(expired)


        //console.log("Sorting order type: " + sortingOrderType)
        for (var i = 0; i < aimList.length; ++i)
        {
            //PLACE OF FUTE REFACTORING:
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
            //END OF FUTER REFACTORING
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
        width:  parent.width//popupWidth
        height: parent.height//popupHeight //ensure manual sizing
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

            sortingOrder.currentIndex = userSettings.getSortingOrderType()
        }

        Item {
            RowLayout{
                spacing: 35
            ColumnLayout{
                CheckBox{
                    id: timeAndDateField
                    text: "Time and date"
                    font.pixelSize: fontSizeMiddle
                }
                CheckBox{
                    id: commentField
                    text: "Comment"
                    font.pixelSize: fontSizeMiddle
                }
                CheckBox{
                    id: tagField
                    text: "Tag"
                    font.pixelSize: fontSizeMiddle
                }
                CheckBox{
                    id: assignToField
                    text: "Assign to"
                    font.pixelSize: fontSizeMiddle
                }
                CheckBox {
                    id: priorityField
                    text: "Priority"
                    font.pixelSize: fontSizeMiddle
                }
                CheckBox{
                    id: progressField
                    text: "Progress"
                    font.pixelSize: fontSizeMiddle
                }
                CheckBox {
                    id: progressTextField
                    text: "Progress text"
                    font.pixelSize: fontSizeMiddle
                }
                CheckBox {
                    id: parentField
                    text: "Parent"
                    font.pixelSize: fontSizeMiddle
                }
                CheckBox {
                    id: childField
                    text: "Child"
                    font.pixelSize: fontSizeMiddle
                }

            }
            ColumnLayout{
                RowLayout{
                    Text{text:"Sorting order: ";color:"lightgray"}
                    ComboBox{
                    id: sortingOrder
                    model:["from first","from last","as tree","last acts","sequencial"]
                    //In tree like there should be also calculated total depth to make offset 20*totalDepth
                    //and in search while tree like there must be checkbox "show children of found"
                    font.pixelSize: fontSizeMiddle
                    }
                }
                CheckBox{
                    text: "Show children in search"
                    enabled: false //start enabling it only when tree like is done
                    font.pixelSize: fontSizeMiddle
                }
                CheckBox{
                    text: "Hide search bar when unused"
                    enabled: false
                    //but yet not implemented
                    font.pixelSize: fontSizeMiddle
                }
                //Here we imagine some other options
                CheckBox{
                    text: "Store aims online"
                    enabled: false
                    font.pixelSize: fontSizeMiddle
                }
                CheckBox{
                    text: "Let unknown people add me"
                    enabled: false
                    font.pixelSize: fontSizeMiddle
                }
                RowLayout{
                    TextField{
                       placeholderText: "nickname"
                       enabled: false
                       font.pixelSize: fontSizeMiddle
                    }
                    Button{
                        text:"Change nick"
                        enabled: false
                        font.pixelSize: fontSizeMiddle
                    }
                }
                Button{
                    id: changePasswordButton
                    text:"Create or change password"
                    enabled: false
                    font.pixelSize: fontSizeMiddle
                }
                RowLayout{
                    Button{
                        text:"Login"
                        enabled: false
                        font.pixelSize: fontSizeMiddle
                    }
                    CheckBox{
                        text:"Don't autologin"
                        font.pixelSize: fontSizeMiddle
                    }
                }

                Button {
                    Layout.fillWidth: true
                    text:"Save settings"
                    font.pixelSize: fontSizeMiddle
                    onClicked:
                    {
                        var viewAimList = [timeAndDateField.checked,commentField.checked,
                                tagField.checked,assignToField.checked,
                                priorityField.checked,progressField.checked,progressTextField.checked,
                                parentField.checked,childField.checked]

                        userSettings.setViewAimSettings(viewAimList)
                        userSettings.setSortingOrderType(sortingOrder.currentIndex)
                        loadViewSettings()
                        viewSettingsPopup.close()

                        //need to reload last type of loading - to apply sorting model
                        aimListItem.loadModel()
                        //it means we need to store the last type of loading an repeat it

                        //also NOTE THAT WE CAN REFACT ALL THE LOAD funcions
                        //BY taking outside loadFromSingleLine( )
                        //so the different code will become so much smaller
                    }
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
