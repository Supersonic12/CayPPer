import QtQuick
import QtQuick.Controls
Menu{
    id:settingsRoot
    MenuItem{
        text:"Vim Keys"
        checkable:true

        onCheckedChanged:{
            if(wallpaperGridLoader.item){
                wallpaperGridLoader.item.setVimMode(checked)
            }
        }
    }
    MenuItem{
        text:"User"
    }
    MenuItem{
        text:"Appearance"
    }
}
