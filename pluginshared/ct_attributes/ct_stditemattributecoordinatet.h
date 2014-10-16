#ifndef CT_STDITEMATTRIBUTECOORDINATET_H
#define CT_STDITEMATTRIBUTECOORDINATET_H

#include "ct_attributes/ct_stditemattributet.h"
#include "ct_global/ct_context.h"
#include "ct_categories/tools/ct_categorymanager.h"
#include "ct_coordinates/tools/ct_coordinatesystemmanager.h"

/**
 * @brief An item attribute that contains a data that will not changed. Use
 *        this class with a CT_IACreator :
 *
 *        CT_AbstractItemAttribute *attribute = CT_IACreator::createCoordinate(DEF_ATTRIBUTE_MODEL_NAME, CATEGORY_NAME, result, dataY, dataX, dataY, dataZ);
 */
template <typename VType>
class CT_StdItemAttributeCoordinateT : public CT_StdItemAttributeT<VType>
{
public:
    CT_StdItemAttributeCoordinateT(const CT_OutAbstractItemAttributeModel *model,
                                   const CT_AbstractCategory *category,
                                   const CT_AbstractResult *result,
                                   const VType &dataX,
                                   const VType &dataY,
                                   const VType &dataZ);

    CT_StdItemAttributeCoordinateT(const QString &modelName,
                                   const QString &categoryName,
                                   const CT_AbstractResult *result,
                                   const VType &dataX,
                                   const VType &dataY,
                                   const VType &dataZ);

    /**
     * @brief Use only this constructor for model !
     */
    CT_StdItemAttributeCoordinateT(const QString &categoryName);

    virtual CT_AbstractItemAttribute* copy(const CT_OutAbstractItemAttributeModel *model, const CT_AbstractResult *result);

protected:
    virtual VType data(const CT_AbstractItemDrawable *item) const;

    VType                                       m_dataY;
    VType                                       m_dataZ;
    CT_AbstractCoordinateSystem::DataToConvert  m_convert;
};

#include "ct_attributes/ct_stditemattributecoordinatet.hpp"

#endif // CT_STDITEMATTRIBUTECOORDINATET_H
