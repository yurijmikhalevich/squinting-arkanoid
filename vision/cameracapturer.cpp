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
#include "cameracapturer.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <QDebug>
#include <QImage>

CameraCapturer::CameraCapturer(QObject *parent)
    : QObject(parent) {
  connect(&timer, SIGNAL(timeout()), this, SLOT(captureFrame()));
}

void CameraCapturer::startCapture(const short &framesPerSecond) {
  if ((framesPerSecond < 1) || (100 < framesPerSecond)) {
    qCritical() << "Capture frame rate must be in range between 1 and 100"
                   " inclusively";
    return;
  }
  if (!camera.open(CV_CAP_ANY)) {
    qCritical() << "No camera detected";
    return;
  }
  camera.set(CV_CAP_PROP_FRAME_WIDTH, 640);
  camera.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
  camera.set(CV_CAP_PROP_FPS, framesPerSecond);
  captureFrame();
  timer.start(1000 / framesPerSecond);
}

void CameraCapturer::stopCapture() {
  timer.stop();
  camera.release();
}

QImage CameraCapturer::convertCvMatToQImage(const cv::Mat &matrix) {
  cv::Mat rgbMatrix;
  cv::cvtColor(matrix, rgbMatrix, CV_BGR2RGB);
  return QImage(rgbMatrix.data, rgbMatrix.cols, rgbMatrix.rows,
                QImage::Format_RGB888);
}

void CameraCapturer::captureFrame() {
  cv::Mat frame;
  if (!camera.read(frame)) {
    return;
  }
  emit this->frame(convertCvMatToQImage(frame));
  emit this->frame(frame);
}
