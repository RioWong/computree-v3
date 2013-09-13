/*
    Cette classe est le point d'entr�e du plugin. C'est elle qui initie la cr�ation d'une
    librairie (.dll ou .so) sp�cifique au plugin.
    Sa struture ne change pas d'un plugin � l'autre en dehors de sa d�nomination
*/

// Directives classique c++, permettant d'�viter d'inclure plusieurs fois un m�me fichier
#ifndef TUFR_PLUGINENTRY_H
#define TUFR_PLUGINENTRY_H

// Inclusion du fichier interface.h
// Ce fichier est le point le liaison avec PluginSharedV2
// En effet, ce fichier regroupe toutes les d�clarations de classes g�n�riques de PluginSharedV2
#include "interfaces.h"

// Inlcusion de la classe de gestion d'�tapes,
// On n'utilise pas de directive #include, � cause d'une inclusion r�ciproque des deux classes
class TUFR_StepPluginManager;

/*! \defgroup Steps_TUFR PluginTutorialFR (TUFR) steps
 */

// Cette classe h�tite de la classe g�n�rique PluginInterface de PluginSharedV2
// C'est le cas pour tout plugin d'�tapes
class TUFR_PluginEntry : public PluginInterface
{

// Macros Qt. La premi�re donne acc�s � certaines fonctionnalit�s Qt utiles
// La second permet de cr�er un plugin sous forme de librairie (.dll, .so)
    Q_OBJECT

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    Q_PLUGIN_METADATA(IID PluginInterface_iid)
#endif

    Q_INTERFACES(PluginInterface)

public:

// Constructeur et destructeur
    TUFR_PluginEntry();
    ~TUFR_PluginEntry();

// Permet de versionner les plugins
    QString getVersion() const;
// Lien avec le gestionnaire d'�tapes
    StepPluginInterface* getStepPluginManager();

private:
// Gestionnaire d'�tapes du plugin
    TUFR_StepPluginManager *_stepPluginManager;
};

#endif // TUFR_PLUGINENTRY_H
