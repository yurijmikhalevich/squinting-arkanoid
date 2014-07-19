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
#ifndef SQUINTDETECTOR_H
#define SQUINTDETECTOR_H

#include <QObject>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>

class SquintDetector : public QObject {
  Q_OBJECT
public:
  explicit SquintDetector(QObject *parent = 0);
signals:
  void leftEye();
  void rightEye();
  void bothEyes();
  void noEye();
public slots:
  void updateFrame(cv::Mat frame);
private:
  bool isEyeClosed(cv::Mat eye, QString debugWindowPrefix = "");
  cv::CascadeClassifier faceCascade;
  cv::CascadeClassifier leftEyeCascade;
  cv::CascadeClassifier rightEyeCascade;
};

#endif // SQUINTDETECTOR_H
