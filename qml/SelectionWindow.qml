import QtQuick 2.15
import QtQuick.Controls 2.0
import QtQuick.Shapes 1.14
import myCpp 1.0
Rectangle
{
     id:selectionwindow;
     color: "antiquewhite"

     property string currentDoctorName

     Rectangle{

         width:200
         height:25
         id:doctorWelcomeText
         color:"transparent"
         anchors.right:returnButton.left
         anchors.top: parent.top
         anchors.topMargin: 20
         anchors.rightMargin: 20
         Text{

             anchors.fill: parent
             font.bold: true
             font.pixelSize: 16
             color:"black"
             text:"你好! "+currentDoctorName+" 医生";
         }
     }

     FancyButton{
         id:returnButton
         anchors.top:doctorWelcomeText.top

         anchors.right: parent.right
         anchors.rightMargin: 10
         text:"返回"
         font.pixelSize: 12
         width:100
         height:25
         color:"#1268D7"
         hoverColor:"#1268D766"
         onClicked: {

             loginWindow.setCurrentIndex(1);
         }

     }

     Text{
         id:infoText
         anchors.left: parent.left
         anchors.top: parent.top
         anchors.topMargin: 20
         anchors.leftMargin: 20

         text:"患者信息"
         font.pixelSize: 20
         font.bold: true

         color:"black"

         width:200
         height:50
     }
     PatientInfo
     {
         id:patientInfo


     }

     property int buttonsButtomMargin: 30
     Button {
         id: button

         anchors.left: parent.left
         anchors.leftMargin: 20
         anchors.bottom:parent.bottom
         anchors.bottomMargin: buttonsButtomMargin


         width: 150
         height: 40
         text: qsTr("查看日志")
         font.bold: true
         font.pixelSize: 16
         contentItem: Text {
             text: button.text
             font: button.font

             opacity: enabled ? 1.0 : 0.5
             color: "black"
             horizontalAlignment: Text.AlignHCenter
             verticalAlignment: Text.AlignVCenter
             elide: Text.ElideRight
         }

         background: Rectangle {
             implicitWidth: 150
             implicitHeight: 70
             opacity: enabled ? 1 : 0.3
             border.color: "green"
             border.width: 1
             radius:4

             gradient: Gradient {
                    GradientStop { position: 0.112; color: "#3df5a7" }
                    GradientStop { position: 0.911; color: "#096fe0" }
                    orientation: Gradient.Horizontal
                }
         }
     }

     Button {
         id: button1

         anchors.left: button.right
         anchors.leftMargin: 20
         anchors.top: button.top
         width: 150
         height: 40
         text: qsTr("TCT分析")
         font.bold: true
         font.pixelSize: 16
         contentItem: Text {
             text: button1.text
             font: button1.font

             opacity: enabled ? 1.0 : 0.5
             color: "black"
             horizontalAlignment: Text.AlignHCenter
             verticalAlignment: Text.AlignVCenter
             elide: Text.ElideRight
         }

         background: Rectangle {
             implicitWidth: 150
             implicitHeight: 70
             opacity: enabled ? 1 : 0.5
             border.color: "green"
             border.width: 1
             radius: 4

             gradient: Gradient {
                    GradientStop { position: 0.112; color: "#3df5a7" }
                    GradientStop { position: 0.911; color: "#096fe0" }
                    orientation: Gradient.Horizontal
                }
         }

         onClicked: {
             loginWindow.hide();
             MainWindow.intoTCTAnalize();
         }

     }


     Button {
         id: button2

         anchors.right: button3.left
         anchors.rightMargin: 20
         anchors.bottom:parent.bottom
         anchors.bottomMargin: buttonsButtomMargin


         width: 150
         height: 40
         text: qsTr("添加病人")
         font.bold: true
         font.pixelSize: 16
         contentItem: Text {
             text: button2.text
             font: button2.font

             opacity: enabled ? 1.0 : 0.5
             color: "black"
             horizontalAlignment: Text.AlignHCenter
             verticalAlignment: Text.AlignVCenter
             elide: Text.ElideRight
         }

         background: Rectangle {
             implicitWidth: 150
             implicitHeight: 70
             opacity: enabled ? 1 : 0.3
             border.color: "green"
             border.width: 1
             radius:4

             gradient: Gradient {
                    GradientStop { position: 0.112; color: "#3df5a7" }
                    GradientStop { position: 0.911; color: "#096fe0" }
                    orientation: Gradient.Horizontal
                }
         }

         onClicked: {
             if(patientInfo.pageIndex===0)
             {
                 patientInfo.switchToAddPatient();
             }


         }

     }

     Button {
         id: button3

         anchors.right: parent.right
         anchors.rightMargin: 20
         anchors.top: button.top
         width: 150
         height: 40
         text: qsTr("关于")
         font.bold: true
         font.pixelSize: 16
         contentItem: Text {
             text: button3.text
             font: button3.font

             opacity: enabled ? 1.0 : 0.5
             color: "black"
             horizontalAlignment: Text.AlignHCenter
             verticalAlignment: Text.AlignVCenter
             elide: Text.ElideRight
         }

         background: Rectangle {
             implicitWidth: 150
             implicitHeight: 70
             opacity: enabled ? 1 : 0.5
             border.color: "green"
             border.width: 1
             radius: 4

             gradient: Gradient {
                    GradientStop { position: 0.112; color: "#3df5a7" }
                    GradientStop { position: 0.911; color: "#096fe0" }
                    orientation: Gradient.Horizontal
                }
         }

     }
}
