import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Effects
import QtQuick.Layouts
import QtCore
ApplicationWindow {
    id: root
    width: 800
    height: 508
    visible: true
    title: qsTr("CayPPer")
    property bool vimMode: controller? controller.vimKeysToggle: false
    property string keyBuffer:""
    Timer{
        id:bufferSequenceTimer
        interval:400
        repeat:false
        onTriggered: root.keyBuffer= ""
    }
    StackView{
        id:stackRoot
        initialItem: mainComponent
        width:parent.width
        height:parent.height
        anchors.fill:parent
        focus:true
        Keys.onPressed: (event) =>{
                            if(event.isAutoRepeat){
                                return
                            }
                            if(!root.vimMode){
                                event.accepted=false
                                return
                            }
                            let handled = false
                            root.keyBuffer+=event.text
                            bufferSequenceTimer.restart()
                            switch(root.keyBuffer){
                                case "gw":
                                if(stackRoot.depth===1){
                                    stackRoot.currentItem.folderDialog.open()
                                }
                                root.keyBuffer=""
                                handled=true
                                break
                                case "gs":
                                if(stackRoot.depth===1){
                                    stackRoot.push(settingsComponent)
                                }
                                root.keyBuffer=""
                                handled=true
                                break
                                case "gr":
                                if(stackRoot.depth===1){
                                    stackRoot.currentItem.gridLoader.forceActiveFocus()
                                }
                                root.keyBuffer=""
                                handled=true
                                break
                                case "gm":
                                if(stackRoot.depth===1){
                                    stackRoot.currentItem.modeBox.popup.open()
                                }
                                root.keyBuffer=""
                                handled=true
                                break
                                case "gW":
                                if(stackRoot.depth===2){
                                    stackRoot.currentItem.configDialog.open()
                                }
                                root.keyBuffer=""
                                handled=true
                                break
                                case "gc":
                                if(stackRoot.depth===2){
                                    stackRoot.currentItem.defaultPathDialog.open()
                                }
                                root.keyBuffer=""
                                handled=true
                                break
                                case "gv":
                                if(stackRoot.depth===2){
                                    stackRoot.currentItem.vimKeysBox.checked=!stackRoot.currentItem.vimKeysBox.checked
                                }
                                root.keyBuffer=""
                                handled=true
                                break
                                case "gn":
                                if(stackRoot.depth===2){
                                    stackRoot.pop(null)
                                    stackRoot.currentItem.gridLoader.forceActiveFocus()
                                }
                                root.keyBuffer=""
                                handled=true
                                break
                                case "j":
                                if(stackRoot.currentItem.modeBox.popup.visible){
                                    stackRoot.currentItem.modeBox.currentIndex =
                                    Math.max(0,
                                             Math.min(stackRoot.currentItem.modeBox.currentIndex + 1,
                                                      stackRoot.currentItem.modeBox.count - 1))
                                    stackRoot.currentItem.modeBox.incrementCurrentIndex()
                                    handled = true
                                }
                                root.keyBuffer=""
                                break
                                case "k":
                                if(stackRoot.currentItem.modeBox.popup.visible){
                                    stackRoot.currentItem.modeBox.currentIndex =
                                    Math.max(0,
                                             Math.min(stackRoot.currentItem.modeBox.currentIndex - 1,
                                                      stackRoot.currentItem.modeBox.count - 1))
                                    stackRoot.currentItem.modeBox.decrementCurrentIndex()
                                    handled = true
                                }
                                root.keyBuffer=""
                                break
                                case "I":
                                if(stackRoot.currentItem.modeBox.popup.visible){
                                    stackRoot.currentItem.modeBox.popup.close()
                                }
                                root.keyBuffer=""
                                handled=true
                                break
                                case "wq":
                                Qt.quit();
                            }

                        }

        Component{
            id:mainComponent
            Rectangle{
                id:mainRoot
                //Search button
                color:root.palette.window
                property alias gridLoader :wallpaperGridLoader
                property alias folderDialog: searchFieldDialog
                property alias modeBox:fillModeBox
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
                    model:[]
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
                            fillModeBox.popup.forceActiveFocus()
                            controller.setSelectedMode(currentText)
                        }

                    }
                    onCurrentIndexChanged: {
                        if(controller){
                            controller.setSelectedMode(currentText)
                        }
                    }
                    Component.onCompleted: {
                        model=controller.getModes
                        if(controller){
                            controller.setSelectedMode(currentText)
                        }
                    }
                    popup.onVisibleChanged: {
                        if(popup.visible){
                            fillModeBox.forceActiveFocus()
                        } else if (wallpaperGridLoader.item){
                            wallpaperGridLoader.item.forceActiveFocus()
                        }

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
                    width:monitorListView.contentWidth
                    height:32
                    anchors.topMargin: 10
                    anchors.leftMargin: 10
                    radius:10
                    color:"transparent"
                    border.color:root.palette.mid
                    border.width: 1
                    ListView{
                        id:monitorListView
                        height:parent.height
                        orientation: ListView.Horizontal
                        spacing:12
                        model: []
                        Component.onCompleted: {
                            model=controller.getConnectedMonitors
                        }

                        delegate:Rectangle{
                            id:monitorCheckBox
                            width:96
                            height:32
                            anchors.centerIn: parent
                            color:"transparent"
                            property bool checked:false
                            border.width:1
                            border.color:root.palette.mid
                            radius:10
                            RowLayout{
                                anchors.centerIn: parent
                                height:32
                                Image{
                                    id:monitorIcon
                                    width:32
                                    height:32
                                    source:"icons/monitor.svg"
                                    sourceSize:Qt.size(32,32)
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
                                Text{
                                    text:modelData
                                    font.pointSize: 12
                                    color:root.palette.text
                                }
                            }
                            MouseArea{
                                anchors.fill:parent
                                onClicked:{
                                    monitorCheckBox.checked=!monitorCheckBox.checked
                                }
                            }

                            onCheckedChanged:{
                                monitorCheckBox.color= checked? root.palette.mid :root.palette.window
                                controller.setSelectedMonitors(checked, modelData)
                                if(wallpaperGridLoader.item){
                                    wallpaperGridLoader.item.forceActiveFocus()
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
                    property int cellWidth:260
                    property int columns:Math.floor(parent.width/cellWidth)
                    width:columns*cellWidth
                    anchors.top:horizontalSeparator.bottom
                    anchors.bottom:parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    source:"Grid.qml"
                    active:true
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


