import QtQuick
import QtQuick.Controls 2.0
import myCpp 1.0
ApplicationWindow{
    id:loginWindow
    width:800
    height:400




    title:"登录"



    Component.onCompleted: {
        loginWindow.setCurrentIndex(1);
        console.log("currentIndex:"+swipeView.currentIndex)
        show();
    }

    signal closed(bool close);

    function setCurrentIndex(val)
    {
        if(val===0)
        {
            width=1000
            height=600
            title="选择"
        }else if(val===1)
        {
            width=800
            height=400
            title="登录"
        }else if(val===2)
        {
            width=800
            height=400
            title="注册"
        }
        swipeView.setCurrentIndex(val);
    }

    Connections{
        id:loginConnection
        target:MainWindow

        function onLoginResult(result,message)
        {
           if(result===true)
           {
                loginWindow.setCurrentIndex(0);
           }
           else
           {
               setErrorMessage(message)
           }
        }

    }




    function setErrorMessage(error){
        errorMessage.text=error
    }



    SwipeView{
        id:swipeView
        currentIndex: 1
        anchors.fill: parent
        orientation: Qt.Horizontal
        interactive:false
        property var easingCurve: Easing.OutCubic
        transitions: [
                Transition {
                    NumberAnimation {
                        properties: "x,width"
                        easing.type: parent.easingCurve.type
                        duration: 500
                    }
                }
            ]

        SelectionWindow{
            id:selectionWindow


        }

        Rectangle{
            id:loginWidget

            Image{
                id:backgroundImage
                anchors.fill: parent
                source:"qrc:/resources/images/背景图.jpg"
                fillMode: Image.Stretch
            }

            Rectangle {
                id: loginArea
                x: 451
                width: 400
                height: 300
                color: "white"
                radius: 4

                anchors.right: parent.right
                anchors.top: parent.top
                anchors.topMargin: 60
                anchors.rightMargin: 30
                activeFocusOnTab: true
                focus: true


                Text {
                    id: text1
                    width: 192
                    height: 42
                    text:"账号登录"
                    anchors.top: rectangle1.top
                    font.pixelSize: 24
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.styleName: "Normal"
                    anchors.topMargin: -70
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Rectangle {
                    id: rectangle1
                    y: 118
                    width: 330
                    height: 46
                    color: "#ffffff"
                    anchors.left: parent.left
                    anchors.bottom: rectangle2.top
                    anchors.leftMargin: 42
                    anchors.bottomMargin: 25

                    border.width: 4
                    border.color: "green"
                    radius:4
                    TextInput{
                        id:userInfoInput
                        height: parent.height-4

                        width:parent.width-parent.width/10*2
                        anchors.left: leftUserIcon.right
                        anchors.top:parent.top
                        anchors.topMargin: 4
                        verticalAlignment: Text.AlignVCenter
                    }

                    Rectangle{
                        id:leftUserIcon
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
                    id: rectangle2//passwordInput
                    y: 160
                    width: 330
                    height: 46
                    color: "#ffffff"
                    anchors.left: parent.left
                    anchors.leftMargin: 42
                    border.width: 4
                    border.color: "green"
                    Rectangle{
                        id:leftIcon
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
                        id:rightIcon
                        width:parent.width/10;
                        height: parent.height
                        anchors.bottom:parent.bottom
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.rightMargin:4
                        anchors.bottomMargin:4
                        anchors.topMargin:4

                        MouseArea{
                            id:iconArea
                            anchors.fill: parent
                            property bool clicked:false;

                            onClicked: {
                                console.log("clicked");
                                iconArea.clicked=!clicked
                            }
                        }

                        Image{
                            anchors.fill: parent
                            source:"qrc:/resources/images/VisibilityIcon.png"

                        }
                    }

                    TextInput{
                        id:passwordInput
                        height: parent.height-4
                        echoMode: !iconArea.clicked?TextInput.Password:TextInput.Normal
                        width:parent.width-parent.width/10*2
                        anchors.left: leftIcon.right
                        anchors.top:parent.top
                        anchors.topMargin: 4
                        verticalAlignment: Text.AlignVCenter
                        function setEchoMode(boolen)
                        {

                        }
                    }

                    radius:4
                }

                Text{
                    id:errorMessage
                    text:""
                    color:"red"
                    anchors.top:rectangle2.bottom
                    anchors.left: rectangle1.left


                }

                FancyButton{
                    id:loginButton
                    anchors.top:rectangle2.bottom
                    anchors.topMargin: 40
                    anchors.left: rectangle1.left
                    width:150
                    height:40
                    color:"#1268D7"
                    hoverColor:"#1268D766"
                    text:"登录"

                    onClicked: {
                        MainWindow.login(userInfoInput,passwordInput.text)
                    }

                }
                FancyButton{
                    id:registerButton
                    anchors.top:rectangle2.bottom
                    anchors.topMargin: 40
                    anchors.right: rectangle1.right
                    text:"注册"
                    width:150
                    height:40
                    color:"#1268D7"
                    hoverColor:"#1268D766"

                    onClicked: {
                        loginWindow.setCurrentIndex(2);
                    }

                }
            }

            Image {
                id: image
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

        RegisterWidget{
            id:registerWidget

            onRegisterSucess: {
                loginWindow.setCurrentIndex(1);
            }
        }


    }




}

/*##^##
Designer {
    D{i:0}D{i:1;invisible:true}
}
##^##*/
