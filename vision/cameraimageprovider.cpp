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
#include "cameraimageprovider.h"

CameraImageProvider::CameraImageProvider(QObject *parent)
    : QObject(parent),
      QQuickImageProvider(QQuickImageProvider::Image) {
  image = QImage(1, 1, QImage::Format_RGB888);
  image.fill(0);
}

QImage CameraImageProvider::requestImage(const QString &id, QSize *size,
                                         const QSize &requestedSize) {
  Q_UNUSED(id)
  *size = image.size();
  if (requestedSize.width() > 0 && requestedSize.height() > 0) {
    return image.scaled(requestedSize);
  } else {
    return image;
  }
}

void CameraImageProvider::setImage(const QImage &image) {
  this->image = image;
}
