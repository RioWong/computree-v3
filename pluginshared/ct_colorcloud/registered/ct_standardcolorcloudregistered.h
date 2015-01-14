#ifndef CT_STANDARDCOLORCLOUDREGISTERED_H
#define CT_STANDARDCOLORCLOUDREGISTERED_H

#include "ct_cloud/registered/ct_stdcloudregisteredt.h"
#include "ct_colorcloud/abstract/ct_abstractcolorcloud.h"

/**
 * A color cloud registered. It's size is sync with a global cloud.
 */
class PLUGINSHAREDSHARED_EXPORT CT_StandardColorCloudRegistered : public CT_StdCloudRegisteredT<CT_AbstractColorCloud>
{
public:
    CT_AbstractColorCloud* abstractColorCloud() const;

protected:

    friend class CT_CloudSyncToGlobalCloudManager;

    CT_StandardColorCloudRegistered(CT_AbstractColorCloud *colorCloud);
};

#endif // CT_STANDARDCOLORCLOUDREGISTERED_H
