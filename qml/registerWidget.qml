import QtQuick 2.15
import myCpp 1.0
Rectangle {

    id:registerWidget
    color:"antiquewhite"

    signal registerSucess();
    FancyButton{
        id:returnButton
        anchors.top:parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        text:"返回"
        font.pixelSize: 12
        width:100
        height:20
        color:"#1268D7"
        hoverColor:"#1268D766"

        onClicked: {

            loginWindow.setCurrentIndex(1);
        }

    }

    Rectangle {
        id: registerArea

        width: 600
        height: 400
        color: "transparent"
        radius: 4

        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 60
        anchors.rightMargin: 100
        activeFocusOnTab: true
        focus: true


        Text {
            id: text2
            width: 192
            height: 42
            text:"账号注册"
            anchors.top: rectangle3.top
            font.pixelSize: 24
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.styleName: "Normal"
            anchors.topMargin: -70
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Rectangle {
            id: rectangle3

            width: 330
            height: 40
            color: "#ffffff"

            anchors.bottom: rectangle4.top

            anchors.bottomMargin: 25

            anchors.horizontalCenter: parent.horizontalCenter

            border.width: 4
            border.color: "green"
            radius:4
            TextInput{
                id:userInfoInput1
                height: parent.height-4

                width:parent.width-parent.width/10*2
                anchors.left: leftUserIcon1.right
                anchors.top:parent.top
                anchors.topMargin: 4
                verticalAlignment: Text.AlignVCenter
            }

            Rectangle{
                id:leftUserIcon1
                width:parent.width/10;
                height: parent.height
                anchors.bottom:parent.bottom
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin:4
                anchors.bottomMargin:6
                anchors.topMargin:4
                Image{
                    anchors.fill: parent
                    source:"qrc:/resources/images/userIcon.png"
                }

            }
        }

        Rectangle {
//定位的是这个rectangle
            radius:4
            id: rectangle4//passwordInput
            y:registerWidget.height/2-50
            width: 330
            height: rectangle3.height
            color: "#ffffff"
            anchors.horizontalCenter: parent.horizontalCenter

            border.width: 4
            border.color: "green"
            Rectangle{
                id:leftIcon1
                width:parent.width/10;
                height: parent.height


                anchors.bottom:parent.bottom
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin:4
                anchors.bottomMargin:6
                anchors.topMargin:4


                Image{
                    anchors.fill: parent
                    source:"qrc:/resources/images/PassWordIcon。.png"
                }

            }
            Rectangle{
                id:rightIcon1
                width:parent.width/10;
                height: parent.height
                anchors.bottom:parent.bottom
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.rightMargin:4
                anchors.bottomMargin:4
                anchors.topMargin:4

                MouseArea{
                    id:iconArea1
                    anchors.fill: parent
                    property bool clicked:false;

                    onClicked: {
                        console.log("clicked");
                        clicked=!clicked
                    }
                }

                Image{
                    anchors.fill: parent
                    source:"qrc:/resources/images/VisibilityIcon.png"

                }
            }

            TextInput{
                id:passwordInput1
                height: parent.height-4
                echoMode: !iconArea1.clicked?TextInput.Password:TextInput.Normal
                width:parent.width-parent.width/10*2
                anchors.left: leftIcon1.right
                anchors.top:parent.top
                anchors.topMargin: 4
                verticalAlignment: Text.AlignVCenter
                function setEchoMode(boolen)
                {

                }
            }


        }

        Rectangle{

            radius:4
            id:docIdInput
            anchors.top:rectangle4.bottom
            anchors.topMargin: 25
            anchors.left: rectangle4.left
            width:rectangle4.width
            height:rectangle3.height
            border.width: 4
            border.color: "green"

            Text{
                id:label
                text:"医生姓名"
                anchors.left:parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: parent.width/10*2+5
                height: parent.height
                font.pixelSize: 15
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment:Text.AlignHCenter
            }

            TextInput{
                id:doctorNameInput
                anchors.top:parent.top
                anchors.left: label.right
                verticalAlignment: Text.AlignVCenter

                height:label.height
                width:parent.width-label.width

                font.pixelSize: 15


            }
        }


        Text{
            id:errorMessage1
            text:"账号或密码为空"
            color:"red"
            anchors.top:docIdInput.bottom
            anchors.left: docIdInput.left



        }

        FancyButton{
            id:registerButton1
            anchors.top:errorMessage1.bottom
            anchors.topMargin: 5
            anchors.horizontalCenter:rectangle4.horizontalCenter
            width:150
            height:40
            color:"#1268D7"
            hoverColor:"#1268D766"
            text:"确定"

            onClicked: {
               var result=MainWindow.tryRegister(userInfoInput1.text,passwordInput1.text,doctorNameInput.text)
               if(result<0)
               {
                   console.log("注册失败");
               }else
               {
                   registerSucess()
               }
            }

        }

    }
    Image {
        id: image1
        width: 200
        height: 100
        anchors.left: parent.left
        anchors.top: parent.top
        //source: "qrc:/resources/1.首页/logo.png"
        anchors.leftMargin: 16
        anchors.topMargin: 25
        fillMode: Image.PreserveAspectFit
    }
}
