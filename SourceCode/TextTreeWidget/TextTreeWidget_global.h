#ifndef TEXTTREEWIDGET_GLOBAL_H
#define TEXTTREEWIDGET_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TEXTTREEWIDGET_LIBRARY)
#  define TEXTTREEWIDGET_EXPORT Q_DECL_EXPORT
#else
#  define TEXTTREEWIDGET_EXPORT Q_DECL_IMPORT
#endif

#endif // TEXTTREEWIDGET_GLOBAL_H
