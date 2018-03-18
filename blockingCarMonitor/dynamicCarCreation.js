
var component
var cars = new Array();
var anim = null


function initialCarCreation() {
    component = Qt.createComponent("car.qml");
    if(component.status == Component.Ready)
        createYellowCarObject();
    else
        component.statusChanged.connect(createYellowCarObject);
}

function createYellowCarObject(){
    if(component.status == Component.Ready){
        cars.push(component.createObject(backgroundId, {"x":getX(1), "y": getY(1), "source": getSrc(1)}));
    }
}

function createCarsObject(){
    if(component.status == Component.Ready){
        var i;
        for(i=2; i<=MapClass.getCarsNumber(); i++){
            cars.push(component.createObject(backgroundId, {"x":getX(i), "y": getY(i), "source": getSrc(i)}));
        }
    }
    //anim = Qt.createQmlObject ('import QtQuick 2.8; PropertyAnimation { }', backgroundId);
}

function getX(id){
    return (45 + (MapClass.getColumn(id)-1)*105);
}

function getY(id){
    return (40 + (MapClass.getRow(id)-1)*105);
}

function getSrc(id){
    var ornt = String.fromCharCode(MapClass.getOrnt(id));
    var len = MapClass.getLen(id);
    var name = (id == 1)? 10: ((len == 2)? (len*10) + Math.floor(Math.random() * 4):(len*10) + Math.floor(Math.random() * 3));
    var src = "img/cars/" + ornt + "/" + ornt + name + ".png";
    return src
}

function createTransition(id){
    anim = Qt.createQmlObject ('import QtQuick 2.8; PropertyAnimation { }', backgroundId);
    anim.target = cars[id-1];
    if(String.fromCharCode(MapClass.getOrnt(id)) == 'h'){
        anim.property = "x";
        anim.from = cars[id-1].x;
        anim.to = (String.fromCharCode(MapClass.getActionDirection()) == 'l')?cars[id-1].x - 105*MapClass.getActionLen():cars[id-1].x + 105*MapClass.getActionLen();
    }
    else{
        anim.property = "y";
        anim.from = cars[id-1].y;
        anim.to = (String.fromCharCode(MapClass.getActionDirection()) == 'u')?cars[id-1].y - 105*MapClass.getActionLen():cars[id-1].y + 105*MapClass.getActionLen();
    }
    anim.duration = 500;
    anim.running = true;
    anim.onStopped.connect(start);
}

function destroyTransitionObject(){
    if (anim != null)
        anim.destroy();
}

function destroyCarObjects(){
    var i
    for(i=0; i<MapClass.getCarsNumber(); i++){
        cars[i].destroy();
    }
}
