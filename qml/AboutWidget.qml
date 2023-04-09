import QtQuick 2.15
import QtQuick.Controls 2.0


Window {

    id: window
     width:300
     height:500
     color: "#0d0d0d"


     title:"关于"
     Rectangle
     {
         id: rectangle
         anchors.fill: parent
         color:"antiquewhite"
         Image {
             id: image
             height: 124
             anchors.left: parent.left
             anchors.right: parent.right
             anchors.top: parent.top
             source: "qrc:/resources/images/flowLogo.png"
             anchors.rightMargin: 4
             anchors.leftMargin: 5
             anchors.topMargin: 20
             fillMode: Image.PreserveAspectFit
         }

         FancyButton {
             id: button
             x: 92
             y: 175
             width: 117
             height: 27
             text: qsTr("关于我们")
             color:"#1268D7"
             hoverColor:"#1268D766"
             font.pixelSize: 16
         }

         FancyButton {
             id: button1
             height: 31
             text: qsTr("退出")
             anchors.left: parent.left
             anchors.right: parent.right
             anchors.top: image.bottom
             anchors.rightMargin: 91
             anchors.leftMargin: 92
             anchors.topMargin: 77
             color:"#1268D7"
             hoverColor:"#1268D766"
             font.pixelSize: 16

             onClicked: {
                 Qt.quit()
             }
         }

         Label {
             id: label
             height: 26
             color: "#0c0c0c"
             text: qsTr("CopyRight 2023")
             anchors.left: parent.left
             anchors.right: parent.right
             anchors.top: parent.top
             font.bold: true
             font.pixelSize: 16
             horizontalAlignment: Text.AlignHCenter
             verticalAlignment: Text.AlignVCenter
             anchors.rightMargin: 91
             anchors.leftMargin: 92
             anchors.topMargin: 288
         }

         Label {
             id: label1
             height: 26
             color: "black"
             text: qsTr("福歌科技(天津)有限公司")
             anchors.left: parent.left
             anchors.right: parent.right
             anchors.bottom: label2.top
             horizontalAlignment: Text.AlignHCenter
             verticalAlignment: Text.AlignVCenter
             font.bold: true
             font.pixelSize: 16
             anchors.rightMargin: 61
             anchors.leftMargin: 61           
         }

         Label {
             id: label2
             height: 26
             color: "#090808"
             text: qsTr("版权所有")
             anchors.left: parent.left
             anchors.right: parent.right
             anchors.top: parent.top
             horizontalAlignment: Text.AlignHCenter
             verticalAlignment: Text.AlignVCenter
             font.bold: true
             font.pixelSize: 16
             anchors.rightMargin: 99
             anchors.leftMargin: 92
             anchors.topMargin: 358
         }

         Label {
             id: label3
             x: 69
             y: 390
             width: 160
             height: 26
             font.bold: true
             font.pixelSize: 16
             color: "#000000"
             text: qsTr("版本信息:v2.0")
             horizontalAlignment: Text.AlignHCenter
             verticalAlignment: Text.AlignVCenter

         }

     }





}
