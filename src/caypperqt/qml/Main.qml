import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Effects
import QtCore
ApplicationWindow {
    id: root
    width: 800
    height: 508
    visible: true
    title: qsTr("CayPPer")
    StackView{
        id:stackRoot
        initialItem: mainComponent
        width:parent.width
        height:parent.height
        anchors.fill:parent
        Component{
            id:mainComponent
            Rectangle{
                id:mainRoot
                //Search button
                color:root.palette.window
                Rectangle{
                    id:searchIconRoot
                    width:32
                    height:32
                    anchors.top:parent.top
                    anchors.left:parent.left
                    anchors.topMargin:10
                    anchors.leftMargin:10
                    radius:2
                    color:root.palette.window
                    Image{
                        id:searchIcon
                        width:parent.width
                        height:parent.height
                        sourceSize.width: 32
                        sourceSize.height: 32
                        anchors.centerIn: parent
                        fillMode:Image.PreserveAspectFit
                        source: "icons/search.svg"
                        asynchronous: true
                        smooth:true
                        mipmap:true
                        layer.enabled:true
                        layer.effect: MultiEffect{
                            colorization: 1.0
                            colorizationColor: root.palette.text
                        }
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
                            searchIconHover.color=root.palette.mid
                        }
                        onExited:{
                            searchIconHover.color="transparent"
                        }
                        onClicked:{
                            //take input function should take inputfields text property
                            result=searchField.text
                            controller.setDirectoryPath(result)
                            wallpaperGridLoader.source=""
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
                    color:root.palette.mid
                }

                TextField{
                    id:searchField
                    anchors.left:separatorSearch.right
                    anchors.top:parent.top
                    anchors.leftMargin:10
                    anchors.topMargin:10
                    width: root.width/3 //160
                    height:32
                    placeholderText:qsTr("Wallpaper Folder..")
                    placeholderTextColor: root.palette.text
                    property string result:""
                    background:Rectangle{
                        radius:6
                        border.width:1
                        border.color:root.palette.mid
                        color:root.palette.window
                    }
                    onAccepted:{
                        result=searchField.text
                        controller.setDirectoryPath(result)
                        wallpaperGridLoader.source="Grid.qml"
                        wallpaperGridLoader.active=true
                    }
                }

                Rectangle{
                    id:searchFieldDialogRect
                    width:32
                    height:32
                    anchors.left:searchField.right
                    anchors.leftMargin:10
                    anchors.top:parent.top
                    anchors.topMargin: 10
                    color:root.palette.window
                    FolderDialog{
                        id:searchFieldDialog
                        property string resultdialog:""
                        currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
                        options:FileDialog.ShowDirsOnly
                        onAccepted:{
                            resultdialog=searchFieldDialog.selectedFolder
                            controller.setDirectoryPath(resultdialog)
                            wallpaperGridLoader.source="Grid.qml"
                            wallpaperGridLoader.active=true
                        }
                    }
                    Image{
                        width:32
                        height:32
                        anchors.centerIn: parent
                        source:"icons/folder.svg"
                        fillMode: Image.PreserveAspectFit
                        smooth:true
                        mipmap:true
                        asynchronous: true
                        layer.enabled:true
                        layer.effect: MultiEffect{
                            colorization: 1.0
                            colorizationColor: root.palette.text
                        }
                    }
                    Rectangle{
                        id:searchDialogHighLight
                        width:32
                        height:1
                        anchors.top:searchFieldDialogRect.bottom
                        anchors.left:parent.left
                        color:root.palette.window
                    }
                    MouseArea{
                        anchors.fill:parent
                        hoverEnabled: true
                        onEntered:{
                            searchDialogHighLight.color=root.palette.mid
                        }
                        onExited:{
                            searchDialogHighLight.color="transparent"
                        }
                        onClicked:{
                            searchFieldDialog.open()
                        }
                    }
                }

                Rectangle{
                    id:separatorField
                    width:1
                    height:36
                    anchors.left:searchFieldDialogRect.right
                    anchors.leftMargin:10
                    anchors.top:parent.top
                    anchors.topMargin: 8
                    color:root.palette.mid
                }

                ComboBox{
                    id:fillModeBox
                    anchors.top:parent.top
                    anchors.topMargin:10
                    anchors.left:separatorField.right
                    anchors.leftMargin:10
                    width:96
                    height:32
                    //Did it like this because it was giving error everytime closing application
                    //need to find better solution if exists
                    model: controller ? controller.getModes : []

                    background: Rectangle{
                        id:fillModeBoxRect
                        border.width: 1
                        border.color: root.palette.mid
                        color:root.palette.window
                        radius:6
                        width:96
                        height:32

                    }

                    onActivated: {
                        if(controller){
                            controller.setSelectedMode(currentText)
                        }

                    }
                    onCurrentIndexChanged: {
                        if(controller){
                            controller.setSelectedMode(currentText)
                        }
                    }
                    Component.onCompleted: {
                        if(controller){
                            controller.setSelectedMode(currentText)
                        }
                    }
                    popup.onVisibleChanged: {
                        if(!popup.visible && wallpaperGridLoader.item){
                            wallpaperGridLoader.item.forceActiveFocus()
                        }
                    }
                }

                Rectangle{
                    id:separatorBox
                    height:32
                    width:1
                    anchors.left:fillModeBox.right
                    anchors.top:parent.top
                    anchors.leftMargin:10
                    anchors.topMargin:10
                    color:root.palette.mid
                }
                Rectangle{
                    anchors.top:parent.top
                    anchors.left:separatorBox.right
                    width:96
                    height:32
                    anchors.topMargin: 10
                    anchors.leftMargin: 10
                    radius:10
                    color:root.palette.window
                    border.color:root.palette.mid
                    border.width: 1
                    ListView{
                        id:monitorListView
                        width:parent.width
                        height:parent.height
                        anchors.centerIn: parent
                        model: []
                        Component.onCompleted: {

                            model=controller.getConnectedMonitors
                        }

                        delegate:Rectangle{
                            width:96
                            height:32
                            color:root.palette.window
                            CheckBox{
                                id:monitorCheckBox
                                anchors.centerIn: parent
                                anchors.fill:parent
                                text:modelData
                                onCheckedChanged:{
                                    controller.setSelectedMonitors(checked, modelData)
                                    if(wallpaperGridLoader.item){
                                        wallpaperGridLoader.item.forceActiveFocus()
                                    }
                                }
                            }
                        }
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
                    color:root.palette.window
                    Image{
                        id:appSettingsIcon
                        anchors.centerIn: parent
                        width:parent.width
                        height:parent.height
                        source:"icons/settings.svg"
                        sourceSize:Qt.size(512,512)
                        fillMode: Image.PreserveAspectFit
                        mipmap:true
                        smooth:true
                        asynchronous: true
                        layer.enabled:true
                        layer.effect: MultiEffect{
                            colorization: 1.0
                            colorizationColor: root.palette.text
                        }
                    }
                    Rectangle{
                        id:appSettingsIconHover
                        width:parent.width
                        anchors.bottom:parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        height:0
                        color:"transparent"
                    }
                    MouseArea{
                        id:appSettingsArea
                        anchors.fill:parent
                        hoverEnabled: true
                        onEntered:{
                            appSettingsIconHover.height=1
                            appSettingsIconHover.color=root.palette.mid
                        }
                        onExited:{
                            appSettingsIconHover.height=0
                            appSettingsIconHover.color="transparent"
                        }
                        onClicked:{
                            if(stackRoot.currentItem!=settingsComponent){
                                stackRoot.push(settingsComponent)
                            }
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
                    color:root.palette.mid
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
                        searchField.focus=false
                        item.currentIndex=0
                        item.forceActiveFocus()
                    }
                }
            }
        }
    }
    Component{
        id:settingsComponent
        SettingsStack{
            id:settingsStack
        }
    }
}


