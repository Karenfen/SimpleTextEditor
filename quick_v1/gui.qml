import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12


Rectangle {
    id: widget
    height: 200
    width: 640
    color: "#ffecd2"
    border.width: 5
    border.color: "#c2e9fb"
    visible: true


    Rectangle{
        id: taskPlain
        width: widget.width - parent.border.width*2
        height: widget.height * 0.66
        x: widget.border.width
        y: widget.border.width
        visible: true
        clip: true
        gradient: Gradient{
            GradientStop { position: 0.0; color: "#fbc2eb" }
            GradientStop { position: 1.0; color: "#a6c1ee" }
        }


        ScrollView{
            id: scroll
            anchors.fill: parent
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            clip: true


            TextEdit {
                id: taskText
                objectName: "textPlane"
                x: taskPlain.x + widget.border.width
                y: taskPlain.y + widget.border.width
                width: taskPlain.width - widget.border.width*2
                height: taskPlain.height - widget.border.width*2
                font.pixelSize: 18
                wrapMode: TextEdit.WordWrap
                verticalAlignment: TextEdit.AlignTop
                ToolTip.visible: !focus
                ToolTip.text: qsTr("Введите новую задачу")

            }

        }

    }

    Label{
         id: lableDate
         x: widget.border.width
         anchors.top: taskPlain.bottom
         text: qsTr("Плановая дата:")

    }

    Rectangle{
        id:dateRec
        x: widget.border.width
        anchors.top: lableDate.bottom
        height: progress.height
        width: 150
        visible: true
        gradient: Gradient{
            GradientStop { position: 0.0; color: "#fad0c4" }
            GradientStop { position: 1.1; color: "#ffd1ff" }
        }

        TextInput{
            id: date
            anchors.fill: parent
            visible: true
            font.pixelSize: height/2
            verticalAlignment: TextInput.AlignVCenter
            horizontalAlignment: TextInput.AlignHCenter
            inputMethodHints: Qt.ImhDate
            inputMask: "99.99.9999"
            text: new Date().toLocaleDateString("dd.MM.yyyy")

        }
    }

    Label{
         id: lableProgress
         anchors.top: taskPlain.bottom
         anchors.left: progress.left
         text: qsTr("Прогресс:")

    }

    SpinBox{
        id: progress
        anchors.top: lableProgress.bottom
        anchors.right: taskPlain.right
        from: 0
        to: 10
        font.pointSize: height/2
        font.bold: true
    }

      MyButton{
          signal createdTask(string task, string date, string status)

          id: addTask
          anchors.top: taskPlain.bottom
          anchors.bottom: dateRec.bottom
          anchors.left: dateRec.right
          anchors.right: progress.left

          onClicked: {
              if(taskText.text.length){
                  createdTask(taskText.text, date.text, progress.value.toString())
                  taskText.clear()
                  date.text = new Date().toLocaleDateString("dd.MM.yyyy")
                  progress.value = 0
              }
          }

      }
}
