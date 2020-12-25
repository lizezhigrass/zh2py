#ifndef ZH2PY_GLOBAL_H
#define ZH2PY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ZH2PY_LIBRARY)
#  define ZH2PY_EXPORT Q_DECL_EXPORT
#else
#  define ZH2PY_EXPORT Q_DECL_IMPORT
#endif

#endif // ZH2PY_GLOBAL_H
