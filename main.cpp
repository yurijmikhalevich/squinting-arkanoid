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
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include "vision/cameracapturer.h"
#include "vision/cameraimageprovider.h"
#include "vision/squintdetector.h"
#include "arkanoidmanager.h"

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;

  QThread *cameraThread = new QThread(&app);
  cameraThread->start();
  CameraCapturer *capturer = new CameraCapturer();
  CameraImageProvider *imageProvider = new CameraImageProvider();
  capturer->moveToThread(cameraThread);
  imageProvider->moveToThread(cameraThread);
  QObject::connect(capturer, static_cast<void (CameraCapturer::*)(QImage)>(
                     &CameraCapturer::frame),
                   imageProvider, &CameraImageProvider::setImage);
  engine.addImageProvider("camera", imageProvider);

  QThread *imageProcessingThread = new QThread(&app);
  imageProcessingThread->start();
  SquintDetector *squintDetector = new SquintDetector();
  squintDetector->moveToThread(imageProcessingThread);
  qRegisterMetaType<cv::Mat>("cv::Mat");
  QObject::connect(capturer, static_cast<void (CameraCapturer::*)(cv::Mat)>(
                     &CameraCapturer::frame),
                   squintDetector, &SquintDetector::updateFrame);

  capturer->startCapture(24);

  engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

  QObject *mainWindow = engine.rootObjects()[0];
  QObject *arkanoid = mainWindow->children()[0];
  ArkanoidManager *aManager = new ArkanoidManager(arkanoid);

  QObject::connect(squintDetector, &SquintDetector::leftEye,
                   aManager, &ArkanoidManager::leftEye);
  QObject::connect(squintDetector, &SquintDetector::rightEye,
                   aManager, &ArkanoidManager::rightEye);
  QObject::connect(squintDetector, &SquintDetector::noEye,
                   aManager, &ArkanoidManager::noEye);
  QObject::connect(squintDetector, &SquintDetector::bothEyes,
                   aManager, &ArkanoidManager::bothEyes);

  QObject::connect(&app, &QGuiApplication::aboutToQuit, [=]() {
//    capturer->stopCapture();
//    delete capturer;
//    delete imageProvider;
//    cameraThread->exit();
//    delete squintDetector;
//    imageProcessingThread->exit();
    qDebug() << "bye";
  });

  return app.exec();
}
