// Balises C++, �vitant les doublons d'includes
#ifndef TUFR_STEPTUTORIAL01_H
#define TUFR_STEPTUTORIAL01_H


// Inclusion de la classe parente des �tapes
#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_step/abstract/ct_abstractstepcanbeaddedfirst.h"

/*! \class TUFR_StepTutorial01
 * \ingroup Steps_TUFR
 *
 * \brief Cette classe ne fait rien
 *
 * Son but est de montrer la gestion des boites de dialogues de param�trage
 *
 * \param _intParam Param�tre entier
 * \param _doubleParam Param�tre double
 * \param _boolParam Param�tre bool�en
 * \param _listParam Param�tre liste
 * \param _choiceParam Param�tre de choix
 * \param _fileNameParam Param�tre de nom de fichier
 *
 * Mod�le IN : Aucun
 *
 * Mod�le OUT : \n
 *  - CT_ResultGroup \n
 *
 */
// Une �tape doit h�riter de CT_AbstractStep (ou d'une classe en h�ritant)
// Ici, l'�tape ne prend pas de donn�es en entr�e, on h�rite donc de CT_AbstractStepCanBeAddedFirst
class TUFR_StepTutorial01 : public CT_AbstractStepCanBeAddedFirst
{
    // Macro Qt, donnant acc�s � certaines fonctionnalit�s
    // En l'occurence, permet de r�cup�rer le nom de l'�tape
    Q_OBJECT

public:

    // Constructeur de l'�tape
    TUFR_StepTutorial01(CT_StepInitializeData &dataInit);

    // M�thode fournissant la description de l'�tape (tooltip dans le menu contectuel)
    QString getStepDescription() const;

    // M�thode de copie de l'�tape, utilis�e lors de l'ajout d'une �tapes � partir du menu contextuel
    CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);

protected:

    // Sp�cifications des r�sultats d'entr�e souhait�s (IN)
    void createInResultModelListProtected();

    // Boite de saisie des param�tres de l'�tape
    void createPostConfigurationDialog();

    // Sp�cification des r�sultats de sortie cr��s par l'�tape (OUT)
    void createOutResultModelListProtected();

    // Etape de calcul, cr�ant les donn�es des r�sultats de sortie
    void compute();

private:
    // Dans la zone priv�e : les param�tres de l'�tapes (initilis�s dans le constructeur)
    int     _intParam;       /*!< Param�tre entier */
    double  _doubleParam;    /*!< Param�tre double */
    bool    _boolParam;      /*!< Param�tre bool�en */
    QString _listParam;      /*!< Param�tre liste */
    int     _choiceParam;    /*!< Param�tre de choix */
    QString _fileNameParam;  /*!< Param�tre de nom de fichier */

};

// Fin de balise
#endif // TUFR_STEPTUTORIAL01_H
