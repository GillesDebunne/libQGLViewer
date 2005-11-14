#include "qglviewer.h"

#ifndef NO_VECTORIAL_RENDER
# if QT_VERSION >= 0x040000
#  include "ui_VRenderInterface.Qt4.h"
# else
#  if QT_VERSION >= 0x030000
#   include "VRenderInterface.Qt3.h"
#  else
#   include "VRenderInterface.Qt2.h"
#  endif
# endif
# include "VRender/VRender.h"
#endif

// Output format list
#if QT_VERSION < 0x040000
# include <qimage.h>
#else
# include <QImageReader>
#endif

#include <qfileinfo.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qmap.h>
#include <qinputdialog.h>
#include <qprogressdialog.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qcursor.h>
#include <qlayout.h>

using namespace std;

////// Static global variables - local to this file //////
// List of available output file formats, formatted for QFileDialog.
static QString formats;
// Converts QFileDialog resulting format to Qt snapshotFormat.
static QMap<QString, QString> Qtformat;
// Converts Qt snapshotFormat to QFileDialog menu string.
static QMap<QString, QString> FDFormatString;
// Converts snapshotFormat to file extension
static QMap<QString, QString> extension;


/*! Sets snapshotFileName(). */
void QGLViewer::setSnapshotFileName(const QString& name)
{
#if QT_VERSION >= 0x040000
  snapshotFileName_ = QFileInfo(name).absoluteFilePath();
#else
  snapshotFileName_ = QFileInfo(name).absFilePath();
#endif
}

#ifndef DOXYGEN
const QString& QGLViewer::snapshotFilename() const
{
  qWarning("snapshotFilename is deprecated. Use snapshotFileName() (uppercase N) instead.");
  return snapshotFileName();
}

void QGLViewer::setSnapshotFilename(const QString& name)
{
  qWarning("setSnapshotFilename is deprecated. Use setSnapshotFileName() (uppercase N) instead.");
  setSnapshotFileName(name);
}
#endif


/*! Opens a dialog that displays the different available snapshot formats.

Then calls setSnapshotFormat() with the selected one (unless the user cancels).

Returns \c false if the user presses the Cancel button and \c true otherwise. */
bool QGLViewer::openSnapshotFormatDialog()
{
  bool ok = false;
#if QT_VERSION >= 0x040000
  QStringList list = formats.split(";;", QString::SkipEmptyParts);
  int current = list.indexOf(FDFormatString[snapshotFormat()]);
  QString format = QInputDialog::getItem(this, "Snapshot format", "Select a snapshot format", list, current, false, &ok);
#else
  QStringList list = QStringList::split(";;", formats);
  int current = list.findIndex(FDFormatString[snapshotFormat()]);
  QString format = QInputDialog::getItem("Snapshot format", "Select a snapshot format", list, current, false, &ok, this);
#endif
  if (ok)
    setSnapshotFormat(Qtformat[format]);
  return ok;
}


// Finds all available Qt output formats, so that they can be available in
// saveSnapshot dialog. Initialize snapshotFormat() to the first one.
void QGLViewer::initializeSnapshotFormats()
{
#if QT_VERSION >= 0x040000
  QList<QByteArray> list = QImageReader::supportedImageFormats();
  QStringList formatList;
  for (int i=0; i < list.size(); ++i)
    formatList << QString(list.at(i).toUpper());
#else
  QStringList formatList = QImage::outputFormatList();
#endif
  //        qWarning("Available image formats: ");
  //        QStringList::Iterator it = formatList.begin();
  //        while( it != formatList.end() )
  //  	      qWarning((*it++).);  QT4 change this. qWarning no longer accepts QString

#ifndef NO_VECTORIAL_RENDER
  // We add the 3 vectorial formats to the list
  formatList += "EPS";
  formatList += "PS";
  formatList += "XFIG";
#endif

  // Check that the interesting formats are available and add them in "formats"
  // Unused formats: XPM XBM PBM PGM
  QStringList QtText, MenuText, Ext;
  QtText += "JPEG";	MenuText += "JPEG (*.jpg)";		Ext += "jpg";
  QtText += "PNG";	MenuText += "PNG (*.png)";		Ext += "png";
  QtText += "EPS";	MenuText += "Encapsulated Postscript (*.eps)";	Ext += "eps";
  QtText += "PS";	MenuText += "Postscript (*.ps)";	Ext += "ps";
  QtText += "PPM";	MenuText += "24bit RGB Bitmap (*.ppm)";	Ext += "ppm";
  QtText += "BMP";	MenuText += "Windows Bitmap (*.bmp)";	Ext += "bmp";
  QtText += "XFIG";	MenuText += "XFig (*.fig)";		Ext += "fig";

#if QT_VERSION < 0x030000
  QStringList::Iterator itText = QtText.begin();
  QStringList::Iterator itMenu = MenuText.begin();
  QStringList::Iterator itExt  = Ext.begin();
#else
  QStringList::iterator itText = QtText.begin();
  QStringList::iterator itMenu = MenuText.begin();
  QStringList::iterator itExt  = Ext.begin();
#endif
  while (itText != QtText.end())
    {
      //QMessageBox::information(this, "Snapshot ", "Trying format\n"+(*itText));
      if (formatList.contains((*itText)))
	{
	  //QMessageBox::information(this, "Snapshot ", "Recognized format\n"+(*itText));
	  if (formats.isEmpty())
	    setSnapshotFormat(*itText);
	  else
	    formats += ";;";
	  formats += (*itMenu);
	  Qtformat[(*itMenu)]  = (*itText);
	  FDFormatString[(*itText)]  = (*itMenu);
	  extension[(*itText)] = (*itExt);
	}
      // Synchronize parsing
      itText++;
      itMenu++;
      itExt++;
    }
}

// Returns false is the user refused to use the fileName
static bool checkFileName(QString& fileName, QWidget* widget, const QString& snapshotFormat)
{
  if (fileName.isEmpty())
    return false;

  // Check that extension has been provided
  QFileInfo info(fileName);

#if QT_VERSION >= 0x040000
  if (info.suffix().isEmpty())
#else
  if (info.extension(false).isEmpty())
#endif
    {
      // No extension given. Silently add one
      if (fileName.right(1) != ".")
	fileName += ".";
      fileName += extension[snapshotFormat];
      info.setFile(fileName);
    }
#if QT_VERSION >= 0x040000
  else if (info.suffix() != extension[snapshotFormat])
#else
  else if (info.extension(false) != extension[snapshotFormat])
#endif
    {
      // Extension is not appropriate. Propose a modification
#if QT_VERSION >= 0x040000
      QString modifiedName = info.absolutePath() + '/' + info.baseName() + "." + extension[snapshotFormat];
#else
# if QT_VERSION >= 0x030000
      QString modifiedName = info.dirPath() + '/' + info.baseName(true) + '.' + extension[snapshotFormat];
# else
      QString modifiedName = info.dirPath() + '/' + info.baseName() + '.' + extension[snapshotFormat];
# endif
#endif
      QFileInfo modifInfo(modifiedName);
      int i=(QMessageBox::warning(widget,"Wrong extension",
				  info.fileName()+" has a wrong extension.\nSave as "+modifInfo.fileName()+" instead ?",
				  QMessageBox::Yes,
				  QMessageBox::No,
				  QMessageBox::Cancel));
      if (i==QMessageBox::Cancel)
	return false;

      if (i==QMessageBox::Yes)
	{
	  fileName = modifiedName;
	  info.setFile(fileName);
	}
    }

  return true;
}

#ifndef NO_VECTORIAL_RENDER
// static void drawVectorial(void* param)
void drawVectorial(void* param)
{
  ( (QGLViewer*) param )->drawVectorial();
}

class ProgressDialog
{
public:
  static void showProgressDialog(QGLWidget* parent);
  static void updateProgress(float progress, const std::string& stepString);
  static void hideProgressDialog();

private:
    static QProgressDialog* progressDialog;
};

QProgressDialog* ProgressDialog::progressDialog = NULL;

void ProgressDialog::showProgressDialog(QGLWidget* parent)
{
  progressDialog = new QProgressDialog(parent);
#if QT_VERSION >= 0x040000
  progressDialog->setWindowTitle("Vectorial rendering progress");
#else
  progressDialog->setCaption("Vectorial rendering progress");
#endif
  progressDialog->setMinimumSize(300, 40);
  progressDialog->setCancelButton(NULL);
  progressDialog->show();
}

void ProgressDialog::updateProgress(float progress, const std::string& stepString)
{
#if QT_VERSION >= 0x040000
  progressDialog->setValue(int(progress*100));
#else
  progressDialog->setProgress(int(progress*100));
#endif
  QString message(stepString.c_str());
  if (message.length() > 33)
    message = message.left(17) + "..." + message.right(12);
  progressDialog->setLabelText(message);
  progressDialog->update();
  qApp->processEvents();
}

void ProgressDialog::hideProgressDialog()
{
  progressDialog->close();
  delete progressDialog;
  progressDialog = NULL;
}

#if QT_VERSION >= 0x040000
class VRenderInterface: public QDialog, public Ui::VRenderInterface
{
 public: VRenderInterface(QWidget *parent) : QDialog(parent) { setupUi(this); }
};
#endif

// Pops-up a vectorial output option dialog box and save to fileName
// Returns -1 in case of Cancel, 0 for success and (todo) error code in case of problem.
static int saveVectorialSnapshot(const QString& fileName, QGLWidget* widget, const QString& snapshotFormat)
{
  static VRenderInterface* VRinterface = NULL;

  if (!VRinterface)
    VRinterface = new VRenderInterface(widget);

  // Configure interface according to selected snapshotFormat
  if (snapshotFormat == "XFIG")
    {
      VRinterface->tightenBBox->setEnabled(false);
      VRinterface->colorBackground->setEnabled(false);
    }
  else
    {
      VRinterface->tightenBBox->setEnabled(true);
      VRinterface->colorBackground->setEnabled(true);
    }

  if (VRinterface->exec() == QDialog::Rejected)
    return -1;

  vrender::VRenderParams vparams;
#if QT_VERSION >= 0x040000
  vparams.setFilename(fileName.toLatin1().data());
#else
  vparams.setFilename(fileName);
#endif

  if (snapshotFormat == "EPS")	vparams.setFormat(vrender::VRenderParams::EPS);
  if (snapshotFormat == "PS")	vparams.setFormat(vrender::VRenderParams::PS);
  if (snapshotFormat == "XFIG")	vparams.setFormat(vrender::VRenderParams::XFIG);

  vparams.setOption(vrender::VRenderParams::CullHiddenFaces, !(VRinterface->includeHidden->isChecked()));
  vparams.setOption(vrender::VRenderParams::OptimizeBackFaceCulling, VRinterface->cullBackFaces->isChecked());
  vparams.setOption(vrender::VRenderParams::RenderBlackAndWhite, VRinterface->blackAndWhite->isChecked());
  vparams.setOption(vrender::VRenderParams::AddBackground, VRinterface->colorBackground->isChecked());
  vparams.setOption(vrender::VRenderParams::TightenBoundingBox, VRinterface->tightenBBox->isChecked());

#if QT_VERSION >= 0x040000
  switch (VRinterface->sortMethod->currentIndex())
#else
  switch (VRinterface->sortMethod->currentItem())
#endif
    {
    case 0: vparams.setSortMethod(vrender::VRenderParams::NoSorting); 		break;
    case 1: vparams.setSortMethod(vrender::VRenderParams::BSPSort); 		break;
    case 2: vparams.setSortMethod(vrender::VRenderParams::TopologicalSort); 	break;
    case 3: vparams.setSortMethod(vrender::VRenderParams::AdvancedTopologicalSort);	break;
    default:
      qWarning("VRenderInterface::saveVectorialSnapshot: Unknown SortMethod");
    }

  widget->makeCurrent();

  vparams.setProgressFunction(&ProgressDialog::updateProgress);
  ProgressDialog::showProgressDialog(widget);
  vrender::VectorialRender(drawVectorial, (void*) widget, vparams);
  ProgressDialog::hideProgressDialog();
#if QT_VERSION < 0x030000
  widget->setCursor(Qt::arrowCursor);
#else
  widget->setCursor(QCursor(Qt::ArrowCursor));
#endif

  // Should return vparams.error(), but this is currently not set.
  return 0;
}
#endif // NO_VECTORIAL_RENDER

/*! Saves a snapshot of the current image displayed by the widget.

 Options are set using snapshotFormat(), snapshotFileName() and snapshotQuality(). For non vectorial
 image formats, the image size is equal to the current viewer's dimensions (see width() and
 height()). See snapshotFormat() for details on supported formats.

 If \p automatic is \c false (or if snapshotFileName() is empty), a file dialog is opened to ask for
 the file name.

 When \p automatic is \c true, the file name is set to \c NAME-NUMBER, where \c NAME is
 snapshotFileName() and \c NUMBER is snapshotCounter(). The snapshotCounter() is automatically
 incremented after each snapshot saving. This is useful to create videos from your application:
 \code
 void Viewer::init()
 {
   resize(720, 576); // PAL DV format (use 720x480 for NTSC DV)
   connect(this, SIGNAL(drawFinished(bool)), SLOT(saveSnapshot(bool)));
 }
 \endcode
 Then call draw() in a loop (for instance using animate() and/or a camera() KeyFrameInterpolator
 replay) to create your image sequence.

 If you want to create a Quicktime VR panoramic sequence, simply use code like this:
 \code
 void Viewer::createQuicktime()
 {
   const int nbImages = 36;
   for (int i=0; i<nbImages; ++i)
     {
       camera()->setOrientation(2.0*M_PI/nbImages, 0.0); // Theta-Phi orientation
       showEntireScene();
       updateGL(); // calls draw(), which emits drawFinished(), which calls saveSnapshot()
     }
 }
 \endcode

 If snapshotCounter() is negative, no number is appended to snapshotFileName() and the
 snapshotCounter() is not incremented. This is useful to force the creation of a file, overwriting
 the previous one.

 When \p overwrite is set to \c false (default), a window asks for confirmation if the file already
 exists. In \p automatic mode, the snapshotCounter() is incremented (if positive) until a
 non-existing file name is found instead. Otherwise the file is overwritten without confirmation.

 The VRender library was written by Cyril Soler (Cyril dot Soler at imag dot fr). Remove that
 anti-aliasing option to correctly display the generated PS and EPS results.

 \note In order to correctly grab the frame buffer, the QGLViewer window is raised in front of
 other windows by this method. */
void QGLViewer::saveSnapshot(bool automatic, bool overwrite)
{
  static QImage snapshot;
  // Viewer must be on top of other windows.
  raise();
  // Hack: Qt has problems if the frame buffer is grabbed after QFileDialog is displayed.
  // We grab the frame buffer before, even if it might be not necessary (vectorial rendering).
  // The problem could not be reproduced on a simple example to submit a Qt bug.
  // However, only grabs the backgroundImage in the eponym example. May come from the driver.
  snapshot = grabFrameBuffer(true);

  // Ask for file name
  if (snapshotFileName().isEmpty() || !automatic)
    {
      QString fileName;
#if QT_VERSION < 0x030000
      if (openSnapshotFormatDialog())
        fileName = QFileDialog::getSaveFileName(snapshotFileName(), FDFormatString[snapshotFormat()]+";;All files (*.*)", this, "Save dialog");
      else
        return;
#else
      QString selectedFormat = FDFormatString[snapshotFormat()];
# if QT_VERSION >= 0x040000
      fileName = QFileDialog::getSaveFileName(this, "Choose a file name to save under", snapshotFileName(), formats, &selectedFormat);
# else
      fileName = QFileDialog::getSaveFileName(snapshotFileName(), formats, this,
					      "Save Snapshot dialog", "Choose a file name to save under", &selectedFormat);
# endif
      setSnapshotFormat(Qtformat[selectedFormat]);
#endif

      if (checkFileName(fileName, this, snapshotFormat()))
	setSnapshotFileName(fileName);
      else
	return;
    }

  QFileInfo fileInfo(snapshotFileName());

  if ((automatic) && (snapshotCounter() >= 0))
    {
      // In automatic mode, names have a number appended
      const QString baseName = fileInfo.baseName();
      QString count;
      count.sprintf("%.04d", snapshotCounter_++);
#if QT_VERSION >= 0x040000
      fileInfo.setFile(fileInfo.absolutePath()+ '/' + baseName + '-' + count + '.' + fileInfo.completeSuffix());
#else
      fileInfo.setFile(fileInfo.dirPath()+ '/' + baseName + '-' + count + '.' + fileInfo.extension());
#endif

      if (!overwrite)
	while (fileInfo.exists())
	  {
	    count.sprintf("%.04d", snapshotCounter_++);
#if QT_VERSION >= 0x040000
	    fileInfo.setFile(fileInfo.absolutePath() + '/' +baseName + '-' + count + '.' + fileInfo.completeSuffix());
#else
	    fileInfo.setFile(fileInfo.dirPath() + '/' + baseName + '-' + count + '.' + fileInfo.extension());
#endif
	  }
    }

  if ((fileInfo.exists()) && (!overwrite) &&
      (QMessageBox::warning(this,"Overwrite file ?",
			    "File "+fileInfo.fileName()+" already exists.\nSave anyway ?",
			    QMessageBox::Yes,
			    QMessageBox::Cancel) == QMessageBox::Cancel))
      return;

  bool saveOK;
#ifndef NO_VECTORIAL_RENDER
  if ( (snapshotFormat() == "EPS") || (snapshotFormat() == "PS") || (snapshotFormat() == "XFIG") )
    {
      // Vectorial snapshot
      int res = saveVectorialSnapshot(fileInfo.filePath(), this, snapshotFormat());
      if (res == -1) // CANCEL
	return;
      else
	saveOK = (res == 0);
    }
  else
#endif
#if QT_VERSION >= 0x040000
    saveOK = snapshot.save(fileInfo.filePath(), snapshotFormat().toLatin1().constData(), snapshotQuality());
#else
    saveOK = snapshot.save(fileInfo.filePath(), snapshotFormat(), snapshotQuality());
#endif

  if (!saveOK)
    QMessageBox::warning(this, "Snapshot problem", "Unable to save snapshot in\n"+fileInfo.filePath());
}

/*! Same as saveSnapshot(), except that it uses \p fileName instead of snapshotFileName().

 If \p fileName is empty, opens a file dialog to select the name.

 Snapshot settings are set from snapshotFormat() and snapshotQuality().

 Asks for confirmation when \p fileName already exists and \p overwrite is \c false (default).

 \attention If \p fileName is a char* (as is "myFile.jpg"), it may be casted into a \c bool, and the
 other saveSnapshot() method may be used instead. Pass QString("myFile.jpg") as a parameter to
 prevent this. */
void QGLViewer::saveSnapshot(const QString& fileName, bool overwrite)
{
  const QString previousName = snapshotFileName();
  const int previousCounter = snapshotCounter();
  setSnapshotFileName(fileName);
  setSnapshotCounter(-1);
  saveSnapshot(true, overwrite);
  setSnapshotFileName(previousName);
  setSnapshotCounter(previousCounter);
}


#if QT_VERSION < 0x030000
// This code is largely inspired from Qt's method available in version 3
// Copyright Trolltech AS
QImage QGLViewer::grabFrameBuffer(bool withAlpha)
{
  makeCurrent();
  QImage res;
  int w = width();
  int h = height();
  if (format().rgba())
  {
	res = QImage(w, h, 32);
	glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, res.bits());
	if (QImage::systemByteOrder() == QImage::BigEndian)
	{
	  // OpenGL gives RGBA; Qt wants ARGB
	  uint *p = (uint*)res.bits();
	  uint *end = p + w*h;
	  if (withAlpha && format().alpha())
	  {
		while (p < end)
		{
		  uint a = *p << 24;
		  *p = (*p >> 8) | a;
		  p++;
		}
	  }
	  else
	  {
		while (p < end)
		  *p++ >>= 8;
	  }
	}
	else
	{
	  // OpenGL gives ABGR (i.e. RGBA backwards); Qt wants ARGB
	  res = res.swapRGB();
	}
	res.setAlphaBuffer(withAlpha && format().alpha());
  }
  else
  {
#if defined (Q_WS_WIN)
	res = QImage(w, h, 8);
	glReadPixels(0, 0, w, h, GL_COLOR_INDEX, GL_UNSIGNED_BYTE, res.bits());
	//int palSize = 0;
	//const QRgb* pal = QColor::palette(&palSize);
	//if (pal && palSize)
	//{
	//  res.setNumColors(palSize);
	//  for (int i = 0; i < palSize; i++)
	//	res.setColor(i, pal[i]);
	//}
#endif
  }

  return res.mirror();
}

#endif // QT_VERSION < 0x030000
