import QtQuick 2.8
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import "dynamicCarCreation.js" as DCC



ApplicationWindow {
    property string mapFilePath: ""
    property string solutionFilePath: ""
    property bool isMapOpen
    property bool isSuccess: false

    id: windowId
    visible: true
    width: 700
    maximumWidth: 700
    minimumWidth: 700
    height: 710
    maximumHeight: 710
    minimumHeight: 710
    title: qsTr("Blocking Car")

    Image {
        id: backgroundId
        source: "img/bg.png"
        anchors.fill: parent
        z:1
    }

    onClosing: {
        DCC.destroyCarObjects()
        //DCC.destroyTransitionObject() //it will be destroy in start function
    }

    function mapSetup(){
        if(mapFilePath != ""){
            MapClass.readMapFile(mapFilePath)
            DCC.initialCarCreation()
            DCC.createCarsObject()
        }
    }

    function actionSetup(){
        if(solutionFilePath != ""){
            MapClass.readActionFile(solutionFilePath)
        }
    }

    function start(){
        DCC.destroyTransitionObject()
        if(DCC.cars[0].x == (4*105) + 45){
            isSuccess = true
            finish(isSuccess)
            return
        }

        if(MapClass.nextAction()){
            DCC.createTransition(MapClass.getActionId())
        }
        else finish(isSuccess)

    }
    function finish(goal){
        if(isSuccess)
            textGoalId.text = "SUCCESS\n" + MapClass.getActionCounter() + " Move"
        else
            textGoalId.text = "FAILURE\n" + MapClass.getActionCounter() + " Move"
        goalAnimId1.start();goalAnimId2.start()
    }

    FileDialog{
        id: fileDialogId
        selectFolder: false
        selectMultiple: false
        nameFilters: "*.txt"

        onAccepted: {
            if(isMapOpen){
                mapFilePath = fileUrl
                mapFilePath = mapFilePath.replace(/^(file:\/{2})|(qrc:\/{2})|(http:\/{2})/,"");
                mapSetup()
            }
            else{
                solutionFilePath = fileUrl
                solutionFilePath = solutionFilePath.replace(/^(file:\/{2})|(qrc:\/{2})|(http:\/{2})/,"");
                actionSetup()
            }
        }
    }

    menuBar: MenuBar{
        Menu{
            title: "File"
            MenuItem{ id: openMapId; text: "Open Map"; onTriggered:{ isMapOpen = true; fileDialogId.open()}}
            MenuItem{ id: openSolutionId; text: "Open Solution"; onTriggered:{ isMapOpen = false; fileDialogId.open()}}
        }
        Menu{
            title: "Play"
            MenuItem{ id: startId; text: "Start"; onTriggered:{ if(mapFilePath != "" && solutionFilePath != "") start()}}
            MenuItem{ id: quitId; text: "Quit"; onTriggered: Qt.quit()}
        }
    }


    Text {
        id: textGoalId
        anchors.centerIn: parent
        text: ""
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 20
        font.bold: true
        color: isSuccess? "green" : "red"
        opacity: 0
        z:2

        PropertyAnimation {
            target: textGoalId
            properties: "font.pointSize"
            id:goalAnimId1
            from: 50
            to: 100
            duration: 3000
            easing.type: Easing.InOutCubic
        }
        PropertyAnimation {
            target: textGoalId
            properties: "opacity"
            id:goalAnimId2
            from: 0
            to: 0.9
            duration: 3000
        }
    }
}

