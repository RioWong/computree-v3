#include "ct_menulevel.h"

#include "ct_step/tools/menu/ct_stepsmenu.h"
#include "ct_step/abstract/ct_virtualabstractstep.h"

CT_MenuLevel *CT_MenuLevel::getOrCreateLevel(const QString &customDisplayableName, CT_MenuLevel *parentLevel)
{
    if(parentLevel == NULL)
        return NULL;

    CT_MenuLevel *level = parentLevel->levelFromDisplayableName(customDisplayableName);

    if(level == NULL) {
        level = new CT_MenuLevel(customDisplayableName);
        parentLevel->addLevel(level);
    }

    return level;
}

CT_MenuLevel *CT_MenuLevel::getOrCreateLevel(CT_StepsMenu::LevelPredefined predefinedLevel, CT_MenuLevel *parentLevel)
{
    return getOrCreateLevel(CT_StepsMenu::staticPredefinedToString(predefinedLevel), parentLevel);
}

QString CT_MenuLevel::displayableName() const
{
    return m_displayableName;
}

bool CT_MenuLevel::addStep(CT_VirtualAbstractStep *step)
{
    return addStepToCollectionOrDeleteIt(step);
}

QList<CT_VirtualAbstractStep *> CT_MenuLevel::steps() const
{
    return m_steps;
}

QList<CT_MenuLevel *> CT_MenuLevel::levels() const
{
    return m_levels;
}

QList<CT_MenuLevel *> CT_MenuLevel::levelsWithSteps() const
{
    QList<CT_MenuLevel*> ret;

    LevelCollectionIt it(m_levels);

    while(it.hasNext()) {
        CT_MenuLevel *l = it.next();

        if(l->isStepsNotEmptyRecursively())
            ret.append(l);
    }

    return ret;
}

CT_MenuLevel* CT_MenuLevel::parentLevel() const
{
    return m_parent;
}

CT_MenuLevel::CT_MenuLevel()
{
    m_parent = NULL;
}

CT_MenuLevel::CT_MenuLevel(const QString &displayableName)
{
    m_displayableName = displayableName;
    m_parent = NULL;
}

CT_MenuLevel::~CT_MenuLevel()
{
    StepCollectionIt itS(m_steps);

    while(itS.hasNext()) {
        CT_VirtualAbstractStep *s = itS.next();
        disconnect(s, NULL, this, NULL);
        delete s;
    }

    LevelCollectionIt it(m_levels);

    while(it.hasNext()) {
        CT_MenuLevel *l = it.next();
        disconnect(l, NULL, this, NULL);
        delete l;
    }
}

bool CT_MenuLevel::isStepsNotEmptyRecursively() const
{
    if(!m_steps.isEmpty())
        return true;

    LevelCollectionIt it(m_levels);

    while(it.hasNext()) {
        CT_MenuLevel *l = it.next();

        if(l->isStepsNotEmptyRecursively())
            return true;
    }

    return false;
}

bool CT_MenuLevel::addStepToCollectionOrDeleteIt(CT_VirtualAbstractStep *step)
{
    if(existStepInCollection(step)) {
        delete step;
        return false;
    }

    m_steps.append(step);

    connect(step, SIGNAL(destroyed(QObject*)), this, SLOT(stepDeleted(QObject*)), Qt::DirectConnection);

    if(isAFavoriteSubLevel())
        step->getPlugin()->initStep(step);

    return true;
}

bool CT_MenuLevel::existStepInCollection(CT_VirtualAbstractStep *step)
{
    QString className = step->metaObject()->className();
    StepCollectionIt it(m_steps);

    while(it.hasNext()) {
        CT_VirtualAbstractStep *s = it.next();

        if(s->metaObject()->className() == className && s->getStepName() == step->getStepName())
            return true;
    }

    return false;
}

void CT_MenuLevel::addLevel(CT_MenuLevel *level)
{
    m_levels.append(level);
    level->setParentLevel(this);

    connect(level, SIGNAL(destroyed(QObject*)), this, SLOT(subLevelDeleted(QObject*)), Qt::DirectConnection);
}

void CT_MenuLevel::setParentLevel(CT_MenuLevel *parent)
{
    m_parent = parent;
}

CT_MenuLevel *CT_MenuLevel::levelFromDisplayableName(const QString &name)
{
    LevelCollectionIt it(m_levels);

    while(it.hasNext()) {
        CT_MenuLevel *l = it.next();

        if(l->displayableName() == name)
            return l;
    }

    return NULL;
}

bool CT_MenuLevel::isAFavoriteSubLevel() const
{
    QString favoritesName = CT_StepsMenu::staticOperationToDisplayableName(CT_StepsMenu::LO_Favorites);
    CT_MenuLevel *l = const_cast<CT_MenuLevel*>(this);

    if(l->parentLevel() != NULL) {
        l = l->parentLevel();

        if(l->displayableName() == favoritesName)
            return true;
    }

    return false;
}

void CT_MenuLevel::subLevelDeleted(QObject *level)
{
    m_levels.removeOne((CT_MenuLevel*)level);
}

void CT_MenuLevel::stepDeleted(QObject *step)
{
    m_steps.removeOne((CT_VirtualAbstractStep*)step);
}
