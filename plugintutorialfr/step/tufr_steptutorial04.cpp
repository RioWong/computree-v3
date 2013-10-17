// Inclusion du fichier d'ent�te
#include "tufr_steptutorial04.h"

// Inclusion des mod�les de r�sultats in et out, PERMETTANT LA COPIE
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"

// Inclusion des mod�les de groupes in et out
#include "ct_itemdrawable/model/inModel/ct_inzeroormoregroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_instandardgroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstandardgroupmodel.h"

// Inclusion des mod�les d'items in et out
#include "ct_itemdrawable/model/inModel/ct_instandarditemdrawablemodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstandarditemdrawablemodel.h"

// Inclusion de la classe de r�sultat standard
#include "ct_result/ct_resultgroup.h"

// Inclusion des item utiles
#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/ct_pointcluster.h"
#include "ct_itemdrawable/ct_referencepoint.h"

// Inclusion du syst�me d'action
#include "ct_tools/model/ct_outmodelcopyactionaddmodelitemingroup.h"

#include "math.h"

// Alias des chaines de caract�res pour l'indexation des mod�les in et out
#define DEF_SearchInGroup           "g"
#define DEF_SearchInPointCluster    "p"
#define DEF_SearchInResult          "r"
#define DEF_SearchOutResult         "r"


TUFR_StepTutorial04::TUFR_StepTutorial04(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    // pas de param�tres
}

// Description de l'�tape (tooltip du menu contextuel)
QString TUFR_StepTutorial04::getStepDescription() const
{
    return tr("G�n�rer des ReferencePoints � partir des barycentres de PointClusters");
}

// M�thode de recopie de l'�tape
CT_VirtualAbstractStep* TUFR_StepTutorial04::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette �tape
    return new TUFR_StepTutorial04(dataInit);
}

//////////////////// PROTECTED //////////////////

// Cr�ation et affiliation des mod�les IN
void TUFR_StepTutorial04::createInResultModelListProtected()
{
    // D�claration et cr�ation du mod�le de groupe racine
    // Groupe optionnel, pouvant se trouver � tout endroit de la hi�rachie
    CT_InZeroOrMoreGroupModel *groupR = new CT_InZeroOrMoreGroupModel();

    // Groupe contenant le cluster (index� car nous allons y ajouter quelque chose)
    CT_InStandardGroupModel *group = new CT_InStandardGroupModel(DEF_SearchInGroup);

    // le groupe doit contenir un ItemDrawable de type nuage de points
    CT_InStandardItemDrawableModel *item = new CT_InStandardItemDrawableModel(DEF_SearchInPointCluster,
                                                                              CT_PointCluster::staticGetType(),
                                                                              tr("Groupe de points"));

    group->addItem(item);
    groupR->addGroup(group);

    // le r�sultat est du type ResultGroupToCopy pour que le syst�me face une copie du
    // r�sultat d'entr�e, que nous compl�terons
    addInResultModel(new CT_InResultModelGroupToCopy(DEF_SearchInResult,
                                                     groupR,
                                                     tr("Groupe(s) de points")));
}

// Cr�ation et affiliation des mod�les OUT
void TUFR_StepTutorial04::createOutResultModelListProtected()
{
    // on r�cup�re le r�sultat mod�le d'entr�e � copier d�fini dans "createInResultModelListProtected()"
    CT_InResultModelGroupToCopy *intResModelToCopy = (CT_InResultModelGroupToCopy*)getInResultModel(DEF_SearchInResult);

    // On cr��e le mod�le d'item (CT_ReferencePoint) � ajouter
    CT_OutStandardItemDrawableModel *refPointModel = new CT_OutStandardItemDrawableModel("", new CT_ReferencePoint(), tr("Barycentre"));

    // On cr��e une liste d'action � executer sur la copie du mod�le
    QList<CT_AbstractOutModelCopyAction*> actions;


    // On ajoute une action permettant d'ajouter le mod�le d'item cr��
    // Cette action prend en param�tre � g�n�rateur de nom automatique : _outRefPointModelName
    actions << new CT_OutModelCopyActionAddModelItemInGroup(DEF_SearchInGroup,
                                                            refPointModel,
                                                            _outRefPointModelName);

    // On ajoute le mod�le de sortie modifier
    // En r�alit� cette m�thode va faire effectivement la copie du r�sultat d'entr�e
    addOutResultModelCopy(intResModelToCopy, actions);

}

// Cr�ation semi-automatique de la boite de dialogue de param�trage de l'�tape
void TUFR_StepTutorial04::createPostConfigurationDialog()
{
    // Pas de boite de dialogue
}

void TUFR_StepTutorial04::compute()
{
    // on r�cup�re le r�sultat copi� (r�sultat de sortie)
    CT_ResultGroup *outRes = getOutResultList().first();

    /********* RECHERCHE DES MODELES DE SORTIE ************/

    // on utilise le mod�le du r�sultat de sortie pour rechercher le mod�le de sortie du CT_ReferencePoint.
    // En utilisant bien sur le nom automatique g�n�r� lors de l'ajout
    CT_OutAbstractItemDrawableModel *outRefPointModel = (CT_OutAbstractItemDrawableModel*) getOutModelForCreation(outRes, _outRefPointModelName.completeName());

    /********* RECHERCHE DES MODELES D'ENTREE ************/
    // On r�cup�re les mod�les d'entr�e du groupe et du groupe de points
    CT_InAbstractGroupModel *inGroupModel = (CT_InAbstractGroupModel*)getInModelForResearchIfUseCopy(DEF_SearchInResult, DEF_SearchInGroup);
    CT_InAbstractItemDrawableModel *inPointClusterModel = (CT_InAbstractItemDrawableModel*)getInModelForResearchIfUseCopy(DEF_SearchInResult, DEF_SearchInPointCluster);

    /**************************************************/
    // on va rechercher tous les groupes contenant des nuages de points (qui ont �t� choisi par l'utilisateur)
    for ( CT_AbstractItemGroup *group = outRes->beginGroup(inGroupModel)
        ; group != NULL  && !isStopped()
        ; group = outRes->nextGroup() )
    {
        // On r�cup�re le groupe de points � l'aide du mod�le
        const CT_PointCluster *item = (const CT_PointCluster*) group->findFirstItem(inPointClusterModel);

        // On v�rifie qu'il existe
        if(item != NULL)
        {
            // On r�cup�re le barycentre auto-calcul� du groupe de points
            const CT_PointClusterBarycenter barycentre = item->getBarycenter();

            // coordonn�es du barycentre
            float xref = barycentre.x();
            float yref = barycentre.y();
            float zref = barycentre.z();

            // calcul du bufferXY
            // Maximum de la distance point/refPoint pour chaque segment
            float buffer = 0;
            const CT_AbstractPointCloud *cloud = item->getPointCloud();
            const CT_AbstractPointCloudIndex *cloudIndex = item->getPointCloudIndex();

            int indexsize = cloudIndex->indexSize();
            for (int i = 0 ; i < indexsize ; i++)
            {
                int index = cloudIndex->operator [](i);
                const CT_Point point = cloud->operator [](index);
                float distance = pow(xref-point.x, 2) + pow(yref-point.y, 2);
                if (distance > buffer) {buffer = distance;}
            }
            if (buffer > 0) {buffer = sqrt(buffer);}

            // et on ajoute un referencePoint
            CT_ReferencePoint *refPoint = new CT_ReferencePoint(outRefPointModel, outRes, xref, yref, zref, buffer);
            group->addItemDrawable(refPoint);

        }
    }

}
