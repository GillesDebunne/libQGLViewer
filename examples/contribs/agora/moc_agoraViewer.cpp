/****************************************************************************
** AgoraViewer meta object code from reading C++ file 'agoraViewer.h'
**
** Created: Fri Jun 10 09:19:51 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "agoraViewer.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *AgoraViewer::className() const
{
    return "AgoraViewer";
}

QMetaObject *AgoraViewer::metaObj = 0;
static QMetaObjectCleanUp cleanUp_AgoraViewer( "AgoraViewer", &AgoraViewer::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString AgoraViewer::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AgoraViewer", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString AgoraViewer::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AgoraViewer", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* AgoraViewer::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QGLViewer::staticMetaObject();
    static const QUMethod slot_0 = {"load", 0, 0 };
    static const QUMethod slot_1 = {"save", 0, 0 };
    static const QUMethod slot_2 = {"saveAs", 0, 0 };
    static const QUMethod slot_3 = {"print", 0, 0 };
    static const QUMethod slot_4 = {"undo", 0, 0 };
    static const QUMethod slot_5 = {"redo", 0, 0 };
    static const QUMethod slot_6 = {"startNewGame", 0, 0 };
    static const QUMethod slot_7 = {"suggestPlay", 0, 0 };
    static const QUParameter param_slot_8[] = {
	{ "on", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"togglePlayAgainstComputer", 1, param_slot_8 };
    static const QUMethod slot_9 = {"levelIsEasy", 0, 0 };
    static const QUMethod slot_10 = {"levelIsAverage", 0, 0 };
    static const QUMethod slot_11 = {"levelIsDifficult", 0, 0 };
    static const QUParameter param_slot_12[] = {
	{ "black", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"toggleComputerIsBlack", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ "on", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"toggleAnimation", 1, param_slot_13 };
    static const QUParameter param_slot_14[] = {
	{ "on", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_14 = {"toggleLight", 1, param_slot_14 };
    static const QUParameter param_slot_15[] = {
	{ "on", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_15 = {"toggleTexture", 1, param_slot_15 };
    static const QUParameter param_slot_16[] = {
	{ "on", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_16 = {"toggleShowPossible", 1, param_slot_16 };
    static const QUMethod slot_17 = {"help", 0, 0 };
    static const QUMethod slot_18 = {"about", 0, 0 };
    static const QUMethod slot_19 = {"simplePlay", 0, 0 };
    static const QUMethod slot_20 = {"swapRevoStartColor", 0, 0 };
    static const QUMethod slot_21 = {"swapRevoEndColor", 0, 0 };
    static const QUMethod slot_22 = {"swapArrivalColor", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "load()", &slot_0, QMetaData::Public },
	{ "save()", &slot_1, QMetaData::Public },
	{ "saveAs()", &slot_2, QMetaData::Public },
	{ "print()", &slot_3, QMetaData::Public },
	{ "undo()", &slot_4, QMetaData::Public },
	{ "redo()", &slot_5, QMetaData::Public },
	{ "startNewGame()", &slot_6, QMetaData::Public },
	{ "suggestPlay()", &slot_7, QMetaData::Public },
	{ "togglePlayAgainstComputer(bool)", &slot_8, QMetaData::Public },
	{ "levelIsEasy()", &slot_9, QMetaData::Public },
	{ "levelIsAverage()", &slot_10, QMetaData::Public },
	{ "levelIsDifficult()", &slot_11, QMetaData::Public },
	{ "toggleComputerIsBlack(bool)", &slot_12, QMetaData::Public },
	{ "toggleAnimation(bool)", &slot_13, QMetaData::Public },
	{ "toggleLight(bool)", &slot_14, QMetaData::Public },
	{ "toggleTexture(bool)", &slot_15, QMetaData::Public },
	{ "toggleShowPossible(bool)", &slot_16, QMetaData::Public },
	{ "help()", &slot_17, QMetaData::Public },
	{ "about()", &slot_18, QMetaData::Public },
	{ "simplePlay()", &slot_19, QMetaData::Private },
	{ "swapRevoStartColor()", &slot_20, QMetaData::Private },
	{ "swapRevoEndColor()", &slot_21, QMetaData::Private },
	{ "swapArrivalColor()", &slot_22, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"AgoraViewer", parentObject,
	slot_tbl, 23,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_AgoraViewer.setMetaObject( metaObj );
    return metaObj;
}

void* AgoraViewer::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "AgoraViewer" ) )
	return this;
    return QGLViewer::qt_cast( clname );
}

bool AgoraViewer::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: load(); break;
    case 1: save(); break;
    case 2: saveAs(); break;
    case 3: print(); break;
    case 4: undo(); break;
    case 5: redo(); break;
    case 6: startNewGame(); break;
    case 7: suggestPlay(); break;
    case 8: togglePlayAgainstComputer((bool)static_QUType_bool.get(_o+1)); break;
    case 9: levelIsEasy(); break;
    case 10: levelIsAverage(); break;
    case 11: levelIsDifficult(); break;
    case 12: toggleComputerIsBlack((bool)static_QUType_bool.get(_o+1)); break;
    case 13: toggleAnimation((bool)static_QUType_bool.get(_o+1)); break;
    case 14: toggleLight((bool)static_QUType_bool.get(_o+1)); break;
    case 15: toggleTexture((bool)static_QUType_bool.get(_o+1)); break;
    case 16: toggleShowPossible((bool)static_QUType_bool.get(_o+1)); break;
    case 17: help(); break;
    case 18: about(); break;
    case 19: simplePlay(); break;
    case 20: swapRevoStartColor(); break;
    case 21: swapRevoEndColor(); break;
    case 22: swapArrivalColor(); break;
    default:
	return QGLViewer::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool AgoraViewer::qt_emit( int _id, QUObject* _o )
{
    return QGLViewer::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool AgoraViewer::qt_property( int id, int f, QVariant* v)
{
    return QGLViewer::qt_property( id, f, v);
}

bool AgoraViewer::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
