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

    property int treeViewHeight : 300
    property int treeViewYOffset : 50
    property int columntWidth : 300

    property int aimListYOffset : 400
    property int aimListHeight : 300



    signal requestOpenAddAim()
    signal requestOpenEditAim(var aimId)
    signal requestOpenSingleAim(var aimId)

    Component.onCompleted:  {
        localBase.fillTreeModelWithTags()
    }


    QMLOld.TreeView
    {
        id: treeView
        model: tagTree

        width: parent.width
        height: treeViewHeight

        x: 0
        y: treeViewYOffset

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
            width: columntWidth
        }


        onClicked:
        {
            //mainWindow.reloadPlaceList(firmTree.getFieldByIndex(index,1))
           // console.log("Argument clicked 1 : " + tagTree.getFieldByIndex(index,1))
            console.log("Argument clicked 0 : " + tagTree.getFieldByIndex(index,0))


            aimList.searchModelTag(tagTree.getFieldByIndex(index,0)) //first argument must be tag name
        }
    }


    //ViewSection
    AimList
    {
        y: aimListYOffset
        height: aimListHeight
        width: parent.width

        id:aimList
    }

}
