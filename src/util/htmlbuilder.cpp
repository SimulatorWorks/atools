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

#include "util/htmlbuilder.h"

#include <QDebug>
#include <QApplication>
#include <QPalette>
#include <QDateTime>
#include <QBuffer>
#include <QIcon>

namespace atools {
namespace util {

HtmlBuilder::HtmlBuilder(bool hasBackColor)
{
  if(hasBackColor)
  {
    // Create darker colors dynamically from default palette
    rowBackColor = QApplication::palette().color(QPalette::Active, QPalette::Base).
                   darker(105).name(QColor::HexRgb);
    rowBackColorAlt = QApplication::palette().color(QPalette::Active, QPalette::AlternateBase).
                      darker(105).name(QColor::HexRgb);

    tableRow.append("<tr bgcolor=\"" + rowBackColor + "\"><td>%1</td><td>%2</td></tr>");
    tableRow.append("<tr bgcolor=\"" + rowBackColorAlt + "\"><td>%1</td><td>%2</td></tr>");

    tableRowAlignRight.append(
      "<tr bgcolor=\"" + rowBackColor + "\"><td>%1</td><td align=\"right\">%2</td></tr>");
    tableRowAlignRight.append(
      "<tr bgcolor=\"" + rowBackColorAlt + "\"><td>%1</td><td align=\"right\">%2</td></tr>");
  }
  else
  {
    tableRow.append("<tr><td>%1</td><td>%2</td></tr>");
    tableRow.append("<tr><td>%1</td><td>%2</td></tr>");

    tableRowAlignRight.append("<tr><td>%1</td><td align=\"right\">%2</td></tr>");
    tableRowAlignRight.append("<tr><td>%1</td><td align=\"right\">%2</td></tr>");
  }

  tableRowBegin.append("<tr bgcolor=\"" + rowBackColor + "\">");
  tableRowBegin.append("<tr bgcolor=\"" + rowBackColorAlt + "\">");

  tableRowHeader = "<tr><td>%1</td></tr>";

}

HtmlBuilder::~HtmlBuilder()
{

}

HtmlBuilder& HtmlBuilder::row2Var(const QString& name, const QVariant& value, html::Flags flags,
                                  QColor color)
{
  QString valueStr;
  switch(value.type())
  {
    case QVariant::Invalid:
      valueStr = "Error: Invalid Variant";
      qWarning() << "Invalid Variant in HtmlBuilder. Name" << name;
      break;
    case QVariant::Bool:
      valueStr = value.toBool() ? tr("Yes") : tr("No");
      break;
    case QVariant::Int:
      valueStr = locale.toString(value.toInt());
      break;
    case QVariant::UInt:
      valueStr = locale.toString(value.toUInt());
      break;
    case QVariant::LongLong:
      valueStr = locale.toString(value.toLongLong());
      break;
    case QVariant::ULongLong:
      valueStr = locale.toString(value.toULongLong());
      break;
    case QVariant::Double:
      valueStr = locale.toString(value.toDouble(), 'f', defaultPrecision);
      break;
    case QVariant::Char:
    case QVariant::String:
      valueStr = value.toString();
      break;
    case QVariant::StringList:
      valueStr = value.toStringList().join(", ");
      break;
    case QVariant::Date:
      valueStr = locale.toString(value.toDate(), dateFormat);
      break;
    case QVariant::Time:
      valueStr = locale.toString(value.toTime(), dateFormat);
      break;
    case QVariant::DateTime:
      valueStr = locale.toString(value.toDateTime(), dateFormat);
      break;
    default:
      qWarning() << "Invalid variant type" << value.typeName() << "in HtmlBuilder. Name" << name;
      valueStr = QString("Error: Invalid variant type \"%1\"").arg(value.typeName());

  }
  htmlText += alt(flags & html::ALIGN_RIGHT ? tableRowAlignRight : tableRow).
              arg(asText(name, flags, color), value.toString());
  tableIndex++;
  numLines++;
  return *this;
}

HtmlBuilder& HtmlBuilder::row2(const QString& name, const QString& value, html::Flags flags, QColor color)
{
  htmlText += alt(flags & html::ALIGN_RIGHT ? tableRowAlignRight : tableRow).
              arg(asText(name, flags, color), value);
  tableIndex++;
  numLines++;
  return *this;
}

HtmlBuilder& HtmlBuilder::row2(const QString& name, float value, int precision, html::Flags flags,
                               QColor color)
{
  htmlText += alt(flags & html::ALIGN_RIGHT ? tableRowAlignRight : tableRow).
              arg(asText(name, flags, color),
                  locale.toString(value, 'f', precision != -1 ? precision : defaultPrecision));
  tableIndex++;
  numLines++;
  return *this;
}

HtmlBuilder& HtmlBuilder::row2(const QString& name, double value, int precision, html::Flags flags,
                               QColor color)
{
  htmlText += alt(flags & html::ALIGN_RIGHT ? tableRowAlignRight : tableRow).
              arg(asText(name, flags, color),
                  locale.toString(value, 'f', precision != -1 ? precision : defaultPrecision));
  tableIndex++;
  numLines++;
  return *this;
}

HtmlBuilder& HtmlBuilder::row2(const QString& name, int value, html::Flags flags, QColor color)
{
  htmlText += alt(flags & html::ALIGN_RIGHT ? tableRowAlignRight : tableRow).
              arg(asText(name, flags, color), locale.toString(value));
  tableIndex++;
  numLines++;
  return *this;
}

HtmlBuilder& HtmlBuilder::td(const QString& str, html::Flags flags, QColor color)
{
  htmlText += QString("<td") + (flags & html::ALIGN_RIGHT ? " align=\"right\"" : "") + ">";
  text(str, flags, color);
  htmlText += "</td>";
  return *this;
}

HtmlBuilder& HtmlBuilder::tr(QColor backgroundColor)
{
  if(backgroundColor.isValid())
    htmlText += "<tr bgcolor=\"" + backgroundColor.name(QColor::HexRgb) + "\">";
  else
    htmlText += alt(tableRowBegin);
  tableIndex++;
  numLines++;
  return *this;
}

HtmlBuilder& HtmlBuilder::trEnd()
{
  htmlText += "</tr>";
  return *this;
}

HtmlBuilder& HtmlBuilder::table()
{
  htmlText += "<table border=\"0\" cellpadding=\"2\" cellspacing=\"0\"><tbody>";
  tableIndex = 0;
  return *this;
}

HtmlBuilder& HtmlBuilder::tableEnd()
{
  htmlText += "</tbody></table>";
  tableIndex = 0;
  return *this;
}

HtmlBuilder& HtmlBuilder::h(int level, const QString& str, html::Flags flags, QColor color,
                            const QString& id)
{
  QString num = QString::number(level);
  htmlText += "<h" + num + (id.isEmpty() ? QString() : " id=\"" + id + "\"") + ">" +
              asText(str, flags, color) + "</h" + num + ">";
  tableIndex = 0;
  numLines++;
  return *this;
}

HtmlBuilder& HtmlBuilder::h1(const QString& str, html::Flags flags, QColor color, const QString& id)
{
  h(1, str, flags, color, id);
  return *this;
}

HtmlBuilder& HtmlBuilder::h2(const QString& str, html::Flags flags, QColor color, const QString& id)
{
  h(2, str, flags, color, id);
  return *this;
}

HtmlBuilder& HtmlBuilder::h3(const QString& str, html::Flags flags, QColor color, const QString& id)
{
  h(3, str, flags, color, id);
  return *this;
}

HtmlBuilder& HtmlBuilder::h4(const QString& str, html::Flags flags, QColor color, const QString& id)
{
  h(4, str, flags, color, id);
  return *this;
}

HtmlBuilder& HtmlBuilder::h5(const QString& str, html::Flags flags, QColor color, const QString& id)
{
  h(4, str, flags, color, id);
  return *this;
}

HtmlBuilder& HtmlBuilder::b(const QString& str)
{
  text(str, html::BOLD);
  return *this;
}

HtmlBuilder& HtmlBuilder::nbsp()
{
  htmlText += "&nbsp;";
  return *this;
}

HtmlBuilder& HtmlBuilder::u(const QString& str)
{
  text(str, html::UNDERLINE);
  return *this;
}

HtmlBuilder& HtmlBuilder::sub(const QString& str)
{
  text(str, html::SUBSCRIPT);
  return *this;
}

HtmlBuilder& HtmlBuilder::sup(const QString& str)
{
  text(str, html::SUPERSCRIPT);
  return *this;
}

HtmlBuilder& HtmlBuilder::small(const QString& str)
{
  text(str, html::SMALL);
  return *this;
}

HtmlBuilder& HtmlBuilder::big(const QString& str)
{
  text(str, html::BIG);
  return *this;
}

HtmlBuilder& HtmlBuilder::br()
{
  htmlText += "<br/>";
  numLines++;
  return *this;
}

HtmlBuilder& HtmlBuilder::p()
{
  htmlText += "<p>";
  numLines++;
  return *this;
}

HtmlBuilder& HtmlBuilder::pEnd()
{
  htmlText += "</p>";
  return *this;
}

HtmlBuilder& HtmlBuilder::brText(const QString& str)
{
  br();
  htmlText += str.toHtmlEscaped();
  return *this;
}

HtmlBuilder& HtmlBuilder::textBr(const QString& str)
{
  htmlText += str.toHtmlEscaped();
  return br();
}

HtmlBuilder& HtmlBuilder::hr(int size, int widthPercent)
{
  htmlText += "<hr size=\"" + QString::number(size) + "\" width=\"" + QString::number(widthPercent) + "%\"/>";
  numLines++;
  return *this;
}

HtmlBuilder& HtmlBuilder::a(const QString& text, const QString& href, html::Flags flags, QColor color)
{
  htmlText += "<a" + (href.isEmpty() ? QString() : " href=\"" + href + "\"") + ">" +
              asText(text, flags, color) + "</a>";
  return *this;
}

HtmlBuilder& HtmlBuilder::img(const QIcon& icon, const QString& alt, const QString& style, QSize size)
{
  QByteArray data;
  QBuffer buffer(&data);
  icon.pixmap(size).save(&buffer, "PNG", 100);

  img(QString("data:image/png;base64, %0").arg(QString(data.toBase64())), alt, style, size);
  return *this;
}

HtmlBuilder& HtmlBuilder::img(const QString& src, const QString& alt, const QString& style, QSize size)
{
  htmlText += "<img src='" + src + "'" + (style.isEmpty() ? QString() : " style=\"" + style + "\"") +
              (alt.isEmpty() ? QString() : " alt=\"" + alt + "\"") +
              (size.isValid() ?
               QString(" width=\"") + QString::number(size.width()) + "\"" +
               " height=\"" + QString::number(size.height()) + "\"" : QString()) + "/>";

  return *this;
}

HtmlBuilder& HtmlBuilder::ol()
{
  htmlText += "<ol>";
  return *this;
}

HtmlBuilder& HtmlBuilder::olEnd()
{
  htmlText += "</ol>";
  return *this;
}

HtmlBuilder& HtmlBuilder::ul()
{
  htmlText += "<ul>";
  return *this;
}

HtmlBuilder& HtmlBuilder::ulEnd()
{
  htmlText += "</ul>";
  return *this;
}

HtmlBuilder& HtmlBuilder::li(const QString& str, html::Flags flags, QColor color)
{
  htmlText += "<li>" + asText(str, flags, color) + "</li>";
  numLines++;
  return *this;
}

QString HtmlBuilder::asText(const QString& str, html::Flags flags, QColor color)
{
  QString prefix, suffix;
  if(flags & html::BOLD)
  {
    prefix.append("<b>");
    suffix.prepend("</b>");
  }

  if(flags & html::ITALIC)
  {
    prefix.append("<i>");
    suffix.prepend("</i>");
  }

  if(flags & html::UNDERLINE)
  {
    prefix.append("<u>");
    suffix.prepend("</u>");
  }

  if(flags & html::STRIKEOUT)
  {
    prefix.append("<s>");
    suffix.prepend("</s>");
  }

  if(flags & html::SUBSCRIPT)
  {
    prefix.append("<sub>");
    suffix.prepend("</sub>");
  }

  if(flags & html::SUPERSCRIPT)
  {
    prefix.append("<sup>");
    suffix.prepend("</sup>");
  }

  if(flags & html::SMALL)
  {
    prefix.append("<small>");
    suffix.prepend("</small>");
  }

  if(flags & html::BIG)
  {
    prefix.append("<big>");
    suffix.prepend("</big>");
  }

  if(color.isValid())
  {
    prefix.append("<span style=\"color:" + color.name(QColor::HexRgb) + "\">");
    suffix.prepend("</span>");
  }

  return prefix + str.toHtmlEscaped() + suffix;
}

bool HtmlBuilder::checklength(int maxLines, const QString& msg)
{
  QString dotText(QString("<b>%1</b>").arg(msg));
  if(numLines > maxLines)
  {
    if(!htmlText.endsWith(dotText))
      hr().b(msg);
    return true;
  }
  return false;
}

HtmlBuilder& HtmlBuilder::text(const QString& str, html::Flags flags, QColor color)
{
  htmlText += asText(str, flags, color);
  return *this;
}

HtmlBuilder& HtmlBuilder::p(const QString& str, html::Flags flags, QColor color)
{
  htmlText += "<p>";
  text(str, flags, color);
  htmlText += "</p>";
  tableIndex = 0;
  numLines++;
  return *this;
}

HtmlBuilder& HtmlBuilder::doc()
{
  htmlText +=
    "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
      "<html>"
        "<head>"
        "</head>"
        "<body style=\"font-family:'sans'; font-size:8pt; font-weight:400; font-style:normal;\">";
  tableIndex = 0;
  return *this;
}

HtmlBuilder& HtmlBuilder::docEnd()
{
  htmlText += "</body></html>";
  return *this;
}

void HtmlBuilder::clear()
{
  htmlText.clear();
  numLines = 0;
}

const QString& HtmlBuilder::alt(const QStringList& list) const
{
  return list.at(tableIndex % list.size());
}

QString HtmlBuilder::getEncodedImageHref(const QIcon& icon, QSize imageSize)
{
  QByteArray data;
  QBuffer buffer(&data);
  icon.pixmap(imageSize).save(&buffer, "PNG", 100);

  return QString("data:image/png;base64, %0").arg(QString(data.toBase64()));
}

} // namespace util
} // namespace atools
