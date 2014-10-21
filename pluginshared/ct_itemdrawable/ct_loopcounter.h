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

#ifndef CT_LOOPCOUNTER_H
#define CT_LOOPCOUNTER_H

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithoutpointcloud.h"
#include "ct_itemdrawable/tools/ct_counter.h"
#include <QSharedPointer>

/*!
 * \class CT_LoopCounter
 * \ingroup PluginShared_Items
 * \brief <b>ItemDrawable managing Step Loop counter</b>
 *
 */
class PLUGINSHAREDSHARED_EXPORT CT_LoopCounter : public CT_AbstractItemDrawableWithoutPointCloud
{
    Q_OBJECT
public:


    /**
      * \brief Empty Contructor vide
      */
    CT_LoopCounter();

    /*!
      * \brief Contructor
      *
      * \param model Item model for creation
      * \param result Result containing the item
      * \param value Value
      */
     CT_LoopCounter(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result, QSharedPointer<CT_Counter> counter);

     /**
      * @brief Create a ItemDrawable with a name of model defined in your step (typically a DEF_...)
      *        and the result that will contains your ItemDrawable
      *
      * @warning The modelName can not be empty and the result can not be NULL to use this constructor
      */
     CT_LoopCounter(const QString &modelName, const CT_AbstractResult *result, QSharedPointer<CT_Counter> counter);

    ~CT_LoopCounter();
    
    virtual QString getType() const;
    static QString staticGetType();

    virtual QString name() const;


    inline size_t getNTurns() const {return _counter->getNTurns();}
    inline size_t getCurrentTurn() const {return _counter->getCurrentTurn();}
    inline bool hasNextTurn() {return _counter->hasNextTurn();}

    inline void beginNextTurn() {_counter->beginNextTurn();}


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
    QSharedPointer<CT_Counter>     _counter;

};

#endif // CT_LOOPCOUNTER_H
