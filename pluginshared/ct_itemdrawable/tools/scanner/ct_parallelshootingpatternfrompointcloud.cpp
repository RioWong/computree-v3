/*
 * ct_parallelshootingpattern.cpp
 *
 *  Created on: Mar 3, 2017
 *      Author: Francis Giraldeau <francis.giraldeau@gmail.com>
 */

#include "ct_cloudindex/registered/abstract/ct_abstractcloudindexregisteredt.h"

#include "ct_parallelshootingpatternfrompointcloud.h"

#include "Eigen/Geometry"

CT_ParallelShootingPatternFromPointCloud::CT_ParallelShootingPatternFromPointCloud(
        const Eigen::Vector3d &point,
        const Eigen::Vector3d &normal,
        CT_PCIR pcir) :
    CT_ShootingPattern(),
    m_point(point),
    m_normal(normal),
    m_pcir(pcir),
    m_plane(normal, point)
{
}

CT_ParallelShootingPatternFromPointCloud::CT_ParallelShootingPatternFromPointCloud(
        const CT_ParallelShootingPatternFromPointCloud& other) :
    CT_ShootingPattern(),
    m_point(other.m_point),
    m_normal(other.m_normal),
    m_pcir(other.m_pcir),
    m_plane(other.m_normal, other.m_point)
{
}

size_t CT_ParallelShootingPatternFromPointCloud::getNumberOfShots() const
{
    return m_pcir->abstractCloudIndexT()->size();
}

CT_Shot CT_ParallelShootingPatternFromPointCloud::getShotAt( const size_t& index)
{
    auto idx = m_pcir->abstractCloudIndexT()->constIndexAt(index);
    CT_Point p = m_pAccessor.pointAt(idx);

    auto origin = m_plane.projection(p);
    return CT_Shot(origin, m_normal);
}

CT_ShootingPattern* CT_ParallelShootingPatternFromPointCloud::clone() const
{
    return new CT_ParallelShootingPatternFromPointCloud(*this);
}
