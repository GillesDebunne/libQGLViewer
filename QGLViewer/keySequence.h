#ifndef QGLVIEWER_QKEYSEQUENCE_H
#define QGLVIEWER_QKEYSEQUENCE_H

#include <qstring.h>

class QKeySequence
{
public:
    QKeySequence(int key);
    operator QString() const;

private:
	int key_;
};

#endif // QGLVIEWER_QKEYSEQUENCE_H
