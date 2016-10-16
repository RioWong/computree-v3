/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michaël Krebs (ENSAM / ARTS)

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

#ifndef CT_SPHERE_H
#define CT_SPHERE_H

#include "ct_itemdrawable/abstract/ct_abstractshape.h"
#include "ct_itemdrawable/tools/drawmanager/ct_standardspheredrawmanager.h"

#include "ct_shapedata/ct_spheredata.h"

/**
 * @brief It represents a 3D sphere, defined by a center and a radius in meters.
 */
class PLUGINSHAREDSHARED_EXPORT CT_Sphere : public CT_AbstractShape
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_Sphere, CT_AbstractShape, Sphere)

public:
    CT_Sphere();

    CT_Sphere(const CT_OutAbstractSingularItemModel *model,
              const CT_AbstractResult *result,
              CT_SphereData *data);

    CT_Sphere(const QString &modelName,
              const CT_AbstractResult *result,
              CT_SphereData *data);

    /**
     * @brief Returns the radius of the sphere
     */
    double getRadius() const;

    /**
     * @brief Copy methods
     * @param copyModeList : not used
     */
    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);
    virtual CT_AbstractItemDrawable* copy(const QString &modelName, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

private:

    CT_DEFAULT_IA_BEGIN(CT_Sphere)
    CT_DEFAULT_IA_V3(CT_Sphere, CT_AbstractCategory::staticInitDataRadius(), &CT_Sphere::getRadius, QObject::tr("Rayon de la sphere"), "ns")
    CT_DEFAULT_IA_END(CT_Sphere)

    const static CT_StandardSphereDrawManager   SPHERE_DRAW_MANAGER;
};

#endif // CT_SPHERE_H
