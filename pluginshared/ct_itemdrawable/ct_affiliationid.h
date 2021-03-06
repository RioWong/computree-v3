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

#ifndef CT_AFFILIATIONID_H
#define CT_AFFILIATIONID_H

#include "ct_math/ct_math.h"
#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithoutpointcloud.h"

/*!
 * \class CT_AffiliationID
 * \ingroup PluginShared_Items
 * \brief <b>ItemDrawable managing affiliation ID</b>
 *
 */
class PLUGINSHAREDSHARED_EXPORT CT_AffiliationID : public CT_AbstractItemDrawableWithoutPointCloud
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_AffiliationID, CT_AbstractItemDrawableWithoutPointCloud, Affiliation ID)

public:
    /**
      * \brief Empty Contructor vide
      */
    CT_AffiliationID();


    /*!
      * \brief Contructor with integer column and row coordinates
      *
      * \param model Item model for creation
      * \param result Result containing the item
      */
     CT_AffiliationID(const CT_OutAbstractSingularItemModel *model, CT_AbstractResult *result);

    /*!
      * \brief Contructor with integer column and row coordinates
      *
      * \param model Item model for creation
      * \param result Result containing the item
      * \param value Value
      */
     CT_AffiliationID(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result, size_t value);

     /**
      * @brief Create a ItemDrawable with a name of model defined in your step (typically a DEF_...)
      *        and the result that will contains your ItemDrawable
      *
      * @warning The modelName can not be empty and the result can not be NULL to use this constructor
      */
     CT_AffiliationID(const QString &modelName,
                      CT_AbstractResult *result);

     /**
      * @brief Create a ItemDrawable with a name of model defined in your step (typically a DEF_...)
      *        and the result that will contains your ItemDrawable
      *
      * @warning The modelName can not be empty and the result can not be NULL to use this constructor
      */
     CT_AffiliationID(const QString &modelName,
                      const CT_AbstractResult *result,
                      const size_t &value);

    virtual ~CT_AffiliationID();

    inline void setValue(size_t value) {_value = value;}
    inline size_t getValue() const {return _value;}
    inline QString getValueAsString() const {return QString("%1").arg(_value);}

    inline virtual double getDoubleValue() {return (double) _value;}

    /*!
     * \brief Copy method
     *
     * \param model Item model for the copy
     * \param result Result containing the copy
     * \param copyModeList Copy mode
     * \return Item copy
     */
    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

protected:
    size_t       _value;

private:
    static size_t                               LASTID;

    CT_DEFAULT_IA_BEGIN(CT_AffiliationID)
    CT_DEFAULT_IA_V3(CT_AffiliationID, CT_AbstractCategory::staticInitDataId(), &CT_AffiliationID::getValue, QObject::tr("Affiliation_ID"), "aid")
    CT_DEFAULT_IA_END(CT_AffiliationID)
};

#endif // CT_AFFILIATIONID_H
