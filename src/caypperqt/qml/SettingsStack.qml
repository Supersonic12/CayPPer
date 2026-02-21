import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCore
Rectangle{
    id:root
    color:root.palette.window
    Rectangle{
        id:backButtonRect
        anchors.left:parent.left
        anchors.top:parent.top
        anchors.margins: 10
        radius:8
        width:32
        height:32
        color:root.palette.window
        Image{
            id:backButtonIcon
            source:"icons/left-arrow-32x32.png"
            width:24
            height:24
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit
        }
        MouseArea{
            anchors.fill:parent
            hoverEnabled: true
            onEntered:{
                backButtonRect.border.width=1
                backButtonRect.border.color=root.palette.mid
            }
            onExited:{
                backButtonRect.border.color="transparent"
                backButtonRect.border.width=0
            }
            onClicked:{
                if(mainComponent!==stackRoot.currentItem){
                    stackRoot.pop(null)
                }
            }
        }
    }
    Rectangle{
        id:horizontalSeparator
        width:parent.width-20
        height:1
        anchors.top:backButtonRect.bottom
        anchors.topMargin:10
        anchors.left:parent.left
        anchors.leftMargin:10
        anchors.right:parent.rigth
        anchors.rightMargin:10
        color:root.palette.mid
    }

    ColumnLayout{
        id:settingsColumn
        spacing:2
        anchors.top:horizontalSeparator.bottom
        anchors.left:parent.left
        anchors.margins:10
        GroupBox{
            id:generalGroup
            title:"General"
            label: Label{
                text:generalGroup.title
                color:root.palette.text
                font.bold:true
            }
            ColumnLayout{
                Text{
                    id:configPathText
                    text:"Current Config Path"
                    Layout.alignment: Qt.AlignLeft
                    color:root.palette.text
                }
                RowLayout{
                    Layout.alignment: Qt.AlignLeft
                    Layout.preferredWidth:root.width-40
                    Rectangle{
                        Layout.alignment: Qt.AlignLeft
                        width:32
                    }
                    Text{
                        id:configPathCurrentText
                        text:controller?controller.configPath:""
                        Layout.alignment: Qt.AlignCenter
                        color:root.palette.text
                    }
                    FolderDialog{
                        id:configPathDialog
                        currentFolder:StandardPaths.standardLocations(StandardPaths.ConfigLocation)[0]
                        options:FileDialog.ShowDirsOnly
                        onAccepted:controller.setConfigPath(selectedFolder)
                    }
                    Rectangle{
                        id:configPathRect
                        width:32
                        height:32
                        radius:8
                        Layout.alignment: Qt.AlignRight
                        color:root.palette.window
                        Image{
                            anchors.centerIn: parent
                            width:24
                            height:24
                            source:"icons/folder-32x32.png"
                            fillMode: Image.PreserveAspectFit
                            smooth:true
                            MouseArea{
                                hoverEnabled: true
                                anchors.fill:parent
                                onEntered:{
                                    configPathRect.border.width=1
                                    configPathRect.border.color="gray"
                                }
                                onExited:{
                                    configPathRect.border.width=0
                                    configPathRect.border.color="transparent"
                                }
                                onClicked:{
                                    configPathDialog.open()
                                }
                            }
                        }
                    }
                }
                Text{
                    id:defaultWallpaperPathText
                    text:"Default Wallpaper Folder"
                    Layout.alignment: Qt.AlignLeft
                    color:root.palette.text
                }
                RowLayout
                {
                    Layout.alignment: Qt.AlignLeft
                    Layout.preferredWidth: root.width-40
                    Rectangle{
                        Layout.alignment: Qt.AlignLeft
                        width:32
                        color:root.palette.window
                    }
                    Text{
                        id:defaultWallpaperCurrentPathText
                        text:controller? controller.defaultWallPath:""
                        color:root.palette.text
                        Layout.alignment: Qt.AlignCenter
                    }

                    FolderDialog{
                        id:defaultWallPathDialog
                        currentFolder:StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
                        options:FileDialog.ShowDirsOnly
                        onAccepted: controller.setDefaultWallPath(selectedFolder)

                    }
                    Rectangle{
                        id:defaultWallPathRect
                        width: 32
                        height: 32
                        radius:8
                        Layout.alignment: Qt.AlignRight
                        color:root.palette.window
                        Image{
                            id:defaultWallPathIcon
                            source:"icons/folder-32x32.png"
                            anchors.centerIn: parent
                            width:24
                            height:24
                            fillMode: Image.PreserveAspectFit
                            MouseArea{
                                id:defaultWallPathArea
                                anchors.fill:parent
                                hoverEnabled: true
                                onEntered:{
                                    defaultWallPathRect.border.width=1
                                    defaultWallPathRect.border.color=root.palette.mid
                                }
                                onExited:{
                                    defaultWallPathRect.border.width=0
                                    defaultWallPathRect.border.color="transparent"
                                }
                                onClicked:{
                                    defaultWallPathDialog.open()
                                }
                            }
                        }
                    }
                }
            }
        }
        GroupBox{
            id:appearanceGroup
            title:"Appearance"
            label: Label{
                text:appearanceGroup.title
                color:root.palette.text
                font.bold:true
            }
            ColumnLayout{

                RowLayout{
                    id:darkModeToggleRow
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                    Layout.preferredWidth: root.width-40
                    Label{
                        id:darkModeToggleText
                        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                        text:"Dark Mode"
                        color:root.palette.text
                    }
                    Item{
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        Layout.preferredHeight: 24
                        Layout.preferredWidth: 24
                        CheckBox{
                            id:darkModeToggleBox
                            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                            checked:controller? controller.darkModeToggle:false
                            indicator:Rectangle{
                                id:darkModeToggleBoxRect
                                width:24
                                height:24
                                border.width:1
                                border.color:root.palette.mid
                                color:root.palette.window
                                radius:4
                                Image{
                                    id:darkModeToggleIndicatorIcon
                                    width:parent.width
                                    height:parent.height
                                    anchors.centerIn:parent
                                    fillMode:Image.PreserveAspectFit
                                    source: "icons/check-32x32.png"
                                    visible:controller?controller.darkModeToggle:false
                                }
                            }
                            onCheckedChanged: {
                                if(controller){
                                    if(checked){
                                        darkModeToggleIndicatorIcon.visible=true
                                    }else{
                                        darkModeToggleIndicatorIcon.visible=false
                                    }
                                    controller.setDarkModeToggle(checked)
                                }
                            }
                        }
                    }
                }
            }
        }
        GroupBox{
            id:behaviorGroup
            title:"Behavior"
            label: Label{
                text:behaviorGroup.title
                color:root.palette.text
                font.bold:true
            }

            ColumnLayout{
                RowLayout{
                    id:vimKeysToggleRow
                    Layout.alignment: Qt.AlignLeft
                    Layout.preferredWidth: root.width-40

                    Label {
                        id: vimKeysToggleText
                        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                        text: "Vim Keys"
                        color:root.palette.text
                    }
                    Item{
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        Layout.preferredHeight: 24
                        Layout.preferredWidth: 24
                        CheckBox{
                            id:vimKeysToggleBox
                            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                            checked:controller? controller.vimKeysToggle:false
                            indicator:Rectangle{
                                id:vimKeysToggleBoxRect
                                width:24
                                height:24
                                border.width:1
                                border.color:root.palette.mid
                                color:root.palette.window
                                radius:4
                                Image{
                                    id:vimKeysToggleIndicatorIcon
                                    width:parent.width
                                    height:parent.height
                                    fillMode:Image.PreserveAspectFit
                                    source:"icons/check-32x32.png"
                                    visible:controller?controller.vimKeysToggle:false
                                }
                            }
                            onCheckedChanged: {
                                if(controller){
                                    if(checked){
                                        vimKeysToggleIndicatorIcon.visible=true
                                    }else{
                                        vimKeysToggleIndicatorIcon.visible=false
                                    }
                                    controller.setVimKeysToggle(checked)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
