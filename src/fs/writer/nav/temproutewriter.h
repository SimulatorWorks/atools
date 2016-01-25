/*
 * TempRouteWriterWriter.h
 *
 *  Created on: 01.05.2015
 *      Author: alex
 */

#ifndef WRITER_TEMPROUTEWRITER_H_
#define WRITER_TEMPROUTEWRITER_H_

#include "fs/writer/writerbase.h"
#include "fs/bgl/nav/routeentry.h"

namespace atools {
namespace fs {
namespace writer {

class TempRouteWriter :
  public atools::fs::writer::WriterBase<atools::fs::bgl::RouteEntry>
{
public:
  TempRouteWriter(atools::sql::SqlDatabase& db, atools::fs::writer::DataWriter& dataWriter)
    : WriterBase(db, dataWriter, "temp_route")
  {
  }

  virtual ~TempRouteWriter()
  {
  }

protected:
  virtual void writeObject(const atools::fs::bgl::RouteEntry *type);

};

} // namespace writer
} // namespace fs
} // namespace atools

#endif /* WRITER_TEMPROUTEWRITER_H_ */