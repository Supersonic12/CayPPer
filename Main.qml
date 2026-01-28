import QtQuick
import QtQuick.Controls
Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("CayPPer")

    //Search button
    Rectangle{
        id:searchIconRoot
        width:32
        height:32
        anchors.top:parent.top
        anchors.left:parent.left
        anchors.topMargin:20
        anchors.leftMargin:20
        radius:2
        Image{
            id:searchIcon
            width:parent.width
            height:parent.height
            sourceSize.width: 32
            sourceSize.height: 32
            anchors.centerIn: parent
            fillMode:Image.PreserveAspectFit
            source: "qrc:/icons/search-32x32.png"
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
        anchors.topMargin: 18
        color:"black"
    }

    TextField{
        id:searchField
        anchors.left:searchIconRoot.right
        anchors.top:parent.top
        anchors.leftMargin:20
        anchors.topMargin:20
        width: 128
        height:32
        placeholderText:qsTr("Wallpaper Folder..")
        background:Rectangle{
            radius:6
            border.width:1
            border.color:"black"
        }
        onAccepted:{
            result=searchField.text
            inputTaker.takeInput(result)
        }
    }

    Rectangle{
        id:appSettingsRoot
        width:32
        height:32
        anchors.right:parent.right
        anchors.top:parent.top
        anchors.rightMargin: 20
        anchors.topMargin: 20
        Image{
            id:appSettingsIcon
            anchors.centerIn: parent
            width:parent.width
            height:parent.height
            source:"qrc:/icons/settings-512x512.png"
            sourceSize:Qt.size(512,512)
            fillMode: Image.PreserveAspectFit
            mipmap:true
            smooth:true
            asynchronous: true
            opacity: 0.95
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
                appSettingsLoader.item.closed.connect(() => {
                                                          appSettingsLoader.active=false
                                                          appSettingsLoader.source=""
                                                      })
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
                    appSettingsLoader.source="qrc:/src/qml/AppSettings.qml"
                    appSettingsLoader.active=true
                }
                appSettingsLoader.item.open()
            }
        }
    }
    Loader{
        id:wallpaperGridLoader
        active:false
    }

}

