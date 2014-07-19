/**
 * @file
 * @author Yurij Mikhalevich <0@39.yt>
 *
 * @section LICENSE
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
import QtQuick 2.2
import QtQuick.Window 2.1

Window {
    id: mainWindow
    title: 'Squint Arkanoid'
    visible: true
    width: 780
    height: 480
    maximumHeight: this.height
    minimumHeight: this.height
    maximumWidth: this.width
    minimumWidth: this.width
    Arkanoid {
        id: arkanoid
        width: parent.width / 13 * 8
        height: parent.height
        anchors.left: parent.left
    }
    Item {
        id: sidebar
        width: parent.width / 13 * 5
        height: parent.height
        anchors.right: parent.right
        Item {
            anchors.fill: parent
            anchors.bottom: camera.top
            anchors.margins: 10
            Text {
                id: credits
                width: parent.width
                height: contentHeight
                text: '«Squint Arkanoid» is an OpenCV demonstration application written\nby Yurij Mikhalevich <0@39.yt>.\nWritten in C++ and JS (used in QML).\nPowered by OpenCV and Qt.'
                wrapMode: Text.WordWrap
            }
            Item {
                anchors.top: credits.bottom
                width: parent.width
                height: parent.height * 2 / 3
                Image {
                    source: 'qrc:///images/opencv-logo.png'
                    width: parent.width / 2 - 30
                    fillMode: Image.PreserveAspectFit
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.margins: 20
                }
                Image {
                    source: 'qrc:///images/qt-logo.png'
                    width: parent.width / 2 - 10
                    fillMode: Image.PreserveAspectFit
                    anchors.right: parent.right
                    anchors.top: parent.top
                }
            }
        }
        Image {
            id: camera
            width: parent.width
            height: parent.width * 0.75
            cache: false
            mirror: true
            anchors.bottom: parent.bottom
            fillMode: Image.PreserveAspectFit
        }
    }
    Timer {
        interval: 1000 / 24
        repeat: true
        running: true
        triggeredOnStart: true
        onTriggered: {
            camera.source = 'image://camera/frame' + Math.random()
        }
    }
}
