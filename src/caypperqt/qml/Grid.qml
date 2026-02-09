import QtQuick
import QtQuick.Controls
GridView{
    id:wallpaperGridRoot
    //it will take directory from InputTaker.dirPath
    //it will load wallpapers with QImage
    cellWidth: 260
    cellHeight: wallpaperGridRoot.cellWidth/16*9
    clip:true
    model:imageModel
    delegate: Rectangle{
        id:delegateRect
        width:256
        height:delegateRect.width/16*9
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
                inputTaker.chooseWallpaper(result)

            }
        }
    }
}
