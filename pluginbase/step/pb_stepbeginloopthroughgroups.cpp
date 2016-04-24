#include "pb_stepbeginloopthroughgroups.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/inModel/tools/ct_instdresultmodelpossibility.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#include "ct_model/tools/ct_modelsearchhelper.h"

#define DEF_inResult_g "inResult_g"
#define DEF_inGroup  "inGroup"
#define DEF_inItem   "inItem"
#define DEF_inAttName   "inAtt"

#define DEF_outResult_g "outResult_g"
#define DEF_outRootGroup "rootGroup"

PB_StepBeginLoopThroughGroups::PB_StepBeginLoopThroughGroups(CT_StepInitializeData &dataInit) : CT_StepBeginLoop(dataInit)
{
}

PB_StepBeginLoopThroughGroups::~PB_StepBeginLoopThroughGroups()
{
}

QString PB_StepBeginLoopThroughGroups::getStepDescription() const
{
    return tr("Boucle sur les groupes");
}

QString PB_StepBeginLoopThroughGroups::getStepDetailledDescription() const
{
    return tr("Nécessite une CT_StepEndLoop pour terminer la boucle");
}

CT_VirtualAbstractStep* PB_StepBeginLoopThroughGroups::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette etape
    return new PB_StepBeginLoopThroughGroups(dataInit);
}

//////////////////// PROTECTED //////////////////

void PB_StepBeginLoopThroughGroups::createInResultModelListProtected()
{
    CT_InResultModelGroup *resultModel = createNewInResultModel(DEF_inResult_g, tr("Résultat"));
    resultModel->setZeroOrMoreRootGroup();
    resultModel->addGroupModel("", DEF_inGroup);
    resultModel->addItemModel(DEF_inGroup, DEF_inItem, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item"));
    resultModel->addItemAttributeModel(DEF_inItem, DEF_inAttName, QList<QString>() << CT_AbstractCategory::DATA_VALUE, CT_AbstractCategory::ANY, tr("Nom"));
}

// Redefine in children steps to complete ConfigurationDialog
void PB_StepBeginLoopThroughGroups::createPostConfigurationDialog(int &nTurns)
{
    Q_UNUSED(nTurns);
}

// Redefine in children steps to complete out Models
void PB_StepBeginLoopThroughGroups::createOutResultModelListProtected(CT_OutResultModelGroup *firstResultModel)
{
    Q_UNUSED(firstResultModel);

    CT_InAbstractResultModel *resultInModel = getInResultModel(DEF_inResult_g);
    CT_OutAbstractResultModel* resultInModelOut = NULL;
    CT_InAbstractGroupModel* groupModel = NULL;

    CT_OutResultModelGroup *resultModel = createNewOutResultModel(DEF_outResult_g, tr("Elément"));
    resultModel->setRootGroup(DEF_outRootGroup);

    // check if model have choice (can be empty if the step want to create a default out model list)
    if(resultInModel!=NULL && !resultInModel->getPossibilitiesSavedSelected().isEmpty())
    {
        resultInModelOut = (CT_OutAbstractResultModel*)resultInModel->getPossibilitiesSavedSelected().first()->outModel();
    }

    if(resultInModelOut != NULL)
    {
        groupModel = (CT_InAbstractGroupModel*)PS_MODELS->searchModel(DEF_inGroup, resultInModelOut, this);
    }

    if((groupModel != NULL) && !groupModel->getPossibilitiesSavedSelected().isEmpty())
    {
        _outModel = (DEF_CT_AbstractGroupModelOut*)groupModel->getPossibilitiesSavedSelected().first()->outModel()->copy();
        resultModel->addGroupModel(DEF_outRootGroup, _outModel);
    }

}

// Redefine in children steps to complete compute method
void PB_StepBeginLoopThroughGroups::compute(CT_ResultGroup *outRes, CT_StandardItemGroup* group)
{
    Q_UNUSED(outRes);
    Q_UNUSED(group);

    CT_ResultGroup *inResult = getInputResults().first();
    CT_ResultGroup *outResult_g = getOutResultList().at(1);

    if (outResult_g != NULL)
    {
        CT_StandardItemGroup* rootGroup = new CT_StandardItemGroup(DEF_outRootGroup, outResult_g);
        outResult_g->addGroup(rootGroup);
        int cpt = 1;

        CT_ResultGroupIterator it(inResult, this, DEF_inGroup);
        while (it.hasNext() && (!isStopped()))
        {
            CT_AbstractItemGroup *group = (CT_AbstractItemGroup*) it.next();

            if (cpt++ == _counter->getCurrentTurn())
            {
                rootGroup->addGroup((CT_AbstractItemGroup*)group->copy((CT_OutAbstractItemModel*)PS_MODELS->searchModel(_outModel->uniqueName(),outResult_g, this), outResult_g, CT_ResultCopyModeList() << CT_ResultCopyModeList::CopyItemDrawableReference));

                QString turnName = QString("Turn%1").arg(cpt - 1);
                CT_AbstractSingularItemDrawable* item = (CT_AbstractSingularItemDrawable*) group->firstItemByINModelName(this, DEF_inItem);
                if (item != NULL)
                {
                    CT_AbstractItemAttribute* att = item->firstItemAttributeByINModelName(inResult, this, DEF_inAttName);
                    if (att !=  NULL)
                    {
                        turnName = att->toString(item, NULL);
                    }
                }
                _counter->setTurnName(turnName);
            }
        }

        if (_counter->getCurrentTurn() == 1)
            _counter->setNTurns(cpt);

        NTurnsSelected();
    }

    setProgress( 100 );
}
