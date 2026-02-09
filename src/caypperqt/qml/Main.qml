import QtQuick
import QtQuick.Controls
Window {
    id: root
    width: 800
    height: 508
    visible: true
    title: qsTr("CayPPer")

    //Search button
    Rectangle{
        id:searchIconRoot
        width:32
        height:32
        anchors.top:parent.top
        anchors.left:parent.left
        anchors.topMargin:10
        anchors.leftMargin:10
        radius:2
        Image{
            id:searchIcon
            width:parent.width
            height:parent.height
            sourceSize.width: 32
            sourceSize.height: 32
            anchors.centerIn: parent
            fillMode:Image.PreserveAspectFit
            source: "icons/search-32x32.png"
            asynchronous: true
            smooth:true
            mipmap:true
        }
        Rectangle{
            id:searchIconHover
            width:parent.width
            height:1
            color:"transparent"
            anchors.bottom:parent.bottom
        }

        MouseArea{
            id:searchMArea
            anchors.fill:parent
            hoverEnabled: true
            property string result:""
            onEntered:{
                searchIconHover.color="black"
            }
            onExited:{
                searchIconHover.color="transparent"
            }
            onClicked:{
                //take input function should take inputfields text property
                result=searchField.text
                inputTaker.takeInput(result)
                wallpaperGridLoader.source="Grid.qml"
                wallpaperGridLoader.active=true
            }
        }
    }
    Rectangle{
        id:separatorSearch
        width:1
        height:36

        anchors.left:searchIconRoot.right
        anchors.leftMargin:10
        anchors.top:parent.top
        anchors.topMargin: 8
        color:"black"
    }

    TextField{
        id:searchField
        anchors.left:separatorSearch.right
        anchors.top:parent.top
        anchors.leftMargin:10
        anchors.topMargin:10
        width: 160
        height:32
        placeholderText:qsTr("Wallpaper Folder..")
        property string result:""
        background:Rectangle{
            radius:6
            border.width:1
            border.color:"black"
        }
        onAccepted:{
            result=searchField.text
            inputTaker.takeInput(result)
            wallpaperGridLoader.source="Grid.qml"
            wallpaperGridLoader.active=true
        }
    }

    ComboBox{
        id:fillModeBox
        anchors.top:parent.top
        anchors.topMargin:10
        anchors.left:searchField.right
        anchors.leftMargin:10
        width:96
        height:32
        //Did it like this because it was giving error everytime closing application
        //need to find better solution if exists
        model: controller ? controller.modes : []
         onActivated: {
             inputTaker.selectedMode(currentText)
         }
        onCurrentIndexChanged: {
            inputTaker.selectedMode(currentText)
        }
        Component.onCompleted: {
            inputTaker.selectedMode(currentText)
        }
    }

    Rectangle{
        id:appSettingsRoot
        width:32
        height:32
        anchors.right:parent.right
        anchors.top:parent.top
        anchors.rightMargin: 10
        anchors.topMargin: 10
        Image{
            id:appSettingsIcon
            anchors.centerIn: parent
            width:parent.width
            height:parent.height
            source:"icons/settings-512x512.png"
            sourceSize:Qt.size(512,512)
            fillMode: Image.PreserveAspectFit
            mipmap:true
            smooth:true
            asynchronous: true
        }
        Rectangle{
            id:appSettingsIconHover
            width:parent.width
            anchors.bottom:parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            height:0
            color:"transparent"
        }

        Loader{
            id:appSettingsLoader
            active:false
            onLoaded:{
                appSettingsLoader.
                item.
                closed.
                connect(() =>
                        {
                            appSettingsLoader.active=false
                            appSettingsLoader.source=""})
            }
        }

        MouseArea{
            id:appSettingsArea
            anchors.fill:parent
            hoverEnabled: true
            onEntered:{
                appSettingsIconHover.height=1
                appSettingsIconHover.color="black"
            }
            onExited:{
                appSettingsIconHover.height=0
                appSettingsIconHover.color="transparent"
            }
            onClicked:{
                if(!appSettingsLoader.active){
                    appSettingsLoader.source="AppSettings.qml"
                    appSettingsLoader.active=true
                }
                appSettingsLoader.item.open()
            }
        }
    }

    // Horizontal Separator that separates buttons from wallpaper grid
    Rectangle{
        id:horizontalSeparator
        anchors.top:searchIconRoot.bottom
        anchors.topMargin:10

        anchors.horizontalCenter: parent.horizontalCenter
        width:parent.width-anchors.topMargin
        height:1
        color:"black"
    }

    Loader{
        id:wallpaperGridLoader
        width:parent.width-20
        height:parent.width-20-50
        anchors.top:horizontalSeparator.bottom
        anchors.left:parent.left
        anchors.bottom:parent.bottom
        anchors.right:parent.right
        anchors.margins: 10
        active:false
        source:""
        onLoaded:{

        }
    }
}

