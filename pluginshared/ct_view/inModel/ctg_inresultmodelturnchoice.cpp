#include "ctg_inresultmodelturnchoice.h"
#include "ui_ctg_inresultmodelturnchoice.h"

#include "ct_turn/inTurn/ct_inturn.h"
#include "ct_outabstractresultmodel.h"
#include "ct_virtualabstractstep.h"

#include "ct_result/model/inModel/tools/ct_instdresultmodelpossibility.h"

#include <QDragEnterEvent>
#include <QMenu>
#include <QMimeData>

CTG_InResultModelTurnChoice::CTG_InResultModelTurnChoice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTG_InResultModelTurnChoice)
{
    ui->setupUi(this);

    ui->treeView->setModel(&_viewModel);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    constructHeader();

    connect(ui->treeView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(currentSelectionChanged(QModelIndex,QModelIndex)));
    connect(ui->treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showViewContextMenu(QPoint)));

    _turn = NULL;

    setAcceptDrops(true);
}

CTG_InResultModelTurnChoice::~CTG_InResultModelTurnChoice()
{
    delete ui;
}

void CTG_InResultModelTurnChoice::setTurn(const CT_InTurn *turn)
{
    _turn = (CT_InTurn*)turn;

    m_possibilityToSelect = NULL;
    m_itemToSelect = NULL;

    constructModel();
}

void CTG_InResultModelTurnChoice::setReadOnly(bool enabled)
{
    setAcceptDrops(!enabled);
}

void CTG_InResultModelTurnChoice::dragEnterEvent(QDragEnterEvent *event)
{
    if((event->source() != NULL)
            && acceptResult(event->mimeData()->text()))
    {
        event->acceptProposedAction();
    }
}

void CTG_InResultModelTurnChoice::dropEvent(QDropEvent *event)
{
    enableResultPossibility(event->mimeData()->text());
}

void CTG_InResultModelTurnChoice::clearModel()
{
    _viewModel.invisibleRootItem()->removeRows(0, _viewModel.invisibleRootItem()->rowCount());

    emit selectedPossibilityChanged(NULL);
}

void CTG_InResultModelTurnChoice::constructModel()
{
    clearModel();

    if(_turn != NULL)
    {
        QListIterator<CT_InAbstractResultModel*> it(_turn->models());

        while(it.hasNext())
        {
            QList<QStandardItem*> items = createItemsForResultModel(it.next());

            if(!items.isEmpty())
                _viewModel.invisibleRootItem()->appendRow(items);
        }
    }

    ui->treeView->expandAll();

    if(m_itemToSelect != NULL)
    {
        QModelIndex current = _viewModel.indexFromItem(m_itemToSelect);

        ui->treeView->selectionModel()->select(current, QItemSelectionModel::Rows | QItemSelectionModel::Select | QItemSelectionModel::Current);
        currentSelectionChanged(current, QModelIndex());
    }
}

void CTG_InResultModelTurnChoice::constructHeader()
{
    QStringList header;
    header << tr("Nom des résultats");
    header << tr("Etape");

    _viewModel.setHorizontalHeaderLabels(header);

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    ui->treeView->header()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(1, QHeaderView::ResizeToContents);
#else
    ui->treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
#endif
}

QList<QStandardItem*> CTG_InResultModelTurnChoice::createItemsForResultModel(CT_InAbstractResultModel *resModel)
{
    QList<QStandardItem*> retList;

    // Nom du modèle d'entrée
    QStandardItem *rootName = new QStandardItem(resModel->displayableName());
    rootName->setDragEnabled(false);
    rootName->setDropEnabled(true);
    rootName->setEditable(false);

    retList.append(rootName);

    QList<CT_InStdModelPossibility*> possibilities = resModel->getPossibilitiesSavedSelected();

    if(possibilities.isEmpty())
        return retList;

    int index = 0;

    QListIterator<CT_InStdModelPossibility*> it(possibilities);

    while(it.hasNext())
    {
        QList<QStandardItem*> list;

        CT_InStdResultModelPossibility *possibility = (CT_InStdResultModelPossibility*)it.next();

        // Nom du modèle de sortie
        CTG_InResultModelTurnChoiceItem *item = new CTG_InResultModelTurnChoiceItem(possibility->outModel()->displayableName());
        item->setDragEnabled(false);
        item->setDropEnabled(false);
        item->setEditable(false);
        item->_possibility = possibility;
        list.append(item);

        item = new CTG_InResultModelTurnChoiceItem(possibility->outModel()->step()->getStepCustomName() == possibility->outModel()->step()->getStepDisplayableName() ? possibility->outModel()->step()->getStepExtendedDisplayableName() : possibility->outModel()->step()->getStepCustomName());
        item->setDragEnabled(false);
        item->setDropEnabled(false);
        item->setEditable(false);
        item->_possibility = possibility;
        list.append(item);

        rootName->appendRow(list);

        if(possibility == m_possibilityToSelect)
            m_itemToSelect = item;

        ++index;
    }

    return retList;
}

CT_InAbstractResultModel *CTG_InResultModelTurnChoice::getModelFromMimeData(const QString &mimeData)
{
    if(acceptResult(mimeData))
    {
        QStringList values = mimeData.split(";;");
        QString name = values.first();

        CT_InAbstractResultModel *model = NULL;
        QListIterator<CT_InAbstractResultModel*> it(_turn->models());

        while((model == NULL)
              && it.hasNext())
        {
            model = it.next();

            if(model->uniqueName() != name)
                model = NULL;
        }

        return model;
    }

    return NULL;
}

CT_InStdModelPossibility* CTG_InResultModelTurnChoice::getPossibilityFromMimeData(const QString &mimeData, int *outPossibilityIndex)
{
    CT_InAbstractResultModel *model =  getModelFromMimeData(mimeData);

    if(model != NULL) {
        QStringList values = mimeData.split(";;");
        int possibilityIndex = values.at(1).toInt();

        if(outPossibilityIndex != NULL)
            *outPossibilityIndex = possibilityIndex;

        if(model->getPossibilitiesSaved().size() > possibilityIndex)
            return model->getPossibilitiesSaved().at(possibilityIndex);
    }

    return NULL;
}

bool CTG_InResultModelTurnChoice::acceptResult(const QString &mimeData)
{
    // on accepte le drag si c'est un texte qui contient deux élément
    // séparés par ";;" (le premier étant le nom du résultat modèle d'entrée
    // et le second l'index de la possibilité à sélectionner) et si la nouvel possibilité
    // peut être sélectionnée
    if(_turn != NULL)
    {
        QStringList values = mimeData.split(";;");

        if(values.size() == 2)
        {
            bool ok;
            QString name = values.first();
            int possibilityIndex = values.at(1).toInt(&ok);

            if(ok)
            {
                CT_InAbstractResultModel *model = NULL;
                QListIterator<CT_InAbstractResultModel*> it(_turn->models());

                while((model == NULL)
                      && it.hasNext())
                {
                    model = it.next();

                    if(model->uniqueName() != name)
                        model = NULL;
                }

                if(model != NULL)
                {
                    QList<CT_InStdModelPossibility*> poss = model->getPossibilitiesSaved();

                    if((possibilityIndex >= 0)
                            && (possibilityIndex < poss.size()))
                        return true;
                }
            }
        }
    }

    return false;
}

// SLOTS //

void CTG_InResultModelTurnChoice::currentSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    if(_turn != NULL)
    {
        CTG_InResultModelTurnChoiceItem *item = dynamic_cast<CTG_InResultModelTurnChoiceItem*>(_viewModel.itemFromIndex(current));

        if(item != NULL)
        {
            emit selectedPossibilityChanged(item->_possibility);
        }
        else
        {
            emit selectedPossibilityChanged(NULL);
        }
    }
    else
    {
        emit selectedPossibilityChanged(NULL);
    }
}

void CTG_InResultModelTurnChoice::showViewContextMenu(const QPoint &point)
{
    QModelIndexList list = ui->treeView->selectionModel()->selectedRows();

    if(!list.isEmpty())
    {
        CTG_InResultModelTurnChoiceItem *item = dynamic_cast<CTG_InResultModelTurnChoiceItem*>(_viewModel.itemFromIndex(list.first()));

        if(item != NULL)
        {
            QMenu *menu = new QMenu();
            QAction *action = menu->addAction("Supprimer");

            connect(action, SIGNAL(triggered()), this, SLOT(removeSelectedPossibility()), Qt::DirectConnection);
            menu->exec(ui->treeView->viewport()->mapToGlobal(point));
        }
    }
}

void CTG_InResultModelTurnChoice::removeSelectedPossibility()
{
    QModelIndexList list = ui->treeView->selectionModel()->selectedRows();

    if(!list.isEmpty())
    {
        CTG_InResultModelTurnChoiceItem *item = dynamic_cast<CTG_InResultModelTurnChoiceItem*>(_viewModel.itemFromIndex(list.first()));

        if(item != NULL)
        {
            item->_possibility->setSelected(false);
            item->parent()->removeRow(item->row());
            emit selectedPossibilityChanged(NULL);
        }
    }
}

void CTG_InResultModelTurnChoice::enableResultPossibility(const QString &mimeData)
{
    m_possibilityToSelect = NULL;
    CT_InAbstractResultModel *model = getModelFromMimeData(mimeData);

    if(model != NULL) {
        int possibilityIndex;
        m_possibilityToSelect = getPossibilityFromMimeData(mimeData, &possibilityIndex);

        if(m_possibilityToSelect != NULL)
        {
            // on coche la possibilité
            m_possibilityToSelect->setSelected(true);

            // on coche par défaut cette possibilité si c'est possible ainsi que ses enfants
            if(model->canSelectPossibilitiesByDefault(QList<int>() << possibilityIndex, true))
                model->selectPossibilitiesByDefault(QList<int>() << possibilityIndex, true);
        }
    }

    // on reconstruit le modèle
    m_itemToSelect = NULL;

    constructModel();
}

void CTG_InResultModelTurnChoice::disableResultPossibility(const QString &mimeData)
{
    m_possibilityToSelect = getPossibilityFromMimeData(mimeData, NULL);

    // on décoche la possibilité
    if(m_possibilityToSelect != NULL)
        m_possibilityToSelect->setSelected(false);

    // on reconstruit le modèle
    m_itemToSelect = NULL;

    constructModel();
}

void CTG_InResultModelTurnChoice::showResultPossibility(const QString &mimeData)
{
    m_possibilityToSelect = getPossibilityFromMimeData(mimeData, NULL);

    // on reconstruit le modèle
    m_itemToSelect = NULL;

    constructModel();
}
