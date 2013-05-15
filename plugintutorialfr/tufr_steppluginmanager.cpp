/*
    Impl�mentation du gestionnaire d'�tapes
*/

// Inclusion des classes g�n�riques de PluginSharedV2 n�cessaires
#include "tufr_steppluginmanager.h"
#include "ct_stepseparator.h"
#include "ct_steploadfileseparator.h"
#include "ct_stepcanbeaddedfirstseparator.h"
#include "step/tufr_steptutorial02.h"

// Inclure ici toutes les ent�tes des classes d�finissant des �tapes dans le plugin
//#include "step/TUFR_stepcalculateplotindicator.h"

// Constructeur (appelle le constructeur g�n�rique)
TUFR_StepPluginManager::TUFR_StepPluginManager() : CT_AbstractStepPlugin()
{
}

// Destructeur (ne fait rien de particulier)
TUFR_StepPluginManager::~TUFR_StepPluginManager()
{
}

// Cette m�thode permet de construire le menu contextuel des �tapes normales du plugin
bool TUFR_StepPluginManager::loadGenericsStep()
{
    // Ajoute une nouvelle section d'�tapes (s�parateur dans le menu contextuel)
    CT_StepSeparator *sep = addNewSeparator(new CT_StepSeparator());

    // Ajout d'un �tape dans le menu (en fait l'�tape est ajout�e au s�parateur)
    sep->addStep(new TUFR_StepTutorial02(*createNewStepInitializeData(NULL)));
    //    sep->addStep(new TUFR_StepCalculatePlotIndicator(*createNewStepInitializeData(NULL)));

    // Si toutes les �tapes ont pu �tre ajout�es, la m�thode renvoie true (on peut continuer)
    return true;
}

// Cette m�thode permet de construire la liste des �tapes de chargement de fichier du plugin
// Si le plugin n'en contient pas, cette m�thode se contente de renvoyer true
bool TUFR_StepPluginManager::loadOpenFileStep()
{
    // Si toutes les �tapes ont pu �tre ajout�es, la m�thode renvoie true (on peut continuer)
    return true;
}

// Cette m�thode permet de construire la liste des �tapes ind�pendantes
// Les �tapes ind�pendantes, ne n�cessitent pas d'�tape m�re pour �tre ajout�e
// Pour autant ce ne sont pas des �tapes standard de chargement de fichier
bool TUFR_StepPluginManager::loadCanBeAddedFirstStep()
{
//    CT_StepCanBeAddedFirstSeparator *sep = addNewSeparator(new CT_StepCanBeAddedFirstSeparator());
//    sep->addStep(new TUFR_StepLoadFieldInventory(*createNewStepInitializeData(NULL)));

    // Si toutes les �tapes ont pu �tre ajout�es, la m�thode renvoie true (on peut continuer)
    return true;
}
