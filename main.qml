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
        //add the first one view : aimView
        prevPages.pageChanged("aimView.qml")
    }

    onClosing: {
        if (userSettings.isDebugBuild())
                Qt.quit()
            else
                window.hide();
    }


   ListModel{
       id: prevPages

       function pageChanged(qmlFile, params){ //maybe we can do param name + param value?

                if (params !== undefined){
                    prevPages.append({"pageFile":qmlFile,"params":params})
                    mainLoader.setSource(qmlFile,params)
                }
                else{ //maybe we don't even need this else statement
                    prevPages.append({"pageFile":qmlFile})
                    mainLoader.setSource(qmlFile)
                }

                console.log("On page " + qmlFile  +" changed f() params " + params)
       }
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
                text: "<"
                font.pixelSize: fontNormalSize
                onClicked: {
                    if (prevPages.count > 1){
                         var qmlFile = prevPages.get(prevPages.count-2).pageFile
                         var params = prevPages.get(prevPages.count-2).params
                         console.log("Moveing to prev page " + qmlFile + " with params " + params)
                         if (params !== undefined)
                            mainLoader.setSource(qmlFile,params)
                         else
                            mainLoader.setSource(qmlFile)
                         prevPages.remove(prevPages.count-1)
                    }
                    else
                        console.log("Prev pages are empty " + prevPages.count)
                }
            }
            ToolButton {
                text: "view"
                onClicked: prevPages.pageChanged("aimView.qml")
                font.pixelSize: fontNormalSize
            }
            ToolButton {
                text: "tree"
                onClicked:  prevPages.pageChanged("aimTree.qml")
                font.pixelSize: fontNormalSize
            }
            ToolButton {
                text: "tags"
                onClicked:  prevPages.pageChanged("tagTree.qml")
                font.pixelSize: fontNormalSize
            }
            ToolButton {
                text: "run"
                onClicked:  prevPages.pageChanged("runningAim.qml")
                font.pixelSize: fontNormalSize
            }
            //ToolSeparator {} (on small phone gets out)
            ToolButton {
                text: "Menu"
                onClicked: toolMenu.open()
                font.pixelSize: fontNormalSize
            }
            //ToolSeparator {} //sepparators yet removed but will come back on images buttons
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
           onTriggered:  prevPages.pageChanged("schedule.qml")
       }
       MenuItem {
           text: "Done acts"
           font.pixelSize: fontNormalSize
           onTriggered:  prevPages.pageChanged("actionsDone.qml")
       }
       MenuItem {
           text: "Done chart"
           font.pixelSize: fontNormalSize
           onTriggered: prevPages.pageChanged("doneActsChart.qml")
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
       MenuItem {
           text: "Console"
           font.pixelSize: fontNormalSize
           onTriggered: prevPages.pageChanged("ConsoleLog.qml")
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

           var check = localBase.checkThereIsSameImportAim(importFileDialog.fileUrl)
           if (check !== 0)
           {
               console.log("Simmilar aim already exists: " + check)
               //IF PROGRESS BIGGER - CAN UPDATE
               //ELSE - ASK SHOULD ADD NEW
           }
           else{
               localBase.importAim(importFileDialog.fileUrl)
               mainLoader.setSource("aimView.qml")
               mainLoader.item.loadModel()
           }
       }
   }


    Loader {
        y: toolBar.height + microYOffset
        anchors.centerIn: parent
        id: mainLoader

        //we set it by pageChanged on Component.onCompleted
        //source: "aimView.qml" //hello by default
    }

    Connections{
        target: mainLoader.item

        /*
        onRequestUpdateParams: {
            if (prevPages.count > 0){
                prevPages.get(prevPages.count-1).params = params
                console.log("Last page params updated to " + params)
            }
            else
                console.log("Prev pages are empty")
        }
        */ //YET we don't use this feature, but later it should work very fine
        //on schedule and done acts we should store data if it was chosen
        //and load it if there was a special param set
        //
        //and on aimView we must store current list index and load in if it was set
        //
        //loading of special data should be applied in the Component.onCompleted()

        onRequestOpenAddAim: {
            addLoader.item.loadForAddNew()
            drawerRight.open()
        }
        onRequestOpenEditAim: {
            addLoader.item.loadAimForEdit(aimId)
            drawerRight.open()
        }
        onRequestOpenSingleAim:{
            //mainLoader.setSource("singleAim.qml",{"aimId":aimId})
            //updating:
            prevPages.pageChanged("singleAim.qml",{"aimId":aimId})
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
             window.show()
             //mainLoader.source = "runningAim.qml"
             //updating:
             prevPages.pageChanged("runningAim.qml") //maybe also nice to find it
             //or just open single aim
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
                   //console.log("Reloading aims..")
                   //mainLoader.setSource("aimView.qml")

                   //updating:
                   prevPages.pageChanged("aimView.qml")
                   mainLoader.item.loadModel() //refresh it
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
