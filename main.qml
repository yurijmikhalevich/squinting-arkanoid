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
