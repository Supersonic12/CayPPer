import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
Popup{
    id:rootPopup
    popupType: Popup.item
    Text {
        id: enableText
        text: qsTr("Enabling Persistence")
    }
    RowLayout{
        id: changeFileRow
        Text{
            id:fileNameText
            //will take this from controller
            text:""
        }
    }
    RowLayout{
        id: addedTextRow
        Text{
            id:addedText
            //also will take this from controller
            text:""
        }
    }
}
