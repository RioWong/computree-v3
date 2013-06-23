// Balises C++, �vitant les doublons d'includes
#ifndef TUFR_STEPTUTORIAL02_H
#define TUFR_STEPTUTORIAL02_H


// Inclusion de la classe parente des �tapes
#include "ct_step/abstract/ct_abstractstepcanbeaddedfirst.h"


/*! \class TUFR_StepTutorial02
 *
 * \brief Cette classe g�n�re une sc�ne de points de positions al�atoires
 *
 * \param _n Nombre de points al�atoires � g�n�rer
 * \param _xmin X maximum de la sc�ne � g�n�rer
 * \param _xmax X minimum de la sc�ne � g�n�rer
 * \param _ymin Y maximum de la sc�ne � g�n�rer
 * \param _ymax Y minimum de la sc�ne � g�n�rer
 * \param _zmin Z minimum de la sc�ne � g�n�rer
 * \param _zmax Z maximum de la sc�ne � g�n�rer
 *
 * Mod�le IN : Aucun
 *
 * Mod�le OUT : \n
 *  - CT_ResultGroup \n
 *      - CT_StandardItemGroup \n
 *          - CT_Scene \n
 *
 */
// Une �tape doit h�riter de CT_AbstractStep (ou d'une classe en h�ritant)
// Ici, l'�tape ne prend pas de donn�es en entr�e, on h�rite donc de CT_AbstractStepCanBeAddedFirst
class TUFR_StepTutorial02 : public CT_AbstractStepCanBeAddedFirst
{
    // Macro Qt, donnant acc�s � certaines fonctionnalit�s
    // En l'occurence, permet de r�cup�rer le nom de l'�tape
    Q_OBJECT

public:

    // Constructeur de l'�tape
    TUFR_StepTutorial02(CT_StepInitializeData &dataInit);

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
    int     _n;      /*!< Nombre de points al�atoires � g�n�rer */
    double _xmin;   /*!< X maximum de la sc�ne � g�n�rer */
    double _xmax;   /*!< X minimum de la sc�ne � g�n�rer */
    double _ymin;   /*!< Y maximum de la sc�ne � g�n�rer */
    double _ymax;   /*!< Y minimum de la sc�ne � g�n�rer */
    double _zmin;   /*!< Z minimum de la sc�ne � g�n�rer */
    double _zmax;   /*!< Z maximum de la sc�ne � g�n�rer */

};

// Fin de balise
#endif // TUFR_STEPTUTORIAL02_H
