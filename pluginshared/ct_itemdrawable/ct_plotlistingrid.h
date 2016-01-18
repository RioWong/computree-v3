/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
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

#ifndef CT_PLOTLISTINGRID_H
#define CT_PLOTLISTINGRID_H

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithoutpointcloud.h"
#include "ct_shape2ddata/ct_areashape2ddata.h"

class PLUGINSHAREDSHARED_EXPORT CT_PlotListInGrid : public CT_AbstractItemDrawableWithoutPointCloud
{
    // IMPORTANT pour avoir le nom de l'ItemDrawable
    Q_OBJECT

public:

    enum Type {
        T_Circle,
        T_Square
    };

    /**
      * \brief Contructeur vide
      */
    CT_PlotListInGrid();
    /**
      * \brief Contructeur
      */
    CT_PlotListInGrid(const CT_OutAbstractSingularItemModel *model,
                      const CT_AbstractResult *result,
                      const CT_AreaShape2DData* areaShape2D,
                      const Eigen::Vector2d& refCoords,
                      double spacing,
                      double size,
                      CT_PlotListInGrid::Type type);

    CT_PlotListInGrid(const QString &modelName,
                     const CT_AbstractResult *result,
                      const CT_AreaShape2DData* areaShape2D,
                      const Eigen::Vector2d& refCoords,
                      double spacing,
                      double size,
                      CT_PlotListInGrid::Type type);

    virtual ~CT_PlotListInGrid();

    /**
      * ATTENTION : ne pas oublier de redfinir ces deux mthodes si vous hrit de cette classe.
      */
    virtual QString getType() const;
    static QString staticGetType();

    virtual bool hasBoundingBox() const {return false;}

    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);


    static bool orderTopLeftToBottomRight(CT_PlotListInGrid *s1, CT_PlotListInGrid *s2)
    {
        if (s1->_min(1) >= s2->_max(1)) {return true;}
        if (s1->_min(0) <  s2->_min(0)) {return true;}
        return false;
    }

protected:

    CT_AreaShape2DData*     _areaXY;
    Eigen::Vector2d         _min;
    Eigen::Vector2d         _max;
    double                  _spacing;
    double                  _size;
    CT_PlotListInGrid::Type _type;


private:
    CT_DEFAULT_IA_BEGIN(CT_PlotListInGrid)
//    CT_DEFAULT_IA_V2(CT_PlotListInGrid, CT_AbstractCategory::staticInitDataNumber(), &CT_PlotListInGrid::getPlotNumber, QObject::tr("Nombre de placettes"))
    CT_DEFAULT_IA_END(CT_PlotListInGrid)

    void computeMinMax(const Eigen::Vector2d &refCoords);

    CT_PlotListInGrid(const CT_OutAbstractSingularItemModel *model,
                      const CT_AbstractResult *result,
                      const CT_AreaShape2DData* areaShape2D,
                      const Eigen::Vector2d& min,
                      const Eigen::Vector2d& max,
                      double spacing,
                      double size,
                      CT_PlotListInGrid::Type type);

};

#endif // CT_PLOTLISTINGRID_H
