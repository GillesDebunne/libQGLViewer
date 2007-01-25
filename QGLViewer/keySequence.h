#ifndef QGLVIEWER_QKEYSEQUENCE_H
#define QGLVIEWER_QKEYSEQUENCE_H

#if QT_VERSION >= 0x040000
# include <QString>
#else
# include <qstring.h>
#endif

class QKeySequence
{
public:
    QKeySequence(int key);
    operator QString() const;

private:
	int key_;
};

#endif // QGLVIEWER_QKEYSEQUENCE_H
