/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michaël KREBS (ARTS/ENSAM)

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

#include "ct_circledata.h"
#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"
#include "ct_cloud/ct_standardcloudstdvectort.h"
#include "ct_iterator/ct_pointiterator.h"

#include <math.h>

CT_CircleData::CT_CircleData() : CT_ShapeData()
{
    _radius = 0;
    _error = 0;
}

CT_CircleData::CT_CircleData(const Eigen::Vector3d &center, const Eigen::Vector3d &direction, double radius) : CT_ShapeData(center, direction)
{
    _radius = radius;
    _error = 0;
}

CT_CircleData::CT_CircleData(const Eigen::Vector3d &center, const Eigen::Vector3d &direction, double radius, double error) : CT_ShapeData(center, direction)
{
    _radius = radius;
    _error = error;
}

void CT_CircleData::setRadius(double radius)
{
    _radius = radius;
}

void CT_CircleData::setError(double error)
{
    _error = error;
}

double CT_CircleData::getRadius() const
{
    return _radius;
}

double CT_CircleData::getError() const
{
    return _error;
}

CT_CircleData* CT_CircleData::clone() const
{
    return new CT_CircleData(getCenter(), getDirection(), getRadius(), getError());
}

CT_CircleData& CT_CircleData::operator= (const CT_CircleData& o)
{
    setRadius(o.getRadius());
    setError(o.getError());
    setCenter(o.getCenter());
    setDirection(o.getDirection());

    return *this;
}

CT_CircleData* CT_CircleData::staticCreateZAxisAlignedCircleDataFromPointCloud(const CT_AbstractPointCloudIndex &pointCloudIndex,
                                                                               double z)
{
    return staticCreateZAxisAlignedCircleDataFromPointCloudWithPreProcessing(pointCloudIndex,
                                                                             NULL,
                                                                             z);
}

CT_CircleData* CT_CircleData::staticCreateZAxisAlignedCircleDataFromPointCloudWithPreProcessing(const CT_AbstractPointCloudIndex &pointCloudIndex,
                                                                                                CT_CircleDataPreProcessingAction *preProcessingAction,
                                                                                                double z)
{

    size_t size = pointCloudIndex.size();

    if(size < 3)
        return NULL;

    std::vector<CT_Point> *newPointCloud = NULL;

    if(preProcessingAction != NULL)
        newPointCloud = new std::vector<CT_Point>(size);

    double somme_x = 0;
    double somme_y = 0;
    double somme_x_fois_y = 0;
    double somme_x_au_carre = 0;
    double somme_y_au_carre = 0;
    double somme_x_au_cube = 0;
    double somme_y_au_cube = 0;
    double somme_x_au_carre_fois_y = 0;
    double somme_y_au_carre_fois_x = 0;

    double x, y;
    double x_au_carre, y_au_carre;

    size_t i = 0;
    CT_PointIterator it(&pointCloudIndex);

    while(it.hasNext())
    {
        it.next();

        const CT_Point &p = it.currentPoint();

        if(preProcessingAction != NULL)
        {
            CT_Point &newP = (*newPointCloud)[i];

            preProcessingAction->preProcessing(p, newP);

            x = newP(0);
            y = newP(1);
        }
        else
        {
            x = p(0);
            y = p(1);
        }

        x_au_carre = x*x;
        y_au_carre = y*y;

        somme_x += x;
        somme_y += y;
        somme_x_fois_y += (x * y);
        somme_x_au_carre += x_au_carre;
        somme_y_au_carre += y_au_carre;
        somme_x_au_cube += (x*x_au_carre);
        somme_y_au_cube += (y*y_au_carre);
        somme_x_au_carre_fois_y += (x_au_carre * y);
        somme_y_au_carre_fois_x += (y_au_carre * x);

        ++i;
    }

    double phi_11 = (size * somme_x_fois_y) - (somme_x * somme_y);
    double phi_20 = (size * somme_x_au_carre) - (somme_x * somme_x);
    double phi_30 = (size * somme_x_au_cube) - (somme_x_au_carre * somme_x);
    double phi_21 = (size * somme_x_au_carre_fois_y) - (somme_x_au_carre * somme_y);
    double phi_02 = (size * somme_y_au_carre) - (somme_y * somme_y);
    double phi_03 = (size * somme_y_au_cube) - (somme_y * somme_y_au_carre);
    double phi_12 = (size * somme_y_au_carre_fois_x) - (somme_x * somme_y_au_carre);

    double denom = 2 * ((phi_20 * phi_02) - (phi_11 * phi_11));

    double a = (((phi_30 + phi_12) * phi_02) - ((phi_03 + phi_21) * phi_11)) / denom;
    double b = (((phi_03 + phi_21) * phi_20) - ((phi_30 + phi_12) * phi_11)) / denom;

    double c = (1.0/((double)size)) * (somme_x_au_carre + somme_y_au_carre - (2 * a * somme_x) - (2 * b * somme_y));

    double radiusT = sqrt(c + (a*a) + (b*b));
    double r2 = 0;

    if(newPointCloud != NULL)
    {
        for(i=0; i<size; ++i)
        {
            const CT_Point &p = (*newPointCloud)[i];
            x = p(0);
            y = p(1);

            double xma = x-a;
            double ymb = y-b;

            double r = radiusT - sqrt((xma*xma) + (ymb*ymb));

            r2 += (r*r);
        }
    }
    else
    {
        it.toFront();

        while(it.hasNext())
        {
            it.next();

            const CT_Point &p = it.currentPoint();

            x = p(0);
            y = p(1);

            double xma = x-a;
            double ymb = y-b;

            double r = radiusT - sqrt((xma*xma) + (ymb*ymb));

            r2 += (r*r);
        }
    }

    r2 = sqrt(r2/((double)size));

    delete newPointCloud;

    return new CT_CircleData(Eigen::Vector3d(a, b, z),
                             Eigen::Vector3d(0, 0, 1),
                             radiusT,
                             r2);
}
