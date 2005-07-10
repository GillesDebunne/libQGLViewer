/****************************************************************************
** Form implementation generated from reading ui file 'myInterface.ui'
**
** Created: Thu Jul 7 10:55:22 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "myInterface.h"

#include <qvariant.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "interface.h"

/*
 *  Constructs a myInterface as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
myInterface::myInterface( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "Form1" );
    Form1Layout = new QGridLayout( this, 1, 1, 6, 2, "Form1Layout"); 

    FPSCheckBox = new QCheckBox( this, "FPSCheckBox" );

    Form1Layout->addWidget( FPSCheckBox, 1, 0 );

    AxisCheckBox = new QCheckBox( this, "AxisCheckBox" );

    Form1Layout->addWidget( AxisCheckBox, 1, 2 );

    QuitButton = new QPushButton( this, "QuitButton" );

    Form1Layout->addWidget( QuitButton, 1, 3 );

    GridCheckBox = new QCheckBox( this, "GridCheckBox" );

    Form1Layout->addWidget( GridCheckBox, 1, 1 );

    viewer = new Viewer( this, "viewer" );

    Form1Layout->addMultiCellWidget( viewer, 0, 0, 0, 3 );
    languageChange();
    resize( QSize(673, 438).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( QuitButton, SIGNAL( released() ), this, SLOT( close() ) );
    connect( FPSCheckBox, SIGNAL( toggled(bool) ), viewer, SLOT( setFPSIsDisplayed(bool) ) );
    connect( AxisCheckBox, SIGNAL( toggled(bool) ), viewer, SLOT( setAxisIsDrawn(bool) ) );
    connect( GridCheckBox, SIGNAL( toggled(bool) ), viewer, SLOT( setGridIsDrawn(bool) ) );
    connect( viewer, SIGNAL( FPSIsDisplayedChanged(bool) ), FPSCheckBox, SLOT( setChecked(bool) ) );
    connect( viewer, SIGNAL( axisIsDrawnChanged(bool) ), AxisCheckBox, SLOT( setChecked(bool) ) );
    connect( viewer, SIGNAL( gridIsDrawnChanged(bool) ), GridCheckBox, SLOT( setChecked(bool) ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
myInterface::~myInterface()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void myInterface::languageChange()
{
    setCaption( tr( "Form1" ) );
    FPSCheckBox->setText( tr( "FPS" ) );
    AxisCheckBox->setText( tr( "Axis" ) );
    QuitButton->setText( tr( "Quit" ) );
    GridCheckBox->setText( tr( "Grid" ) );
}

