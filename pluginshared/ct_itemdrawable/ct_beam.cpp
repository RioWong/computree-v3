/****************************************************************************

 Copyright (C) 2012-2012 Universite de Sherbrooke, Quebec, CANADA
                     All rights reserved.

 Contact :  richard.fournier@usherbrooke.ca
            jean-francois.cote@nrcan-rncan.gc.ca
            joris.ravaglia@gmail.com

 Developers : Joris RAVAGLIA
 Adapted by Alexandre Piboule for Computree 2.0

 This file is part of Computree version 2.0.

 Computree is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Computree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Computree.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/

#include "ct_beam.h"
#include <limits>

const CT_StandardBeamDrawManager CT_Beam::BEAM_DRAW_MANAGER;

CT_Beam::CT_Beam() : CT_AbstractItemDrawableWithoutPointCloud()
{
    m_shot = new CT_Shot(true);

    // Setting the center attribute from the CT_AbstractItemDrawableWithoutPointCloud class
    setCenterX( 0 );
    setCenterY( 0 );
    setCenterZ( 0 );

    setBaseDrawManager(&BEAM_DRAW_MANAGER);
}

CT_Beam::CT_Beam(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
    m_shot = new CT_Shot(true);

    // Setting the center attribute from the CT_AbstractItemDrawableWithoutPointCloud class
    setCenterX( 0 );
    setCenterY( 0 );
    setCenterZ( 0 );

    setBaseDrawManager(&BEAM_DRAW_MANAGER);
}

CT_Beam::CT_Beam(const CT_OutAbstractSingularItemModel *model,
                 const CT_AbstractResult *result,
                 const Eigen::Vector3d &origin,
                 const Eigen::Vector3d &direction) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
    Q_ASSERT( !(direction(0) == 0 && direction(1) == 0 && direction(2) == 0) );

    m_shot = new CT_Shot(origin, direction.normalized());

    // Setting the center attribute from the CT_AbstractItemDrawableWithoutPointCloud class
    setCenterCoordinate(origin);

    setBaseDrawManager(&BEAM_DRAW_MANAGER);
}


CT_Beam::CT_Beam(const QString &modelName, const CT_AbstractResult *result) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{
    m_shot = new CT_Shot(true);

    // Setting the center attribute from the CT_AbstractItemDrawableWithoutPointCloud class
    setCenterX( 0 );
    setCenterY( 0 );
    setCenterZ( 0 );

    setBaseDrawManager(&BEAM_DRAW_MANAGER);
}

CT_Beam::CT_Beam(const QString &modelName, const CT_AbstractResult *result, const Eigen::Vector3d &origin, const Eigen::Vector3d &direction) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{
    Q_ASSERT( !(direction(0) == 0 && direction(1) == 0 && direction(2) == 0) );

    m_shot = new CT_Shot(origin, direction.normalized());

    // Setting the center attribute from the CT_AbstractItemDrawableWithoutPointCloud class
    setCenterCoordinate(origin);

    setBaseDrawManager(&BEAM_DRAW_MANAGER);
}

CT_Beam::~CT_Beam()
{
    delete m_shot;
}

bool CT_Beam::intersect(const Eigen::Vector3d& bot, const Eigen::Vector3d& top, Eigen::Vector3d &nearP, Eigen::Vector3d &farP) const
{
    double t0 = 0;
    double t1 = std::numeric_limits<double>::max();

    if (!updateIntervals(bot(0), top(0), m_shot->getOrigin()(0), m_shot->getDirection()(0), t0, t1)) {return false;}
    if (!updateIntervals(bot(1), top(1), m_shot->getOrigin()(1), m_shot->getDirection()(1), t0, t1)) {return false;}
    if (!updateIntervals(bot(2), top(2), m_shot->getOrigin()(2), m_shot->getDirection()(2), t0, t1)) {return false;}


    nearP = m_shot->getOrigin() + m_shot->getDirection()*t0;
    farP  = m_shot->getOrigin() + m_shot->getDirection()*t1;

    return true;
}

bool CT_Beam::intersect(const Eigen::Vector3d& bot, const Eigen::Vector3d& top) const
{
    double t0 = 0;
    double t1 = std::numeric_limits<double>::max();

    if (!updateIntervals(bot(0), top(0), m_shot->getOrigin()(0), m_shot->getDirection()(0), t0, t1)) {return false;}
    if (!updateIntervals(bot(1), top(1), m_shot->getOrigin()(1), m_shot->getDirection()(1), t0, t1)) {return false;}
    if (!updateIntervals(bot(2), top(2), m_shot->getOrigin()(2), m_shot->getDirection()(2), t0, t1)) {return false;}

    return true;
}

bool CT_Beam::updateIntervals(const double &bot, const double &top, const double &origin, const double &direction, double &t0, double &t1) const
{
    // Update interval for bounding box slab
    double invRayDir = 1.f / direction;
    double tNear = (bot - origin) * invRayDir;
    double tFar  = (top - origin) * invRayDir;

    // Update parametric interval from slab intersection $t$s
    if (tNear > tFar) std::swap(tNear, tFar);

    t0 = tNear > t0 ? tNear : t0;
    t1 = tFar  < t1 ? tFar  : t1;

    if (t0 > t1 && t0 - t1 > EPSILON_INTERSECTION_RAY ) // t0 being always > t1, (t0-t1) is always positive
    {
        return false;
    }
    return true;
}

CT_Beam* CT_Beam::copy(const CT_OutAbstractItemModel *model,
                     const CT_AbstractResult *result,
                     CT_ResultCopyModeList copyModeList)
{
    CT_Beam *ray = new CT_Beam((const CT_OutAbstractSingularItemModel *)model, result, m_shot->getOrigin(), m_shot->getDirection());
    ray->setId(id());

    ray->setAlternativeDrawManager(getAlternativeDrawManager());

    return ray;
}
