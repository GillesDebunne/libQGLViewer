#include "config.h"

#if QT_VERSION >= 0x040000
# include <QDomElement>
# include <QString>
# include <QStringList>
# include <QColor>
#else
# include <qapplication.h>
# include <qdom.h>
# include <qstring.h>
# include <qstringlist.h>
# include <qcolor.h>
#endif

#include <math.h>

#ifndef DOXYGEN

// QDomElement loading with syntax checking.
class DomUtils
{
private:
  static void warning(const QString& message)
  {
#if QT_VERSION >= 0x040000
    qWarning("%s", message.toLatin1().constData());
#else
    qWarning("%s", message.latin1());
#endif
  }

public:
  static float floatFromDom(const QDomElement& e, const QString& attribute, float defValue)
  {
    float value = defValue;
    if (e.hasAttribute(attribute)) {
        const QString s = e.attribute(attribute);
        bool ok;
        value = s.toFloat(&ok);
        if (!ok) {
          warning("Bad float syntax for attribute \""+attribute+"\" in initialization of \""+e.tagName()+"\". Setting value to "+QString::number(value)+".");
          value = defValue;
        }
    } else
      warning("\""+attribute+"\" attribute missing in initialization of \""+e.tagName()+"\". Setting value to "+QString::number(value)+".");

#if defined(isnan)
    // The "isnan" method may not be available on all platforms.
    // Find its equivalent or simply remove these two lines
    if (isnan(value))
      warning("Warning, attribute \""+attribute+"\" initialized to Not a Number in \""+e.tagName()+"\"");
#endif

    return value;
  }

  static double doubleFromDom(const QDomElement& e, const QString& attribute, double defValue)
  {
    double value = defValue;
    if (e.hasAttribute(attribute)) {
      const QString s = e.attribute(attribute);
      bool ok;
      value = s.toDouble(&ok);
      if (!ok) {
        warning("Bad double syntax for attribute \""+attribute+"\" in initialization of \""+e.tagName()+"\". Setting value to "+QString::number(value)+".");
        value = defValue;
      }
    } else
      warning("\""+attribute+"\" attribute missing in initialization of \""+e.tagName()+"\". Setting value to "+QString::number(value)+".");

#if defined(isnan)
    // The "isnan" method may not be available on all platforms.
    // Find its equivalent or simply remove these two lines
    if (isnan(value))
      warning("Warning, attribute \""+attribute+"\" initialized to Not a Number in \""+e.tagName()+"\"");
#endif

    return value;
  }

  static int intFromDom(const QDomElement& e, const QString& attribute, int defValue)
  {
    int value = defValue;
    if (e.hasAttribute(attribute))
      {
    const QString s = e.attribute(attribute);
    bool ok;
    s.toInt(&ok);
    if (ok)
      value = s.toInt();
    else
      warning("Bad integer syntax for attribute \""+attribute+"\" in initialization of \""+e.tagName()+"\". Setting value to "+QString::number(value)+".");
      }
    else
      warning("\""+attribute+"\" attribute missing in initialization of \""+e.tagName()+"\". Setting value to "+QString::number(value)+".");
    return value;
  }

  static bool boolFromDom(const QDomElement& e, const QString& attribute, bool defValue)
  {
    bool value = defValue;
    if (e.hasAttribute(attribute))
      {
    const QString s = e.attribute(attribute);
#if QT_VERSION >= 0x040000
    if (s.toLower() == QString("true"))
#else
    if (s.lower() == QString("true"))
#endif
      value = true;
#if QT_VERSION >= 0x040000
    else if (s.toLower() == QString("false"))
#else
    else if (s.lower() == QString("false"))
#endif
      value = false;
    else
      {
        warning("Bad boolean syntax for attribute \""+attribute+"\" in initialization of \""+e.tagName()+"\" (should be \"true\" or \"false\").");
        warning("Setting value to "+(value?QString("true."):QString("false.")));
      }
      }
    else
      warning("\""+attribute+"\" attribute missing in initialization of \""+e.tagName()+"\". Setting value to "+(value?QString("true."):QString("false.")));
    return value;
  }

  static QDomElement QColorDomElement(const QColor& color, const QString& name, QDomDocument& doc)
  {
    QDomElement de = doc.createElement(name);
    de.setAttribute("red", QString::number(color.red()));
    de.setAttribute("green", QString::number(color.green()));
    de.setAttribute("blue", QString::number(color.blue()));
    return de;
  }

  static QColor QColorFromDom(const QDomElement& e)
  {
    int color[3];
    QStringList attribute;
    attribute << "red" << "green" << "blue";
#if QT_VERSION >= 0x040000
    for (int i=0; i<attribute.count(); ++i)
#else
    for (unsigned int i=0; i<attribute.count(); ++i)
#endif
      color[i] = DomUtils::intFromDom(e, attribute[i], 0);
    return QColor(color[0], color[1], color[2]);
  }
};

#endif // DOXYGEN
