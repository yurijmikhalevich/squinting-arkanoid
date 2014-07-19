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
#include "squintdetector.h"
#include <QDebug>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>

SquintDetector::SquintDetector(QObject *parent)
    : QObject(parent) {
  faceCascade.load("haarcascades/haarcascade_frontalface_alt.xml");
  leftEyeCascade.load("haarcascades/haarcascade_mcs_lefteye.xml");
  rightEyeCascade.load("haarcascades/haarcascade_mcs_righteye.xml");
}

void SquintDetector::updateFrame(cv::Mat frame) {
  std::vector<cv::Rect> faces;
  cv::Mat frameGray;
  cv::cvtColor(frame, frameGray, CV_BGR2GRAY);
  cv::equalizeHist(frameGray, frameGray);
  faceCascade.detectMultiScale(frameGray, faces, 1.2, 3,
                               0|CV_HAAR_SCALE_IMAGE, cv::Size(40, 60));
  if (faces.size() > 0) {
    cv::Rect face = faces[0];
    cv::Rect leftEye = face;
    cv::Rect rightEye = face;
    leftEye.height *= 0.65;
    rightEye.height = leftEye.height;
    rightEye.width *= 0.65;
    leftEye.x += leftEye.width - rightEye.width;
    leftEye.width = rightEye.width;
    cv::Mat leftEyeROI = frameGray(leftEye);
    std::vector<cv::Rect> leftEyes;
    leftEyeCascade.detectMultiScale(leftEyeROI, leftEyes, 1.1, 2,
                                    0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
    cv::Mat rightEyeROI = frameGray(rightEye);
    std::vector<cv::Rect> rightEyes;
    rightEyeCascade.detectMultiScale(rightEyeROI, rightEyes, 1.1, 2,
                                     0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
    bool leftEyeFound = (1 == leftEyes.size());
    bool leftEyeClosed;
    if (leftEyeFound) {
      leftEyes[0].x += leftEye.x;
      leftEyes[0].y += leftEye.y;
      leftEye = leftEyes[0];
      leftEyeClosed = isEyeClosed(frame(leftEye), "left");
    }
    bool rightEyeFound = (1 == rightEyes.size());
    bool rightEyeClosed;
    if (rightEyeFound) {
      rightEyes[0].x += rightEye.x;
      rightEyes[0].y += rightEye.y;
      rightEye = rightEyes[0];
//      rightEyeClosed = isEyeClosed(frame(rightEye), "right");
    }
//    qDebug() << "right" << rightEyeFound << rightEyeClosed;
    qDebug() << "left" << leftEyeFound << leftEyeClosed;
    if (leftEyeFound || rightEyeFound) {
      if (leftEyeClosed && rightEyeClosed) {
        emit bothEyes();
      } else if (leftEyeClosed) {
        emit this->leftEye();
      } else if (rightEyeClosed) {
        emit this->rightEye();
      } else {
        emit noEye();
      }
    } else {
      emit noEye();
    }
  }
}

bool SquintDetector::isEyeClosed(cv::Mat eye,
                                 QString debugWindowPrefix) {
  Q_UNUSED(debugWindowPrefix)
  cv::Rect moreExactEyeArea;
  moreExactEyeArea.height = eye.size().height;
  moreExactEyeArea.width = eye.size().width;
  moreExactEyeArea.height /= 2;
  moreExactEyeArea.y += moreExactEyeArea.height / 2;
  eye = eye(moreExactEyeArea);
//  cv::imshow(debugWindowPrefix.append("eye").toStdString(), eye);
  cv::Mat gray;
  cv::cvtColor(~eye, gray, CV_BGR2GRAY);
  cv::imshow(debugWindowPrefix.append("gray").toStdString(), gray);
  cv::threshold(gray, gray, 220, 255, cv::THRESH_BINARY);
  cv::imshow(debugWindowPrefix.append("thresholded").toStdString(), gray);
  int whitePixels = 0;
  int columnsWithWhitePixelCount = 0;
  int columnsCount = gray.size().width;
  for (int i = 0; i < columnsCount; ++i) {
    cv::Mat column = gray.col(i);
    bool currentOwnWhitePixel = false;
    for (cv::MatIterator_<int> it = column.begin<int>();
         it != column.end<int>(); ++it) {
      if (*it > 0) {
        ++whitePixels;
        currentOwnWhitePixel = true;
      }
    }
    if (currentOwnWhitePixel) {
      ++columnsWithWhitePixelCount;
    }
  }
  qDebug() << (double)whitePixels / (double)columnsWithWhitePixelCount;
  if (((double)whitePixels / (double)columnsWithWhitePixelCount) < 2) {
    return true;
  }
  return false;
}

