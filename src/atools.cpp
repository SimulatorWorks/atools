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

#include "atools.h"

#include <QDebug>
#include <QLocale>
#include <QVector>

namespace atools {

QString version()
{
  return "1.9.8.develop";
}

QString gitRevision()
{
  return GIT_REVISION_ATOOLS;
}

void capWord(QString& lastWord, QChar last, const QSet<QString>& toUpper,
             const QSet<QString>& toLower, const QSet<QString>& ignore)
{
  static QLocale locale;
  if(toUpper.contains(lastWord))
    lastWord = locale.toUpper(lastWord);
  else if(toLower.contains(lastWord))
    lastWord = locale.toLower(lastWord);
  else if(!ignore.contains(lastWord))
  {
    // Convert all letters after an apostrophe to lower case (St. Mary's)
    if(last == '\'' && lastWord.size() == 1)
      lastWord[0] = lastWord.at(0).toLower();
    else
    {
      lastWord = lastWord.toLower();
      lastWord[0] = lastWord.at(0).toUpper();
    }
  }
}

QString capString(const QString& str, const QSet<QString>& toUpper, const QSet<QString>& toLower,
                  const QSet<QString>& ignore)
{
  if(str.isEmpty())
    return str;

  QString retval, lastWord;
  QChar last, lastSep;
  for(QChar c : str)
  {
    if(!c.isLetterOrNumber())
    {
      if(last.isLetterOrNumber())
      {
        capWord(lastWord, lastSep, toUpper, toLower, ignore);
        retval += lastWord;
        lastWord.clear();
      }
      retval += c;
      lastSep = c;
    }
    else
      lastWord += c;

    last = c;
  }
  if(!lastWord.isEmpty())
  {
    QChar lastC = str.at(str.size() >= 2 ? str.size() - 2 : 0);
    capWord(lastWord, lastC, toUpper, toLower, ignore);
    retval += lastWord;
  }

  return retval.replace('_', ' ');
}

QString ratingString(int value, int maxValue)
{
  return QString("★").repeated(value) + QString("−").repeated(maxValue - value);
}

} // namespace atools
