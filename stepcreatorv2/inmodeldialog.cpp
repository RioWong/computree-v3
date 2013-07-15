#include "inmodeldialog.h"
#include "ui_inmodeldialog.h"

#include "qdebug.h"

#include "qmessagebox.h"

#include "models/inresultmodel.h"
#include "models/ingroupmodel.h"
#include "models/initemmodel.h"
#include "models/abstractmodel.h"

INModelDialog::INModelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::INModelDialog)
{
    ui->setupUi(this);
    _model = new QStandardItemModel();
    ui->treeView->setModel(_model);
    _layout = new QVBoxLayout();
    ui->widgetZone->setLayout(_layout);
    _activeWidget = NULL;
}

INModelDialog::~INModelDialog()
{
    delete ui;
}

void INModelDialog::on_pb_addResult_clicked()
{
    QStandardItem *item = _model->invisibleRootItem();

    INResultModel *item2 = new INResultModel();

    item->appendRow(item2);
    if (_activeWidget!=NULL)
    {
        _layout->removeWidget(_activeWidget);
        _activeWidget->setParent(0);
    }
    _activeWidget = item2->getWidget();
    _layout->addWidget(_activeWidget);

    ui->treeView->setExpanded(ui->treeView->currentIndex(), true);
}


void INModelDialog::on_pb_addGroup_clicked()
{
    AbstractModel *item = (AbstractModel*) _model->itemFromIndex(ui->treeView->currentIndex());
    if (item == NULL)
    {
        return;
    }

    if (item->getModelType()!=AbstractModel::M_Result_IN && item->getModelType()!=AbstractModel::M_Group_IN)
    {
        return;
    }

    if (item->getModelType()==AbstractModel::M_Result_IN && item->rowCount()!=0)
    {
        return;
    }


    INGroupModel *item2 = new INGroupModel();

    item->appendRow(item2);
    if (_activeWidget!=NULL)
    {
        _layout->removeWidget(_activeWidget);
        _activeWidget->setParent(0);
    }
    _activeWidget = item2->getWidget();
    _layout->addWidget(_activeWidget);

    ui->treeView->setExpanded(ui->treeView->currentIndex(), true);

}


void INModelDialog::on_pb_addItem_clicked()
{
    AbstractModel *item = (AbstractModel*) _model->itemFromIndex(ui->treeView->currentIndex());
    if (item == NULL)
    {
        return;
    }
    if (item->getModelType()!=AbstractModel::M_Group_IN)
    {
        return;
    }

    INItemModel *item2 = new INItemModel();

    item->appendRow(item2);
    if (_activeWidget!=NULL)
    {
        _layout->removeWidget(_activeWidget);
        _activeWidget->setParent(0);
    }
    _activeWidget = item2->getWidget();
    _layout->addWidget(_activeWidget);

    ui->treeView->setExpanded(ui->treeView->currentIndex(), true);
}

void INModelDialog::on_pb_delete_clicked()
{
    QModelIndex index = ui->treeView->currentIndex();
    AbstractModel *item = (AbstractModel*) _model->itemFromIndex(index);
    ui->treeView->clearSelection();
    if (item != NULL)
    {
        //delete all children of parent;
        QStandardItem * loopItem = item; //main loop item
        QList<QStandardItem *> carryItems; //Last In First Out stack of items
        QList<QStandardItem *> itemsToBeDeleted; //List of items to be deleted
        while (loopItem->rowCount())
        {
            itemsToBeDeleted << loopItem->takeRow(0);
            //if the row removed has children:
            if (itemsToBeDeleted.at(0)->hasChildren())
            {
                carryItems << loopItem; //put on the stack the current loopItem
                loopItem = itemsToBeDeleted.at(0); //set the row with children as the loopItem
            }
            //if current loopItem has no more rows but carryItems list is not empty:
            if (!loopItem->rowCount() && !carryItems.isEmpty()) loopItem = carryItems.takeFirst();

        }
        if (item->parent()!=NULL)
        {
            item->parent()->takeRow(item->row());
        } else
        {
            QStandardItem *root = _model->invisibleRootItem();
            root->takeRow(item->row());
        }

        itemsToBeDeleted.append(item);

        while (!itemsToBeDeleted.isEmpty())        {
            AbstractModel* itemToDelete = (AbstractModel*) itemsToBeDeleted.takeLast();
            if (itemToDelete->getWidget() == _activeWidget) {_activeWidget = NULL;}
            delete itemToDelete;
        }

    }
}

void INModelDialog::on_treeView_clicked(const QModelIndex &index)
{
    AbstractModel *item = (AbstractModel*) _model->itemFromIndex(index);
    if (_activeWidget!=NULL)
    {
        _layout->removeWidget(_activeWidget);
        _activeWidget->setParent(0);
    }
    _activeWidget = item->getWidget();
    _layout->addWidget(_activeWidget);

}

void INModelDialog::on_pb_clear_clicked()
{
    ui->treeView->clearSelection();

    //delete all children of parent;
    QStandardItem * loopItem = _model->invisibleRootItem();; //main loop item
    QList<QStandardItem *> carryItems; //Last In First Out stack of items
    QList<QStandardItem *> itemsToBeDeleted; //List of items to be deleted
    while (loopItem->rowCount())
    {
        itemsToBeDeleted << loopItem->takeRow(0);
        //if the row removed has children:
        if (itemsToBeDeleted.at(0)->hasChildren())
        {
            carryItems << loopItem; //put on the stack the current loopItem
            loopItem = itemsToBeDeleted.at(0); //set the row with children as the loopItem
        }
        //if current loopItem has no more rows but carryItems list is not empty:
        if (!loopItem->rowCount() && !carryItems.isEmpty()) loopItem = carryItems.takeFirst();
    }
    qDeleteAll(itemsToBeDeleted);
    _activeWidget = NULL;
}

void INModelDialog::on_buttonBox_rejected()
{
    on_pb_clear_clicked();
}

void INModelDialog::accept()
{
    bool ok = true;
    int count = _model->rowCount();
    QList<QString> liste;
    for (int i = 0 ; i < count && ok; i++)
    {
        AbstractModel* item = (AbstractModel*) _model->item(i);
        if (liste.contains(item->getAlias()) || !item->isValid())
        {
            ok = false;
        } else {
            liste.append(item->getAlias());
        }
    }

    if (!ok) {
        QMessageBox::warning(this, "Validation impossible", "Erreurs possibles :\n- Les alias ne sont pas tous d�finis et / ou uniques\n- Chaque r�sultat doit contenir exactement un groupe racine");
    } else {
        QStandardItem* root = _model->invisibleRootItem();
        int count = root->rowCount();

        qDebug() << "------------IN Models -------------\n";
        for (int i = 0 ; i < count ; i++)
        {
            AbstractModel* item = (AbstractModel*) root->child(i);

            qDebug() << item->getInModelsDefinition();
        }

        qDebug() << "------------Compute -------------\n";
        for (int i = 0 ; i < count ; i++)
        {
            AbstractModel* item = (AbstractModel*) root->child(i);

            qDebug() << item->getInComputeContent();
        }

        done(QDialog::Accepted);
    }
}
