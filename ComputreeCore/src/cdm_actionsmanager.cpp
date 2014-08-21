#include "cdm_actionsmanager.h"

#include "ct_actions/abstract/ct_abstractaction.h"
#include "ct_abstractstepplugin.h"

CDM_ActionsManager::CDM_ActionsManager()
{
    m_pluginManager = NULL;
    m_manualModeEnabled = false;
    m_manualModeEnabledByStep = NULL;
}

CDM_ActionsManager::~CDM_ActionsManager()
{
    qDeleteAll(m_actions.begin(), m_actions.end());
}

void CDM_ActionsManager::setPluginManager(const CDM_PluginManager *pluginManager)
{
    if(m_pluginManager != NULL)
        disconnect(m_pluginManager, NULL ,this, NULL);

    m_pluginManager = (CDM_PluginManager*)pluginManager;

    if(m_pluginManager != NULL)
    {
        connect(m_pluginManager, SIGNAL(finishLoading()), this, SIGNAL(actionsCollectionChanged()));
    }
}

CDM_PluginManager* CDM_ActionsManager::pluginManager() const
{
    return m_pluginManager;
}

void CDM_ActionsManager::setStepManager(const CDM_StepManager *stepManager)
{
    connect(stepManager, SIGNAL(stepQuitManualMode(CT_VirtualAbstractStep*)), this, SLOT(stepFinished(CT_VirtualAbstractStep*)), Qt::DirectConnection);
    connect(stepManager, SIGNAL(stepRequiredManualMode(CT_VirtualAbstractStep*)), this, SLOT(stepRequiredManualMode(CT_VirtualAbstractStep*)), Qt::DirectConnection);
}

bool CDM_ActionsManager::addAction(CT_AbstractAction *action)
{
    if(existAction(action))
        return true;

    if(m_manualModeEnabled)
        m_actionsAddedInManualMode.append(action);

    m_actions.append(action);

    emit actionAdded(action);
    emit actionsCollectionChanged();

    return true;
}

bool CDM_ActionsManager::removeAction(CT_AbstractAction *action)
{
    return removeAction(action->uniqueName());
}

bool CDM_ActionsManager::removeAction(const QString &uniqueName)
{
    QMutableListIterator<CT_AbstractAction*> it(m_actions);

    while(it.hasNext())
    {
        CT_AbstractAction *ac = it.next();

        if(ac->uniqueName() == uniqueName)
        {
            m_actionsAddedInManualMode.removeOne(ac);

            emit actionToRemove(ac);

            it.remove();

            emit actionsCollectionChanged();

            delete ac;

            return true;
        }
    }

    return false;
}

QList<CT_AbstractAction *> CDM_ActionsManager::actions() const
{
    QList<CT_AbstractAction*> list;

    list.append(actionsFromPlugins());
    list.append(m_actions);

    return list;
}

bool CDM_ActionsManager::existAction(const CT_AbstractAction *action) const
{
    if(action == NULL)
        return false;

    return existAction(action->uniqueName());
}

bool CDM_ActionsManager::existAction(const QString &uniqueName) const
{
    if(m_pluginManager != NULL)
    {
        int size = m_pluginManager->countPluginLoaded();

        for(int i=0; i<size; ++i)
        {
            QList<CT_ActionsSeparator*> actionsSep = m_pluginManager->getPlugin(i)->getActionsAvailable();
            QListIterator<CT_ActionsSeparator*> itS(actionsSep);

            while(itS.hasNext())
            {
                CT_ActionsSeparator *sep = itS.next();

                QList<CT_AbstractAction*> actions = sep->actions();
                QListIterator<CT_AbstractAction*> it(actions);

                while(it.hasNext())
                {
                    CT_AbstractAction *ac = it.next();

                    if(ac->uniqueName() == uniqueName)
                        return true;
                }
            }
        }
    }

    QListIterator<CT_AbstractAction*> it(m_actions);

    while(it.hasNext())
    {
        if(it.next()->uniqueName() == uniqueName)
            return true;
    }

    return false;
}

bool CDM_ActionsManager::existActionCompareAddress(const CT_AbstractAction *action) const
{
    if(action == NULL)
        return false;

    QString uniqueName = action->uniqueName();

    if(m_pluginManager != NULL)
    {
        int size = m_pluginManager->countPluginLoaded();

        for(int i=0; i<size; ++i)
        {
            QList<CT_ActionsSeparator*> actionsSep = m_pluginManager->getPlugin(i)->getActionsAvailable();
            QListIterator<CT_ActionsSeparator*> itS(actionsSep);

            while(itS.hasNext())
            {
                CT_ActionsSeparator *sep = itS.next();

                QList<CT_AbstractAction*> actions = sep->actions();
                QListIterator<CT_AbstractAction*> it(actions);

                while(it.hasNext())
                {
                    CT_AbstractAction *ac = it.next();

                    if(ac->uniqueName() == uniqueName)
                        return (ac == action);
                }
            }
        }
    }

    QListIterator<CT_AbstractAction*> it(m_actions);

    while(it.hasNext())
    {
        CT_AbstractAction *ac = it.next();

        if(ac->uniqueName() == uniqueName)
            return (ac == action);
    }

    return false;
}

CT_AbstractAction* CDM_ActionsManager::action(const QString &uniqueName) const
{
    QList<CT_AbstractAction*> act = actions();
    QListIterator<CT_AbstractAction*> it(act);

    while(it.hasNext())
    {
        CT_AbstractAction *ac = it.next();

        if(ac->uniqueName() == uniqueName)
            return ac;
    }

    return NULL;
}

int CDM_ActionsManager::nPlugins() const
{
    if(m_pluginManager == NULL)
        return 0;

    return m_pluginManager->countPluginLoaded();
}

QList<CT_ActionsSeparator *> CDM_ActionsManager::actionsFromPlugin(const int &index) const
{
    if(m_pluginManager != NULL)
    {
        return m_pluginManager->getPlugin(index)->getActionsAvailable();
    }

    return QList<CT_ActionsSeparator *>();
}

QList<CT_AbstractAction *> CDM_ActionsManager::actionsFromPlugins() const
{
    QList<CT_AbstractAction *> list;
    int size = nPlugins();

    for(int i=0; i<size; ++i)
    {
        QList<CT_ActionsSeparator*> actionsSep = actionsFromPlugin(i);
        QListIterator<CT_ActionsSeparator*> it(actionsSep);

        while(it.hasNext())
            list.append(it.next()->actions());
    }

    return list;
}

QList<CT_AbstractAction *> CDM_ActionsManager::actionsFromSteps() const
{
    return m_actions;
}

void CDM_ActionsManager::clearActions()
{
    while(!m_actions.isEmpty())
    {
        CT_AbstractAction *ac = m_actions.last();

        emit actionToRemove(ac);

        delete m_actions.takeLast();

        emit actionsCollectionChanged();
    }
}

void CDM_ActionsManager::stepRequiredManualMode(CT_VirtualAbstractStep *step)
{
    if((m_manualModeEnabledByStep != NULL)
            && (m_manualModeEnabledByStep != step))
    {
        stepFinished(m_manualModeEnabledByStep);
    }

    if(m_manualModeEnabledByStep == NULL)
    {
        m_manualModeEnabled = true;
        m_manualModeEnabledByStep = step;
    }
}

void CDM_ActionsManager::stepFinished(CT_VirtualAbstractStep *step)
{
    if((m_manualModeEnabledByStep != NULL)
            && (m_manualModeEnabledByStep == step))
    {
        // remove all actions added in manual mode
        while(!m_actionsAddedInManualMode.isEmpty())
            removeAction(m_actionsAddedInManualMode.takeFirst());

        m_manualModeEnabledByStep = NULL;
        m_manualModeEnabled = false;
    }
}

