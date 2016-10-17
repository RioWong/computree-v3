#ifndef CT_FACEATTRIBUTESSCALART_HPP
#define CT_FACEATTRIBUTESSCALART_HPP

#include "ct_itemdrawable/ct_faceattributesscalart.h"

#include <typeinfo>

template<typename SCALAR>
CT_FaceAttributesScalarT<SCALAR>::CT_FaceAttributesScalarT() : CT_AbstractFaceAttributesScalar(), CT_AttributesScalarT<SCALAR>()
{
}

template<typename SCALAR>
CT_FaceAttributesScalarT<SCALAR>::CT_FaceAttributesScalarT(const CT_OutAbstractSingularItemModel *model,
                                                           const CT_AbstractResult *result,
                                                           CT_FCIR pcir) : CT_AbstractFaceAttributesScalar(model,
                                                                                                                                                                      result,
                                                                                                                                                                      pcir), CT_AttributesScalarT<SCALAR>(pcir.data() != NULL ? pcir->size() : 0)
{
}

template<typename SCALAR>
CT_FaceAttributesScalarT<SCALAR>::CT_FaceAttributesScalarT(const CT_OutAbstractSingularItemModel *model,
                                                           const CT_AbstractResult *result,
                                                           CT_FCIR pcir,
                                                           CT_StandardCloudStdVectorT<SCALAR> *collection) : CT_AbstractFaceAttributesScalar(model,
                                                                                                                                             result,
                                                                                                                                             pcir), CT_AttributesScalarT<SCALAR>(collection)
{
}

template<typename SCALAR>
CT_FaceAttributesScalarT<SCALAR>::CT_FaceAttributesScalarT(const CT_OutAbstractSingularItemModel *model,
                                                           const CT_AbstractResult *result,
                                                           CT_FCIR pcir,
                                                           CT_StandardCloudStdVectorT<SCALAR> *collection,
                                                           const SCALAR &min,
                                                           const SCALAR &max) : CT_AbstractFaceAttributesScalar(model,
                                                                                                                result,
                                                                                                                pcir), CT_AttributesScalarT<SCALAR>(collection, min, max)
{
}



template<typename SCALAR>
CT_FaceAttributesScalarT<SCALAR>::CT_FaceAttributesScalarT(const QString &modelName,
                                                           const CT_AbstractResult *result,
                                                           CT_FCIR pcir) : CT_AbstractFaceAttributesScalar(modelName,
                                                                                                                                                                      result,
                                                                                                                                                                      pcir), CT_AttributesScalarT<SCALAR>(pcir.data() != NULL ? pcir->size() : 0)
{
}

template<typename SCALAR>
CT_FaceAttributesScalarT<SCALAR>::CT_FaceAttributesScalarT(const QString &modelName,
                                                           const CT_AbstractResult *result,
                                                           CT_FCIR pcir,
                                                           CT_StandardCloudStdVectorT<SCALAR> *collection) : CT_AbstractFaceAttributesScalar(modelName,
                                                                                                                                             result,
                                                                                                                                             pcir), CT_AttributesScalarT<SCALAR>(collection)
{
}

template<typename SCALAR>
CT_FaceAttributesScalarT<SCALAR>::CT_FaceAttributesScalarT(const QString &modelName,
                                                           const CT_AbstractResult *result,
                                                           CT_FCIR pcir,
                                                           CT_StandardCloudStdVectorT<SCALAR> *collection,
                                                           const SCALAR &min,
                                                           const SCALAR &max) : CT_AbstractFaceAttributesScalar(modelName,
                                                                                                                result,
                                                                                                                pcir), CT_AttributesScalarT<SCALAR>(collection, min, max)
{
}

template<typename SCALAR>
QString CT_FaceAttributesScalarT<SCALAR>::getType() const
{
    return staticGetType();
}

template<typename SCALAR>
QString CT_FaceAttributesScalarT<SCALAR>::staticGetType()
{
    QString type = CT_AbstractFaceAttributesScalar::staticGetType() + "/CT_FaceAttributesScalarT<" + CT_TypeInfo::name<SCALAR>() + ">";
    CT_AbstractItemDrawable::addNameTypeCorresp(type, staticName());
    return type;
}

template<typename SCALAR>
QString CT_FaceAttributesScalarT<SCALAR>::name() const
{
    return staticName();
}

template<typename SCALAR>
QString CT_FaceAttributesScalarT<SCALAR>::staticName()
{
    return tr("Face %1 attributes").arg(CT_TypeInfo::name<SCALAR>());
}


template<typename SCALAR>
CT_AbstractItemDrawable* CT_FaceAttributesScalarT<SCALAR>::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    CT_StandardCloudStdVectorT<SCALAR>* collect = this->collection();

    if (collect == NULL)
        return new CT_FaceAttributesScalarT<SCALAR>((const CT_OutAbstractSingularItemModel *)model, result, getFaceCloudIndexRegistered());
    else
        return new CT_FaceAttributesScalarT<SCALAR>((const CT_OutAbstractSingularItemModel *)model, result, getFaceCloudIndexRegistered(), dynamic_cast< CT_StandardCloudStdVectorT<SCALAR>* >(collect->copy()), this->min(), this->max());
}

#endif // CT_FACEATTRIBUTESSCALART_HPP
