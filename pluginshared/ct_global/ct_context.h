#ifndef CT_CONTEXT_H
#define CT_CONTEXT_H

#include "ct_global/ct_repositorymanager.h"
#include "ct_global/ct_repository.h"

#define PS_CONTEXT CT_Context::staticInstance()
#define PS_CATEGORY_MANAGER CT_Context::staticInstance()->categoryManager()
#define PS_REPOSITORY_MANAGER CT_Context::staticInstance()->repositoryManager()
#define PS_REPOSITORY CT_Context::staticInstance()->repositoryManager()->repository()
#define PS_LOG CT_Context::staticInstance()->log()
#define PS_DIAM CT_Context::staticInstance()->defaultItemAttributesManager()

#define CT_ABSTRACT_NMPCIR CT_Repository::CT_AbstractNotModifiablePCIR
#define CT_ABSTRACT_MPCIR CT_Repository::CT_AbstractModifiablePCIR
#define CT_ABSTRACT_PCIR CT_Repository::CT_AbstractPCIR

class CT_DefaultItemAttributeManager;
class CT_CategoryManager;
class CT_TemporaryLog;

class PLUGINSHAREDSHARED_EXPORT CT_Context
{
public:

    static CT_Context* staticInstance()
    {
        if(UNIQUE_INSTANCE == NULL)
            UNIQUE_INSTANCE = new CT_Context();

        return UNIQUE_INSTANCE;
    }

    static void deleteStaticInstance()
    {
        delete UNIQUE_INSTANCE;
        UNIQUE_INSTANCE = NULL;
    }

    ~CT_Context();

    /**
     * @brief Returns the repository manager
     */
    CT_RepositoryManager* repositoryManager() const;

    /**
     * @brief Returns the current repository
     */
    CT_Repository *repository() const;

    /**
     * @brief Returns the category manager for item attributes
     */
    CT_CategoryManager* categoryManager() const;

    /**
     * @brief Returns the default item attributes manager
     */
    CT_DefaultItemAttributeManager* defaultItemAttributesManager() const;

    /**
     * @brief Returns the log (CORE)
     */
    LogInterface* log() const;

    /**
     * @brief Returns the plugin manager (CORE)
     */
    PluginManagerInterface* pluginManager() const;

protected:

    friend class CT_AbstractStepPlugin;
    friend class CT_AbstractCloudSyncToGlobalCloudManager;

    void setCoreInterface(const CoreInterface *core);
    void setGuiManager(const GuiManagerInterface *gManager);

    template<typename T> friend class CT_AbstractGlobalCloudManagerT;
    template<typename T> friend class CT_AbstractCloudIndexRegistrationManagerT;

    GuiManagerInterface* guiManager() const;
    DocumentManagerInterface* documentManager() const;

private:
    static CT_Context               *UNIQUE_INSTANCE;
    CT_RepositoryManager            *m_repositoryManager;
    CT_CategoryManager              *m_categoryManager;
    GuiManagerInterface             *m_guiManager;
    CoreInterface                   *m_coreInterface;
    CT_TemporaryLog                 *m_tmpLog;
    CT_DefaultItemAttributeManager  *m_diam;
    CT_Context();
};

#endif // CT_CONTEXT_H
