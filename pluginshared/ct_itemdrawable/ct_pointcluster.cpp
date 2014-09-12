/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michal KREBS (ARTS/ENSAM)

 This file is part of PluginShared library 2.0.

 PluginShared is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginShared is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginShared.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

*****************************************************************************/

#include "ct_pointcluster.h"

#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_global/ct_context.h"
#include <limits>

const CT_StandardPointClusterDrawManager CT_PointCluster::POINTCLUSTER_DRAW_MANAGER;

CT_PointCluster::CT_PointCluster() : CT_AbstractItemDrawableWithPointCloud()
{
    m_pIndex = NULL;

    setBaseDrawManager(&POINTCLUSTER_DRAW_MANAGER);

    _barycenter.reset();
}

CT_PointCluster::CT_PointCluster(const CT_OutAbstractSingularItemModel *model,
                                 const CT_AbstractResult *result) : CT_AbstractItemDrawableWithPointCloud(model, result)
{
    CT_PointCloudIndexVector *cloudIndex = new CT_PointCloudIndexVector();
    m_pIndex = cloudIndex;

    cloudIndex->setSortType(CT_AbstractPointCloudIndex::NotSorted); // Important to be able to store Polylines
    setPointCloudIndexRegisteredProtected(PS_REPOSITORY->registerPointCloudIndex(cloudIndex));
    setBaseDrawManager(&POINTCLUSTER_DRAW_MANAGER);

    _barycenter.reset();
}

CT_PointCluster::CT_PointCluster(const QString &modelName,
                                 const CT_AbstractResult *result) : CT_AbstractItemDrawableWithPointCloud(modelName, result)
{
    CT_PointCloudIndexVector *cloudIndex = new CT_PointCloudIndexVector();
    m_pIndex = cloudIndex;

    cloudIndex->setSortType(CT_AbstractPointCloudIndex::NotSorted); // Important to be able to store Polylines
    setPointCloudIndexRegisteredProtected(PS_REPOSITORY->registerPointCloudIndex(cloudIndex));
    setBaseDrawManager(&POINTCLUSTER_DRAW_MANAGER);

    _barycenter.reset();
}

CT_PointCluster::~CT_PointCluster()
{
}

QString CT_PointCluster::getType() const
{
    return staticGetType();
}

QString CT_PointCluster::staticGetType()
{
    return CT_AbstractItemDrawableWithPointCloud::staticGetType() + "/CT_PointCluster";
}

bool CT_PointCluster::addPoint(int index, bool verifyIfExist, bool firstPosition)
{
    if(verifyIfExist && m_pIndex->contains(index))
    {
        return false;
    }

    const CT_Point& point = m_pIndex->constTAtGlobalIndex(index);

    if (firstPosition)
    {
        m_pIndex->push_front(index);
    } else {
        m_pIndex->addIndex(index);
    }

    _barycenter.addPoint(point);

    if (point.x < _minCoordinates.x()) {_minCoordinates.setX(point.x);}
    if (point.y < _minCoordinates.y()) {_minCoordinates.setY(point.y);}
    if (point.z < _minCoordinates.z()) {_minCoordinates.setZ(point.z);}

    if (point.x > _maxCoordinates.x()) {_maxCoordinates.setX(point.x);}
    if (point.y > _maxCoordinates.y()) {_maxCoordinates.setY(point.y);}
    if (point.z > _maxCoordinates.z()) {_maxCoordinates.setZ(point.z);}

    updateCenterFromBoundingBox();

    return true;
}

const CT_PointClusterBarycenter& CT_PointCluster::getBarycenter() const
{
    return _barycenter;
}

CT_PointCluster* CT_PointCluster::merge(CT_PointCluster &pCLuster1, CT_PointCluster &pCLuster2, const CT_OutAbstractSingularItemModel *model, quint64 id, CT_AbstractResult &result, bool verifyDuplicated)
{
    Q_UNUSED(id)

    CT_PointCluster *pMerged = new CT_PointCluster((const CT_OutAbstractSingularItemModel *)model, &result);

    const CT_AbstractPointCloudIndex *pIndex1 = pCLuster1.getPointCloudIndex();
    const CT_AbstractPointCloudIndex *pIndex2 = pCLuster2.getPointCloudIndex();

    size_t size = pIndex1->size();
    size_t index;

    for(size_t i=0; i<size; ++i)
    {
        pIndex1->indexAt(i, index);
        pMerged->addPoint(index);
    }

    size = pIndex2->size();

    for(size_t i=0; i<size; ++i)
    {
        pIndex2->indexAt(i, index);
        pMerged->addPoint(index, verifyDuplicated);
    }

    return pMerged;
}

CT_PointCluster* CT_PointCluster::merge(CT_PointCluster &pCLuster1, CT_PointCluster &pCLuster2, const QString &modelName, quint64 id, CT_AbstractResult &result, bool verifyDuplicated)
{
    Q_UNUSED(id)

    CT_PointCluster *pMerged = new CT_PointCluster(modelName, &result);

    const CT_AbstractCloudIndexT<CT_Point> *pIndex1 = pCLuster1.getPointCloudIndex();
    const CT_AbstractCloudIndexT<CT_Point> *pIndex2 = pCLuster2.getPointCloudIndex();

    size_t size = pIndex1->size();
    size_t index;

    for(size_t i=0; i<size; ++i)
    {
        pIndex1->indexAt(i, index);
        pMerged->addPoint(index);
    }

    size = pIndex2->size();

    for(size_t i=0; i<size; ++i)
    {
        pIndex2->indexAt(i, index);
        pMerged->addPoint(index, verifyDuplicated);
    }

    return pMerged;
}

CT_AbstractItemDrawable* CT_PointCluster::copy(const CT_OutAbstractItemModel *model,
                                               const CT_AbstractResult *result,
                                               CT_ResultCopyModeList copyModeList)
{
    CT_ResultCopyModeList::CopyMode copyMode = CT_ResultCopyModeList::CopyItemDrawableReference;

    if(!copyModeList.isEmpty())
    {
        copyMode = copyModeList.takeFirst();
    }

    if(copyMode == CT_ResultCopyModeList::DontCopyItemDrawable)
    {
        copyMode = CT_ResultCopyModeList::CopyItemDrawableReference;
    }

    CT_PointCluster *pCluster;

    if(copyMode == CT_ResultCopyModeList::CopyItemDrawableReference)
    {
        pCluster = new CT_PointCluster((const CT_OutAbstractSingularItemModel *)model, result);
        pCluster->setPointCloudIndexRegisteredProtected(getPointCloudIndexRegistered());
        pCluster->updateBoundingBox();
        pCluster->initBarycenter();
    }
    else if(copyMode == CT_ResultCopyModeList::CopyItemDrawableCompletely)
    {
        pCluster = new CT_PointCluster((const CT_OutAbstractSingularItemModel *)model, result);

        if(getPointCloudIndexRegistered() != NULL)
        {
            pCluster->setPointCloudIndexRegisteredProtected(PS_REPOSITORY->copyPointCloud(getPointCloudIndexRegistered()));
        }

        pCluster->updateBoundingBox();
        pCluster->initBarycenter();
    }

    return pCluster;
}

///////// PROTECTED ///////////

void CT_PointCluster::initBarycenter()
{
    _barycenter.reset();

    const CT_AbstractPointCloudIndex *pIndex = getPointCloudIndex();

    if(pIndex != NULL)
    {
        size_t size = pIndex->size();
        for(size_t i=0 ; i<size ; ++i)
        {
            const CT_Point& point = pIndex->constTAt(i);
            _barycenter.addPoint(point);
        }
        _barycenter.compute();
    }
}

#ifdef USE_BOOST_OLD
BOOST_CLASS_EXPORT_IMPLEMENT(CT_PointCluster)
#endif
