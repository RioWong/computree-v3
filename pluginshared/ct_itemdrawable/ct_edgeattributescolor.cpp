#include "ct_edgeattributescolor.h"
#include "ct_colorcloud/ct_colorcloudstdvector.h"

CT_EdgeAttributesColor::CT_EdgeAttributesColor() : CT_AbstractEdgeAttributes(), CT_AttributesColor()
{
}

CT_EdgeAttributesColor::CT_EdgeAttributesColor(const CT_OutAbstractItemModel *model,
                                               const CT_AbstractResult *result,
                                               CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractCIR pcir,
                                               bool withAlphaInformation) : CT_AbstractEdgeAttributes(model,
                                                                                                      result,
                                                                                                      pcir), CT_AttributesColor(new CT_ColorCloudStdVector(pcir->size(), withAlphaInformation))
{
}

CT_EdgeAttributesColor::CT_EdgeAttributesColor(const CT_OutAbstractItemModel *model,
                                               const CT_AbstractResult *result,
                                               CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractCIR pcir,
                                               CT_AbstractColorCloud *cc) : CT_AbstractEdgeAttributes(model,
                                                                                                      result,
                                                                                                      pcir), CT_AttributesColor(cc)
{
}


CT_EdgeAttributesColor::CT_EdgeAttributesColor(const QString &modelName,
                                               const CT_AbstractResult *result,
                                               CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractCIR pcir,
                                               bool withAlphaInformation) : CT_AbstractEdgeAttributes(modelName,
                                                                                                      result,
                                                                                                      pcir), CT_AttributesColor(new CT_ColorCloudStdVector(pcir->size(), withAlphaInformation))
{
}

CT_EdgeAttributesColor::CT_EdgeAttributesColor(const QString &modelName,
                                               const CT_AbstractResult *result,
                                               CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractCIR pcir,
                                               CT_AbstractColorCloud *cc) : CT_AbstractEdgeAttributes(modelName,
                                                                                                      result,
                                                                                                      pcir), CT_AttributesColor(cc)
{
}

QString CT_EdgeAttributesColor::getType() const
{
    return staticGetType();
}

QString CT_EdgeAttributesColor::staticGetType()
{
    return CT_AbstractEdgeAttributes::staticGetType() + "/CT_EdgeAttributesColor";
}

CT_AbstractItemDrawable* CT_EdgeAttributesColor::copy(const CT_OutAbstractItemModel *model,
                                                        const CT_AbstractResult *result,
                                                        CT_ResultCopyModeList copyModeList)
{
    CT_AbstractColorCloud* colorCloud = getColorCloud();

    if (colorCloud == NULL)
        return new CT_EdgeAttributesColor(model, result, getEdgeCloudIndexRegistered(), (CT_AbstractColorCloud*) NULL);
    else
        return new CT_EdgeAttributesColor(model, result, getEdgeCloudIndexRegistered(), dynamic_cast<CT_AbstractColorCloud*>(colorCloud->copy()));
}
