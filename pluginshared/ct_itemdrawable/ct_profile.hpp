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

#ifndef CT_PROFILE_HPP
#define CT_PROFILE_HPP

#include "ct_itemdrawable/ct_profile.h"
#include "ct_itemdrawable/tools/drawmanager/ct_standardprofiledrawmanager.h"

#include <math.h>
#include <typeinfo>
#include <limits>

#include "ct_math/ct_math.h"
#include "qdebug.h"

template< typename DataT > CT_DEFAULT_IA_INIT(CT_Profile<DataT>)

template< typename DataT>
const CT_StandardProfileDrawManager<DataT> CT_Profile<DataT>::ABSPROFILE_DRAW_MANAGER;

template< typename DataT>
CT_Profile<DataT>::CT_Profile() : CT_AbstractProfile()
{
    _minCoordinates.setX(0);
    _minCoordinates.setY(0);
    _minCoordinates.setZ(0);
    _res = 1;
    _dim = 0;
    _maxCoordinates.setX(0);
    _maxCoordinates.setY(0);
    _maxCoordinates.setZ(0);
    _NAdata = -1;
    _dataMax = -1;
    _dataMin = -1;

    setBaseDrawManager(&ABSPROFILE_DRAW_MANAGER);
}



template< typename DataT>
CT_Profile<DataT>::CT_Profile(const CT_OutAbstractSingularItemModel *model,
                              const CT_AbstractResult *result,
                              float xmin,
                              float ymin,
                              float zmin,
                              float xdir,
                              float ydir,
                              float zdir,
                              size_t dim,
                              float resolution,
                              DataT na,
                              DataT initValue) : CT_AbstractProfile(model, result)
{
    _minCoordinates.setX(xmin);
    _minCoordinates.setY(ymin);
    _minCoordinates.setZ(zmin);

    _direction.setX(xdir);
    _direction.setY(ydir);
    _direction.setZ(zdir);

    _direction.normalize();

    _res = resolution;
    _dim = dim;
    _maxLength = _dim * _res;

    _maxCoordinates.setX(xmin + _res * _dim * _direction.x());
    _maxCoordinates.setY(ymin + _res * _dim * _direction.y());
    _maxCoordinates.setZ(zmin + _res * _dim * _direction.z());

    _NAdata = na;

    setCenterX (minX() + (maxX() - minX())/2.0);
    setCenterY (minY() + (maxY() - minY())/2.0);
    setCenterZ (minZ() + (maxZ() - minZ())/2.0);

    _data.resize(nCells());

    initGridWithValue(initValue);

    setBaseDrawManager(&ABSPROFILE_DRAW_MANAGER);
}

template< typename DataT>
CT_Profile<DataT>::CT_Profile(const QString &modelName,
                              const CT_AbstractResult *result,
                              float xmin,
                              float ymin,
                              float zmin,
                              float xdir,
                              float ydir,
                              float zdir,
                              size_t dim,
                              float resolution,
                              DataT na,
                              DataT initValue) : CT_AbstractProfile(modelName, result)
{
    _minCoordinates.setX(xmin);
    _minCoordinates.setY(ymin);
    _minCoordinates.setZ(zmin);

    _direction.setX(xdir);
    _direction.setY(ydir);
    _direction.setZ(zdir);

    _direction.normalize();

    _res = resolution;
    _dim = dim;
    _maxLength = _dim * _res;

    _maxCoordinates.setX(xmin + _res * _dim * _direction.x());
    _maxCoordinates.setY(ymin + _res * _dim * _direction.y());
    _maxCoordinates.setZ(zmin + _res * _dim * _direction.z());

    _NAdata = na;

    setCenterX (minX() + (maxX() - minX())/2.0);
    setCenterY (minY() + (maxY() - minY())/2.0);
    setCenterZ (minZ() + (maxZ() - minZ())/2.0);

    _data.resize(nCells());

    initGridWithValue(initValue);

    setBaseDrawManager(&ABSPROFILE_DRAW_MANAGER);
}

template< typename DataT>
CT_Profile<DataT>* CT_Profile<DataT>::createProfileFromSegment(const CT_OutAbstractSingularItemModel *model,
                                                               const CT_AbstractResult *result,
                                                               float xmin,
                                                               float ymin,
                                                               float zmin,
                                                               float xmax,
                                                               float ymax,
                                                               float zmax,
                                                               float resolution,
                                                               DataT na,
                                                               DataT initValue)
{

    float xdir = xmax - xmin;
    float ydir = ymax - ymin;
    float zdir = zmax - zmin;

    float length = sqrt(xdir*xdir + ydir*ydir + zdir*zdir);

    int dim = ceil(length/resolution);

    CT_Profile<DataT>* grid = new CT_Profile((const CT_OutAbstractSingularItemModel *)model, result, xmin, ymin, zmin, xdir, ydir, zdir, dim, resolution, na, initValue);

    // to ensure a point exactly on a maximum limit of the grid will be included in the grid
    while (length >= grid->maxLength())
    {
        grid->_dim++;
        grid->_maxLength = grid->maxLength() + resolution;
    }

    return grid;
}

template< typename DataT>
CT_Profile<DataT>* CT_Profile<DataT>::createProfileFromSegment(const QString &modelName,
                                                               const CT_AbstractResult *result,
                                                               float xmin,
                                                               float ymin,
                                                               float zmin,
                                                               float xmax,
                                                               float ymax,
                                                               float zmax,
                                                               float resolution,
                                                               DataT na,
                                                               DataT initValue)
{

    float xdir = xmax - xmin;
    float ydir = ymax - ymin;
    float zdir = zmax - zmin;

    float length = sqrt(xdir*xdir + ydir*ydir + zdir*zdir);

    int dim = ceil(length/resolution);

    CT_Profile<DataT>* grid = new CT_Profile(modelName, result, xmin, ymin, zmin, xdir, ydir, zdir, dim, resolution, na, initValue);

    // to ensure a point exactly on a maximum limit of the grid will be included in the grid
    while (length >= grid->maxLength())
    {
        grid->_dim++;
        grid->_maxLength = grid->maxLength() + resolution;
    }

    return grid;
}


template< typename DataT>
CT_Profile<DataT>::~CT_Profile()
{
    _data.clear();
}

template< typename DataT>
void CT_Profile<DataT>::initGridWithValue(const DataT &val)
{
    for (size_t  i = 0 ; i < nCells() ; i++)
    {
        _data[i] = val;
    }
    _dataMin = val;
    _dataMax = val;
}


template< typename DataT>
void CT_Profile<DataT>::setValueAtIndexFromDouble(const size_t &index, const double &value)
{
    setValueAtIndex(index, (DataT) value);
}


template< typename DataT>
void CT_Profile<DataT>::computeMinMax()
{
    size_t ncells = nCells();
    if (ncells > 0) {
        _dataMin = _data[0];
        _dataMax = _data[0];

        for (size_t i = 1 ; i < ncells ; i++)
        {
            if (_data[i] != NA())
            {
                if (_dataMax==NA() || _data[i] > _dataMax) {_dataMax = _data[i];}
                if (_dataMin==NA() || _data[i] < _dataMin) {_dataMin = _data[i];}
            }
        }
    }
}



template< typename DataT>
QString CT_Profile<DataT>::getType() const
{
    return staticGetType();
}

template< typename DataT>
QString CT_Profile<DataT>::staticGetType()
{
    return CT_AbstractProfile::staticGetType() + "/CT_Profile<" + typeid(DataT).name() + ">";
}

template< typename DataT>
QString CT_Profile<DataT>::name() const
{
    return QString("CT_Profile<") + typeid(DataT).name() + QString(">");
}

template< typename DataT>
bool CT_Profile<DataT>::setValueAtIndex(const size_t &index, const DataT &value)
{
    if (index >= nCells()) {return false;}
    _data[index] = value;

    return true;
}

template< typename DataT>
bool CT_Profile<DataT>::setValueAtLength(const float &l, const DataT &value)
{
    size_t index;
    if (indexForLength(l, index))
    {
        return setValueAtIndex(index, value);
    }
    return false;
}

template< typename DataT>
bool CT_Profile<DataT>::setValueForXYZ(const float &x, const float &y, const float &z, const DataT &value)
{
    size_t index;
    if (indexForXYZ(x, y, z, index))
    {
        return setValueAtIndex(index, value);
    }
    return false;
}


template< typename DataT>
DataT CT_Profile<DataT>::valueAtIndex(const size_t &index) const
{
    if (index >= nCells()) {return NA();}
    return _data[index];
}

template< typename DataT>
float CT_Profile<DataT>::ratioValueAtIndex(const size_t &index) const
{
    if (_dataMax <= _dataMin) {return 1;}
    DataT value = valueAtIndex(index);
    if (value == NA()) {return -1;}
    return (float) (((double)(value - _dataMin))/((double)(_dataMax - _dataMin)));
}

template< typename DataT>
double CT_Profile<DataT>::valueAtIndexAsDouble(const size_t &index) const
{
    DataT value = valueAtIndex(index);
    if (value == NA()) {return std::numeric_limits<DataT>::quiet_NaN();}
    return value;
}

template< typename DataT>
QString CT_Profile<DataT>::valueAtIndexAsString(const size_t &index) const
{
    DataT value = valueAtIndex(index);
    return QString::number(value);
}

template< typename DataT>
QString CT_Profile<DataT>::NAAsString() const
{
    return QString::number(NA());
}

template< typename DataT>
DataT CT_Profile<DataT>::valueForXYZ(const float &x, const float &y, const float &z) const
{
    size_t index;
    if (indexForXYZ(x, y, z, index))
    {
        return valueAtIndex(index);
    }
    return NA();
}


template< typename DataT>
bool CT_Profile<DataT>::setMaxValueAtIndex(const size_t &index, const DataT &value)
{
    if (index >= nCells()) {return false;}

    DataT currentValue = _data[index];

    if ((currentValue == NA()) || (value > currentValue)) {
        return setValueAtIndex(index, value);
    }
    return false;
}

template< typename DataT>
bool CT_Profile<DataT>::setMaxValueAtLength(const float &l, const DataT &value)
{
    size_t index;
    if (indexForLength(l, index))
    {
        return setMaxValueAtIndex(index, value);
    }
    return false;
}

template< typename DataT>
bool CT_Profile<DataT>::setMaxValueForXYZ(const float &x, const float &y, const float &z, const DataT &value)
{
    size_t index;
    if (indexForXYZ(x, y, z, index))
    {
        return setMaxValueAtIndex(index, value);
    }
    return false;
}


template< typename DataT>
bool CT_Profile<DataT>::setMinValueAtIndex(const size_t &index, const DataT &value)
{
    if (index >= nCells()) {return false;}

    DataT currentValue = _data[index];

    if ((currentValue == NA()) || (value < currentValue)) {
        return setValueAtIndex(index, value);
    }
    return false;
}

template< typename DataT>
bool CT_Profile<DataT>::setMinValueAtLength(const float &l, const DataT &value)
{
    size_t index;
    if (indexForLength(l, index))
    {
        return setMinValueAtIndex(index, value);
    }
    return false;
}

template< typename DataT>
bool CT_Profile<DataT>::setMinValueForXYZ(const float &x, const float &y, const float &z, const DataT &value)
{
    size_t index;
    if (indexForXYZ(x, y, z, index))
    {
        return setMinValueAtIndex(index, value);
    }
    return false;
}

template< typename DataT>
bool CT_Profile<DataT>::addValueAtIndex(const size_t &index, const DataT &value)
{
    if (index >= nCells()) {return false;}

    DataT currentValue = _data[index];

    if (currentValue == NA())
    {
        return setValueAtIndex(index, value);
    } else if (value != NA())
    {
        return setValueAtIndex(index, value + currentValue);
    }
    return false;
}

template< typename DataT>
bool CT_Profile<DataT>::addValueAtLength(const float &l, const DataT &value)
{
    size_t index;
    if (indexForLength(l, index))
    {
        return addValueAtIndex(index, value);
    }
    return false;
}

template< typename DataT>
bool CT_Profile<DataT>::addValueForXYZ(const float &x, const float &y, const float &z, const DataT &value)
{
    size_t index;
    if (indexForXYZ(x, y, z, index))
    {
        return addValueAtIndex(index, value);
    }
    return false;
}

template< typename DataT>
QList<DataT> CT_Profile<DataT>::neighboursValues(const size_t &index, const size_t &distance, const bool keepNAs, const CenterMode centermode) const
{
    QList<DataT> liste;

    if (distance < 1) {return liste;}
    if (index >= _dim) {return liste;}

    size_t firstCol = index-distance;
    size_t lastCol = index+distance;

    if (firstCol >= _dim) {firstCol = 0;}
    if (lastCol >= _dim) {lastCol = _dim - 1;}


    for (size_t ii = firstCol ; ii <= lastCol ; ii++)
    {

        DataT val = valueAtIndex(ii);
        if ((ii == index)) {
            if (centermode == CM_KeepCenter) {
                if ((val != NA()) || keepNAs) {liste.append(val);}
            } else if (centermode == CM_NAasCenter) {
                liste.append(NA());
            }
        } else {
            if ((val != NA()) || keepNAs) {liste.append(val);}
        }
    }

    return liste;
}

template< typename DataT>
CT_AbstractItemDrawable* CT_Profile<DataT>::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);

    CT_Profile<DataT>* cpy = new CT_Profile<DataT>((const CT_OutAbstractSingularItemModel *)model, result, minX(), minY(), minZ(), _direction.x(), _direction.y(), _direction.z(), _dim, _res, _NAdata, _NAdata);
    cpy->setId(id());

    for (size_t i = 0 ; i < nCells() ; i++)
    {
        cpy->setValueAtIndex(i, valueAtIndex(i));
    }

    if (nCells() >0)
    {
        cpy->computeMinMax();
    }
    cpy->setAlternativeDrawManager(getAlternativeDrawManager());

    return cpy;
}

template< typename DataT>
CT_AbstractItemDrawable* CT_Profile<DataT>::copy(const QString &modelName, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);

    CT_Profile<DataT>* cpy = new CT_Profile<DataT>(modelName, result, minX(), minY(), minZ(), _direction.x(), _direction.y(), _direction.z(), _dim, _res, _NAdata, _NAdata);
    cpy->setId(id());

    for (size_t i = 0 ; i < nCells() ; i++)
    {
        cpy->setValueAtIndex(i, valueAtIndex(i));
    }

    if (nCells() >0)
    {
        cpy->computeMinMax();
    }
    cpy->setAlternativeDrawManager(getAlternativeDrawManager());

    return cpy;
}

#endif // CT_PROFILE_HPP
