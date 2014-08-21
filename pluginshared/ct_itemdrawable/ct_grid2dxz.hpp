/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forets (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre PIBOULE (ONF)

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

#ifndef CT_GRID2DXZ_HPP
#define CT_GRID2DXZ_HPP

#include "ct_itemdrawable/ct_grid2dxz.h"
#include "ct_itemdrawable/tools/drawmanager/ct_standardgrid2dxzdrawmanager.h"
#include <math.h>
#include <typeinfo>

#include "qdebug.h"

template< typename DataT>
const CT_StandardGrid2DXZDrawManager<DataT> CT_Grid2DXZ<DataT>::ABSGRID2DXZ_DRAW_MANAGER;

template< typename DataT>
CT_Grid2DXZ<DataT>::CT_Grid2DXZ() : CT_VirtualGrid2D<DataT>()
{
    this->setBaseDrawManager(&ABSGRID2DXZ_DRAW_MANAGER);
}

template< typename DataT>
CT_Grid2DXZ<DataT>::CT_Grid2DXZ(const CT_OutAbstractSingularItemModel *model,
                                const CT_AbstractResult *result,
                                float xmin,
                                float zmin,
                                size_t dimx,
                                size_t dimz,
                                float resolution,
                                float ylevel,
                                DataT na,
                                DataT initValue) : CT_VirtualGrid2D<DataT>(model, result)
{
    this->_minCoordinates.setX(xmin);
    this->_minCoordinates.setY(ylevel);
    this->_minCoordinates.setZ(zmin);
    this->_res = resolution;
    this->_dimCol = dimx;
    this->_dimLin = dimz;
    this->_maxCoordinates.setX(this->minX() + this->_res * this->_dimCol);
    this->_maxCoordinates.setY(ylevel);
    this->_maxCoordinates.setZ(this->minZ() + this->_res * this->_dimLin);

    this->_minColCoord = xmin;
    this->_minLinCoord = zmin;

    this->_level = ylevel;
    this->_NAdata = na;

    this->setCenterX (this->minX() + (this->maxX() - this->minX())/2.0);
    this->setCenterY (ylevel);
    this->setCenterZ (this->minZ() + (this->maxZ() - this->minZ())/2.0);

    this->_data.resize(this->nCells());
    this->initGridWithValue(initValue);

    this->setBaseDrawManager(&ABSGRID2DXZ_DRAW_MANAGER);
}

template< typename DataT>
CT_Grid2DXZ<DataT>::CT_Grid2DXZ(const QString &modelName,
                                const CT_AbstractResult *result,
                                float xmin,
                                float zmin,
                                size_t dimx,
                                size_t dimz,
                                float resolution,
                                float ylevel,
                                DataT na,
                                DataT initValue) : CT_VirtualGrid2D<DataT>(modelName, result)
{
    this->_minCoordinates.setX(xmin);
    this->_minCoordinates.setY(ylevel);
    this->_minCoordinates.setZ(zmin);
    this->_res = resolution;
    this->_dimCol = dimx;
    this->_dimLin = dimz;
    this->_maxCoordinates.setX(this->minX() + this->_res * this->_dimCol);
    this->_maxCoordinates.setY(ylevel);
    this->_maxCoordinates.setZ(this->minZ() + this->_res * this->_dimLin);

    this->_minColCoord = xmin;
    this->_minLinCoord = zmin;

    this->_level = ylevel;
    this->_NAdata = na;

    this->setCenterX (this->minX() + (this->maxX() - this->minX())/2.0);
    this->setCenterY (ylevel);
    this->setCenterZ (this->minZ() + (this->maxZ() - this->minZ())/2.0);

    this->_data.resize(this->nCells());
    this->initGridWithValue(initValue);

    this->setBaseDrawManager(&ABSGRID2DXZ_DRAW_MANAGER);
}

template< typename DataT>
CT_Grid2DXZ<DataT>* CT_Grid2DXZ<DataT>::createGrid2DXZFromXZCoords(const CT_OutAbstractSingularItemModel *model,
                                                                   const CT_AbstractResult *result,
                                                                   float xmin,
                                                                   float zmin,
                                                                   float xmax,
                                                                   float zmax,
                                                                   float resolution,
                                                                   float ylevel,
                                                                   DataT na,
                                                                   DataT initValue)
{
    size_t dimx = ceil((xmax - xmin)/resolution);
    size_t dimz = ceil((zmax - zmin)/resolution);

    CT_Grid2DXZ<DataT>* grid = new CT_Grid2DXZ((const CT_OutAbstractSingularItemModel *)model, result, xmin, zmin, dimx, dimz, resolution, ylevel, na, initValue);

    // to ensure a point exactly on a maximum limit of the grid will be included in the grid
    while (xmax >= grid->maxX())
    {
        grid->_dimCol++;
        grid->_maxCoordinates.setX(grid->maxX() + resolution);
    }

    while (zmax >= grid->maxZ())
    {
        grid->_dimLin++;
        grid->_maxCoordinates.setZ(grid->maxZ() + resolution);
    }

    return grid;
}

template< typename DataT>
CT_Grid2DXZ<DataT>* CT_Grid2DXZ<DataT>::createGrid2DXZFromXZCoords(const QString &modelName,
                                                                   const CT_AbstractResult *result,
                                                                   float xmin,
                                                                   float zmin,
                                                                   float xmax,
                                                                   float zmax,
                                                                   float resolution,
                                                                   float ylevel,
                                                                   DataT na,
                                                                   DataT initValue)
{
    size_t dimx = ceil((xmax - xmin)/resolution);
    size_t dimz = ceil((zmax - zmin)/resolution);

    CT_Grid2DXZ<DataT>* grid = new CT_Grid2DXZ(modelName, result, xmin, zmin, dimx, dimz, resolution, ylevel, na, initValue);

    // to ensure a point exactly on a maximum limit of the grid will be included in the grid
    while (xmax >= grid->maxX())
    {
        grid->_dimCol++;
        grid->_maxCoordinates.setX(grid->maxX() + resolution);
    }

    while (zmax >= grid->maxZ())
    {
        grid->_dimLin++;
        grid->_maxCoordinates.setZ(grid->maxZ() + resolution);
    }

    return grid;
}

template< typename DataT>
CT_Grid2DXZ<DataT>::~CT_Grid2DXZ()
{

}

template< typename DataT>
void CT_Grid2DXZ<DataT>::initDrawManager(QString drawConfigurationName, bool mapMode, bool scale)
{
    this->setBaseDrawManager(new CT_StandardGrid2DXZDrawManager<DataT>(drawConfigurationName, mapMode, scale));
}

template< typename DataT>
QString CT_Grid2DXZ<DataT>::getType() const
{
    return staticGetType();
}

template< typename DataT>
QString CT_Grid2DXZ<DataT>::staticGetType()
{
    return CT_AbstractGrid2D::staticGetType() + "/CT_Grid2DXZ<" + typeid(DataT).name() + ">";
}

template< typename DataT>
QString CT_Grid2DXZ<DataT>::name() const
{
    return QString("CT_Grid2DXZ<") + typeid(DataT).name() + QString(">");
}

template< typename DataT>
CT_AbstractItemDrawable* CT_Grid2DXZ<DataT>::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    CT_Grid2DXZ<DataT>* cpy = new CT_Grid2DXZ<DataT>((const CT_OutAbstractSingularItemModel *)model, result, this->minX(), this->minZ(), this->_dimCol, this->_dimLin, this->_res, this->_level, this->_NAdata, this->_NAdata);
    cpy->setId(this->id());
    size_t size = this->nCells();
    for (size_t i = 0 ; i < size ; i++)
    {
        cpy->_data[i] = this->_data[i];
    }
    if (size >0)
    {
        cpy->computeMinMax();
    }
    cpy->setAlternativeDrawManager(this->getAlternativeDrawManager());

    return cpy;
}

template< typename DataT>
CT_AbstractItemDrawable* CT_Grid2DXZ<DataT>::copy(const QString &modelName, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    CT_Grid2DXZ<DataT>* cpy = new CT_Grid2DXZ<DataT>(modelName, result, this->minX(), this->minZ(), this->_dimCol, this->_dimLin, this->_res, this->_level, this->_NAdata, this->_NAdata);
    cpy->setId(this->id());
    size_t size = this->nCells();
    for (size_t i = 0 ; i < size ; i++)
    {
        cpy->_data[i] = this->_data[i];
    }
    if (size >0)
    {
        cpy->computeMinMax();
    }
    cpy->setAlternativeDrawManager(this->getAlternativeDrawManager());

    return cpy;
}

#endif // CT_GRID2DXZ_HPP
