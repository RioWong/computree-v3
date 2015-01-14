#include "ct_standardcolorcloudregistered.h"

CT_AbstractColorCloud* CT_StandardColorCloudRegistered::abstractColorCloud() const
{
    return cloudT();
}

CT_StandardColorCloudRegistered::CT_StandardColorCloudRegistered(CT_AbstractColorCloud *colorCloud) : CT_StdCloudRegisteredT<CT_AbstractColorCloud>(colorCloud)
{
}


