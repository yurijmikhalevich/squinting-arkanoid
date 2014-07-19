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
#ifndef CAMERAIMAGEPROVIDER_H
#define CAMERAIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QImage>

class CameraImageProvider : public QObject, public QQuickImageProvider {
  Q_OBJECT
public:
  explicit CameraImageProvider(QObject *parent = 0);
  QImage requestImage(const QString &id, QSize *size,
                      const QSize &requestedSize);
public slots:
  void setImage(const QImage &image);
private:
  QImage image;
};

#endif // CAMERAIMAGEPROVIDER_H
