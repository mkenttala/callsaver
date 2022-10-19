import QtQuick 2.2
import Sailfish.Silica 1.0
import fi.kende.callsavermodel 1.0

ApplicationWindow {
    initialPage: Component {
        Page {
            SilicaListView {
                id: listView
                anchors.fill: parent
                anchors.margins: {Theme.paddingLarge, Theme.paddingLarge, Theme.paddingLarge, Theme.paddingLarge}
                model: callSaverModel
                highlightFollowsCurrentItem: true
                highlight: Rectangle { color: Theme.highlightBackgroundColor }
                focus: true
                currentIndex: -1
                delegate: Item {
                    width: parent.width
                    height: Theme.itemSizeSmall

                    Column {
                        anchors.fill: parent
                        Row {
                            Label {
                                font.pixelSize: Theme.fontSizeMedium
                                text: number
                            }
                        }
                        Row {
                            spacing: 10
                            Label {
                                font.pixelSize: Theme.fontSizeExtraSmall
                                text: date
                            }
                            Label {
                                font.pixelSize: Theme.fontSizeExtraSmall
                                text: length
                            }
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            listView.currentIndex = index;
                            callSaverModel.play(index);
                        }
                    }
                }
                CallSaverModel {
                    id: callSaverModel

                    onPlayPositionChanged: {
                        listView.highlightItem.width = listView.width * playPosition / 100;
                    }
                }
            }
        }
    }
}
