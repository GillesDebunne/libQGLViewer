#include <qdom.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qcolor.h>
#include <math.h>

#ifndef DOXYGEN

// QDomElement loading with syntax checking.
class DomUtils
{
public:
  static float floatFromDom(const QDomElement& e, const QString& attribute, float defValue)
  {
    float value = defValue;
    if (e.hasAttribute(attribute))
      {
	const QString s = e.attribute(attribute);
	bool ok;
	s.toFloat(&ok);
	if (ok)
	  value = s.toFloat();
	else
	  qWarning("Bad float syntax for attribute \""+attribute+"\" in initialization of \""+e.tagName()+"\". Setting value to "+QString::number(value)+".");
      }
    else
      qWarning("\""+attribute+"\" attribute missing in initialization of \""+e.tagName()+"\". Setting value to "+QString::number(value)+".");

#ifdef isnan
    if (isnan(value))
      qWarning("Warning, attribute \""+attribute+"\" initialized to Not a Number in \""+e.tagName()+"\"");
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
	  qWarning("Bad integer syntax for attribute \""+attribute+"\" in initialization of \""+e.tagName()+"\". Setting value to "+QString::number(value)+".");
      }
    else
      qWarning("\""+attribute+"\" attribute missing in initialization of \""+e.tagName()+"\". Setting value to "+QString::number(value)+".");
    return value;
  }

  static bool boolFromDom(const QDomElement& e, const QString& attribute, bool defValue)
  {
    bool value = defValue;
    if (e.hasAttribute(attribute))
      {
	const QString s = e.attribute(attribute);
	if (s.lower() == QString("true"))
	  value = true;
	else if (s.lower() == QString("false"))
	  value = false;
	else
	  {
	    qWarning("Bad boolean syntax for attribute \""+attribute+"\" in initialization of \""+e.tagName()+"\" (should be \"true\" or \"false\").");
	    qWarning("Setting value to "+(value?QString("true."):QString("false.")));
	  }
      }
    else
      qWarning("\""+attribute+"\" attribute missing in initialization of \""+e.tagName()+"\". Setting value to "+(value?QString("true."):QString("false.")));
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
    for (unsigned int i=0; i<attribute.count(); ++i)
      color[i] = DomUtils::intFromDom(e, attribute[i], 0);
    return QColor(color[0], color[1], color[2]);
  }
};

#endif // DOXYGEN
