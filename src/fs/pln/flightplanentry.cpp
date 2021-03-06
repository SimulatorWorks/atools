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

#include "fs/pln/flightplanentry.h"

namespace atools {
namespace fs {
namespace pln {

FlightplanEntry::FlightplanEntry()
{
}

FlightplanEntry::~FlightplanEntry()
{

}

FlightplanEntry::FlightplanEntry(const FlightplanEntry& other)
{
  this->operator=(other);

}

FlightplanEntry& FlightplanEntry::operator=(const FlightplanEntry& other)
{
  waypointType = other.waypointType;
  waypointId = other.waypointId;
  airway = other.airway;
  icaoRegion = other.icaoRegion;
  icaoIdent = other.icaoIdent;
  name = other.name;
  parking = other.parking;
  position = other.position;
  return *this;
}

const QString& FlightplanEntry::getWaypointTypeAsString() const
{
  return waypointTypeToString(waypointType);
}

atools::fs::pln::entry::WaypointType FlightplanEntry::getWaypointType() const
{
  return waypointType;
}

void FlightplanEntry::setWaypointType(const QString& value)
{
  waypointType = stringToWaypointType(value);
}

void FlightplanEntry::setWaypointType(const atools::fs::pln::entry::WaypointType& value)
{
  waypointType = value;
}

const QString& FlightplanEntry::getWaypointId() const
{
  return waypointId;
}

const QString& FlightplanEntry::getAirway() const
{
  return airway;
}

void FlightplanEntry::setAirway(const QString& value)
{
  airway = value;
}

const QString& FlightplanEntry::getIcaoRegion() const
{
  return icaoRegion;
}

const QString& FlightplanEntry::getIcaoIdent() const
{
  return icaoIdent;
}

const atools::geo::Pos& FlightplanEntry::getPosition() const
{
  return position;
}

void FlightplanEntry::setPosition(const atools::geo::Pos& value)
{
  position = value;
}

const QString& FlightplanEntry::waypointTypeToString(entry::WaypointType type)
{
  static const QString airportName("Airport"), unknownName("Unknown"), isecName("Intersection"),
  vorName("VOR"), ndbName("NDB"), userName("User"), emptyName;

  switch(type)
  {
    case atools::fs::pln::entry::AIRPORT:
      return airportName;

    case atools::fs::pln::entry::UNKNOWN:
      return unknownName;

    case atools::fs::pln::entry::INTERSECTION:
      return isecName;

    case atools::fs::pln::entry::VOR:
      return vorName;

    case atools::fs::pln::entry::NDB:
      return ndbName;

    case atools::fs::pln::entry::USER:
      return userName;

  }
  return emptyName;
}

entry::WaypointType FlightplanEntry::stringToWaypointType(const QString& str)
{
  if(str == "Airport")
    return entry::AIRPORT;
  else if(str == "Intersection")
    return entry::INTERSECTION;
  else if(str == "VOR")
    return entry::VOR;
  else if(str == "NDB")
    return entry::NDB;
  else if(str == "User")
    return entry::USER;

  return entry::UNKNOWN;
}

void FlightplanEntry::setIcaoRegion(const QString& value)
{
  icaoRegion = value;
}

void FlightplanEntry::setIcaoIdent(const QString& value)
{
  icaoIdent = value;
}

void FlightplanEntry::setWaypointId(const QString& value)
{
  waypointId = value;
}

} // namespace pln
} // namespace fs
} // namespace atools
