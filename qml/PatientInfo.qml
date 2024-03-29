import QtQuick 2.15
import QtQuick.Controls 2.0
import myCpp 1.0
Rectangle {
    border.width: 1
    border.color:"gray"
    radius:10
    anchors.left: parent.left
    anchors.leftMargin: 20
    anchors.right: parent.right
    anchors.rightMargin: 20
    anchors.top: infoText.bottom

    //病人和检查信息应当是两个不同的概念，同一个病人可以有多个检查信息

    function sendCurrentPatient(){
        var cI=listView.currentIndex
        if(cI===-1)
        {
            return false;
        }

        MainWindow.setCurrentPatient(patientModel.get(cI).identity)
        return true;
    }

    Connections{
        /*
        emit sendPatient(p.m_patientName,p.m_patientAge,
                         p.m_patientSex,p.loginDate,
                         p.m_patientId,p.m_patientPhone,
                         p.m_patientState1,p.m_patientState2);
          */
        target:MainWindow
        function onSendPatient(p_name,p_age,p_Sex,p_time,p_identity,p_phone,p_hasCancer,p_hasHPV,){
            patientModel.addListElement(p_name,p_age,p_time,p_identity
                                        ,p_phone,p_hasCancer,p_hasHPV,p_Sex)
        }
    }

    Component.onCompleted: {
        MainWindow.populatePatients()
    }

    property int pageIndex: 0
    function switchToAddPatient(){
        var addPatient=stackView.push("qrc:/qml/AddPatientPage.qml")
         addPatient.returnSignal.connect(function(){
              switchToPatientInfo()
         })
        addPatient.addPatientSignal.connect(patientModel.addListElement)
        pageIndex=1;
    }
    function switchToPatientInfo()
    {
        stackView.pop()
        pageIndex=0
    }

    height:parent.height-70-40-buttonsButtomMargin-5;
    width:parent.width-40

    StackView{
        id:stackView
        anchors.fill: parent


        initialItem: Rectangle{
            ListView{
                focus:true
                id:listView
                anchors.fill: parent

                function getHightListData(){

                }

                /* inusable
                ScrollBar.horizontal: ScrollBar{
                    background: Rectangle{
                        anchors.fill: parent
                        color:"red"

                    }

                    active:true
                    parent:listView.parent

                    width:listView.width
                    anchors.left: listView.left
                    anchors.top: listView.bottom
                }
                ScrollBar.vertical: ScrollBar{

                    parent:listView.parent
                    height: listView.height

                    anchors.top: listView.top
                    anchors.left: listView.right
                }
                */

                headerPositioning:ListView.OverlayHeader
                header:Row {
                    height: 30
                    Repeater{
                        id:repeater
                        model:[{label:"姓名",w:75},{label:"年龄",w:50},{label:"性别",w:50}
                            ,{label:"检查时间",w:75},{label:"身份证号",w:200},{label:"电话号码",w:200},
                            {label:"有无宫颈癌或其他癌症病史",w:200},{label:"有无HPV感染史",w:100}]
                        delegate:
                            Rectangle
                            {
                                border.width: 1
                                border.color: "gray"
                                width:modelData.w
                                height: 30
                                Text{
                                    anchors.fill: parent
                                    text:modelData.label

                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter

                                    font.pixelSize: 14
                                    color:"black"
                                }
                           }
                    }

                }

                model:patientModel

                delegate: lineDelegate

            }
        }
    }

    Component{

        id:lineDelegate

        Rectangle
        {
            id:element
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    listView.currentIndex=index
                }
            }

            width:parent.width;
            height:30

            color:!ListView.isCurrentItem?"transparent":"green"
            Row{
              anchors.fill: parent
              Repeater{
                    id:delegateRepeater
                    model:[{label:name,w:75},{label:age,w:50}
                        ,{label:sex,w:50},{label:time,w:75}
                        ,{label:identity,w:200},{label:phone,w:200},
                        {label:hasCancerHistory,w:200},{label:hasHPV,w:100},
                        ]
                    delegate:Rectangle
                    {
                        color:element.color
                        border.width: 1
                        border.color: "gray"
                        width:modelData.w
                        height:30
                        Text{
                            text:modelData.label

                            font.pixelSize: 16
                            color:"black"
                            font.bold: true
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }


                }

            }
        }
    }

    Connections{

    }

    ListModel
    {
        id:patientModel


        function addListElement(p_name,p_age,p_time,p_identity,p_phone,p_hasCancer,p_hasHPV,p_Sex){
              patientModel.append({
                                     name:p_name,
                                     age:p_age,
                                     time:p_time,
                                     identity:p_identity,
                                     phone:p_phone,
                                     hasCancerHistory:p_hasCancer,
                                     hasHPV:p_hasHPV,
                                     sex:p_Sex,
                                 }
                                 )
        }       
    }
}




