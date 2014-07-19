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
.pragma library

var BRICK_COLORS = [
            'green',
            'red',
            'yellow'
        ];

var stick = undefined;
var ball = undefined;
var bricks = [];


function spawnBricks(arkanoid) {
    var brickComponent = Qt.createComponent('Brick.qml');
    var infoBrick = brickComponent.createObject(arkanoid, {});
    var brickHeight = infoBrick.height;
    var brickWidth = infoBrick.width;
    infoBrick.destroy();
    var brickRowsCount = ((arkanoid.height / 2) / brickHeight) - 1;
    var brickColumnsCount = (arkanoid.width / brickWidth) - 1;
    var brickX;
    var brickY = 0;
    for (var row = 0; row < brickRowsCount; ++row) {
        brickX = brickWidth / 2;
        brickY += brickHeight;
        for (var column = 0; column < brickColumnsCount; ++column) {
            bricks.push(brickComponent.createObject(
                            arkanoid, {
                                x: brickX,
                                y: brickY,
                                color: BRICK_COLORS[Math.floor(Math.random() * BRICK_COLORS.length)]
                            }));
            brickX += brickWidth;
        }
    }
}


function reload(arkanoid) {
    if (stick) {
        stick.destroy();
        ball.destroy();
        bricks.forEach(function(brick) {
            delete brick.destroy();
        });
        bricks = [];
    }
    stick = loadStick(arkanoid);
    stick.x = (arkanoid.width / 2) - (stick.width / 2);
    ball = loadBall(arkanoid);
    ball.y = arkanoid.height - stick.height - ball.height;
    ball.x = (arkanoid.width / 2) - (ball.width / 2);
    arkanoid.stickMovingDirection = 'none';
    arkanoid.ballXVelocity = 0;
    arkanoid.ballYVelocity = 0;
    spawnBricks(arkanoid);
    arkanoid.data[0].start(); // arkanoid.data[0] is an arkanoid's timer object
}


function loadStick(arkanoid) {
    var stickComponent = Qt.createComponent('Stick.qml');
    var stick = stickComponent.createObject(
                arkanoid, {id: 'stick', 'anchors.bottom': arkanoid.bottom});
    return stick;
}


function loadBall(arkanoid) {
    var ballComponent = Qt.createComponent('Ball.qml');
    var ball = ballComponent.createObject(arkanoid, {id: 'ball'});
    return ball;
}


function tick(arkanoid) {
    moveStick(arkanoid);
    if ((0 === arkanoid.ballXVelocity) && (0 === arkanoid.ballYVelocity) &&
            (-1 !== ['left', 'right'].indexOf(arkanoid.stickMovingDirection))) {
        arkanoid.ballYVelocity = -3;
        arkanoid.ballXVelocity = 3;
        if ('right' === arkanoid.stickMovingDirection) {
            arkanoid.ballXVelocity = -arkanoid.ballXVelocity;
        }
    }
    var collision = findNearestCollision(findBallCollisions(arkanoid));
    if (null === collision) {
        ball.x += arkanoid.ballXVelocity;
        ball.y += arkanoid.ballYVelocity;
    } else {
        ball.x = collision.newX;
        ball.y = collision.newY;
        if ('reload' === collision.object) {
            reload(arkanoid);
        } else {
            arkanoid.ballXVelocity = collision.newXVelocity;
            arkanoid.ballYVelocity = collision.newYVelocity;
            if ('brick' === collision.object) {
                collision.objectReference.visible = false;
            }
        }
    }
}


function moveStick(arkanoid) {
    var newStickX;
    switch (arkanoid.stickMovingDirection) {
    case 'left':
        newStickX = stick.x - 3;
        if (newStickX < 0) {
            stick.x = 0;
        } else {
            stick.x = newStickX;
        }
        break;
    case 'right':
        newStickX = stick.x + 3;
        if ((newStickX + stick.width) > arkanoid.width) {
            stick.x = arkanoid.width - stick.width;
        } else {
            stick.x = newStickX;
        }
        break;
    case 'none':
    default:
        break;
    }
}


function findBallCollisions(arkanoid) {
    var collisions = [];
    var newBallY = ball.y + arkanoid.ballYVelocity;
    var newBallX = ball.x + arkanoid.ballXVelocity;
    if (newBallY <= 0) {
        collisions.push({
                            object: 'wall',
                            newY: 0,
                            newX: ball.x + arkanoid.ballXVelocity,
                            newXVelocity: arkanoid.ballXVelocity,
                            newYVelocity: -arkanoid.ballYVelocity
                        });
    }
    if (newBallX <= 0) {
        collisions.push({
                            object: 'wall',
                            newY: ball.y + arkanoid.ballYVelocity,
                            newX: 0,
                            newXVelocity: -arkanoid.ballXVelocity,
                            newYVelocity: arkanoid.ballYVelocity
                        });
    }
    if ((newBallX + ball.width) >= arkanoid.width) {
        collisions.push({
                            object: 'wall',
                            newY: ball.y + arkanoid.ballYVelocity,
                            newX: arkanoid.width - ball.width,
                            newXVelocity: -arkanoid.ballXVelocity,
                            newYVelocity: arkanoid.ballYVelocity
                        });
    }
    if ((newBallY + ball.height) >= stick.y) {
        var ballMiddleX = newBallX + (ball.width / 2)
        if ((stick.x <= ballMiddleX) && (ballMiddleX <= (stick.x + stick.width))) {
            collisions.push({
                                object: 'stick',
                                newY: arkanoid.height - ball.height - stick.height,
                                newX: ball.x + arkanoid.ballXVelocity,
                                newXVelocity: arkanoid.ballXVelocity,
                                newYVelocity: -arkanoid.ballYVelocity
                            });
        } else {
            collisions.push({
                                object: 'reload',
                                newY: arkanoid.height - ball.height,
                                newX: ball.x + arkanoid.ballXVelocity
                            });
        }
    }
    var newBallMiddleX = newBallX + (ball.width / 2);
    var newBallMiddleY = newBallY + (ball.height / 2);
    bricks.forEach(function(brick) {
        if (!brick.visible) {
            return;
        }
        var brickTopLeft = {x: brick.x, y: brick.y};
        var brickBottomLeft = {x: brick.x, y: brick.y + brick.height};
        var brickTopRight = {x: brick.x + brick.width, y: brick.y};
        var brickBottomRight = {x: brick.x + brick.width, y: brick.y + brick.height};
        if ((((newBallX + ball.width) >= brick.x) && (brick.x >= (ball.x + ball.width))) &&
                ((brickTopLeft.y <= newBallMiddleY) && (newBallMiddleY <= brickBottomLeft.y))) {
            collisions.push({
                                object: 'brick',
                                objectReference: brick,
                                newY: ball.y + arkanoid.ballYVelocity,
                                newX: brick.x - ball.width,
                                newXVelocity: -arkanoid.ballXVelocity,
                                newYVelocity: arkanoid.ballYVelocity
                            });
        } else if ((((newBallX + ball.width) <= brickTopRight.x) && (brickTopRight.x <= (ball.x + ball.width))) &&
                   ((brickTopRight.y <= newBallMiddleY) && (newBallMiddleY <= brickBottomRight.y))) {
            collisions.push({
                                object: 'brick',
                                objectReference: brick,
                                newY: ball.y + arkanoid.ballYVelocity,
                                newX: brickTopRight.x,
                                newXVelocity: -arkanoid.ballXVelocity,
                                newYVelocity: arkanoid.ballYVelocity
                            });
        } else if ((((newBallY + ball.height) >= brick.y) && (brick.y >= (ball.y + ball.height))) &&
                   ((brickTopLeft.x <= newBallMiddleX) && (newBallMiddleX <= brickTopRight.x))) {
            collisions.push({
                                object: 'brick',
                                objectReference: brick,
                                newY: brick.y - ball.height,
                                newX: brick.x + arkanoid.ballXVelocity,
                                newXVelocity: arkanoid.ballXVelocity,
                                newYVelocity: -arkanoid.ballYVelocity
                            });
        } else if ((((newBallY + ball.height) <= brickBottomRight.y) && (brickBottomRight.y <= (ball.y + ball.height))) &&
                   ((brickBottomLeft.x <= newBallMiddleX) && (newBallMiddleX <= brickBottomRight.x))) {
            collisions.push({
                                object: 'brick',
                                objectReference: brick,
                                newY: brickBottomLeft.y,
                                newX: brick.x + arkanoid.ballXVelocity,
                                newXVelocity: arkanoid.ballXVelocity,
                                newYVelocity: -arkanoid.ballYVelocity
                            });
        }
    });
    return collisions;
}


function findNearestCollision(collisions) {
    if (0 === collisions.length) {
        return null;
    }
    if (1 === collisions.length) {
        return collisions[0];
    }
    var collision = collisions[0];
    collision.distance = getDistance(ball.x, ball.y, collision.newX, collision.newY);
    for (var i = 1; i < collisions.length; ++i) {
        var collisionDistance = getDistance(ball.x, ball.y, collisions[i].newX, collisions[i].newY);
        if (collisionDistance < collision.distance) {
            collision = collisions[i];
            collision.distance = collisionDistance;
        }
    }
    return collision;
}


function getDistance(x1, y1, x2, y2) {
    return Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));
}
