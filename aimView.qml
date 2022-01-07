import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3

//import QtQuick.Controls.Styles.Flat 1.0 as Flat

Item {

    id: aimViewWindow
    width: screenGlobal.getScreenWidth()//480
    height: screenGlobal.getScreenHeight()//800

    signal requestOpenAddAim()
    signal requestOpenEditAim(var aimId)
    signal requestOpenSingleAim(var aimId)

    property int elementHeight: screenGlobal.adaptYSize(45)
    property int widthOffset: screenGlobal.adaptXSize(70)
    property int yOffset: screenGlobal.adaptYSize(50) //be aware of low screens try everywhere

    property int microOffset : screenGlobal.adaptXSize(10)

    property int roundButtonFontSize : screenGlobal.adaptYSize(20)
    property int aimListMicroOffset : screenGlobal.adaptYSize(25)

    property bool desktopClient: Qt.platform.os != "android" && Qt.platform.os != "ios"

    Item
    {
      y: yOffset
      x: microOffset //widthOffset

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
           font.pixelSize: roundButtonFontSize

           onTextChanged:
           {
               searchTimer.restart()

               if (searchByName.checked)
               {
                   if (text.length > 0)
                       aimList.searchModel(text)
                   else
                       aimList.loadModel()
               }

               if (searchByTag.checked)
               {
                   if (text.length > 0)
                       aimList.searchModelTag(text)
                   else
                       aimList.loadModel()
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
            font.pixelSize: roundButtonFontSize
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
            font.pixelSize: roundButtonFontSize

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
        y: yOffset - microOffset*2
        x: searchBox.width + widthOffset + microOffset*2//there was 15 oups

        width: widthOffset-microOffset

        id: settingsBox
        visible: desktopClient //false

        RoundButton
        {
            text:  "*" // "\u2699"
            font.pixelSize: roundButtonFontSize
            onClicked:
            {
                //HEY it should be called from the view item!

                aimList.requestSettingsPopup()
            }

            ToolTip.visible: hovered
            ToolTip.text: qsTr("Open settings")
        }
    }

    //please note it yet not hided like another button does, so it would have to make some tricks on android around it, but its simple
    Item
    {
        //settings box
        y: yOffset*2
        x: searchBox.width + widthOffset + microOffset*2

        width: widthOffset-microOffset

        id: addBox
       // visible: desktopClient //false

        RoundButton
        {
            text:  "+"
            font.pixelSize: roundButtonFontSize
            onClicked:
            {
                aimViewWindow.requestOpenAddAim()
            }

            ToolTip.visible: hovered
            ToolTip.text: qsTr("Add aim")
        }
    }




    Timer{ //should avoid it for desktop
            id: searchTimer; running: false; interval: 10000; repeat: false
            onTriggered: {
                //THIS timer now broken - should repair it later
                if (desktopClient == false)
                {
                    searchBox.visible = false
                    settingsBox.visible = false

                    //seams we miss good animation here

                    //probably later we would make it the same as desktop

                    /*
                    if (listModel.count == 0){
                        running = true }
                    else
                    {}*/
                }
            }
        }


    Rectangle
    {
         y: searchBox.visible ? searchBox.fullHeight + yOffset +microOffset: yOffset+microOffset
         x: 0
         width: parent.width
         height:microOffset

         border.color: "black"
         color: "gray"
    }

    AimList
    {
        id: aimList
        width: parent.width
        y: searchBox.visible ? searchBox.fullHeight + yOffset+aimListMicroOffset : yOffset+aimListMicroOffset
        height: parent.height - yOffset*4 //if put someting down make it *4

        Component.onCompleted:
        {
            aimList.loadModel()
        }
    }

}
