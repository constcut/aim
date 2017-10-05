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

    Button{

        y: 550
        x: parent.width - 10 - width

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
        y: 600
        x: 10

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

        width: 480
        height: 500

        x: 0
        y: 50

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
            width: 150
        }
        QMLOld.TableViewColumn {
            role: treeModel.getColumnName(1)
            title: "Время"
            width: 70
        }
        QMLOld.TableViewColumn {
            role: treeModel.getColumnName(2)
            title: "Дата" //will autocalculate
            width: 70
        }
        QMLOld.TableViewColumn {
            role: treeModel.getColumnName(3)
            title: "Комментарий" //will autocalculate
            width: 250
        }

        onClicked:
        {
            //mainWindow.reloadPlaceList(firmTree.getFieldByIndex(index,1))
            console.log("Place code for reload : " + treeModel.getFieldByIndex(index,1))
        }
    }
}
