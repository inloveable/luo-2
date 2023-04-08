import QtQuick 2.15
import QtQuick.Controls 6.3

Rectangle {
    id:addPatientPage

    width:1000
    height:600

    signal returnSignal();

    color:"antiquewhite"
    radius:10
    Text {
        id: text1
        width: 150
        height: 50

        font.pixelSize: 20
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.bold: true
        anchors.leftMargin: 20
        anchors.topMargin: 20
        text:"添加病人"
        anchors.left: parent.left
        anchors.top: parent.top
    }

    Image {
        id: image
        width: 246
        height: 93
        anchors.left: text1.right
        anchors.top: parent.top
        source: "qrc:/resources/images/flowLogo.png"
        anchors.topMargin: 42
        anchors.leftMargin: 144
        fillMode: Image.PreserveAspectFit
    }



    SwitchDelegate {
        id: control1
        y: 348
        width: 169
        height: 28
        text: qsTr("有无HPV感染史")
        anchors.left: control2.right
        anchors.leftMargin: 76
        checked: false

        contentItem: Text {
            id:contentText
            anchors.left:parent.left
            text: control1.text
            font: control1.font
            opacity: enabled ? 1.0 : 0.3
            color: control1.checked ? "red" : "green"
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }

        indicator: Rectangle {
            implicitWidth: 48
            implicitHeight: 26
            anchors.right:parent.right
            anchors.top:parent.top
            radius: 13
            color: control1.checked ? "red" : "transparent"
            border.color: control1.checked ? "#17a81a" : "#cccccc"

            Rectangle {
                x: control1.checked ? parent.width - width : 0
                width: 26
                height: 26
                radius: 13
                color: control1.down ? "#cccccc" : "#ffffff"
                border.color: control1.checked ? (control1.down ? "#17a81a" : "#21be2b") : "#999999"
            }
        }

        background: Rectangle {
            implicitWidth: 100
            implicitHeight: 40
            visible: control1.down || control1.highlighted
            color: control1.down ? "#bdbebf" : "#eeeeee"
        }
    }

    Label {
        id: label
        x: 128
        y: 157
        width: 70
        height: 33
        color: "#020000"
        text:"姓名:"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 16
        font.bold: true


    }

    SwitchDelegate {
        id: control2
        x: 112
        y: 348
        width: 264
        height: 28
        text: qsTr("有无宫颈癌或其他癌症感染史")
        checked: false
        background: Rectangle {
            visible: control2.down || control2.highlighted
            color: control2.down ? "#bdbebf" : "#eeeeee"
            implicitHeight: 40
            implicitWidth: 100
        }
        contentItem: Text {
            opacity: enabled ? 1.0 : 0.3
            color: control2.checked ? "red" : "green"
            text: control2.text
            elide: Text.ElideRight
            anchors.left: parent.left
            verticalAlignment: Text.AlignVCenter
            font: control2.font
        }
        indicator: Rectangle {
            color: control2.checked ? "red" : "transparent"
            radius: 13
            border.color: control2.checked ? "#17a81a" : "#cccccc"
            anchors.right: parent.right
            anchors.top: parent.top
            implicitHeight: 26
            Rectangle {
                x: control2.checked ? parent.width - width : 0
                width: 26
                height: 26
                color: control2.down ? "#cccccc" : "#ffffff"
                radius: 13
                border.color: control2.checked ? (control2.down ? "#17a81a" : "#21be2b") : "#999999"
            }
            implicitWidth: 48
        }
    }

    Label {
        id: label1
        x: 128
        y: 208
        width: 70
        height: 33
        color: "#020000"
        text: "年龄:"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 16
        font.bold: true
    }

    Rectangle {
        id: textInput1
        x: 204
        y: 208
        width: 164
        height: 33
        border.color: "#808080"
        border.width: 1
        anchors.left: label.right
        TextInput {
            anchors.fill: parent
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.bold: true
        }
        anchors.leftMargin: 0
    }

    Label {
        id: label2
        x: 84
        y: 272
        width: 114
        height: 33
        color: "#020000"
        text:"联系方式:"
        anchors.right: label.right
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        font.pointSize: 16
        font.bold: true
    }

    Rectangle {
        id: textInput2
        x: 204
        y: 272
        width: 164
        height: 33
        border.color: "#808080"
        border.width: 1
        anchors.left: label.right
        TextInput {
            anchors.fill: parent
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.bold: true
        }
        anchors.leftMargin: 0
    }

    Label {
        id: label3
        x: 433
        y: 150
        width: 94
        height: 33
        color: "#020000"
        text:"身份证号:"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 16
        font.bold: true
    }

    Rectangle {
        id: textInput3
        x: 534
        y: 157
        width: 164
        height: 33
        border.color: "#808080"
        border.width: 1
        anchors.left: label.right
        TextInput {
            anchors.fill: parent
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.bold: true
        }
        anchors.leftMargin: 0
    }

    Label {
        id: label4
        text:"性别:"
        x: 418
        y: 208
        width: 94
        height: 33
        color: "#020000"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 16
        font.bold: true
    }
    
    ComboBox {
        id: comboBox
        y: 209
        width: 121
        height: 41
        anchors.left: label4.right
        font.bold: true
        font.pointSize: 18
        anchors.leftMargin: 22
        model:["男","女"]
    }

    Rectangle {
        id: textInput4
        x: 538
        y: 150
        width: 164
        height: 33
        border.color: "#808080"
        border.width: 1
        anchors.left: label3.right
        TextInput {
            anchors.fill: parent
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.bold: true
        }
        anchors.leftMargin: 7


    }



    FancyButton {
        id: fancyButton

        text:"确定"

        anchors.left: label3.left

        anchors.top: control1.bottom
        anchors.topMargin: 20
        color:"#1268D7"
        hoverColor:"#1268D766"
        font.pixelSize: 16
        width:100
        height:25
    }

    FancyButton {
        id: returnButton
        x: 502
        y: 470

        width:100
        height:25
        text:"取消"

        font.pixelSize: 16

        anchors.left: fancyButton.right
        anchors.leftMargin: 25

        anchors.top:fancyButton.top
        anchors.topMargin: 0

        color:"#1268D7"
        hoverColor:"#1268D766"

        onClicked: {
            returnSignal()
        }
    }


}

/*##^##
Designer {
    D{i:0}D{i:1;locked:true}
}
##^##*/
