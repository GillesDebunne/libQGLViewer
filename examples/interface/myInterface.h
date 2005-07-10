/****************************************************************************
** Form interface generated from reading ui file 'myInterface.ui'
**
** Created: Thu Jul 7 10:55:22 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FORM1_H
#define FORM1_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class Viewer;
class QCheckBox;
class QPushButton;

class myInterface : public QWidget
{
    Q_OBJECT

public:
    myInterface( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~myInterface();

    QCheckBox* FPSCheckBox;
    QCheckBox* AxisCheckBox;
    QPushButton* QuitButton;
    QCheckBox* GridCheckBox;
    Viewer* viewer;

protected:
    QGridLayout* Form1Layout;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

};

#endif // FORM1_H
