import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3
import QtQuick.Extras 1.4 as QmlExtra
import QtQuick.Controls 1.4 as QMLOld
import QtQuick.Controls.Styles.Flat 1.0 as Flat


Item {

    id: structureItem

    width: 480
    height: 800

    QMLOld.TreeView
    {
        id: treeView
        model: tagTree

        width: 480
        height: 300

        x: 0
        y: 50

        itemDelegate:   Item {
            id: treeDelegate
            TextEdit {
                id: editingField
                anchors.fill: parent
                color: styleData.textColor
                text: styleData.value
            }
            Connections {
                target: editingField
                onEditingFinished: {


                    tagTree.setItemText(tagTree.currentIndex,editingField.text, tagTree.getColumnIndex(styleData.role))
                }
            }
        }

        QMLOld.TableViewColumn {
            role: tagTree.getColumnName(0)
            title: "tag"
            width: 300
        }


        onClicked:
        {
            //mainWindow.reloadPlaceList(firmTree.getFieldByIndex(index,1))
            //console.log("Argument clicked 1 : " + tagTree.getFieldByIndex(index,1))
            //console.log("Argument clicked 0 : " + tagTree.getFieldByIndex(index,0))



            searchModelTag(tagTree.getFieldByIndex(index,0)) //first argument must be tag name
        }
    }


    //ViewSection



    //ACTUAL FUNCTION FOR FUTURE
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
            var childAim = aimList[i][11]

            var repeatable = aimList[i][12]
            var privacy = aimList[i][13]
            //also could run as set of filters making another list - first filter name, then tag etc

            //if (tag.search(regExpTag) !== -1) //MUST THEN MAKE TOTAL EQUAL!!
            if (tag == searchTag)
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
        }
    }

    //COMPONENTS THAT USED IN 3 FILES and should be placed out..

    Component {
        id: highlightBar
        Rectangle {
            id: highlightBarRect
            width: 200; height: 50
            color: userSettings.getColor("ListHightlight")//"#FFFF88"
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
                    Text { color:userSettings.getColor("Text");text: 'Comment: ' + comment;  font.pointSize: 10}

                    Text { color:userSettings.getColor("Text");text: 'Parent: ' + parentAim;  font.pointSize: 10}
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




    ListView
    {
        id:aimList
        clip: true
        width: parent.width
        y: 400
        height: 300 //if put someting down make it *4

        model: listModel

        Behavior on y {
                    NumberAnimation{ duration: 200 }
                }

        delegate: anotherDeligate//listDelegate

        highlight: highlightBar//Rectangle { color: "lightsteelblue"; radius: 5 }
        focus:  true

        ScrollBar.vertical: ScrollBar {}
    }

}
