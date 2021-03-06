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

#ifndef ATOOLS_UTIL_VERSION_H
#define ATOOLS_UTIL_VERSION_H

#include <QString>

namespace atools {
namespace util {

/*
 * Parses version numbers like "0.9.5.develop" (major.minor.patchlevel[.name]) into their parts.
 */
class Version
{
public:
  Version(int verMajor, int verMinor, int verPatchlevel, const QString& verName = QString());
  Version(const QString& str);
  ~Version();

  int getMajor() const
  {
    return majorVersion;
  }

  int getMinor() const
  {
    return minorVersion;
  }

  int getPatchlevel() const
  {
    return patchlevelVersion;
  }

  const QString& getName() const
  {
    return name;
  }

  const QString& getVersionString() const
  {
    return versionString;
  }

  /* Compare major, minor and patchlevel */
  bool operator<(const Version& other) const;

  bool operator>(const Version& other) const
  {
    return other < *this;
  }

  bool operator<=(const Version& other) const
  {
    return !(*this > other);
  }

  bool operator>=(const Version& other) const
  {
    return !(*this < other);
  }

  bool operator==(const Version& other) const
  {
    return versionString == other.versionString;
  }

  bool operator!=(const Version& other) const
  {
    return !(*this == other);
  }

  bool isValid() const;

private:
  friend QDebug operator<<(QDebug out, const atools::util::Version& version);

  int majorVersion = -1, minorVersion = -1, patchlevelVersion = -1;
  QString name, versionString;

};

} // namespace util
} // namespace atools

#endif // ATOOLS_UTIL_VERSION_H
