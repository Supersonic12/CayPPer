import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtCore
Window {
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
                    color:"black"
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
                    property string result:""
                    background:Rectangle{
                        radius:6
                        border.width:1
                        border.color:"black"
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
                        source:"icons/folder-32x32.png"
                        fillMode: Image.PreserveAspectFit
                    }
                    Rectangle{
                        id:searchDialogHighLight
                        width:32
                        height:1
                        anchors.top:searchFieldDialogRect.bottom
                        anchors.left:parent.left
                    }
                    MouseArea{
                        anchors.fill:parent
                        hoverEnabled: true
                        onEntered:{
                            searchDialogHighLight.color="black"
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
                    color:"black"
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
                        border.color: "black"
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
                }

                Rectangle{
                    id:separatorBox
                    height:32
                    width:1
                    anchors.left:fillModeBox.right
                    anchors.top:parent.top
                    anchors.leftMargin:10
                    anchors.topMargin:10
                    color:"black"
                }
                Rectangle{
                    anchors.top:parent.top
                    anchors.left:separatorBox.right
                    width:96
                    height:32
                    anchors.topMargin: 10
                    anchors.leftMargin: 10
                    radius:10
                    border.color:"black"
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
                            color:"transparent"
                            CheckBox{
                                anchors.centerIn: parent
                                text:modelData
                                onCheckedChanged:{
                                    controller.setSelectedMonitors(checked, modelData)
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


