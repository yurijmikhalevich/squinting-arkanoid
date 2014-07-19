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
#ifndef CAMERACAPTURER_H
#define CAMERACAPTURER_H

#include <QObject>
#include <QTimer>
#include <QPixmap>
#include <opencv2/highgui/highgui.hpp>

class CameraCapturer : public QObject {
  Q_OBJECT
public:
  explicit CameraCapturer(QObject *parent = 0);
  void startCapture(const short &framesPerSecond = 24);
  void stopCapture();
signals:
  void frame(QImage frame);
  void frame(cv::Mat frame);
private:
  QImage convertCvMatToQImage(const cv::Mat &matrix);
  QTimer timer;
  cv::VideoCapture camera;
private slots:
  void captureFrame();
};

#endif // CAMERACAPTURER_H
