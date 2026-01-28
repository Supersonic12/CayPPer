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

            // Component.onCompleted: {
            //     if(delegateImage.actualWidth/delegateImage.actualHeight===16/9){

            //     }
            // }

            anchors.fill:parent
            sourceSize:Qt.size(1920,1080)
            source:model.imagePath
            fillMode:Image.PreserveAspectCrop
            smooth:true
            mipmap:true
            asynchronous:true
            cache:true
        }
    }
    highlight:Rectangle{
        color:"gray"
        anchors.bottom:delegateImage.top
        height:1
        width:delegateImage.width
    }
}
