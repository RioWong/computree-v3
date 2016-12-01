/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forets (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Metiers (ENSAM), Cluny, France.
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

#ifndef CT_GRID3D_SPARSE_H
#define CT_GRID3D_SPARSE_H

#ifdef USE_OPENCV

#include "ct_itemdrawable/abstract/ct_abstractgrid3d.h"
#include "ct_itemdrawable/tools/ct_itemplateddata3darray.h"

#include "opencv2/core/core.hpp"

#include "ct_math/ct_math.h"
#include <QMutex>

template< typename DataT > class CT_StandardGrid3DDrawManager;

/*!
 * \class CT_Grid3D_Sparse
 * \ingroup PluginShared_Items
 * \brief <b>ItemDrawable managing a 3D grid</b>
 *
 * It represents grid in 3D, with templated values in cells.
 *
 */
template< typename DataT>
class CT_Grid3D_Sparse : public CT_AbstractGrid3D, public CT_ITemplatedData3DArray<DataT>
{
public:

    /*!
     *  \brief How to manage central cell when using neighboursValues method
     *
     */
    enum CenterMode
    {
        CM_KeepCenter = 1,      /*!< Gives neighbours and central value */
        CM_NAasCenter = 2,      /*!< Gives neighbours and NA for central value */
        CM_DropCenter = 3       /*!< Gives neighbours only */
    };

    /**
      * \brief Empty Contructor vide
      */
    CT_Grid3D_Sparse();

    /*!
     * \brief Contructor with integer column and row coordinates
     *
     * \param model Item model for creation
     * \param result Result containing the item
     * \param xmin Minimum X coordinate (bottom left corner)
     * \param ymin Minimum Y coordinate (bottom left corner)
     * \param zmin Minimum Z coordinate (bottom left corner)
     * \param dimx Number of colums
     * \param dimy Number of rows
     * \param dimz Number of zlevels
     * \param resolution Size of a cell
     * \param na Value used to code NA
     * \param initValue Initialisation value for grid cells
     */
    CT_Grid3D_Sparse(const CT_OutAbstractSingularItemModel *model,
              const CT_AbstractResult *result,
              double xmin,
              double ymin,
              double zmin,
              size_t dimx,
              size_t dimy,
              size_t dimz,
              double resolution,
              DataT na,
              DataT initValue);

    CT_Grid3D_Sparse(const QString &modelName,
              const CT_AbstractResult *result,
              double xmin,
              double ymin,
              double zmin,
              size_t dimx,
              size_t dimy,
              size_t dimz,
              double resolution,
              DataT na,
              DataT initValue);

    /*!
     * \brief Constructor with min and max (X,Y) coordinates
     *
     * \param model Item model for creation
     * \param result Result containing the item
     * \param xmin Minimum X coordinate (bottom left corner)
     * \param ymin Minimum Y coordinate (bottom left corner)
     * \param zmin Minimum Z coordinate (bottom left corner)
     * \param xmax Maximum X coordinate (upper right corner)
     * \param ymax Maximum Y coordinate (upper right corner)
     * \param zmax Maximum Z coordinate (upper right corner)
     * \param resolution Size of a cell
     * \param na Value used to code NA
     * \param initValue Initialisation value for grid cells
     * \param coordConstructor Not used, only to ensure constructor different signatures
     */
    CT_Grid3D_Sparse(const CT_OutAbstractSingularItemModel *model,
              const CT_AbstractResult *result,
              double xmin,
              double ymin,
              double zmin,
              double xmax,
              double ymax,
              double zmax,
              double resolution,
              DataT na,
              DataT initValue);

    CT_Grid3D_Sparse(const QString& model,
              const CT_AbstractResult *result,
              double xmin,
              double ymin,
              double zmin,
              double xmax,
              double ymax,
              double zmax,
              double resolution,
              DataT na,
              DataT initValue);


    /*!
     * \brief Factory with min and max (X,Y,Z) coordinates
     *
     * \param model Item model for creation
     * \param result Result containing the item
     * \param xmin Minimum X coordinate (bottom left corner)
     * \param ymin Minimum Y coordinate (bottom left corner)
     * \param zmin Minimum Z coordinate (bottom left corner)
     * \param xmax Maximum X coordinate (upper right corner)
     * \param ymax Maximum Y coordinate (upper right corner)
     * \param zmax Maximum Z coordinate (upper right corner)
     * \param resolution Size of a cell
     * \param na Value used to code NA
     * \param initValue Initialisation value for grid cells
     * \param coordConstructor Not used, only to ensure constructor different signatures
     */
    static CT_Grid3D_Sparse<DataT>* createGrid3DFromXYZCoords(const CT_OutAbstractSingularItemModel *model,
                                                          const CT_AbstractResult *result,
                                                          double xmin,
                                                          double ymin,
                                                          double zmin,
                                                          double xmax,
                                                          double ymax,
                                                          double zmax,
                                                          double resolution,
                                                          DataT na,
                                                          DataT initValue,
                                                          bool extends = true);

    static CT_Grid3D_Sparse<DataT>* createGrid3DFromXYZCoords(const QString &modelName,
                                                          const CT_AbstractResult *result,
                                                          double xmin,
                                                          double ymin,
                                                          double zmin,
                                                          double xmax,
                                                          double ymax,
                                                          double zmax,
                                                          double resolution,
                                                          DataT na,
                                                          DataT initValue,
                                                          bool extends = true);

    /*!
     * \brief Destructor
     */
    virtual ~CT_Grid3D_Sparse();

    virtual QString getType() const;
    static QString staticGetType();

    virtual QString name() const;
    static QString staticName();

    /*!
     * \brief Copy method
     *
     * \param model Item model for the copy
     * \param result Result containing the copy
     * \param copyModeList Copy mode
     * \return Item copy
     */
    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);
    virtual CT_AbstractItemDrawable* copy(const QString &modelName, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

    const CT_ITemplatedData3DArray<DataT>* iTemplatedData3DArray() const { return this; }

    /*!
     * \brief Generic setter taking a double as input value
     * \param index Index of cell to modify
     * \param value Double value to cast in effective type
     */
    virtual void setValueAtIndexFromDouble(const size_t &index, const double &value);

    /*!
     * \brief Compute min and max values
     */
    void computeMinMax();

    /**
      * \brief Gives the NA value
      * \return Valeur NA value
      */
    inline DataT NA() const {return _NAdata;}

    /*!
     * \brief Gives the max value of the grid
     * \return Max value
     */
    inline DataT dataMax() const { return _dataMax; }

    /*!
     * \brief Gives the min value of the grid
     * \return Min value
     */
    inline DataT dataMin() const { return _dataMin; }


    // CT_ITemplatedData3DArray
    size_t xArraySize() const { return xdim(); }

    // CT_ITemplatedData3DArray
    size_t yArraySize() const { return ydim(); }

    // CT_ITemplatedData3DArray
    size_t zArraySize() const { return zdim(); }


    /*!
     * \brief Set value at specified index
     * \param index Index
     * \param value Value
     * \return True if the value has actually been set
     */
    bool setValueAtIndex(const size_t index, const DataT value);


    /**
      * \brief Set the value at row liny, column colx and z level levz to value
      * \param colx column, first one is 0
      * \param liny row, first one is 0
      * \param levz z level, first one is 0
      * \param value Value
      * \return True if the value has actually been set
      */
    bool setValue(const size_t colx, const size_t liny, const size_t levz, DataT value);

    /**
      * \brief Set the value for the cell containing (x,y,z) coordinate to value
      * \param x X coordinate
      * \param y Y coordinate
      * \param z Z coordinate
      * \param value Value
      * \return True if the value has actually been set
      */
    bool setValueAtXYZ(const double x, const double y, const double z, const DataT value);


    /*!
     * \brief ives the value at specified index
     * \param index Index
     * \return Value
     */
    DataT valueAtIndex(const size_t index) const;

    /*!
     * \brief Generic [0;1]  (or -1 for NA) value accessor for use as CT_AbstractGrid3D
     * \param index index in the grid
     * \return A double value between 0 (min value) and 1 (max value), or -1 for NA
     */
    virtual double ratioValueAtIndex(const size_t index) const;


    /*!
     * \brief Return a double value for any type (or NAN for NA)
     * \param index index in the grid
     * \return A double value
     */
    virtual double valueAtIndexAsDouble(const size_t index) const;


    /*!
     * \brief Return a QString value for any type (or -1 for NA)
     * \param index index in the grid
     * \return A QString représenting value
     */
    virtual QString valueAtIndexAsString(const size_t index) const;

    /*!
     * \brief return na value as a string
     *
     */
    virtual QString NAAsString() const;

    /**
      * \brief Gives the value at row liny and column colx and z level levz
      * \param colx column, first one is 0
      * \param liny row, first one is 0
      * \param levz z level, first one is 0
      * \return Value at row liny and column clox
      */
    DataT value(const size_t colx, const size_t liny, const size_t levz) const;

    // CT_ITemplatedData3DArray
    DataT dataFromArray(const size_t &x, const size_t &y, const size_t &z) const;

    // CT_ITemplatedData3DArray
    DataT dataFromArray(const size_t &index) const;

    /**
      * \brief Gives the value at (x,y,z) coordinate
      * \param x X coordinate
      * \param y Y coordinate
      * \param z Z coordinate
      * \return Value at (x,y,z)
      */
    DataT valueAtXYZ(const double x, const double y, const double z) const;


    /**
      * \brief Set the value for the cell at specified index, if value is superior to actual one
      * \param index Index
      * \param value Value
      * \return True if the value has actually been set
      */
    bool setMaxValueAtIndex(const size_t index, const DataT value);

    /**
      * \brief Set the value for the cell containing (x,y,z) coordinate to value, if value is superior to actual one
      * \param x X coordinate
      * \param y Y coordinate
      * \param z Z coordinate
      * \param value Value
      * \return True if the value has actually been set
      */
    bool setMaxValueAtXYZ(const double x, const double y, const double z, const DataT value);

    /**
      * \brief Set the value for the cell at specified index, if value is inferior to actual one
      * \param index Index
      * \param value Value
      * \return True if the value has actually been set
      */
    bool setMinValueAtIndex(const size_t index, const DataT value);

    /**
      * \brief Set the value for the cell containing (x,y,z) coordinate to value, if value is inferior to actual one
      * \param x X coordinate
      * \param y Y coordinate
      * \param z Z coordinate
      * \param value Value
      * \return True if the value has actually been set
      */
    bool setMinValueAtXYZ(const double x, const double y, const double z, const DataT value);

    /**
      * \brief Add value to the cell at the specified index
      * \param index Index
      * \param value Value
      * \return True if the value has actually been set
      */
    bool addValueAtIndex(const size_t index, const DataT value);

    /**
      * \brief Add value to the cell containing (x,y,z) coordinate
      * \param x X coordinate
      * \param y Y coordinate
      * \param z Z coordinate
      * \param value Value
      * \return True if the value has actually been updated
      */
    bool addValueAtXYZ(const double x, const double y, const double z, const DataT value);

    /**
      * \brief Gives neighbours values
      * \param colx column, first one is 0
      * \param liny row, first one is 0
      * \param levz z level, first one is 0
      * \param distance : Distance for neighbours search in cells
      * \param keepNAs : Should the NA values be kept ?
      * \param centermode : How should be treated the central cell ?
      * \return List of neighbours values
      */
    QList<DataT> neighboursValues(const size_t colx, const size_t liny, const size_t levz, const size_t distance, const bool keepNAs = false, const CenterMode centermode = CT_Grid3D_Sparse::CM_DropCenter) const;


    /** Methods for colors management
     */
    inline bool colorsDefined() const {return !_colorMap.isEmpty();}

    inline void clearColors() {_colorMap.clear();}

    inline void setDefaultColor(const QColor &color) {_defaultColor = color;}

    inline void addColorForValue(DataT value, const QColor &color) {_colorMap.insert(value, color);}

    inline QColor getColorForValue(DataT value) const {return _colorMap.value(value, _defaultColor);}

protected:
    DataT       _NAdata;            /*!< Valeur codant NA */

    DataT       _dataMax;           /*!< valeur maximale du grid*/
    DataT       _dataMin;           /*!< valeur minimale du grid*/
    cv::SparseMat_<DataT> _data;       /*!< Tableau contenant les donnees pour chaque case de la grille*/

    QMap<DataT, QColor> _colorMap;
    QColor              _defaultColor;

    CT_DEFAULT_IA_BEGIN(CT_Grid3D_Sparse<DataT>)
    CT_DEFAULT_IA_V3(CT_Grid3D_Sparse<DataT>, CT_AbstractCategory::staticInitDataXDimension(), &CT_Grid3D_Sparse<DataT>::xdim, QObject::tr("X dimension"), "xd")
    CT_DEFAULT_IA_V3(CT_Grid3D_Sparse<DataT>, CT_AbstractCategory::staticInitDataYDimension(), &CT_Grid3D_Sparse<DataT>::ydim, QObject::tr("Y dimension"), "yd")
    CT_DEFAULT_IA_V3(CT_Grid3D_Sparse<DataT>, CT_AbstractCategory::staticInitDataZDimension(), &CT_Grid3D_Sparse<DataT>::zdim, QObject::tr("Z dimension"), "zd")
    CT_DEFAULT_IA_V3(CT_Grid3D_Sparse<DataT>, CT_AbstractCategory::staticInitDataX(), &CT_Grid3D_Sparse<DataT>::minX, QObject::tr("X min"), "xmin")
    CT_DEFAULT_IA_V3(CT_Grid3D_Sparse<DataT>, CT_AbstractCategory::staticInitDataY(), &CT_Grid3D_Sparse<DataT>::minY, QObject::tr("Y min"), "ymin")
    CT_DEFAULT_IA_V3(CT_Grid3D_Sparse<DataT>, CT_AbstractCategory::staticInitDataZ(), &CT_Grid3D_Sparse<DataT>::minZ, QObject::tr("Z min"), "zmin")
    CT_DEFAULT_IA_V3(CT_Grid3D_Sparse<DataT>, CT_AbstractCategory::staticInitDataResolution(), &CT_Grid3D_Sparse<DataT>::resolution, QObject::tr("Resolution"), "res")
    CT_DEFAULT_IA_V3(CT_Grid3D_Sparse<DataT>, CT_AbstractCategory::staticInitDataNa(), &CT_Grid3D_Sparse<DataT>::NA, QObject::tr("NA"), "na")
    CT_DEFAULT_IA_END(CT_Grid3D_Sparse<DataT>)

    //const static CT_StandardGrid3DDrawManager<DataT> ABSGRID3D_DRAW_MANAGER;

};

// Spécialisations

template<>
inline bool CT_Grid3D_Sparse<bool>::NA() const {return false;}

template<>
PLUGINSHAREDSHARED_EXPORT void CT_Grid3D_Sparse<bool>::computeMinMax();

template<>
PLUGINSHAREDSHARED_EXPORT double CT_Grid3D_Sparse<bool>::ratioValueAtIndex(const size_t index) const;

template<>
PLUGINSHAREDSHARED_EXPORT double CT_Grid3D_Sparse<bool>::valueAtIndexAsDouble(const size_t index) const;

template<>
PLUGINSHAREDSHARED_EXPORT QString CT_Grid3D_Sparse<bool>::valueAtIndexAsString(const size_t index) const;

template<>
PLUGINSHAREDSHARED_EXPORT bool CT_Grid3D_Sparse<bool>::setMaxValueAtIndex(const size_t index, const bool value);

template<>
PLUGINSHAREDSHARED_EXPORT bool CT_Grid3D_Sparse<bool>::setMinValueAtIndex(const size_t index, const bool value);

template<>
PLUGINSHAREDSHARED_EXPORT bool CT_Grid3D_Sparse<bool>::addValueAtIndex(const size_t index, const bool value);

template<>
PLUGINSHAREDSHARED_EXPORT QList<bool> CT_Grid3D_Sparse<bool>::neighboursValues(const size_t colx, const size_t liny, const size_t levz, const size_t distance, const bool keepNAs, const CenterMode centermode) const;

template<>
PLUGINSHAREDSHARED_EXPORT QString CT_Grid3D_Sparse<unsigned long>::valueAtIndexAsString(const size_t index) const;

template<>
PLUGINSHAREDSHARED_EXPORT QString CT_Grid3D_Sparse<unsigned long>::NAAsString() const;


// fin des spécialisations


// Includes the template implementations
#include "ct_itemdrawable/ct_grid3d_sparse.hpp"

#endif

#endif // CT_GRID3D_SPARSE_H
