#ifndef CT_GRID4D_DENSE_HPP
#define CT_GRID4D_DENSE_HPP

#ifdef USE_OPENCV

#include "ct_itemdrawable/ct_grid4d_dense.h"

#include <math.h>
#include <typeinfo>

#include "ct_math/ct_math.h"

template< typename DataT>
CT_Grid4D_Dense<DataT>::CT_Grid4D_Dense() : CT_Grid4D<DataT>()
{
    int ncells[1];
    ncells[0] = this->nCells();
    this->_data.create(1, ncells);
}

template< typename DataT>
CT_Grid4D_Dense<DataT>::CT_Grid4D_Dense(const CT_OutAbstractSingularItemModel *model,
                                        const CT_AbstractResult *result,
                                        double wmin,
                                        double xmin,
                                        double ymin,
                                        double zmin,
                                        size_t dimw,
                                        size_t dimx,
                                        size_t dimy,
                                        size_t dimz,
                                        double resw,
                                        double resx,
                                        double resy,
                                        double resz,
                                        DataT na,
                                        DataT initValue) : CT_Grid4D<DataT>(model,
                                                                            result,
                                                                            wmin,
                                                                            xmin,
                                                                            ymin,
                                                                            zmin,
                                                                            dimw,
                                                                            dimx,
                                                                            dimy,
                                                                            dimz,
                                                                            resw,
                                                                            resx,
                                                                            resy,
                                                                            resz,
                                                                            na)
{
    int ncells[1];
    ncells[0] = this->nCells();
    this->_data.create(1, ncells);
    initGridWithValue(initValue);
}

template< typename DataT>
CT_Grid4D_Dense<DataT>::CT_Grid4D_Dense(const QString &modelName,
                                        const CT_AbstractResult *result,
                                        double wmin,
                                        double xmin,
                                        double ymin,
                                        double zmin,
                                        size_t dimw,
                                        size_t dimx,
                                        size_t dimy,
                                        size_t dimz,
                                        double resw,
                                        double resx,
                                        double resy,
                                        double resz,
                                        DataT na,
                                        DataT initValue) : CT_Grid4D<DataT>(modelName,
                                                                            result,
                                                                            wmin,
                                                                            xmin,
                                                                            ymin,
                                                                            zmin,
                                                                            dimw,
                                                                            dimx,
                                                                            dimy,
                                                                            dimz,
                                                                            resw,
                                                                            resx,
                                                                            resy,
                                                                            resz,
                                                                            na)
{
    int ncells[1];
    ncells[0] = this->nCells();
    this->_data.create(1, ncells);
    initGridWithValue(initValue);
}

template< typename DataT>
CT_Grid4D_Dense<DataT>::CT_Grid4D_Dense(const CT_OutAbstractSingularItemModel *model,
                                        const CT_AbstractResult *result,
                                        double wmin,
                                        double xmin,
                                        double ymin,
                                        double zmin,
                                        double wmax,
                                        double xmax,
                                        double ymax,
                                        double zmax,
                                        double resw,
                                        double resx,
                                        double resy,
                                        double resz,
                                        DataT na,
                                        DataT initValue) : CT_Grid4D<DataT>(model,
                                                                            result,
                                                                            wmin,
                                                                            xmin,
                                                                            ymin,
                                                                            zmin,
                                                                            wmax,
                                                                            xmax,
                                                                            ymax,
                                                                            zmax,
                                                                            resw,
                                                                            resx,
                                                                            resy,
                                                                            resz,
                                                                            na)
{
    int ncells[1];
    ncells[0] = this->nCells();
    initGridWithValue(initValue);
}

template< typename DataT>
CT_Grid4D_Dense<DataT>::CT_Grid4D_Dense(const QString& modelName,
                                        const CT_AbstractResult *result,
                                        double wmin,
                                        double xmin,
                                        double ymin,
                                        double zmin,
                                        double wmax,
                                        double xmax,
                                        double ymax,
                                        double zmax,
                                        double resw,
                                        double resx,
                                        double resy,
                                        double resz,
                                        DataT na,
                                        DataT initValue) : CT_Grid4D<DataT>(modelName,
                                                                            result,
                                                                            wmin,
                                                                            xmin,
                                                                            ymin,
                                                                            zmin,
                                                                            wmax,
                                                                            xmax,
                                                                            ymax,
                                                                            zmax,
                                                                            resw,
                                                                            resx,
                                                                            resy,
                                                                            resz,
                                                                            na)
{
    int ncells[1];
    ncells[0] = this->nCells();
    this->_data.create(1, ncells);
    initGridWithValue(initValue);
}

template< typename DataT>
CT_Grid4D_Dense<DataT>* CT_Grid4D_Dense<DataT>::createGrid4DFromWXYZCoords(const CT_OutAbstractSingularItemModel *model,
                                                                           const CT_AbstractResult *result,
                                                                           double wmin,
                                                                           double xmin,
                                                                           double ymin,
                                                                           double zmin,
                                                                           double wmax,
                                                                           double xmax,
                                                                           double ymax,
                                                                           double zmax,
                                                                           double resw,
                                                                           double resx,
                                                                           double resy,
                                                                           double resz,
                                                                           DataT na,
                                                                           DataT initValue)
{
    size_t dimw = ceil((wmax - wmin)/resw);
    size_t dimx = ceil((xmax - xmin)/resx);
    size_t dimy = ceil((ymax - ymin)/resy);
    size_t dimz = ceil((zmax - zmin)/resz);

    // to ensure a point exactly on a maximum limit of the grid will be included in the grid
    while (wmax >= (wmin + dimw * resw))
    {
        dimw++;
    }

    while (xmax >= (xmin + dimx * resx))
    {
        dimx++;
    }

    while (ymax >= (ymin + dimy * resy))
    {
        dimy++;
    }

    while (zmax >= (zmin + dimz * resz))
    {
        dimz++;
    }

    return new CT_Grid4D_Dense<DataT>(model, result, wmin, xmin, ymin, zmin, dimw, dimx, dimy, dimz, resw, resx, resy, resz, na, initValue);
}

template< typename DataT>
CT_Grid4D_Dense<DataT>* CT_Grid4D_Dense<DataT>::createGrid4DFromWXYZCoords(const QString &modelName,
                                                                           const CT_AbstractResult *result,
                                                                           double wmin,
                                                                           double xmin,
                                                                           double ymin,
                                                                           double zmin,
                                                                           double wmax,
                                                                           double xmax,
                                                                           double ymax,
                                                                           double zmax,
                                                                           double resw,
                                                                           double resx,
                                                                           double resy,
                                                                           double resz,
                                                                           DataT na,
                                                                           DataT initValue)
{
    size_t dimw = ceil((wmax - wmin)/resw);
    size_t dimx = ceil((xmax - xmin)/resx);
    size_t dimy = ceil((ymax - ymin)/resy);
    size_t dimz = ceil((zmax - zmin)/resz);

    // to ensure a point exactly on a maximum limit of the grid will be included in the grid
    while (wmax >= (wmin + dimw * resw))
    {
        dimw++;
    }

    while (xmax >= (xmin + dimx * resx))
    {
        dimx++;
    }

    while (ymax >= (ymin + dimy * resy))
    {
        dimy++;
    }

    while (zmax >= (zmin + dimz * resz))
    {
        dimz++;
    }

    return new CT_Grid4D_Dense<DataT>(modelName, result, wmin, xmin, ymin, zmin, dimw, dimx, dimy, dimz, resw, resx, resy, resz, na, initValue);
}

template< typename DataT>
CT_Grid4D_Dense<DataT>::~CT_Grid4D_Dense()
{
    _data.release();
}

template< typename DataT>
void CT_Grid4D_Dense<DataT>::initGridWithValue(const DataT val)
{
    for (size_t  i = 0 ; i < this->nCells() ; i++)
    {
        _data(i) = val;
    }
    this->_dataMin = val;
    this->_dataMax = val;
}

template< typename DataT>
CT_AbstractItemDrawable* CT_Grid4D_Dense<DataT>::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);

    CT_Grid4D_Dense<DataT>* cpy = new CT_Grid4D_Dense<DataT>((const CT_OutAbstractSingularItemModel *)model, result, this->_bot.w(), this->_bot.x(), this->_bot.y(), this->_bot.z(), this->_dimw, this->_dimx, this->_dimy, this->_dimz, this->_resw, this->_resx, this->_resy, this->_resz, this->_NAdata, this->_NAdata);
    cpy->setId(this->id());

    size_t ncells = this->nCells();
    for (size_t i = 0 ; i < ncells ; i++)
    {
        cpy->setValueAtIndex(i, valueAtIndex(i));
    }

    if ( ncells > 0 )
    {
        cpy->computeMinMax();
    }

    cpy->setAlternativeDrawManager( this->getAlternativeDrawManager() );

    return cpy;
}

template< typename DataT>
CT_AbstractItemDrawable* CT_Grid4D_Dense<DataT>::copy(const QString &modelName, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);

    CT_Grid4D_Dense<DataT>* cpy = new CT_Grid4D_Dense<DataT>(modelName, result, this->_bot.w(), this->_bot.x(), this->_bot.y(), this->_bot.z(), this->_dimw, this->_dimx, this->_dimy, this->_dimz, this->_resw, this->_resx, this->_resy, this->_resz, this->_NAdata, this->_NAdata);
    cpy->setId(this->id());

    size_t ncells = this->nCells();
    for (size_t i = 0 ; i < ncells ; i++)
    {
        cpy->setValueAtIndex(i, valueAtIndex(i));
    }

    if ( ncells > 0 )
    {
        cpy->computeMinMax();
    }

    cpy->setAlternativeDrawManager( this->getAlternativeDrawManager() );

    return cpy;
}

template< typename DataT>
QString CT_Grid4D_Dense<DataT>::getType() const
{
    return staticGetType();
}

template< typename DataT>
QString CT_Grid4D_Dense<DataT>::staticGetType()
{
    QString type = CT_AbstractGrid4D::staticGetType() + "/CT_Grid4D_Dense<" + CT_TypeInfo::name<DataT>() + ">";
    CT_AbstractItemDrawable::addNameTypeCorresp(type, staticName());
    return type;
}

template< typename DataT>
QString CT_Grid4D_Dense<DataT>::name() const
{
    return staticName();
}

template< typename DataT>
QString CT_Grid4D_Dense<DataT>::staticName()
{
    return CT_Grid4D_Dense<DataT>::tr("4D grid<%1>, dense").arg(CT_TypeInfo::name<DataT>());
}


#endif

#endif // CT_GRID4D_DENSE_HPP
