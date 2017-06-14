#include <qwidgetplugin.h>

class QGLViewerPlugin : public QWidgetPlugin {
public:
  QStringList keys() const;
  QWidget *create(const QString &key, QWidget *parent, const char *name);
  QString group(const QString &key) const;
  QIconSet iconSet(const QString &) const;
  QString includeFile(const QString &key) const;
  QString toolTip(const QString &key) const;
  QString whatsThis(const QString &key) const;
  bool isContainer(const QString &) const;
};
