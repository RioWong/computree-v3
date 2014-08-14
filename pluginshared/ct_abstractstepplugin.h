/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michaël KREBS (ARTS/ENSAM)

 This file is part of PluginShared library 2.0.

 PluginShared is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginShared is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginShared.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

*****************************************************************************/

#ifndef CT_ABSTRACTSTEPPLUGIN_H
#define CT_ABSTRACTSTEPPLUGIN_H

#include "interfaces.h"
#include "pluginShared_global.h"

class CT_StepSeparator;
class CT_StepLoadFileSeparator;
class CT_StepCanBeAddedFirstSeparator;
class CT_StepInitializeData;

class CT_ActionsSeparator;
class CT_StandardExporterSeparator;
class CT_StandardReaderSeparator;

class CT_VirtualAbstractStep;
class CT_AbstractStepLoadFile;

class QSettings;

class PLUGINSHAREDSHARED_EXPORT CT_AbstractStepPlugin
{
public:
    CT_AbstractStepPlugin();
    virtual ~CT_AbstractStepPlugin();

    /**
     * @brief Must be called by the core or the gui to define the interface that can be used to dialog with the core. For all plugins
     *        loaded you must call this method.
     * @param core : the core of the application (can not be null ! and must be obligatory set)
     */
    void setCoreInterface(const CoreInterface *core);

    /**
     * @brief Must be called by the gui (if a gui exist) to define the interface that can be used to dialog with the gui. For all plugins
     *        loaded you must call this method.
     * @param gManager : the gui manager (can be null if it does not exist a gui)
     */
    void setGuiManager(const GuiManagerInterface *gManager);

    /**
     * @brief Initialize the plugin (load step / readers / exporters / actions / settings /etc....). Must be called from the core
     *        when the plugin is loaded.
     *
     *        If you want to save settings for your plugins show method "initQSettings()". This method will be called in the "init" method.
     *
     * @warning If you overload this method remember to call the method of the superclass !
     * @return true if the initialization is a success.
     */
    virtual bool init();

    /**
     * @brief Initialize step / readers / exporters / actions / etc.... Must be called from the core for each plugins AFTER ALL plugins was loaded.
     * @warning If you overload this method remember to call the method of the superclass !
     * @return true if the initialization is a success.
     */
    virtual bool initAfterAllPluginsLoaded();

    /**
     * @brief Returns a list of step separator. A step separator group multiple step by a title. This
     *        separator contain standard step (can be added only in the middle of the tree)
     */
    QList<CT_StepSeparator*> getGenericsStepAvailable() const;

    /**
     * @brief Returns a list of step separator. A step separator group multiple step by a title. This
     *        separator contain step that load file (can be added only in the root of the tree)
     */
    QList<CT_StepLoadFileSeparator*> getOpenFileStepAvailable() const;

    /**
     * @brief Returns a list of step separator. A step separator group multiple step by a title. This
     *        separator contain step that can be added first or not (can be added in the root of the tree or in the middle)
     */
    QList<CT_StepCanBeAddedFirstSeparator*> getCanBeAddedFirstStepAvailable() const;

    /**
     * @brief Returns a list of action separator. A action separator group multiple action by a title. This
     *        separator contain standard action that can be installed on GraphicsViewInterface / TreeViewInterface / etc...
     */
    QList<CT_ActionsSeparator*> getActionsAvailable() const;

    /**
     * @brief Returns a list of exporter separator. A exporter separator group multiple exporter by a title. This
     *        separator contain standard exporters that can export items / points / faces /edges / etc...
     */
    QList<CT_StandardExporterSeparator*> getExportersAvailable() const;

    /**
     * @brief Returns a list of reader separator. A reader separator group multiple reader by a title. This
     *        separator contain standard readers that can read file / stream / etc... to generate items
     */
    QList<CT_StandardReaderSeparator *> getReadersAvailable() const;

    /**
     * @brief Returns a list of step that can read the file define by the filepath passed in parameter
     * @warning If you overload this method remember to call the method of the superclass !
     */
    virtual QList<CT_AbstractStepLoadFile*> getOpenFileStep(const QString &filePath) const;

    /**
     * @brief Retuns a key to retrieve the step passed in parameter later (if you load a script per example). Used
     *        the method "getStepFromKey" to get the step from a key.
     * @warning If you overload this method remember to call the method of the superclass !
     */
    virtual QString getKeyForStep(const CT_VirtualAbstractStep &step) const;

    /**
     * @brief Returns the step that correspond to the key passed in parameter. Used the method "getKeyForStep" if you want
     *        to get a key from a step.
     * @warning If you overload this method remember to call the method of the superclass !
     */
    virtual CT_VirtualAbstractStep* getStepFromKey(QString key) const;

    /**
     * @brief Create a copy of the step (model) passed in parameter
     * @param step : the step to copy
     * @param parent : the future parent of the step (can be NULL if the step must be root)
     * @return NULL if the copy is impossible
     * @warning If you overload this method remember to call the method of the superclass !
     */
    virtual CT_VirtualAbstractStep* createNewInstanceOfStep(const CT_VirtualAbstractStep &step, CT_VirtualAbstractStep *parent) const;

    /**
     * @brief Returns true if this plugin can be configured. By default returns false.
     * @warning If you overload this method remember to call the method of the superclass !
     */
    virtual bool canBeConfigured() const { return false; }

    /**
     * @brief Configure this plugin and returns true if the configuration is accepted. By default returns false.
     * @warning If you overload this method remember to call the method of the superclass !
     */
    virtual bool configure() { return false; }

    /**
     * @brief Get the official plugin url (= project ex.: http://rdinnovation.onf.fr:8080/projects/plugin-base for Plugin Base)
     * @return Project URL
     */
    virtual QString getPluginURL() const = 0;

protected:

    /**
     * @brief Overload this method to load your standard step. Use "addNewSeparator" method to create a new separator and a step to it.
     * @return true if load is a success.
     * @warning Method called in the "init" method. If you must wait that all plugins is loaded to create
     *          your elements prefer overload the method "loadAfterAllPluginsLoaded"
     */
    virtual bool loadGenericsStep() = 0;

    /**
     * @brief Overload this method to load your step that can load file. Use "addNewSeparator" method to create a new separator and a step to it.
     * @return true if load is a success.
     * @warning Method called in the "init" method. If you must wait that all plugins is loaded to create
     *          your elements prefer overload the method "loadAfterAllPluginsLoaded"
     */
    virtual bool loadOpenFileStep() = 0;

    /**
     * @brief Overload this method to load your step that can be added first. Use "addNewSeparator" method to create a new separator and a step to it.
     * @return true if load is a success.
     * @warning Method called in the "init" method. If you must wait that all plugins is loaded to create
     *          your elements prefer overload the method "loadAfterAllPluginsLoaded"
     */
    virtual bool loadCanBeAddedFirstStep() = 0;

    /**
     * @brief Overload this method to load your actions. Use "addNewSeparator" method to create a new separator and a actions to it.
     * @return true if load is a success.
     * @warning Method called in the "init" method. If you must wait that all plugins is loaded to create
     *          your elements prefer overload the method "loadAfterAllPluginsLoaded"
     */
    virtual bool loadActions() = 0;

    /**
     * @brief Overload this method to load your exporters. Use "addNewSeparator" method to create a new separator and a exporters to it.
     * @return true if load is a success.
     * @warning Method called in the "init" method. If you must wait that all plugins is loaded to create
     *          your elements prefer overload the method "loadAfterAllPluginsLoaded"
     */
    virtual bool loadExporters() = 0;

    /**
     * @brief Overload this method to load your readers. Use "addNewSeparator" method to create a new separator and a readers to it.
     * @return true if load is a success.
     * @warning Method called in the "init" method. If you must wait that all plugins is loaded to create
     *          your elements prefer overload the method "loadAfterAllPluginsLoaded"
     */
    virtual bool loadReaders() = 0;

    /**
     * @brief Inherit this method to add your step, actions, exporters, etc... after all plugins was loaded (if you must use elements from
     *        another plugin). Use "addNewSeparator" method to create a new separator and a elements to it.
     */
    virtual bool loadAfterAllPluginsLoaded() { return true; }

    /**
     * @brief Remove from memory all standard step
     */
    void clearGenericsStep();

    /**
     * @brief Remove from memory all load file step
     */
    void clearOpenFileStep();

    /**
     * @brief Remove from memory all can be added first step
     */
    void clearCanBeAddedFirstStep();

    /**
     * @brief Remove from memory all actions
     */
    void clearActions();

    /**
     * @brief Remove from memory all exporters
     */
    void clearExporters();

    /**
     * @brief Remove from memory all readers
     */
    void clearReaders();

    /**
     * @brief Overload this method if you want to return a new QSettings that will be added to the CT_StepInitializeData. So you can
     *        save some settings of your steps (like the last filepath opened, etc...). Settings will be automatically saved when the
     *        QSettings object is destroyed. This class manage the life in memory of the QSettings object.
     *
     * @return NULL by default. A new QSettings if you want to save settings.
     */
    virtual QSettings* initQSettings() { return NULL; }

    /**
     * @brief Create a new standard initialization data for your step.
     * @param parent : the future parent of your new step
     */
    virtual CT_StepInitializeData* createNewStepInitializeData(CT_VirtualAbstractStep *parent) const;

    /**
     * @brief Create a new separator that can contains standard step. You must pass a new object to this method, the object
     *        is added to the list of separator and returned.
     */
    CT_StepSeparator* addNewSeparator(CT_StepSeparator *sep);

    /**
     * @brief Create a new separator that can contains step that load file. You must pass a new object to this method, the object
     *        is added to the list of separator and returned.
     */
    CT_StepLoadFileSeparator* addNewSeparator(CT_StepLoadFileSeparator *sep);

    /**
     * @brief Create a new separator that can contains step that can be added first. You must pass a new object to this method, the object
     *        is added to the list of separator and returned.
     */
    CT_StepCanBeAddedFirstSeparator* addNewSeparator(CT_StepCanBeAddedFirstSeparator *sep);

    /**
     * @brief Create a new separator that can contains action. You must pass a new object to this method, the object
     *        is added to the list of separator and returned.
     */
    CT_ActionsSeparator* addNewSeparator(CT_ActionsSeparator *sep);

    /**
     * @brief Create a new separator that can contains exporter. You must pass a new object to this method, the object
     *        is added to the list of separator and returned.
     */
    CT_StandardExporterSeparator* addNewSeparator(CT_StandardExporterSeparator *sep);

    /**
     * @brief Create a new separator that can contains reader. You must pass a new object to this method, the object
     *        is added to the list of separator and returned.
     */
    CT_StandardReaderSeparator* addNewSeparator(CT_StandardReaderSeparator *sep);

    /**
     * @brief Clear all steps / actions / readers / exporters / etc... from this plugins (call methods "clearXXX")
     */
    virtual void clearMemory();

private:

    QList<CT_StepSeparator*>                    _stepAvailable;
    QList<CT_StepLoadFileSeparator*>            _stepOpenFileAvailable;
    QList<CT_StepCanBeAddedFirstSeparator*>     _stepCanBeAddedFirstAvailable;
    QList<CT_ActionsSeparator*>                 m_actions;
    QList<CT_StandardExporterSeparator*>        m_exporters;
    QList<CT_StandardReaderSeparator*>          m_readers;

    QSettings                                   *_pluginSettings;

    /**
     * @brief Initialize the step passed in parameter. Call this method after all plugins is loaded !
     * @param step : the step to init
     */
    void initStep(CT_VirtualAbstractStep *step) const;
};

#endif // CT_ABSTRACTSTEPPLUGIN_H
