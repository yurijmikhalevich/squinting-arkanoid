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
  QString newDirection;
  switch(state) {
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

//  lastStates.enqueue(state);
//  if (2 == lastStates.size()) {
//    lastStates.dequeue();
//  }
//  int left = 0;
//  int right = 0;
//  int none = 0;
//  for (State s : lastStates) {
//      switch (s) {
//      case State::LeftEye:
//        left += 1;
//        break;
//      case State::RightEye:
//        right += 1;
//        break;
//      case State::BothEyes:
//        left += 1;
//        right += 1;
//        break;
//      case State::NoEye:
//      default:
//        none += 1;
//        break;
//      }
//  }
//  QString newDirection = "none";
//  if (left > right) {
//    newDirection = "left";
//  } else if (right > left) {
//    newDirection = "right";
//  }
//  arkanoid->setProperty("stickMovingDirection", newDirection);
}
