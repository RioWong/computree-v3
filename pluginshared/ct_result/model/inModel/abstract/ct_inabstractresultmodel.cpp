#include "ct_inabstractresultmodel.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibilitygroup.h"
#include "ct_result/model/inModel/tools/ct_instdresultmodelpossibility.h"

CT_InAbstractResultModel::CT_InAbstractResultModel(const QString &uniqueName,
                                                   const QString &description,
                                                   const QString &displayableName,
                                                   bool recursive) : CT_InAbstractModel(uniqueName,
                                                                                        description,
                                                                                        displayableName)
{
    m_recursive = recursive;
    m_backupModel = NULL;
}

void CT_InAbstractResultModel::setRecursive(bool r)
{
    m_recursive = r;
}

bool CT_InAbstractResultModel::isRecursive() const
{
    return m_recursive;
}

bool CT_InAbstractResultModel::setMinimumNumberOfPossibilityThatMustBeSelectedForOneTurn(int n)
{
    return possibilitiesGroup()->setMinimumNumberOfPossibilityThatMustBeSelected(n);
}

int CT_InAbstractResultModel::minimumNumberOfPossibilityThatMustBeSelectedForOneTurn() const
{
    return possibilitiesGroup()->minimumNumberOfPossibilityThatMustBeSelected();
}

bool CT_InAbstractResultModel::setMaximumNumberOfPossibilityThatCanBeSelectedForOneTurn(int n)
{
    return possibilitiesGroup()->setMaximumNumberOfPossibilityThatCanBeSelected(n);
}

int CT_InAbstractResultModel::maximumNumberOfPossibilityThatCanBeSelectedForOneTurn() const
{
    return possibilitiesGroup()->maximumNumberOfPossibilityThatCanBeSelected();
}

bool CT_InAbstractResultModel::recursiveIsAtLeastOnePossibilitySelectedIfItDoes() const
{
    // if this model not need possiblities
    if(!needOutputModel())
        return true;

    if(!isAtLeastOnePossibilitySelectedIfItDoes())
        return false;

    QList<CT_AbstractModel *> r;
    QList<CT_InStdModelPossibility*> lP = getPossibilitiesSavedSelected();
    QListIterator<CT_InStdModelPossibility*> itP(lP);

    while(itP.hasNext())
        r.append(((CT_InStdResultModelPossibility*)itP.next())->inModel());

    QListIterator<CT_AbstractModel*> it(r);

    while(it.hasNext())
    {
        // if no possibilities of this children (and recursively) is selected : we return false
        if(!((CT_InAbstractModel*)it.next())->recursiveIsAtLeastOnePossibilitySelectedIfItDoes())
            return false;
    }

    // all it's ok
    return true;
}

bool CT_InAbstractResultModel::canSelectPossibilitiesByDefault(const QList<int> &possibilitiesIndex, bool selectChildrensTooRecursively) const
{
    if(!CT_InAbstractModel::canSelectPossibilitiesByDefault(possibilitiesIndex, false))
        return false;

    if(selectChildrensTooRecursively)
    {
        // check if a children is not valid
        foreach (int v, possibilitiesIndex) {
            if(!((CT_InStdResultModelPossibility*)possibilitiesGroup()->getPossibilities().at(v))->inModel()->recursiveCanSelectPossibilitiesByDefault())
                return false;
        }
    }

    return true;
}

bool CT_InAbstractResultModel::selectPossibilitiesByDefault(const QList<int> &possibilitiesIndex, bool selectChildrensTooRecursively)
{
    if(!canSelectPossibilitiesByDefault(possibilitiesIndex, selectChildrensTooRecursively))
        return false;

    if(selectChildrensTooRecursively)
    {
        foreach (int v, possibilitiesIndex) {
            if(!((CT_InStdResultModelPossibility*)possibilitiesGroup()->getPossibilities().at(v))->inModel()->selectAllPossibilitiesByDefault())
                return false;
        }
    }

    return true;
}

CT_InAbstractModel* CT_InAbstractResultModel::findModelInTreeOfModelInPossibility(int pIndex, const QString &uniqueName) const
{
    return (CT_InAbstractModel*)((CT_InStdResultModelPossibility*)possibilitiesGroup()->getPossibilities().at(pIndex))->inModel()->findModelInTree(uniqueName);
}

QList<CT_AbstractModel *> CT_InAbstractResultModel::childrensToFindPossibilities(bool savePossibilities) const
{
    if(!savePossibilities)
        return childrens();

    QList<CT_AbstractModel *> r;
    QList<CT_AbstractModel *> l = childrens();
    QListIterator<CT_AbstractModel*> it(l);

    while(it.hasNext())
        r.append(((CT_InAbstractModel*)it.next())->copy(false));

    return r;
}

QList<CT_AbstractModel *> CT_InAbstractResultModel::childrensOfPossibilities() const
{
    QList<CT_AbstractModel *> r;

    QList<CT_InStdModelPossibility*> l = getPossibilitiesSaved();
    QListIterator<CT_InStdModelPossibility*> it(l);

    while(it.hasNext())
        r.append(((CT_InStdResultModelPossibility*)it.next())->inModel());

    return r;
}

void CT_InAbstractResultModel::inModelComparisonResult(CT_AbstractModel *inModel, bool ok, bool savePossibilities)
{
    CT_InAbstractModel::inModelComparisonResult(inModel, ok, savePossibilities);

    m_backupModel = NULL;

    if(savePossibilities)
    {
        if(!ok)
            delete inModel;
        else
            m_backupModel = (CT_InAbstractModel*)inModel;
    }
}

CT_InStdModelPossibility* CT_InAbstractResultModel::createNewPossibility() const
{
    return new CT_InStdResultModelPossibility();
}

void CT_InAbstractResultModel::possibilityCreated(CT_InStdModelPossibility *p)
{
    if(m_backupModel != NULL)
        dynamic_cast<CT_InStdResultModelPossibility*>(p)->setInModel(m_backupModel);

    m_backupModel = NULL;
}

void CT_InAbstractResultModel::possibilityNotCreated()
{
    delete m_backupModel;
    m_backupModel = NULL;
}
