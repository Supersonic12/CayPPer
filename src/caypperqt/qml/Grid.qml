import QtQuick
import QtQuick.Controls
import "settings" 1.0 as SettingsStore
GridView{
    id:wallpaperGridRoot
    //it will take directory from InputTaker.dirPath
    //it will load wallpapers with QImage
    cellWidth: 260
    cellHeight: wallpaperGridRoot.cellWidth/16*9
    clip:true
    highlightFollowsCurrentItem: true
    focus:true
    activeFocusOnTab: true
    property bool vimMode:false
    keyNavigationEnabled:true
    model:controller? controller.getImageModel : []
    signal setVimMode(bool enabled)

    onSetVimMode:{
        vimMode=enabled
        keyNavigationEnabled=!enabled
    }

    delegate: Rectangle{
        id:delegateRect
        width:256
        height:delegateRect.width/16*9
        color:"transparent"

        Image{
            id:delegateImage
            property bool isSixteentoNine:true
            width:254
            height:142
            anchors.centerIn: parent
            sourceSize:Qt.size(1920,1080)
            source:model.imagePath
            fillMode:Image.PreserveAspectCrop
            smooth:true
            mipmap:true
            asynchronous:true
            cache:true
        }

        MouseArea{
            id:delegateMArea
            anchors.fill:parent
            hoverEnabled: true
            property string result:""


            onEntered:{
                delegateRect.border.width=1
                delegateRect.border.color="black"
            }
            onExited:{
                delegateRect.border.width=0
                delegateRect.border.color="transparent"
            }
            onClicked:{
                //index
                result=index
                controller.setWallpaper(result)
            }
        }

    }

    //Vim Keys
    property int columns:wallpaperGridRoot.width/cellWidth
    property string keyBuffer:""
    Timer{
        id:bufferSequenceTimer
        interval:400
        repeat:false
        onTriggered: wallpaperGridRoot.keyBuffer = ""
    }

    Keys.onPressed: (event) => {
                        if(event.isAutoRepeat){
                            return;
                        }
                        if(!wallpaperGridRoot.vimMode){
                            event.accepted=false
                            return
                        }

                        let handled=false


                        wallpaperGridRoot.keyBuffer += event.text
                        bufferSequenceTimer.restart()
                        switch(wallpaperGridRoot.keyBuffer){

                            case "h":
                            wallpaperGridRoot.currentIndex--
                            wallpaperGridRoot.keyBuffer = ""
                            handled=true
                            break
                            case "l":
                            wallpaperGridRoot.currentIndex++
                            wallpaperGridRoot.keyBuffer = ""
                            handled=true
                            break
                            case "j":
                            wallpaperGridRoot.currentIndex+=wallpaperGridRoot.columns
                            wallpaperGridRoot.keyBuffer = ""
                            handled=true
                            break
                            case "k":
                            wallpaperGridRoot.currentIndex-=wallpaperGridRoot.columns
                            wallpaperGridRoot.keyBuffer = ""
                            handled=true
                            break
                            case "gg":
                            wallpaperGridRoot.currentIndex=0
                            wallpaperGridRoot.keyBuffer = ""
                            handled=true
                            break
                            case "G":
                            wallpaperGridRoot.currentIndex=wallpaperGridRoot.count-1
                            wallpaperGridRoot.keyBuffer = ""
                            handled=true
                            break
                            default:
                            if(wallpaperGridRoot.keyBuffer.length>2){
                                wallpaperGridRoot.keyBuffer=""
                                handled=true
                                break
                            }
                        }
                        if(handled){
                            wallpaperGridRoot.currentIndex=Math.max(0,
                                                                    Math.min(wallpaperGridRoot.count - 1,
                                                                             wallpaperGridRoot.currentIndex))
                            wallpaperGridRoot.keyBuffer=""
                            event.accepted=true
                        }
                    }

    highlight:Rectangle{
        color:"orange"
    }


}
