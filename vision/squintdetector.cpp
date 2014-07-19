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
    bool leftEyeClosed = false;
    if (leftEyeFound) {
      leftEyes[0].x += leftEye.x;
      leftEyes[0].y += leftEye.y;
      leftEye = leftEyes[0];
      leftEyeClosed = isEyeClosed(frame(leftEye), "left");
    }
    bool rightEyeFound = (1 == rightEyes.size());
    bool rightEyeClosed = false;
    if (rightEyeFound) {
      rightEyes[0].x += rightEye.x;
      rightEyes[0].y += rightEye.y;
      rightEye = rightEyes[0];
      rightEyeClosed = isEyeClosed(frame(rightEye), "right");
    }
//    qDebug() << "right" << rightEyeFound << rightEyeClosed;
//    qDebug() << "left" << leftEyeFound << leftEyeClosed;
    if (leftEyeClosed && rightEyeClosed) {
      emit bothEyes();
    } else if (leftEyeClosed) {
      emit this->leftEye();
    } else if (rightEyeClosed) {
      emit this->rightEye();
    } else {
      emit noEye();
    }
  }
}


bool SquintDetector::isEyeClosed(cv::Mat eye, QString debugWindowPrefix) {
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
  cv::equalizeHist(gray, gray);
  cv::threshold(gray, gray, 220, 255, cv::THRESH_BINARY);
  cv::GaussianBlur(gray, gray, cv::Size(5, 5), 2, 2);
//  cv::imshow(debugWindowPrefix.append("thresholded").toStdString(), gray);

  std::vector<cv::Vec3f> circles;
  cv::HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 3, 60, 30, 25,
                   gray.rows / 2.7, gray.rows);

//  for( size_t i = 0; i < circles.size(); i++ )
//  {
//      cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
//      int radius = cvRound(circles[i][2]);
//      cv::circle(eye, center, radius, cv::Scalar(0, 0, 255), 1);
//  }

//  cv::imshow(debugWindowPrefix.append("eye").toStdString(), eye);

//  qDebug() << circles.size();

  if (0 == circles.size()) {
    return true;
  }
  return false;
}
