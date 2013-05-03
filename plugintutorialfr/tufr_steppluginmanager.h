/*
  Classe d�finissant le gestionnaire d'�tapes
  C'est elle qui rend effectivement disponible les �tapes impl�ment�es
*/

#ifndef TUFR_STEPPLUGINMANAGER_H
#define TUFR_STEPPLUGINMANAGER_H

// Inclusion de l'interface g�n�rique de gestionnaire d'�tapes, issue de PluginSharedV2
#include "ct_abstractstepplugin.h"

class TUFR_StepPluginManager : public CT_AbstractStepPlugin
{
public:
    // Constructeur et destructeur
    TUFR_StepPluginManager();
    ~TUFR_StepPluginManager();

protected:

    // D�claration des �tapes normales
    bool loadGenericsStep();
    // D�claration des �tapes de chargement de fichier (d�but de script)
    bool loadOpenFileStep();
    // D�claration des �tapes pouvant �tre ajout�e ind�pendament d'�tapes pr�c�dentes
    bool loadCanBeAddedFirstStep();

};

#endif // TUFR_STEPPLUGINMANAGER_H
