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

#ifndef ATOOLS_GUI_APPLICATION_H
#define ATOOLS_GUI_APPLICATION_H

#include <QApplication>

/* Use this macro to report an fatal error after an exception including location information */
#define ATOOLS_HANDLE_EXCEPTION(e) (atools::gui::Application::handleException(__FILE__, __LINE__, e))

/* Use this macro to report an fatal error after an unknownexception including location information */
#define ATOOLS_HANDLE_UNKNOWN_EXCEPTION (atools::gui::Application::handleException(__FILE__, __LINE__))

namespace atools {
namespace gui {

/*
 * Adds an exception handler for the event queue and provides methods to handle fatal errors.
 * Catches any exceptions that was thrown in an event handler and shows an error dialog
 * also containing application paths etc.
 */
class Application :
  public QApplication
{
  Q_OBJECT

public:
  Application(int& argc, char **argv, int = ApplicationFlags);
  virtual ~Application();

#if defined(Q_CC_MSVC)
  // MSVC cannot deal with newer C++ features
  static void handleException(const char *file, int line, const std::exception& e);
  static void handleException(const char *file, int line);

#else
  /*
   * Shows an error dialog with the exception message and after that exits the application with code 1.
   */
  [[noreturn]] static void handleException(const char *file, int line, const std::exception& e);

  /*
   * Shows an error dialog and after that exits the application with code 1.
   */
  [[noreturn]] static void handleException(const char *file, int line);

#endif

  /* Add a list of paths that will be added as links in any error dialog.
   * @param header Header for the list of paths
   * @param paths A list of directory or file paths
   */
  static void addReportPath(const QString& header, const QStringList& paths);

  /*
   * Get the paths that were added using addReportPath in a HTML formatted text
   * as links
   */
  static QString getReportPaths();

private:
  virtual bool notify(QObject *receiver, QEvent *event) override;

  static QHash<QString, QStringList> reportFiles;

};

} // namespace gui
} // namespace atools

#endif // ATOOLS_GUI_APPLICATION_H
