#include <qglobal.h>

#if QT_VERSION >= 0x050000
#include <QtUiPlugin/QDesignerCustomWidgetInterface>
#else
#include <QtDesigner/QDesignerCustomWidgetInterface>
#endif

#include <QtCore/qplugin.h>
#include <QtGui/QIcon>

class QGLViewerPlugin : public QObject, public QDesignerCustomWidgetInterface {
  Q_OBJECT
#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "QGLViewerPlugin" FILE "designerplugindescription.json")
#endif
  Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
  QGLViewerPlugin(QObject *parent = NULL);
  bool isContainer() const;
  bool isInitialized() const;
  QIcon icon() const;
  QString codeTemplate() const;
  QString domXml() const;
  QString group() const;
  QString includeFile() const;
  QString name() const;
  QString toolTip() const;
  QString whatsThis() const;
  QWidget *createWidget(QWidget *parent);
  void initialize(QDesignerFormEditorInterface *core);

private:
  bool initialized;
};
