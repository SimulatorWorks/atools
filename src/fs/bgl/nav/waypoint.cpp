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

#include "fs/bgl/nav/waypoint.h"
#include "fs/bgl/converter.h"
#include "fs/bgl/nav/airwaysegment.h"
#include "io/binarystream.h"
#include "fs/navdatabaseoptions.h"

namespace atools {
namespace fs {
namespace bgl {

using atools::io::BinaryStream;

QString Waypoint::waypointTypeToStr(nav::WaypointType type)
{
  switch(type)
  {
    case nav::NAMED:
      return "NAMED";

    case nav::UNNAMED:
      return "UNNAMED";

    case nav::VOR:
      return "VOR";

    case nav::NDB:
      return "NDB";

    case nav::OFF_AIRWAY:
      return "OFF_AIRWAY";

    case nav::IAF:
      return "IAF";

    case nav::FAF:
      return "FAF";
  }
  qWarning().nospace().noquote() << "Unknown waypoint type " << type;
  return QString();
}

Waypoint::Waypoint(const NavDatabaseOptions *options, BinaryStream *bs)
  : Record(options, bs)
{
  type = static_cast<nav::WaypointType>(bs->readUByte());
  int numAirways = bs->readUByte();
  position = BglPosition(bs);
  magVar = converter::adjustMagvar(bs->readFloat());
  unsigned int identInt = bs->readUInt();
  ident = converter::intToIcao(identInt);

  unsigned int regionFlags = bs->readUInt();
  region = converter::intToIcao(regionFlags & 0x7ff, true);
  airportIdent = converter::intToIcao((regionFlags >> 11) & 0x1fffff, true);

  if(region.isEmpty())
    qWarning().nospace().noquote() << "Waypoint at " << position << " ident " << ident << " has no region";

  if(ident.isEmpty() && type != nav::UNNAMED)
    qWarning().nospace().noquote() << "Waypoint at " << position << " region " << region << " has no ident";

  if(options->isIncludedBglObject(type::AIRWAY))
  {
    // Read airways if desired by configuration
    for(int i = 0; i < numAirways; i++)
      airways.append(AirwaySegment(options, bs));
  }

  for(const AirwaySegment& re : airways)
  {
    if(re.getAirwayType() == atools::fs::bgl::nav::BOTH)
    {
      numVictorAirway++;
      numJetAirway++;
    }
    else if(re.getAirwayType() == atools::fs::bgl::nav::VICTOR)
      numVictorAirway++;
    else if(re.getAirwayType() == atools::fs::bgl::nav::JET)
      numJetAirway++;
  }
}

Waypoint::~Waypoint()
{
}

QDebug operator<<(QDebug out, const Waypoint& record)
{
  QDebugStateSaver saver(out);

  out.nospace().noquote() << static_cast<const Record&>(record)
  << " Waypoint[type " << Waypoint::waypointTypeToStr(record.type)
  << ", " << record.position
  << ", magVar " << record.magVar
  << ", ident " << record.ident
  << ", region " << record.region
  << ", airport ID " << record.airportIdent << endl;
  out << record.airways;
  out << "]";
  return out;
}

} // namespace bgl
} // namespace fs
} // namespace atools
