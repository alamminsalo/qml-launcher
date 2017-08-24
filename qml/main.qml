import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls.Material 2.1

ApplicationWindow {
    visible: true
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    title: qsTr("Hello World")
    visibility: Window.FullScreen

    color: "transparent"
    background: Rectangle{
        color: "black"
        opacity: 0.666
    }

    id: rootWindow
    property int columns: 6

    header: ToolBar {
        height: 120
        background: Item{}
        TextField {
            id: searchField
            anchors {
                horizontalCenter: parent.horizontalCenter
                bottom: parent.bottom
            }

            width: 300
            onTextEdited: {
                fill(searchField.text)
            }
            Keys.onEscapePressed: Qt.quit()
            onAccepted: {
                var appQml = swipeView.currentItem.children[0].children[0]
                if (appQml && appQml.app) {
                    exec(appQml.app[2])
                }
            }
        }
    }

    property var appPages: []

    property int cellWidth: 180
    property int cellHeight: cellWidth

    function fill(str){
        var page = 0
        var itemsPerPage = 24
        console.log(itemsPerPage)
        appPages = [];
        appPages[page] = []

        for (var i in apps) {
            if (appPages[page].length >= itemsPerPage)
                page++

            if (!appPages[page])
                appPages[page] = []

            if (!str || apps[i][0].toLowerCase().match(str.toLowerCase()))
                appPages[page].push(apps[i])
        }

        pageRepeater.model = appPages.length
    }

    Component.onCompleted: {
        fill()
        searchField.forceActiveFocus()
    }

    SwipeView {
        Keys.onEscapePressed: Qt.quit()
        id: swipeView
        anchors.fill: parent

        property int selectedIndex: 0

        function select(index) {
            selectedIndex = index
        }

        Keys.onUpPressed: {
            var place = selectedIndex - rootWindow.columns
            if (place >= 0)
                selectedIndex = place
        }

        Keys.onDownPressed: {
            var place = selectedIndex + rootWindow.columns
            if (place < appPages[currentIndex].length)
                selectedIndex = place
        }

        Keys.onRightPressed: selectedIndex++
        Keys.onLeftPressed: selectedIndex--

        Keys.onReturnPressed: {
            var app = appPages[currentIndex][selectedIndex]
            if (app) {
                exec(app[2])
            }
        }

        onFocusChanged: {
            if (focus) {
                select(0)
            }
        }

        Repeater {
            id: pageRepeater
            model: appPages.length

            Item {
                property var page: appPages[index]

                Grid {
                    spacing: 10
                    anchors.centerIn: parent
                    columns: rootWindow.columns
                    horizontalItemAlignment: Grid.AlignHCenter


                    populate: Transition {
                        id: trans
                        SequentialAnimation {
                            NumberAnimation {
                                properties: "opacity";
                                from: 1
                                to: 0
                                duration: 0
                            }
                            PauseAnimation {
                                duration: (trans.ViewTransition.index -
                                           trans.ViewTransition.targetIndexes[0]) * 20
                            }
                            ParallelAnimation {
                                NumberAnimation {
                                    properties: "opacity";
                                    from: 0
                                    to: 1
                                    duration: 600
                                    easing.type: Easing.OutCubic
                                }
                                NumberAnimation {
                                    properties: "y";
                                    from: trans.ViewTransition.destination.y + 50
                                    duration: 620
                                    easing.type: Easing.OutCubic
                                }
                            }
                        }
                    }

                    Repeater {
                        model: page.length

                        AppEntry {
                            app: page[index]
                            height: cellWidth
                            width: height
                            padding: 10
                            selected: swipeView.selectedIndex === index
                            onHovered: {
                                swipeView.select(index)
                            }
                            onClicked: {
                                exec(app[2])
                            }
                        }
                    }
                }
            }
        }
    }

    function exec(program) {
        console.debug("Exec: " + program)
        proc.start(program)
        Qt.quit();
    }

    footer: ToolBar {
        background: Item{}
        height: 50
        PageIndicator {
            count: swipeView.count
            currentIndex: swipeView.currentIndex
            anchors.centerIn: parent
        }
    }
}
