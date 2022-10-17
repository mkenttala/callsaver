import QtQuick 2.2
import Sailfish.Silica 1.0
import fi.kende.callsavermodel 1.0

ApplicationWindow {
    initialPage: Component {
        Page {
            SilicaListView {
                width: 480; height: 800
                model: callSaverModel
                delegate: Item {
                    width: ListView.view.width
                    height: Theme.itemSizeSmall

                    Label { text: number }
                }

                Component.onCompleted: console.log("count:", callSaverModel.rowCount())
            }
        }
    }

    CallSaverModel {
        id: callSaverModel
    }
}
