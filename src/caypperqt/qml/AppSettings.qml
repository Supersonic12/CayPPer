import QtQuick
import QtQuick.Controls
Menu{
    id:settingsRoot
    //For Debugging, Delete in release
    Component.onDestruction: {
    }

    MenuItem{
        text:"Edit"
    }
    MenuItem{
        text:"User"
    }
    MenuItem{
        text:"Appearance"
    }
}
