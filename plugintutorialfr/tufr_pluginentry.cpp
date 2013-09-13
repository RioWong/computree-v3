/*
    Impl�mentation de la classe TUFR_PluginEntry
*/

// Inclusion du fichier d'ent�te des du pluginManager (gestionnaire d'�tapes)
#include "tufr_pluginentry.h"
#include "tufr_steppluginmanager.h"

// Constructeur
TUFR_PluginEntry::TUFR_PluginEntry()
{
    // cr�ation effective et initialisation du gestionnaire d'�tapes
    _stepPluginManager = new TUFR_StepPluginManager();
    _stepPluginManager->init();
}

// Desctructeur : suppression du gestionnaire d'�tapes
TUFR_PluginEntry::~TUFR_PluginEntry()
{
    delete _stepPluginManager;
}

// Version du plugin
// Changer la cha�ne de caract�re lorsqu'une nouvelle version commence
// Attentions les scripts d'�tapes tiennent compte de la version du plugin
QString TUFR_PluginEntry::getVersion() const
{
    return "1.0";
}

// Renvoie le gestionnaire d'�tape cr��
StepPluginInterface* TUFR_PluginEntry::getStepPluginManager()
{
    return _stepPluginManager;
}

// Macro QT permettant la cr�ation de la librairie (.dll ou .so)
// Le premier param�tre doit �tre identique � celui pass� � la directive TARGET du fichier .pro
// Le second est le nom de la pr�sente classe
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(plug_tutorialfr, TUFR_PluginEntry)
#endif
