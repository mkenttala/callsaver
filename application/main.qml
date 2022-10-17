import QtQuick 2.2
import Sailfish.Silica 1.0
import fi.kende.callsavermodel 1.0

ApplicationWindow {
    initialPage: Component {
        Page {
            SilicaListView {
                anchors.fill: parent
                model: callSaverModel
                delegate: Item {
                    width: parent.width
                    height: Theme.itemSizeSmall
                    Row {
                        spacing: 10

                        Label { text: number }
                        Label { text: length }
                    }
                }

                Component.onCompleted: console.log("count:", callSaverModel.rowCount())
            }
        }
    }

    CallSaverModel {
        id: callSaverModel
    }
}
