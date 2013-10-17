// Inclusion du fichier d'ent�te
#include "tufr_steptutorial03.h"

// Inclusion des mod�les de r�sultats in et out
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"

// Inclusion des mod�les de groupes in et out
#include "ct_itemdrawable/model/inModel/ct_inoneormoregroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstandardgroupmodel.h"

// Inclusion des mod�les d'items in et out
#include "ct_itemdrawable/model/inModel/ct_instandarditemdrawablemodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstandarditemdrawablemodel.h"

// Inclusion de la classe de r�sultat standard
#include "ct_result/ct_resultgroup.h"

// Inclusion des CT_ItemDrawables utiles dans l'�tape
#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/ct_pointcluster.h"

// Inclusion des autres �l�ments n�cessaires � la cr�ation d'une sc�ne
#include "ct_pointcloudindexvector.h"
#include "ct_element/ct_axisalignedboundingbox.h"

#include "qdebug.h" // pour afficher des messages � la console

#include "math.h"

// Alias des chaines de caract�res pour l'indexation des mod�les in et out
#define DEF_SearchInResult  "r"
#define DEF_SearchInScene   "sc"

#define DEF_SearchOutResult "r"
#define DEF_SearchOutGroup  "g"
#define DEF_SearchOutCluster  "cl"

// Constructeur : appel du constructeur de la classe m�re
//                Initialisation des param�tres (valeurs par d�faut)
TUFR_StepTutorial03::TUFR_StepTutorial03(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _nx = 3;
    _ny = 3;
    _nz = 3;
}

// Description de l'�tape (tooltip du menu contextuel)
QString TUFR_StepTutorial03::getStepDescription() const
{
    return "Extrait une placette entre zmin et zmax";
}

// M�thode de recopie de l'�tape
CT_VirtualAbstractStep* TUFR_StepTutorial03::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette �tape
    return new TUFR_StepTutorial03(dataInit);
}

//////////////////// PROTECTED //////////////////

// Cr�ation et affiliation des mod�les IN
void TUFR_StepTutorial03::createInResultModelListProtected()
{
    // D�claration et cr�ation du mod�le de groupe racine
    // Ici on utilise un CT_InOneOrMoreGroupModel, pour permettre
    // d'utiliser une sc�ne n'importe o� dans une hi�rachie des groupes
    CT_InOneOrMoreGroupModel *groupModel;
    groupModel = new CT_InOneOrMoreGroupModel();

    // D�claration et cr�ation du mod�le d'item sc�ne
    // Le 1er param�tre est l'alias d'indexation de ce mod�le
    // Le 2ieme param�tre sp�cifie la classe souhait�e pour l'Item NomDeClasse::staticGetType()
    // Le 3ieme param�tre donne un nom � ce mod�le d'item affichable par l'interface
    CT_InStandardItemDrawableModel *sceneItemModel;
    sceneItemModel = new CT_InStandardItemDrawableModel(DEF_SearchInScene,
                                                        CT_Scene::staticGetType(),
                                                        "Sc�ne");

    // On ajoute le mod�le d'item (sc�ne) au mod�le du groupe racine
    groupModel->addItem(sceneItemModel);

    // D�claration et cr�ation du mod�le de r�sultat
    // Le 1er param�tre est l'alias d'indexation du mod�le de r�sultat
    // Le 2ieme param�tre est le mod�le du groupe racine
    // Le 3ieme param�tre donne un nom au r�sultat
    CT_InResultModelGroup *resultModel = new CT_InResultModelGroup(DEF_SearchInResult,
                                                                   groupModel,
                                                                   "Une/des sc�nes");

    // Ajout du mod�le de r�sultat s�quenciellement dans cette �tape
    addInResultModel(resultModel);


  }

  // Cr�ation et affiliation des mod�les OUT
  void TUFR_StepTutorial03::createOutResultModelListProtected()
  {
      // D�claration et cr�ation du mod�le de groupe racine
      CT_OutStandardGroupModel *groupModel = new CT_OutStandardGroupModel(DEF_SearchOutGroup);

      // D�claration et cr�ation du mod�le d'item sc�ne
      // Le 1er param�tre est l'alias d'indexation de ce mod�le
      // Le 2ieme param�tre sp�cifie la classe qui sera cr�e, en fournissant une instance de cette classe
      // Le 3ieme param�tre donne un nom � ce mod�le d'item affichable par l'interface
      CT_OutStandardItemDrawableModel *clusterItemModel;
      clusterItemModel = new CT_OutStandardItemDrawableModel(DEF_SearchOutCluster,
                                                           new CT_PointCluster(),
                                                           "Groupe de points");

      // On ajoute le mod�le d'item (sc�ne) au mod�le du groupe racine
      groupModel->addItem(clusterItemModel);

      // D�claration et cr�ation du mod�le de r�sultat
      // Le 1er param�tre est l'alias d'indexation du mod�le de r�sultat
      // Le 2ieme param�tre est le mod�le du groupe racine
      // Le 3ieme param�tre donne un nom au r�sultat
      CT_OutResultModelGroup *resultModel = new CT_OutResultModelGroup(DEF_SearchOutResult,
                                                                       groupModel,
                                                                       "Clusters");

      // Ajout du mod�le de r�sultat s�quenciellement dans cette �tape
      addOutResultModel(resultModel);
  }

  // Cr�ation semi-automatique de la boite de dialogue de param�trage de l'�tape
  void TUFR_StepTutorial03::createPostConfigurationDialog()
  {
      // Cr�ation de la boite de dialog en elle-m�me
      // La m�thode newStandardPostConfigurationDialog cr�e la boite et l'affecte � l'�tape en cours
      // Ainsi la gestion et la suppression de cette boite sont g�r�s automatiquement
      CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

      // Ajout de contr�les unitaires de param�trages � la boite (un par param�tre)
      configDialog->addInt("Nombre clusters selon x", "", 1, 100, _nx);
      configDialog->addInt("Nombre clusters selon x", "", 1, 100, _ny);
      configDialog->addInt("Nombre clusters selon x", "", 1, 100, _nz);
  }

  void TUFR_StepTutorial03::compute()
  {
      // RESULTATS IN
      // R�cup�ration de la liste des r�sultats d'entr�e s�l�ctionn� par createInResultModelListProtected()
      // L'ordre dans cette liste est celui des addInResultModel() successifs
      QList<CT_ResultGroup*> inResultList = getInputResults();

      // Ici on a fait un seul addInResultModel(), donc on a un seul r�sultat � r�cup�rer : le premier
      CT_ResultGroup* inResult = inResultList.at(0);

      // On va r�cup�rer les mod�les IN des groupes et items que l'on souhaite indexer
      CT_InStandardItemDrawableModel *inSceneModel = (CT_InStandardItemDrawableModel*) getInModelForResearch(inResult, DEF_SearchInScene);


      // RESULTATS OUT
      // R�cup�ration de la liste des r�sultats de sortie cr��s par createOutResultModelListProtected()
      // L'ordre dans cette liste est celui des addOutResultModel() successifs
      QList<CT_ResultGroup*> outResultList = getOutResultList();

      // Ici on a fait un seul addOutResultModel(), donc on a un seul r�sultat � r�cup�rer : le premier
      CT_ResultGroup* outResult = outResultList.at(0);

      // On va r�cup�rer les mod�les OUT des groupes et items que l'on souhaite cr�er
      CT_OutStandardGroupModel* groupModel = (CT_OutStandardGroupModel*) getOutModelForCreation(outResult, DEF_SearchOutGroup);
      CT_OutStandardItemDrawableModel* clusterItemModel = (CT_OutStandardItemDrawableModel*) getOutModelForCreation(outResult, DEF_SearchOutCluster);

      // Cr�ation d'un tableau qui stockera les clusters cr��s
      QVector<CT_PointCluster*> clusterVector;
      clusterVector.resize(_nx*_ny*_nz);

      // ALGORITHME
      // Cr�ation d'un it�rateur sur les items ayant pour mod�le : inSceneModel
      if (inResult->recursiveBeginIterateItems(*inSceneModel))
      {
          // On r�cup�re la premi�re (la seule ici) sc�ne correspondant au mod�le dans ce groupe
          // HDE si on s'attend � plusieurs sc�nes, on peut mettre un for ou un while.
          //CT_Scene *scene;
          //while (scene = (CT_scene *scene) inResult->recursiveNextItem()) !=NULL)
          //{
          //}
          // ou for et meme on peut mettre le if dedans dans le for puisque c'est une init
          const CT_Scene *scene = (CT_Scene*) inResult->recursiveNextItem();

          // On s'assure qu'elle existe
          if (scene != NULL)
          {

              // R�cup�ration des limites min et max en (x,y,z) de la sc�ne d'entr�e
              QVector3D min, max;
              scene->getBoundingShape()->getBoundingBox(min, max);

              // Calcul de la taille des clusters
              float xClusterSize = (max.x() - min.x())/_nx;
              float yClusterSize = (max.y() - min.y())/_ny;
              float zClusterSize = (max.z() - min.z())/_nz;

              // Recup�ration du nuage de points et des index de la sc�ne
              const CT_AbstractPointCloud *pointcloud = scene->getPointCloud();
              const CT_AbstractPointCloudIndex *pointcloudIndex = scene->getPointCloudIndex();
              int cloudSize = pointcloudIndex->indexSize();

              // Parcours des points de la sc�ne
              for (int i = 0 ; i < cloudSize ; ++i)
              {
                  const CT_Point &point = (*pointcloud)[(*pointcloudIndex)[i]];

                  // Calcul des indices x, y et z du cluster o� affecter le point
                  int indiceX = (int) floor((point.x - min.x()) / xClusterSize);
                  int indiceY = (int) floor((point.y - min.y()) / yClusterSize);
                  int indiceZ = (int) floor((point.z - min.z()) / zClusterSize);

                  // Indice dans le tableau de clusters
                  int indice = indiceX + indiceY*_nx + indiceZ*_nx*_ny;

                  // Si le cluster n'existe pas on le cr��e
                  if (clusterVector[indice]==NULL)
                  {
                      clusterVector[indice] = new CT_PointCluster(clusterItemModel,
                                                                     outResult,
                                                                     (CT_AbstractPointCloud*) pointcloud,
                                                                     new CT_AxisAlignedBoundingBox());

                      // En s'assurant que sa boite englobante se mettra � jour lors de l'ajout de points
                      clusterVector[indice]->setUpdateBoundingShapeAutomatically(true);
                  }

                  // Ajout du point au cluster (son index en fait)
                  clusterVector.at(indice)->addPoint((*pointcloudIndex)[i]);
              }

              // On parcours les clusters cr��s
              for (int i = 0 ; i < clusterVector.size() ; i++)
              {
                  CT_PointCluster* cluster = clusterVector[i];
                  if (cluster!=NULL)
                  {
                      // Pour chaque cluster, ajout � un groupe et ajout du groupe au r�sultat
                      // Avec les mod�les OUT ad�quats
                      CT_StandardItemGroup* group = new CT_StandardItemGroup(groupModel,
                                                                             outResult);
                      group->addItemDrawable(cluster);
                      outResult->addGroup(group);
                  }
              }
          }
      }
  }
