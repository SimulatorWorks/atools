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

#include "sql/sqlexception.h"
#include "sql/sqlrecord.h"

#include <QSqlField>
#include <QVariant>

namespace atools {
namespace sql {

SqlRecord::SqlRecord()
{

}

SqlRecord::SqlRecord(const QSqlRecord& other, const QString& query)
{
  sqlRecord = other;
  queryString = query;
}

SqlRecord::SqlRecord(const SqlRecord& other)
{
  sqlRecord = other.sqlRecord;
  queryString = other.queryString;

}

SqlRecord& SqlRecord::operator=(const SqlRecord& other)
{
  sqlRecord = other.sqlRecord;
  queryString = other.queryString;
  return *this;
}

SqlRecord::~SqlRecord()
{

}

bool SqlRecord::operator==(const SqlRecord& other) const
{
  return sqlRecord == other.sqlRecord;
}

QVariant SqlRecord::value(int i) const
{
  QVariant retval = sqlRecord.value(i);
  if(!retval.isValid())
    throw SqlException("SqlRecord::value(): Value index " + QString::number(
                         i) + " does not exist in query \"" + queryString + "\"");
  return retval;
}

QVariant SqlRecord::value(const QString& name) const
{
  QVariant retval = sqlRecord.value(name);
  if(!retval.isValid())
    throw SqlException("SqlRecord::value(): Value name \"" +
                       name + "\" does not exist in query \"" + queryString + "\"");
  return retval;

}

bool SqlRecord::isNull(int i) const
{
  if(sqlRecord.fieldName(i).isEmpty())
    throw SqlException("SqlRecord::isNull(): Field index " + QString::number(i) +
                       " does not exist in query \"" + queryString + "\"");

  return sqlRecord.isNull(i);
}

bool SqlRecord::isNull(const QString& name) const
{
  if(sqlRecord.indexOf(name) == -1)
    throw SqlException("SqlRecord::indexOf(): Field name \"" +
                       name + "\" does not exist in query \"" + queryString + "\"");
  return sqlRecord.isNull(name);
}

int SqlRecord::indexOf(const QString& name) const
{
  int retval = sqlRecord.indexOf(name);
  if(retval == -1)
    throw SqlException("SqlRecord::indexOf(): Field name \"" +
                       name + "\" does not exist in query \"" + queryString + "\"");
  return retval;
}

QString SqlRecord::fieldName(int i) const
{
  QString retval = sqlRecord.fieldName(i);
  if(retval.isEmpty())
    throw SqlException("SqlRecord::fieldName(): Field index " + QString::number(i) +
                       " does not exist in query \"" + queryString + "\"");
  return retval;
}

QVariant::Type SqlRecord::fieldType(int i) const
{
  QSqlField retval = sqlRecord.field(i);
  if(sqlRecord.fieldName(i).isEmpty())
    throw SqlException("SqlRecord::fieldType(): Field index " + QString::number(i) +
                       " does not exist in query \"" + queryString + "\"");
  return retval.type();
}

QVariant::Type SqlRecord::fieldType(const QString& name) const
{
  QSqlField retval = sqlRecord.field(name);
  if(!contains(name))
    throw SqlException("SqlRecord::fieldType(): Field name \"" +
                       name + "\" does not exist in query \"" + queryString + "\"");

  return retval.type();
}

bool SqlRecord::isGenerated(int i) const
{
  if(sqlRecord.fieldName(i).isEmpty())
    throw SqlException("SqlRecord::isGenerated(): Field index " + QString::number(i) +
                       " does not exist in query \"" + queryString + "\"");

  return sqlRecord.isGenerated(i);
}

bool SqlRecord::isGenerated(const QString& name) const
{
  if(!contains(name))
    throw SqlException("SqlRecord::isGenerated(): Field name \"" +
                       name + "\" does not exist in query \"" + queryString + "\"");
  return sqlRecord.isGenerated(name);
}

bool SqlRecord::isEmpty() const
{
  return sqlRecord.isEmpty();
}

bool SqlRecord::contains(const QString& name) const
{
  return sqlRecord.contains(name);
}

int SqlRecord::count() const
{
  return sqlRecord.count();
}

void SqlRecord::appendField(const QString& fieldName, QVariant::Type type)
{
  sqlRecord.append(QSqlField(fieldName, type));
}

void SqlRecord::setValue(int i, const QVariant& val)
{
  if(sqlRecord.fieldName(i).isEmpty())
    throw SqlException("SqlRecord::setValue(): Field index " + QString::number(i) +
                       " does not exist in query \"" + queryString + "\"");
  sqlRecord.setValue(i, val);
}

void SqlRecord::setValue(const QString& name, const QVariant& val)
{
  if(sqlRecord.indexOf(name) == -1)
    throw SqlException("SqlRecord::setValue(): Field name \"" +
                       name + "\" does not exist in query \"" + queryString + "\"");
  sqlRecord.setValue(name, val);
}

void SqlRecord::setNull(int i)
{
  if(sqlRecord.fieldName(i).isEmpty())
    throw SqlException("SqlRecord::setNull(): Field index " + QString::number(i) +
                       " does not exist in query \"" + queryString + "\"");
  sqlRecord.setNull(i);
}

void SqlRecord::setNull(const QString& name)
{
  if(sqlRecord.indexOf(name) == -1)
    throw SqlException("SqlRecord::setNull(): Field name \"" +
                       name + "\" does not exist in query \"" + queryString + "\"");
  sqlRecord.setNull(name);
}

void SqlRecord::clear()
{
  sqlRecord.clear();
}

void SqlRecord::clearValues()
{
  sqlRecord.clearValues();
}

} // namespace sql
} // namespace atools
