import QtQuick 2.0

Rectangle {
    id: root
    color: "cornflowerblue"
    radius: 10

    property string displayText: "X"

    border.color: "black"
    border.width: 1

    Text{
        text: root.displayText
        font.pointSize: Math.min(root.width, root.height) / 2
        font.bold: true
        anchors.centerIn: root
    }
}
