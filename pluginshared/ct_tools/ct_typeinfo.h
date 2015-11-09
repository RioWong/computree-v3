#ifndef CT_TYPEINFO_H
#define CT_TYPEINFO_H

#include "pluginShared_global.h"

#include <QString>

#define CT_TYPEINFO_MACRO( SCALARTYPE, SCALARTYPENAME )\
template<>\
inline QString CT_TypeInfo::name<SCALARTYPE>() { return QString(#SCALARTYPENAME); }

class PLUGINSHAREDSHARED_EXPORT CT_TypeInfo
{
public:
    template<typename TT>
    static inline QString name() { return ""; }
};

CT_TYPEINFO_MACRO(bool, bool)
CT_TYPEINFO_MACRO(float, float)
CT_TYPEINFO_MACRO(double, double)
CT_TYPEINFO_MACRO(long, long)
CT_TYPEINFO_MACRO(unsigned long, ulong)
CT_TYPEINFO_MACRO(long double, longdouble)
CT_TYPEINFO_MACRO(qint8, qint8)
CT_TYPEINFO_MACRO(quint8, quint8)
CT_TYPEINFO_MACRO(qint16, qint16)
CT_TYPEINFO_MACRO(quint16, quint16)
CT_TYPEINFO_MACRO(qint32, qint32)
CT_TYPEINFO_MACRO(quint32, quint32)
CT_TYPEINFO_MACRO(qint64, qint64)
CT_TYPEINFO_MACRO(quint64, quint64)
CT_TYPEINFO_MACRO(QString, QString)

#endif // CT_TYPEINFO_H
