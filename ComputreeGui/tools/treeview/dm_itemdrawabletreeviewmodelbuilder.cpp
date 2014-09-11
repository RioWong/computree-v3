#include "dm_itemdrawabletreeviewmodelbuilder.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"
#include "ct_itemdrawable/tools/iterator/ct_itemiterator.h"
#include "ct_item/tools/iterator/ct_childiterator.h"

DM_ItemDrawableTreeViewModelBuilder::DM_ItemDrawableTreeViewModelBuilder() : DM_AbstractWorker()
{
    m_collection = NULL;
    m_itemModelBuilder = NULL;
    m_nLevel = 1;
    m_itemsToUpdate.clear();
}

void DM_ItemDrawableTreeViewModelBuilder::setCollection(const QVector<QList<QStandardItem*> > *collection)
{
    m_collection = (QVector<QList<QStandardItem*> >*)collection;
}

void DM_ItemDrawableTreeViewModelBuilder::setStandardItemBuilder(const DM_IItemDrawableStandardItemBuilder *builder)
{
    m_itemModelBuilder = (DM_IItemDrawableStandardItemBuilder*)builder;
}

void DM_ItemDrawableTreeViewModelBuilder::setNLevelToBuild(const int &nLevel)
{
    m_nLevel = nLevel;
}

void DM_ItemDrawableTreeViewModelBuilder::setItemDrawable(const QList<CT_AbstractItemDrawable *> &list)
{
    m_items = list;
    m_itemsToUpdate.clear();
}

void DM_ItemDrawableTreeViewModelBuilder::setQStandardItemToUpdate(const QList<QPair<QStandardItem *, CT_AbstractItemDrawable *> > &list)
{
    m_itemsToUpdate = list;
    m_items.clear();
}

void DM_ItemDrawableTreeViewModelBuilder::recursiveCreateItemForNextLevel(CT_AbstractItemDrawable *item, QStandardItem *parent, const int &level)
{
    CT_ChildIterator it(item);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *child = dynamic_cast<CT_AbstractItemDrawable*>((CT_AbstractItem*)it.next());

        QList<QStandardItem*> items = m_itemModelBuilder->createItems(*child, level);

        if(!items.isEmpty())
        {
            parent->appendRow(items);

            if((level+1) < m_nLevel)
                recursiveCreateItemForNextLevel(child, items.first(), level+1);
        }
    }
}

void DM_ItemDrawableTreeViewModelBuilder::apply()
{
    if(m_itemsToUpdate.isEmpty())
    {
        int size = m_items.size();
        int i = 0;

        m_collection->resize(size);

        QListIterator<CT_AbstractItemDrawable *> it(m_items);

        while(it.hasNext())
        {
            CT_AbstractItemDrawable *item = it.next();

            QList<QStandardItem*> items = m_itemModelBuilder->createItems(*item, 0);

            if(!items.isEmpty())
            {
                (*m_collection)[i] = items;

                if(m_nLevel > 1)
                    recursiveCreateItemForNextLevel(item, items.first(), 1);
            }

            ++i;
            setProgress((i*100)/size);
        }
    }
    else
    {
        int size = m_itemsToUpdate.size();
        int i = 0;

        m_collection->resize(size);

        QListIterator< QPair<QStandardItem*, CT_AbstractItemDrawable*> > it(m_itemsToUpdate);

        while(it.hasNext())
        {
            const QPair<QStandardItem*, CT_AbstractItemDrawable*> &pair = it.next();

            int level = 0;
            QStandardItem *parent = pair.first->parent();

            while(parent != NULL)
            {
                ++level;
                parent = parent->parent();
            }

            QList<QStandardItem*> items = m_itemModelBuilder->createItems(*pair.second, level);

            if(!items.isEmpty())
            {
                (*m_collection)[i] = items;

                if(m_nLevel > (level+1))
                    recursiveCreateItemForNextLevel(pair.second, items.first(), level+1);
            }

            ++i;
            setProgress((i*100)/size);
        }
    }

    setFinished();
}
