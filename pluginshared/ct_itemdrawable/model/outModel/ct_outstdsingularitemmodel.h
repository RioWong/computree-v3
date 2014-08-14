#ifndef CT_OUTSTDSINGULARITEMMODEL_H
#define CT_OUTSTDSINGULARITEMMODEL_H

#include "ct_itemdrawable/model/outModel/abstract/def_ct_abstractitemdrawablemodelout.h"

/**
 * @brief Use this class if you will inform that you want create a singular item and add it to an
 *        other OUTPUT model that represent a group
 */
class PLUGINSHAREDSHARED_EXPORT CT_OutStdSingularItemModel : public DEF_CT_AbstractItemDrawableModelOut
{
public:

    /**
     * @brief Construct a model
     * @param uniqueName : a unique name in all the hierarchy
     * @param group : the real singular item that you will create in the method "compute" of your step. MUST NOT BE NULL !
     * @param displayableName : a displayable name
     * @param description : a description
     */
    CT_OutStdSingularItemModel(const QString &uniqueName,
                               CT_AbstractSingularItemDrawable *item,
                               const QString &displayableName = "",
                               const QString &description = "");

    /**
     * @brief Returns a copy of this model
     */
    CT_OutAbstractModel* copy() const;
};

#endif // CT_OUTSTDSINGULARITEMMODEL_H
