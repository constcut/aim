import QtQuick 2.8
import QtQuick.Controls 2.4
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.11
import QtQuick.Extras 1.4 as QmlExtra
import QtQuick.Controls 1.4 as QmlOld
import QtQuick.Controls.Styles.Flat 1.0 as Flat

Item {
    id: aimSelectWindow

    //width: screenGlobal.getScreenWidth()//480
    //height: screenGlobal.getScreenHeight()//800

    anchors.fill: parent

    signal selectionCanceled()
    signal aimSelected(var selectedAimId)

    //property string aimId : ""

    property int smallFontSize : screenGlobal.adaptYSize(15)
    property int fontSizeBig : screenGlobal.adaptYSize(20)

    property int highlightWidth : screenGlobal.adaptXSize(200)
    property int highlightHeight : screenGlobal.adaptYSize(50)
    property string highlightAimColor : userSettings.getColor("ListHightlight") //to settings please

    property int listElementHeight : screenGlobal.adaptYSize(40)

    Component.onCompleted: {
        console.log("Aim Selection loading")
        loadModel()
        console.log("Aim Selection loaded " + aimNameSearch.y)
        console.log("List len " +  listModel.length)
    }


    function loadModel(){
        listModel.clear()
        var aimList =  localBase.getAims() //sorting order not used here

        for (var i = 0; i < aimList.length; ++i)
        {
            var aimId = aimList[i][0]
            var aimName = aimList[i][1]
            var parentAim = aimList[i][10]

            var parentName = ""
            var parentLine = localBase.getSingleAim(parentAim)
            if (parentLine.length > 1)
                parentName = parentLine[1]

            listModel.append({"aimId":aimId,"aimName":aimName,"parentName":parentName})
        }
    }

    function searchNameModel(searchName){
        listModel.clear()
        var aimList =  localBase.getAims() //sorting order not used here
        var regExpName = new RegExp(searchName)

        for (var i = 0; i < aimList.length; ++i)
        {
            var aimId = aimList[i][0]
            var aimName = aimList[i][1]
            var parentAim = aimList[i][10]

            var parentName = ""
            var parentLine = localBase.getSingleAim(parentAim)
            if (parentLine.length > 1)
                parentName = parentLine[1]

            if (aimName.search(regExpName) !== -1)
                listModel.append({"aimId":aimId,"aimName":aimName,"parentName":parentName})
        }
    }



    TextField{
        id: aimNameSearch
        placeholderText: "Aim name"
        x:0
        y:screenGlobal.adaptYSize(50)
        //height: screenGlobal.adaptYSize(40) //?

        visible: true
        width: parent.width
        font.pixelSize: smallFontSize//fontSizeBig

        onTextChanged:{
                if (text.length > 0)
                    aimSelectWindow.searchNameModel(text)
                else
                    aimSelectWindow.loadModel()
        }
    }

    ListModel {
        id: listModel
    }


    ListView{
        id:aimList
        clip: true

        y: aimNameSearch.height + aimNameSearch.y + screenGlobal.adaptYSize(10)
        x: 0
        width: aimSelectWindow.width
        height: aimSelectWindow.height - aimNameSearch.height - selectButton.height - screenGlobal.adaptYSize(10)

        model: listModel
        Behavior on y { NumberAnimation{ duration: 200 }}
        delegate: anotherDeligate
        highlight: highlightBar
        focus:  true
        ScrollBar.vertical: ScrollBar {}
    }

    Component {
        id: anotherDeligate
        Item {
            id: wrapper //rename on refact
            width: aimList.width
            height: listElementHeight  //means need to know fonts also

            //name + [parent name]
            Text { id: oneLineText; color:userSettings.getColor("Text");
                text: '<b>' + aimName + '</b>  [' + parentName + ']';
                font.pixelSize: fontSizeBig }

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
            }
        }
    }

    Component {
        id: highlightBar
        Rectangle {
            id: highlightBarRect
            width: highlightWidth; height: highlightHeight
            color: highlightAimColor
            y: aimList.currentItem.y; //warning clean out please

            Behavior on y {
               NumberAnimation{ duration: 200 } }
        }
    }

    Button {
        y: parent.height - height - screenGlobal.adaptYSize(10)
        x: 0
        text: "Cancel"
        onPressed: aimSelectWindow.selectionCanceled()
    }

    Button{
        id: selectButton
        y: parent.height - height - screenGlobal.adaptYSize(10)
        x: parent.width - width //- screenGlobal.adaptXSize(10)
        text: "Select"
        onPressed: {
            aimSelectWindow.aimSelected(listModel.get(aimList.currentIndex).aimId)
        }
    }

    //cancel & select buttons
}
