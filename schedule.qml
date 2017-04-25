import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import QtQuick.Controls.Styles.Flat 1.0 as Flat


Item {
    Calendar
    {
        y: 0
        x: 100 //SHOULD BE REPLACE WITH SOME SCREEN VALUE

        width: parent.width - x*2
        height: parent.width - x*2 //special logic for always vertical Application

        id: calendar
    }

}
