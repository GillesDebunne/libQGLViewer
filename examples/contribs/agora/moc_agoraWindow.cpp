/****************************************************************************
** AgoraWindow meta object code from reading C++ file 'agoraWindow.h'
**
** Created: Fri Jun 10 09:19:51 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "agoraWindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *AgoraWindow::className() const
{
    return "AgoraWindow";
}

QMetaObject *AgoraWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_AgoraWindow( "AgoraWindow", &AgoraWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString AgoraWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AgoraWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString AgoraWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AgoraWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* AgoraWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUMethod slot_0 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "languageChange()", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"AgoraWindow", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_AgoraWindow.setMetaObject( metaObj );
    return metaObj;
}

void* AgoraWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "AgoraWindow" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool AgoraWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: languageChange(); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool AgoraWindow::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool AgoraWindow::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool AgoraWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
