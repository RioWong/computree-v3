#ifndef CT_ABSTRACTFILTER_H
#define CT_ABSTRACTFILTER_H

#include "pluginShared_global.h"
#include "ct_filter/abstract/ct_abstractconfigurableelement.h"

class PLUGINSHAREDSHARED_EXPORT CT_AbstractFilter : public CT_AbstractConfigurableElement
{
    Q_OBJECT
public:
    CT_AbstractFilter();
    CT_AbstractFilter(const CT_AbstractFilter* other);
    ~CT_AbstractFilter();

};

#endif // CT_ABSTRACTFILTER_H
