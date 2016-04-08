#include "gfavoritesmenudialog.h"
#include "ui_gfavoritesmenudialog.h"

#include "dm_guimanager.h"
#include "ct_step/tools/menu/ct_menulevel.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QMenu>

GFavoritesMenuDialog::GFavoritesMenuDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GFavoritesMenuDialog)
{
    ui->setupUi(this);
    m_step = NULL;

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(displayContextMenu(QPoint)));
}

GFavoritesMenuDialog::~GFavoritesMenuDialog()
{
    delete ui;
}

void GFavoritesMenuDialog::init()
{
    ui->treeWidget->clear();

    ui->pushButtonAddSubLevel->setEnabled(false);
    ui->pushButtonRemoveLevel->setEnabled(false);

    CT_MenuLevel *favorites = GUI_MANAGER->getPluginManager()->stepsMenu()->getOrCreateRootLevel(CT_StepsMenu::LO_Favorites);

    QList<CT_MenuLevel*> levels = favorites->levels();
    QListIterator<CT_MenuLevel*> it(levels);

    while(it.hasNext())
        ui->treeWidget->addTopLevelItem(recursiveAddLevelToTreeWidget(it.next()));

    ui->treeWidget->expandAll();
}

void GFavoritesMenuDialog::setStepToAdd(CT_VirtualAbstractStep *step, GStepViewDefault::DisplayNameConfigs config)
{
    m_step = step;
    m_nameConfig = config;

    updateStepLabel();
}

QTreeWidgetItem* GFavoritesMenuDialog::recursiveAddLevelToTreeWidget(CT_MenuLevel *level)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << level->displayableName() << QString().setNum(level->steps().size() + level->stepsNotFounded().size()));
    setLevelToItem(level, item);

    QList<CT_MenuLevel*> levels = level->levels();
    QListIterator<CT_MenuLevel*> it(levels);

    while(it.hasNext())
        item->addChild(recursiveAddLevelToTreeWidget(it.next()));

    return item;
}

void GFavoritesMenuDialog::recursiveAddOrRemoveLevelToFavorites(QTreeWidgetItem *parentItem, CT_MenuLevel *parentLevel)
{
    int n = parentItem->childCount();
    setLevelToItem(parentLevel, parentItem);

    for(int i=0; i<n; ++i) {
        QTreeWidgetItem *item = parentItem->child(i);

        if(item->isHidden())
            GUI_MANAGER->getPluginManager()->stepsMenu()->removeLevelFromFavorites(levelFromItem(item));
        else {
            CT_MenuLevel *level = CT_MenuLevel::getOrCreateLevel(item->text(0), parentLevel);
            recursiveAddOrRemoveLevelToFavorites(item, level);
        }
    }
}

void GFavoritesMenuDialog::updateStepLabel()
{
    QTreeWidgetItem *selected = ui->treeWidget->currentItem();

    if(m_step != NULL) {
        QString stepName = GStepViewDefault::staticGetStepNameFromConfiguration(m_step, m_nameConfig);

        if((selected == NULL) || selected->isHidden())
            ui->labelStepToAdd->setText(tr("Vous devez créer un niveau pour ajouter l'étape \"%1\"").arg(stepName));
        else
            ui->labelStepToAdd->setText(tr("Ajout de l'étape \"%1\" au niveau \"%2\"").arg(stepName).arg(selected->text(0)));
    }
    else
        ui->labelStepToAdd->setText("");
}

void GFavoritesMenuDialog::setLevelToItem(CT_MenuLevel *level, QTreeWidgetItem *item)
{
    item->setData(0, Qt::UserRole+1, qVariantFromValue((void*)level));
}

CT_MenuLevel *GFavoritesMenuDialog::levelFromItem(QTreeWidgetItem *item)
{
    return (CT_MenuLevel*)item->data(0, Qt::UserRole+1).value<void*>();
}

void GFavoritesMenuDialog::accept()
{
    CT_MenuLevel *favorites = GUI_MANAGER->getPluginManager()->stepsMenu()->getOrCreateRootLevel(CT_StepsMenu::LO_Favorites);

    int n = ui->treeWidget->topLevelItemCount();

    for(int i=0; i<n; ++i) {
        QTreeWidgetItem *item = ui->treeWidget->topLevelItem(i);

        if(item->isHidden())
            GUI_MANAGER->getPluginManager()->stepsMenu()->removeLevelFromFavorites(levelFromItem(item));
        else {
            CT_MenuLevel *level = CT_MenuLevel::getOrCreateLevel(item->text(0), favorites);

            recursiveAddOrRemoveLevelToFavorites(item, level);
        }
    }

    if(m_step != NULL) {
        QTreeWidgetItem *ci = ui->treeWidget->currentItem();

        if(ci != NULL) {
            CT_MenuLevel *levelChoosed = levelFromItem(ci);

            if(levelChoosed != NULL)
                levelChoosed->addStep(m_step->getPlugin()->createNewInstanceOfStep(*m_step, NULL));
        }
    }

    QDialog::accept();
}

void GFavoritesMenuDialog::displayContextMenu(QPoint p)
{
    QTreeWidgetItem *i = ui->treeWidget->itemAt(p);

    QMenu *contextMenu =  new QMenu(this);

    if(i == NULL) {
        contextMenu->addAction(QIcon(":/Icones/Icones/add.png"), tr("Ajouter un niveau"), this, SLOT(addRootLevel()));
    } else {
        contextMenu->addAction(QIcon(":/Icones/Icones/add.png"), tr("Ajouter un sous-niveau"), this, SLOT(addSubLevel()));
        contextMenu->addAction(QIcon(":/Icones/Icones/delete.png"), tr("Supprimer"), this, SLOT(removeLevel()));
        /*contextMenu->addSeparator();
        contextMenu->addAction(tr("Renommer"), this, SLOT(renameLevel()));*/
    }

    contextMenu->exec(ui->treeWidget->viewport()->mapToGlobal(p));
}

void GFavoritesMenuDialog::addRootLevel()
{
    QString text = QInputDialog::getText(this, tr("Ajouter un niveau"), tr("Nom du niveau"));

    if(!text.isEmpty()) {

        bool ok = true;

        QList<QTreeWidgetItem*> l = ui->treeWidget->findItems(text, Qt::MatchFixedString);
        QListIterator<QTreeWidgetItem*> it(l);

        while(it.hasNext() && ok) {
            ok = it.next()->isHidden();
        }

        if(!ok)
            QMessageBox::critical(this, tr("Erreur"), tr("Un niveau ayant ce nom existe déjà"));
        else {
            QTreeWidgetItem *child = new QTreeWidgetItem(QStringList() << text << "0");
            ui->treeWidget->addTopLevelItem(child);
            ui->treeWidget->setCurrentItem(child);
        }
    }
}

void GFavoritesMenuDialog::addSubLevel()
{
    QString text = QInputDialog::getText(this, tr("Ajouter un sous-niveau"), tr("Nom du sous-niveau"));

    if(!text.isEmpty())
        addSubLevel(text);
}

void GFavoritesMenuDialog::removeLevel()
{
    QTreeWidgetItem *selected = ui->treeWidget->currentItem();

    if(QMessageBox::question(this, tr("Question"), tr("Voulez vous vraiment supprimer ce niveau ?")) == QMessageBox::Yes) {
        selected->setHidden(true);
        on_treeWidget_currentItemChanged(NULL, selected);
    }
}

void GFavoritesMenuDialog::renameLevel()
{
    /*QTreeWidgetItem *selected = ui->treeWidget->currentItem();

    QString name = selected->text(0);

    bool ok;
    QString newName = QInputDialog::getText(this, tr("Renommer"), tr("Nom"), QLineEdit::Normal, name, &ok);

    if(ok && newName != name) {
    }*/
}

void GFavoritesMenuDialog::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous)

    if((current == NULL) && (ui->treeWidget->topLevelItemCount() > 0)) {
        for(int i=0; (i<ui->treeWidget->topLevelItemCount()) && (current == NULL); ++i) {
            current = ui->treeWidget->topLevelItem(i);

            if(current->isHidden())
                current = NULL;
        }

        if(current != NULL)
            ui->treeWidget->setCurrentItem(current);
    }

    if(current != NULL)
        current->setSelected(true);

    ui->pushButtonAddSubLevel->setEnabled(current != NULL);
    ui->pushButtonRemoveLevel->setEnabled(current != NULL);

    updateStepLabel();
}

void GFavoritesMenuDialog::on_pushButtonAddRootLevel_clicked()
{
    addRootLevel();
}

void GFavoritesMenuDialog::on_pushButtonAddSubLevel_clicked()
{
    addSubLevel();
}

void GFavoritesMenuDialog::on_pushButtonRemoveLevel_clicked()
{
    removeLevel();
}

bool GFavoritesMenuDialog::addSubLevel(const QString &name)
{
    bool ok = true;

    QTreeWidgetItem *selected = ui->treeWidget->currentItem();

    QList<QTreeWidgetItem*> l = ui->treeWidget->findItems(name, Qt::MatchFixedString|Qt::MatchRecursive);
    QListIterator<QTreeWidgetItem*> it(l);

    while(it.hasNext() && ok) {
        QTreeWidgetItem *item = it.next();
        ok = item->isHidden() || (item->parent() != selected);
    }

    if(!ok)
        QMessageBox::critical(this, tr("Erreur"), tr("Un niveau ayant ce nom existe déjà"));
    else {
        QTreeWidgetItem *child = new QTreeWidgetItem(QStringList() << name << "0");
        selected->addChild(child);
        ui->treeWidget->expandItem(selected);
        ui->treeWidget->setCurrentItem(child);
    }

    return ok;
}

bool GFavoritesMenuDialog::removeLevel(const QString &name)
{
    QList<QTreeWidgetItem*> l = ui->treeWidget->findItems(name, Qt::MatchFixedString|Qt::MatchRecursive);

    if(l.isEmpty())
        return false;

    QTreeWidgetItem *item = l.first();

    item->setHidden(true);

    return true;
}
