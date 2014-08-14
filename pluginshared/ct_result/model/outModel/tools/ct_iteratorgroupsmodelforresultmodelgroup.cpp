#include "ct_iteratorgroupsmodelforresultmodelgroup.h"

#include "ct_result/tools/iterator/private/ct_treestructureforiterator.h"
#include "ct_itemdrawable/abstract/ct_abstractitemgroup.h"

CT_IteratorGroupsModelForResultModelGroup::CT_IteratorGroupsModelForResultModelGroup(const CT_OutAbstractResultModelGroup *resultModel)
{
    _rootTreeModels = NULL;
    _currentTreeModelsChild = NULL;

    _resultGroup = (CT_OutAbstractResultModelGroup*)resultModel;

    initBeginIterate();
}

CT_IteratorGroupsModelForResultModelGroup::~CT_IteratorGroupsModelForResultModelGroup()
{
    clearTreeStructure();
}

bool CT_IteratorGroupsModelForResultModelGroup::recursiveBeginIterateGroups(const DEF_CT_AbstractGroupModelIn *model)
{
    initBeginIterate();

    // si on n'a pas de groupe on n'a rien à parcourir
    if(_resultGroup->rootGroup() == NULL)
        return false;

    if(model == NULL)
        return false;

    CT_TreeStructureConstructor constructor;

    // création de la structure à parcourir en fonction des possibilités sélectionnées du modèle d'entrée
    QList<CT_InStdModelPossibility*> savedChecked = model->getPossibilitiesSavedSelected();

    QListIterator<CT_InStdModelPossibility*> itChecked(savedChecked);

    while(itChecked.hasNext())
        constructor.addModelsForGroupsToStructure(itChecked.next());

    _rootTreeModels = constructor.takeStructure();

    // si il n'y a aucun élément dans la structure on n'a rien à parcourir
    if(_rootTreeModels == NULL)
        return false;

    // si les groupes de ce résultat modèle ont un modèle différent de celui qu'on cherche : on n'a rien à parcourir
    if(_resultGroup->rootGroup()->uniqueName() != _rootTreeModels->outModel()->uniqueName())
        return false;

    // le groupe de ce résultat
    _currentGroup = _resultGroup->rootGroup();

    // si on ne doit pas renvoyer les groupes de ce résultat
    // mais des groupes enfants
    if(_rootTreeModels->nChildren() > 0)
    {
        _currentTreeModelsChild = _rootTreeModels;

        // on construit la liste des groupes à parcourir (en descendant dans l'arbre à partir des modèles)
        if(!continueSearch())
            return false;
    }

    // c'est bon on a trouver les groupes contenant l'item recherché
    return true;
}

DEF_CT_AbstractGroupModelOut* CT_IteratorGroupsModelForResultModelGroup::recursiveNextGroup()
{
    DEF_CT_AbstractGroupModelOut *group = NULL;

    // si on doit renvoyer les groupes de ce résultat car c'est eux qui
    // contiennent l'item recherché
    if(_currentTreeModelsChild == NULL)
    {
        group = _currentGroup;

        _currentGroup = NULL;
        return group;
    }

    if(_currentGroup == NULL)
        return NULL;

    // sinon
    do
    {
        if(_currentTreeModelsChild->isLastIncrementChildIndexSuccess())
        {
            // si il y a encore un groupe à retourner
            if((group = _currentGroup->findGroup(_currentTreeModelsChild->currentChild()->outModel()->uniqueName())) != NULL)
            {
                // on passe au modèle suivant pour le prochain appel
                _currentTreeModelsChild->incrementChildIndexAndSetCurrentChild();

                return group;
            }

            // sinon on essaye de passer au modèle suivant
            _currentTreeModelsChild->incrementChildIndexAndSetCurrentChild();
        }

        // si on ne peux plus continuer la recherche on n'a plus rien à retourner
        if(!continueSearch())
            return NULL;

    }while(group != NULL);

    return NULL;
}

// PRIVATE //

void CT_IteratorGroupsModelForResultModelGroup::initBeginIterate()
{
    clearTreeStructure();
    _currentTreeModelsChild = NULL;
    _groups.clear();

    _currentGroup = NULL;
}

bool CT_IteratorGroupsModelForResultModelGroup::continueSearch()
{
    bool find;
    DEF_CT_AbstractGroupModelOut *lastParentGroup = NULL;

    do
    {
        find = false;

        // si le groupe courant est un groupe de ce résultat
        if(_currentTreeModelsChild == _rootTreeModels)
        {
            // si le dernier passage au modèle suivant à échoué
            if(!_currentTreeModelsChild->isLastIncrementChildIndexSuccess())
            {
                // on n'a plus rien à retourner
                _currentGroup = NULL;
                return false;
            }

            bool continueLoop;

            do
            {
                continueLoop = false;

                // si aucun groupe fils correspondant au modèle recherché n'a été trouvé
                if(_currentGroup->findGroup(_rootTreeModels->currentChild()->outModel()->uniqueName()) == NULL)
                {
                    // et si il n'y a plus de modèle suivant
                    if(!_rootTreeModels->incrementChildIndexAndSetCurrentChild())
                    {
                        // on a plus rien à parcourir
                        _currentGroup = NULL;
                        return false;
                    }
                    else // sinon
                    {
                        // on recommence le parcours de la liste
                        _currentGroup = _resultGroup->rootGroup();

                        continueLoop = true;

                        // mais avec le modèle suivant
                    }
                }

            }while(continueLoop);

            // si on doit renvoyer les groupes des groupes de ce résultat, on dit qu'on a trouvé
            // puisqu'on aura pas besoin de descendre dans l'arbre et il faudra sortir de la boucle principale
            if(_rootTreeModels->currentChild()->currentChild() == NULL)
                find = true;
        }

        DEF_CT_AbstractGroupModelOut *lastValidGroup = _currentGroup;

        // si on a pas trouvé le groupe recherché
        if(!find)
        {
            if(_currentTreeModelsChild->isLastIncrementChildIndexSuccess())
            {
                // on descend dans l'arbre tant qu'on doit (currentChild() != NULL)
                // et tant qu'on peut (nextGroup() != NULL)
                while((_currentTreeModelsChild->currentChild()->currentChild() != NULL)
                        && ((_currentGroup = _currentGroup->findGroup(_currentTreeModelsChild->currentChild()->outModel()->uniqueName())) != NULL))
                {
                    // on se déplace dans la liste des modèles
                    _currentTreeModelsChild = _currentTreeModelsChild->currentChild();

                    // et on continue de descendre dans l'arbre
                    find = (_currentGroup->findGroup(_currentTreeModelsChild->currentChild()->outModel()->uniqueName()) != NULL);

                    lastValidGroup = _currentGroup;
                }
            }
        }

        // si on a pas trouver ce que l'on cherche : il faut remonter dans l'arbre
        // d'un cran puis redescendre jusqu'à trouver

        // si on a pas trouvé ce que l'on cherche
        if(!find)
        {
            bool goBackInTreeModels = true;

            // on remonte dans l'arbre des groupes :

            // si on est sortie de la boucle de tout à l'heure
            // car on n'a pas trouvé un groupe suivant dans l'arbre
            if(_currentGroup == NULL)
            {
                // si ce groupe était déjà le groupe parent du groupe qui n'avait plus de groupe suivant (je vous met
                // au défi de comprendre cette phrase ! faut la lire doucement.....)
                if(lastParentGroup == lastValidGroup)
                {
                    // alors on va essayer de passer au modèle suivant pour ce groupe

                    // si il y a un modèle suivant
                    if(_currentTreeModelsChild->incrementChildIndexAndSetCurrentChild())
                    {
                        // on ne remonte pas dans l'arbre des modèles puisqu'on
                        // ne remonte pas non plus dans l'arbre des groupes
                        goBackInTreeModels = false;
                    }
                    else // sinon
                    {
                        // on remonte encore d'un cran
                        _currentGroup = lastParentGroup->parentGroup();
                    }
                }
                else
                {
                    // on remonte d'un cran pour recommencer le parcours à partir du groupe parent
                    _currentGroup = lastValidGroup;
                }
            }
            // sinon c'est qu'on n'est pas rentré du tout dans la boucle
            // ce qui veut dire que l'on vient de la méthode "recursiveNextItem()"
            // qui a appelé cette méthode pour continuer la recherche
            else
            {
                // il n'y a plus de groupe suivant donc il faut remonter dans
                // l'arbre en passant au groupe parent pour recommencer
                // le parcours à partir de celui-ci
                _currentGroup = _currentGroup->parentGroup();
                lastParentGroup = _currentGroup;
            }

            // on remonte dans l'arbre des modèles
            if(goBackInTreeModels)
            {
                _currentTreeModelsChild = _currentTreeModelsChild->parent();

                // on passe au modèle suivant
                _currentTreeModelsChild->incrementChildIndexAndSetCurrentChild();

                // si on est revenu à la racine
                if(_currentTreeModelsChild == _rootTreeModels)
                {
                    // si on a n'a pas pu passer au modèle suivant
                    if(!_currentTreeModelsChild->isLastIncrementChildIndexSuccess())
                    {
                        // on a plus rien à parcourir
                        _currentGroup = NULL;
                        return false;
                    }
                }
            }
        }

    // on recommence tant qu'on est pas arrivé au bout et tant qu'on a pas
    // trouvé ce qu'on cherchait
    }while((_currentGroup != NULL)
           && !find);

    // si on est arrivé au bout sans rien trouver on retourne false sinon c'est bon on peut utiliser _currentGroup
    return (_currentGroup != NULL);
}

void CT_IteratorGroupsModelForResultModelGroup::clearTreeStructure()
{
    delete _rootTreeModels;
    _rootTreeModels = NULL;
}

