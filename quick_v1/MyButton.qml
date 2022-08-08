import QtQuick 2.12
import QtQuick.Controls 2.12

Button{

    id: root
    objectName: "addButton"
    text: qsTr("Добавить")
    font.pixelSize: height/2

    contentItem: Text {
        text: root.text
        font: root.font
        color: root.down ? "white" : "black"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        width: parent.width
        height: parent.height
        gradient: Gradient{
            GradientStop { position: 0.0; color: "#84fab0" }
            GradientStop { position: 1.1; color: "#8fd3f4" }
        }
    }
}
