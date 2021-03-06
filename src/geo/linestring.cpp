/*****************************************************************************
* Copyright 2015-2016 Alexander Barthel albar965@mailbox.org
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
*****************************************************************************/

#include "linestring.h"

namespace atools {
namespace geo {

LineString::LineString()
{

}

LineString::LineString(std::initializer_list<Pos> list)
  : QList(list)
{
}

void LineString::append(float longitudeX, float latitudeY, float alt)
{
  QList::append(Pos(longitudeX, latitudeY, alt));
}

void LineString::append(double longitudeX, double latitudeY, double alt)
{
  QList::append(Pos(longitudeX, latitudeY, alt));
}

Rect LineString::boundingRect()
{
  if(!isEmpty())
  {
    Rect bounding(first());

    for(const Pos& p : *this)
      bounding.extend(p);
    return bounding;
  }
  else
    return Rect();
}

float LineString::lengthMeter() const
{
  float length = 0.f;
  for(int i = 0; i < size() - 1; i++)
    length += at(i).distanceMeterTo(at(i + 1));
  return length;
}

} // namespace geo
} // namespace atools
