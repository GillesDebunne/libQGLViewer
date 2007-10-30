#include "simpleViewer.h"
#include <qapplication.h>

#include <qlocale.h>
#include <qtranslator.h>

int main(int argc, char** argv)
{
  // Read command lines arguments.
  QApplication application(argc,argv);


       QString locale = QLocale::system().name();

     QTranslator translator;
	 //QString trF = QString("../../../QGLViewer/qglviewer_fr");
	 QString trF = QString("qglviewer_" + locale);
	 qWarning(qPrintable(QString("TR file=") + trF));
     bool ok = translator.load(trF, "C:\\Users\\debunne\\Documents\\Code\\libQGLViewer\\QGLViewer\\trunk\\QGLViewer");
	 if (ok)
		 qWarning("ok");
	 else
		 qWarning("unable to load");
     application.installTranslator(&translator); 

	   // Instantiate the viewer.
  Viewer viewer;

#if QT_VERSION < 0x040000
  // Set the viewer as the application main widget.
  application.setMainWidget(&viewer);
#else
  viewer.setWindowTitle("simpleViewer");
#endif

  // Make the viewer window visible on screen.
  viewer.show();

  // Run main loop.
  return application.exec();
}
