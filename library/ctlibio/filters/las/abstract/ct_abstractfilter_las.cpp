#include "ct_abstractfilter_las.h"
#include "ct_iterator/ct_pointiterator.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"

CT_AbstractFilter_LAS::CT_AbstractFilter_LAS() : CT_AbstractFilter_XYZ()
{
    _lasAttributes = NULL;
    _LASPointCloudIndex  = NULL;
}

CT_AbstractFilter_LAS::CT_AbstractFilter_LAS(const CT_AbstractFilter_LAS *other) : CT_AbstractFilter_XYZ(other)
{
    _lasAttributes = other->_lasAttributes;
    _LASPointCloudIndex  = other->_LASPointCloudIndex;
}

CT_AbstractFilter_LAS::~CT_AbstractFilter_LAS()
{
    _lasAttributes = NULL;
    _LASPointCloudIndex  = NULL;
}

CT_PointCloudIndexVector* CT_AbstractFilter_LAS::filterPointCloudIndex() const
{
    CT_LASData lasData;

    CT_PointIterator itP(_inCloud);
    while(itP.hasNext())
    {
        size_t index = itP.next().currentGlobalIndex();

        if (_LASPointCloudIndex->contains(index))
        {
            size_t localIndex = _LASPointCloudIndex->indexOf(index);
            _lasAttributes->getLASDataAt(localIndex, lasData);

            validatePoint(itP, lasData);
        } else {
            PS_LOG->addMessage(LogInterface::info, LogInterface::filter, tr("Pas d'informations LAS pour le point %1 : point non conservé").arg(index));
        }
    }

    return _outCloud;
}

void CT_AbstractFilter_LAS::validatePoint(CT_PointIterator &pointIt) const
{
    Q_UNUSED(pointIt);
    // No used : do nothing
}


bool CT_AbstractFilter_LAS::setLASAttributesContainer(const CT_StdLASPointsAttributesContainer *lasAttributes)
{
    _lasAttributes = NULL;
    _LASPointCloudIndex  = NULL;

    if (lasAttributes == NULL) {return false;}

    _lasAttributes = lasAttributes;

    QHashIterator<CT_LasDefine::LASPointAttributesType, CT_AbstractPointAttributesScalar *> it(_lasAttributes->lasPointsAttributes());
    if (!it.hasNext()) {return false;}

    CT_AbstractPointAttributesScalar *firstAttribute = it.next().value();
    if (firstAttribute == NULL) {return false;}

    _LASPointCloudIndex = firstAttribute->getPointCloudIndex();
    if (_LASPointCloudIndex == NULL) {return false;}

    return true;
}

