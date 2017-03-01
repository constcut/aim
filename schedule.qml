import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import QtQuick.Controls.Styles.Flat 1.0 as Flat


Item {

    RowLayout
    {
        y:50
        RadioButton
        {
            text: "calendar"
        }
        RadioButton
        {
            text: "upcoming"
        }
        RadioButton
        {
            text: "day"
        }
    }

    Calendar
    {
        y: 150

        width: 200
        height: 200

        id: calendar
    }
}
