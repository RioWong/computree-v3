#ifndef DEF_CT_ABSTRACTITEMDRAWABLEMODELOUT_H
#define DEF_CT_ABSTRACTITEMDRAWABLEMODELOUT_H

#include "ct_itemdrawable/model/abstract/ct_abstractsingularitemmodelt.h"

#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractgroupmodel.h"
#include "ct_attributes/model/outModel/abstract/ct_outabstractitemattributemodel.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractsingularitemmodel.h"

#define DEF_CT_AbstractItemDrawableModelOut CT_AbstractSingularItemModelT<CT_OutAbstractGroupModel, CT_OutAbstractItemAttributeModel, CT_OutAbstractSingularItemModel>

#endif // DEF_CT_ABSTRACTITEMDRAWABLEMODELOUT_H
