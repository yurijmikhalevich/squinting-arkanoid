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
import "arkanoid-logic.js" as ArkanoidLogic

Rectangle {
    id: arkanoid
    color: 'blue'
    Component.onCompleted: {
        ArkanoidLogic.reload(arkanoid);
    }
    property string type: 'arkanoid'
    property string stickMovingDirection: 'none'
    property int ballXVelocity: 0
    property int ballYVelocity: 0
    Timer {
        id: timer
        interval: 1000 / 24
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            ArkanoidLogic.tick(arkanoid);
        }
    }
}

