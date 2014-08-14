#include "pb_stepuseritemselection.h"

// Inclusion of in models
#include "ct_itemdrawable/model/inModel/ct_inzeroormoregroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_instandardgroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_instandarditemdrawablemodel.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"

// Inclusion of out models
#include "ct_itemdrawable/model/outModel/ct_outstandardgroupmodel.h"

// Inclusion of standard result class
#include "ct_result/ct_resultgroup.h"

// Inclusion of used ItemDrawable classes
#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

//Inclusion of actions
#include "actions/pb_actionselectitemdrawablegv.h"

#include <QMessageBox>

// Alias for indexing in models
#define DEF_resultIn_R "R"
#define DEF_groupIn_G "G"
#define DEF_itemIn_I "I"

// Constructor : initialization of parameters
PB_StepUserItemSelection::PB_StepUserItemSelection(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    m_doc = NULL;
    m_removeGroupsWithoutItemResearched = true;
    m_removeParents = true;

    setManual(true);
}

// Step description (tooltip of contextual menu)
QString PB_StepUserItemSelection::getStepDescription() const
{
    return "Etape qui permet de sélectionner manuellement des ItemDrawable et génère un résultat de sortie avec les items sélectionnés";
}

// Step copy method
CT_VirtualAbstractStep* PB_StepUserItemSelection::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepUserItemSelection(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepUserItemSelection::createInResultModelListProtected()
{
    CT_InZeroOrMoreGroupModel *groupInModel_Z = new CT_InZeroOrMoreGroupModel();

    CT_InStdGroupModel *groupInModel_G = new CT_InStdGroupModel(DEF_groupIn_G,
                                                                          "Groupe");

    CT_InStandardItemDrawableModel *itemInModel_I = new CT_InStandardItemDrawableModel(DEF_itemIn_I, 
                                                                                       CT_AbstractSingularItemDrawable::staticGetType(),
                                                                                       tr("Item"));

    groupInModel_G->addItem(itemInModel_I);
    groupInModel_Z->addGroup(groupInModel_G);


    CT_InResultModelGroupToCopy *resultInModel_R = new CT_InResultModelGroupToCopy(DEF_resultIn_R, 
                                                                                   groupInModel_Z,
                                                                                   tr("Result"), 
                                                                                   tr(""), 
                                                                                   false);

    addInResultModel(resultInModel_R);
}

// Creation and affiliation of OUT models
void PB_StepUserItemSelection::createOutResultModelListProtected()
{
    // Get IN model corresponding to DEF_resultIn_R
    CT_InResultModelGroupToCopy *resultCopyModel_R = (CT_InResultModelGroupToCopy*)getInResultModel(DEF_resultIn_R);

    // Create an action list to modify the in model (empty in no modifications)
    QList<CT_AbstractOutModelCopyAction*> actions_resultCopyModel_R;

    // Add IN model copy (eventually modified) to OUT results
    addOutResultModelCopy(resultCopyModel_R, actions_resultCopyModel_R);
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepUserItemSelection::createPostConfigurationDialog()
{
    // No parameter dialog for this step
    CT_StepConfigurableDialog *dialog = newStandardPostConfigurationDialog();

    dialog->addBool("", "", tr("Supprimer les groupes ne contenant pas l'ItemDrawable recherché"), m_removeGroupsWithoutItemResearched);
    dialog->addBool("", "", tr("Supprimer les groupes parents si l'ItemDrawable recherché n'existe plus dans la structure sous-jacente"), m_removeParents);
}

void PB_StepUserItemSelection::compute()
{
    m_doc = NULL;
    m_status = 0;

    // DONT'T FORGET TO ADD THIS STEP TO THE STEPPLUGINMANAGER !!!!!


    // --------------------------
    // Gets IN results and models

    // ----------------------------------------------------------------------------
    // Get the group model corresponding to DEF_groupIn_G
    CT_InAbstractGroupModel* groupInModel_G = (CT_InAbstractGroupModel*)getInModelForResearchIfUseCopy(DEF_resultIn_R, DEF_groupIn_G);
    // Get the group model corresponding to DEF_itemIn_I
    CT_InAbstractItemDrawableModel* itemInModel_I = (CT_InAbstractItemDrawableModel*)getInModelForResearchIfUseCopy(DEF_resultIn_R, DEF_itemIn_I);


    // ---------------------------
    // Gets OUT results and models
    QList<CT_ResultGroup*> outResultList = getOutResultList();

    CT_ResultGroup *resultOut_R = outResultList.first();

    m_itemDrawableToAdd.clear();

    QList<CT_AbstractItemGroup*> groupsToRemove;

    // create a list of itemdrawable to add in the document
    for ( CT_AbstractItemGroup *groupOut_G = resultOut_R->beginGroup(groupInModel_G)
        ; groupOut_G != NULL  && !isStopped()
        ; groupOut_G = resultOut_R->nextGroup() )
    {
        CT_AbstractSingularItemDrawable *itemOut_I = groupOut_G->findFirstItem(itemInModel_I);

        if (itemOut_I != NULL)
            m_itemDrawableToAdd.insert(itemOut_I, groupOut_G);
        else if(m_removeGroupsWithoutItemResearched || m_removeParents)
            groupsToRemove.append(groupOut_G);
    }

    // request the manual mode
    requestManualMode();

    // remove item selected from the list (to not remove them from the out result)
    QListIterator<ItemDrawable*> it(m_itemDrawableSelected);

    while(it.hasNext()
          && !isStopped())
        m_itemDrawableToAdd.remove(it.next());

    groupsToRemove.append(m_itemDrawableToAdd.values());

    // we remove the parent group of all ItemDrawable that must be deleted from the out result
    // and all groups that don't contains a ItemDrawable researched
    QListIterator<CT_AbstractItemGroup*> itE(groupsToRemove);

    while(itE.hasNext())
    {
        CT_AbstractItemGroup *group = itE.next();

        recursiveRemoveGroup(group->parentGroup(), group);
    }

    m_status = 1;
    requestManualMode();
}

void PB_StepUserItemSelection::recursiveRemoveGroup(CT_AbstractItemGroup *parent, CT_AbstractItemGroup *group) const
{
    if(parent != NULL)
    {
        parent->removeGroup(group);

        if(m_removeParents && parent->isEmptyOfGroups())
            recursiveRemoveGroup(parent->parentGroup(), parent);
    }
    else
    {
        ((CT_ResultGroup*)group->result())->removeGroupSomethingInStructure(group);
    }
}

void PB_StepUserItemSelection::initManualMode()
{
    if(m_doc == NULL)
    {
        // create a new 3D document
        m_doc = getGuiContext()->documentManager()->new3DDocument();

        // set the action (a copy of the action is added at all graphics view, and the action passed in parameter is deleted)
        m_doc->setCurrentAction(new PB_ActionSelectItemDrawableGV());
    }

    m_itemDrawableSelected.clear();
    m_doc->removeAllItemDrawable();

    // TODO add async with GuiManagerInterface
    QHashIterator<ItemDrawable*, CT_AbstractItemGroup*> it(m_itemDrawableToAdd);

    while(it.hasNext())
        m_doc->addItemDrawable(*it.next().key());

    QMessageBox::information(NULL, tr("Mode manuel"), tr("Bienvenue dans le mode manuel de cette "
                                                         "étape de filtrage. Veuillez sélectionner les "
                                                         "éléments dans la vue graphique puis valider en cliquant "
                                                         "sur le pouce en haut de la fenêtre principale. Les éléments "
                                                         "sélectionnés seront gardés dans le résultat de sortie."), QMessageBox::Ok);
}

void PB_StepUserItemSelection::useManualMode(bool quit)
{
    if(m_status == 0)
    {
        if(quit)
        {
            m_itemDrawableSelected = m_doc->getSelectedItemDrawable();
        }
    }
    else if(m_status == 1)
    {
        if(!quit)
        {
            m_doc = NULL;

            quitManualMode();
        }
    }
}
