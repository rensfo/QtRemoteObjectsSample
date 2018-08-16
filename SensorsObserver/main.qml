import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.Controls 1.4 as OldControls

import my.sensors 1.0

Window {
    visible: true
    width: 1024
    height: 480
    title: qsTr("Sensor observer")

    MainFormViewModel {
        id: viewModel
        onSensorsChanged: {
            list.model = viewModel.sensors
        }
    }

    Component {
        id: floatDataDelegate
        Text {
            id: text
            text: styleData.value.toFixed(1)
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            onTextChanged: {
                animateColor.start()
            }
            SequentialAnimation {
                id: animateColor
                PropertyAnimation {
                    target: text
                    property: "color"
                    to: "red"
                    duration: 500
                }
                PropertyAnimation {
                    target: text
                    property: "color"
                    to: "black"
                    duration: 500
                }
            }
        }
    }

    Component {
        id: textDataDelegate
        Text {
            id: text
            text: styleData.value
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            onTextChanged: {
                animateColor.start()
            }
            SequentialAnimation {
                id: animateColor
                PropertyAnimation {
                    target: text
                    property: "color"
                    to: "red"
                    duration: 1000
                }
                PropertyAnimation {
                    target: text
                    property: "color"
                    to: "black"
                    duration: 1000
                }
            }
        }
    }

    Component {
        id: headerDelegate
        Label {
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            background: Rectangle {
                anchors.fill: parent
                border.color: "black"
            }
            text: styleData.value
        }
    }

    OldControls.TableView {
        id: list
        model: viewModel.sensorObservers
        anchors.fill: parent
        OldControls.TableViewColumn {
            title: "Host"
            role: "host"
            delegate: textDataDelegate
        }
        OldControls.TableViewColumn {
            title: "ID"
            role: "id"
            delegate: textDataDelegate
        }
        OldControls.TableViewColumn {
            title: "Name"
            role: "name"
            delegate: textDataDelegate
        }
        OldControls.TableViewColumn {
            title: "Humidity"
            role: "humidity"
            delegate: floatDataDelegate
        }
        OldControls.TableViewColumn {
            title: "Temperature"
            role: "temperature"
            delegate: floatDataDelegate
        }
        OldControls.TableViewColumn {
            title: "Connection"
            role: "connection"
            delegate: Item {
                RowLayout {
                    CheckDelegate {
                        Layout.alignment:  Qt.AlignVCenter | Qt.AlignHCenter
                        checked: styleData.value
                        enabled: false
                    }
                }
            }
        }
        headerDelegate: headerDelegate

        rowDelegate: Rectangle {
            height: 40
        }
    }
}
