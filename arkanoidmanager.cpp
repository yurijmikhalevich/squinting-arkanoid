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
#include "arkanoidmanager.h"
#include <QVariant>

ArkanoidManager::ArkanoidManager(QObject *arkanoid, QObject *parent)
    : QObject(parent) {
  this->arkanoid = arkanoid;
  currentState = State::Undefined;
  lastState = State::Undefined;
  lastStateCount = 0;
}

void ArkanoidManager::leftEye() {
  processNewState(State::LeftEye);
}

void ArkanoidManager::rightEye() {
  processNewState(State::RightEye);
}

void ArkanoidManager::bothEyes() {
  processNewState(State::BothEyes);
}

void ArkanoidManager::noEye() {
  processNewState(State::NoEye);
}

void ArkanoidManager::processNewState(const State &state) {
  if (state == currentState) {
    lastState = state;
    return;
  }
  if (state == lastState) {
    lastStateCount += 1;
    if (2 == lastStateCount) {
      currentState = state;
      QString newDirection;
      switch (state) {
      case State::LeftEye:
        newDirection = "left";
        break;
      case State::RightEye:
        newDirection = "right";
        break;
      case State::BothEyes:
      case State::NoEye:
      default:
        newDirection = "none";
        break;
      }
      arkanoid->setProperty("stickMovingDirection", newDirection);
    }
  } else {
    lastStateCount = 1;
  }
  lastState = state;
}
