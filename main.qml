import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.1


ApplicationWindow {
    id: window
    title: qsTr("Aim")
    width: screenGlobal.getScreenWidth()//480
    height: screenGlobal.getScreenHeight()//800
    visible: true

    property int microYOffset : screenGlobal.adaptYSize(5)
    property int fontNormalSize : screenGlobal.adaptYSize(15)*1.5

    property bool desktopClient: Qt.platform.os != "android" && Qt.platform.os != "ios"

    Material.accent: userSettings.getColor("Accent");
    Material.primary: userSettings.getColor("Primary");
    //Material.foreground: userSettings.getColor("Foreground");
    //Material.background: userSettings.getColor("Background");
    Material.theme: Material.Dark

    Component.onCompleted: {
        //console.log (window.Material.foreground)
    }

    onClosing: {
        if (userSettings.isDebugBuild())
                Qt.quit()
            else
                window.hide();
    }

   header:  ToolBar
   {
       Material.background: userSettings.getColor("Background");
       //Material.foreground: userSettings.getColor("Foreground");
       //Material.accent: userSettings.getColor("Accent");
       //Material.primary: userSettings.getColor("Primary");

        id: toolBar
        RowLayout {
            ToolButton {
                text: "view"
                onClicked: mainLoader.source = "aimView.qml"
                font.pixelSize: fontNormalSize
            }
            ToolButton {
                text: "tree"
                onClicked:  mainLoader.source = "aimTree.qml"
                font.pixelSize: fontNormalSize
            }
            ToolButton {
                text: "tags"
                onClicked:  mainLoader.source = "tagTree.qml"
                font.pixelSize: fontNormalSize
            }
            ToolButton {
                text: "run"
                onClicked:  mainLoader.source = "runningAim.qml"
                font.pixelSize: fontNormalSize
            }
            //ToolSeparator {} (on small phone gets out)
            ToolButton {
                text: "Menu"
                onClicked: toolMenu.open()
                font.pixelSize: fontNormalSize
            }
            ToolSeparator {}
            ToolButton {
                text: "add"
                onClicked:  drawerRight.open()
                font.pixelSize: fontNormalSize
            }
            //http://doc.qt.io/qt-5/qml-qtquick-controls2-toolbar.html
        }
    }

   Menu {
       id: toolMenu
       x: window.width - width - 10
       width: screenGlobal.adaptXSize(150)

       MenuItem {
           text: "Schedule"
           font.pixelSize: fontNormalSize
           onTriggered:  mainLoader.source = "schedule.qml"
       }
       MenuItem {
           text: "Done acts"
           font.pixelSize: fontNormalSize
           onTriggered:  mainLoader.source = "actionsDone.qml"
       }
       MenuItem {
           text: "Notify"
           font.pixelSize: fontNormalSize
           onTriggered: androidNotify.setNotification("Hello!")
       }
       MenuItem {
           text: "Chat"
           font.pixelSize: fontNormalSize
           onTriggered: drawerDown.open()
       }
       MenuItem{
           text: "Import"
           font.pixelSize: fontNormalSize
           onTriggered: importFileDialog.open()
       }
   }

   FileDialog {
       id: importFileDialog
       title: "Select aim export file name"
       //folder:
       nameFilters: [ "Aim files (*.aim)", "All files (*)" ]
       selectMultiple: false
       selectExisting: true
       onAccepted: {
           localBase.importAim(importFileDialog.fileUrl)
           mainLoader.setSource("aimView.qml")
           mainLoader.item.loadModel()
       }
   }


    Loader {
        y: toolBar.height + microYOffset
        anchors.centerIn: parent
        id: mainLoader
        source: "aimView.qml" //hello by default
    }

    Connections
    {
        target: mainLoader.item

        onRequestOpenAddAim: {
            addLoader.item.loadForAddNew()
            drawerRight.open()
        }
        onRequestOpenEditAim: {
            addLoader.item.loadAimForEdit(aimId)
            drawerRight.open()
        }
        onRequestOpenSingleAim:{
            mainLoader.setSource("singleAim.qml",{aimId:aimId})
        }
    }


    MessageDialog {
        id: confirmExitDialog
        standardButtons: StandardButton.Ok | StandardButton.Cancel
        width: 400
        title: "Ожидание подтверждения"
        text: "Вы уверенны что хотите выйти из приложения?\nНовые уведомления не смогут быть полученны пока приложение не запущенно."

        onAccepted: {
           Qt.quit();
        }
        onRejected: {
            systemTray.showNotification(userCheck.getDomainName(),userCheck.getUserName(),5);
        }
    }

    Connections {
         target: systemTray

         onSignalShow: {
             window.hide();
             window.show();
         }
         //HIDE
         onSignalAdd: {
             window.hide();
             window.show();
            drawerRight.open()
         }
         //AND notifications test may lay here so
         onSignalQuit: {

             if (userSettings.isDebugBuild() === false)
                confirmExitDialog.open()
             else
                Qt.quit()
         }
         //AND just one more test button for anything we need to test...
         onSignalIconActivated: {
              if(window.visibility === Window.Hidden) {
                  window.show()
              }
         }
     } //Connections

    Connections {
         target: popUpItem

         onRequestNotificationsViewFromCpp:{
             window.show();
             mainLoader.source = "runningAim.qml"
         }
     }

    Drawer {
           id: drawerLeft
           width: 0.66 * window.width
           height: window.height

           ColumnLayout{
               Button{
                   text: "Check button"
               }
               TextEdit{
                   text: "Sometext"
               }
           }
       }


    Drawer {
           id: drawerRight
           width: 0.66 * window.width
           height: window.height

            edge: Qt.RightEdge

           Loader {
               y: 0
               anchors.centerIn: parent
               anchors.fill:  parent
               id: addLoader
               source: "aimAdd.qml"
           }

           Connections{
               target: addLoader.item
               onRequestOpenViewAims: {
                   drawerRight.close()
                   console.log("Reloading aims..")
                   mainLoader.setSource("aimView.qml")
                   mainLoader.item.loadModel()
               }
           }
       }

    Drawer {
           id: drawerDown
           width: window.width
           height: 0.5*window.height

           edge: Qt.BottomEdge

           Loader {
               y: 0
               anchors.centerIn: parent
               anchors.fill:  parent
               id: chatLoader
               source: "chat.qml"
           }
       }
}
