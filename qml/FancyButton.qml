import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id:root
    width: 80
    height: 30
    color: "#ffffff"
    clip: true
    property string text;
    property alias defaultColor:root.color
    property alias hoverColor:fxSquare.color




    radius:root.width>root.height?height/6:width/6


    property alias font: realText.font
    signal clicked;
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onClicked: root.clicked();
        onContainsMouseChanged: cc()
        function cc() {
            fxSquare.centerX = mouseX
            fxSquare.centerY = mouseY
            if(fxSquare.state === "") { fxSquare.state = "hovered"}else{ fxSquare.state = ""}

        }


        Rectangle {
            id: fxSquare
            property real ra: 0
            height:2* ra
            width: 2*ra
            x:centerX-ra
            y:centerY-ra
            property real centerX: 0
            property real centerY: 0
            radius: Math.max(parent.width, parent.height)
            color: "red"

            states: [
                State {
                    name: "hovered"
                    PropertyChanges {
                        target: fxSquare
                        ra: root.height + root.width
                    }
                },
                State {
                    name: ""
                    PropertyChanges {
                        target: fxSquare
                         ra:0

                    }
                }
            ]
            transitions: [
                Transition {
                    from: "*"
                    to: "*"
                    ParallelAnimation {
                        NumberAnimation {
                            properties: "ra"
                            duration: 200
                        }
                        NumberAnimation {
                            properties: "x"
                            duration: 200
                        }
                        NumberAnimation {
                            properties: "y"
                            duration: 200
                        }
                    }
                }
            ]

        }
        Text{
            id:realText
            anchors.centerIn:parent
            text:root.text

            color:"white"
            font.bold: true
            font.pixelSize: 25

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment:Text.AlignVCenter
        }
    }
}
