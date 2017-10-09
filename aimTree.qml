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

    Component.onCompleted:  {
        localBase.fillTreeModelWithAims()
    }

    property int microOffset : 10
    property int saveButtonOffset : 550 //y

    property int treeOldButtonsOffset : 600

    property int treeViewHeight : 500
    property int yOffset : 50

    property int column1Width : 150
    property int column2Width : 70
    property int column3Width : 70
    property int column4Width : 250

    Button{

        y: saveButtonOffset
        x: parent.width - microOffset - width

        text: "Cохранить"
        onClicked:
        {
            localBase.editTreeAims(treeModel)
        }

        onDoubleClicked:
        {
            if (text == "Сохранить")
            {
                text = "Сохранять";
                highlighted = true
            }
            else
            {
                text = "Сохранить";
                highlighted = false
            }


        }

        highlighted: false
    }

    RowLayout
    {
        y: treeOldButtonsOffset
        x: microOffset

    Button{
        text: "Добавить узел"
        onClicked: treeModel.addTopItem(aimsTree.currentIndex)
        highlighted: true
    }
    Button{
        text: "Добавить под-узел"
        onClicked:  treeModel.addChildItem(aimsTree.currentIndex)
        highlighted: true
    }
    Button{
        text: "Удалить узел"
        onClicked: treeModel.removeItem(aimsTree.currentIndex)
        highlighted: true
    }
    }

    QMLOld.TreeView
    {
        id: aimsTree
        model: treeModel //hierarchy

        width: parent.width
        height: treeViewHeight

        x: 0
        y: yOffset

        //y: 30
        //x: 5
        //width: parent.width/2 - 10
        //height: parent.height/2 - 20 - 20
/*
        style: TreeViewStyle
        {
            activateItemOnSingleClick: true

            alternateBackgroundColor: "darkgray"
            backgroundColor: "gray"

            textColor: "white"

            control: aimsTree

            headerDelegate: Item {
                Rectangle {
                    color: styleData.containsMouse ? "darkgray" : (styleData.pressed ? "black" : "gray")
                    Text {
                        color:  styleData.pressed ? "white" : "black"
                        text: styleData.value
                    }
                }
            }
        }

*/

      //  anchors.fill: parent

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


                    treeModel.setItemText(aimsTree.currentIndex,editingField.text, treeModel.getColumnIndex(styleData.role))
                }
            }
        }

        QMLOld.TableViewColumn {
            role: treeModel.getColumnName(0)
            title: "Имя цели"
            width: column1Width
        }
        QMLOld.TableViewColumn {
            role: treeModel.getColumnName(1)
            title: "Время"
            width: column2Width
        }
        QMLOld.TableViewColumn {
            role: treeModel.getColumnName(2)
            title: "Дата" //will autocalculate
            width: column3Width
        }
        QMLOld.TableViewColumn {
            role: treeModel.getColumnName(3)
            title: "Комментарий" //will autocalculate
            width: column4Width
        }

        onClicked:
        {
            //mainWindow.reloadPlaceList(firmTree.getFieldByIndex(index,1))
            console.log("Place code for reload : " + treeModel.getFieldByIndex(index,1))
        }
    }
}
