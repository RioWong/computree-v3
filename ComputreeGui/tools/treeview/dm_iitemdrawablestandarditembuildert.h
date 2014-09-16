#ifndef DM_IITEMDRAWABLESTANDARDITEMBUILDER_H
#define DM_IITEMDRAWABLESTANDARDITEMBUILDER_H

#include <QStandardItem>

#include "interfaces.h"

/**
 * @brief You must inherit this class if you want to use the DM_ItemDrawableTreeViewController.
 */
template<class Item>
class DM_IItemDrawableStandardItemBuilderT
{
public:
    virtual ~DM_IItemDrawableStandardItemBuilderT() {}

    /**
     * @brief Return the list of Item to add in a row for the CT_AbstractItemDrawable 'item' at level 'level'. If
     *        this method return a empty list the row is not created but the build of the QStandardItemModel continue.
     *
     *        If you want to filter certains CT_AbstractItemDrawable you can return a empty list.
     */
    virtual QList<Item*> createItems(const CT_AbstractItemDrawable &item, const int &level) const = 0;
};

#endif // DM_IITEMDRAWABLESTANDARDITEMBUILDER_H
