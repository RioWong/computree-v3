#include "cdm_stepsmenumanager.h"

#include "ct_step/tools/menu/ct_menulevel.h"
#include "ct_step/abstract/ct_virtualabstractstep.h"

#include "src/cdm_pluginmanager.h"

#include <QtXml>

#define XML_FAVORITES_START_KEY "FAVORITES"

#define XML_LEVEL_START_KEY "LEVEL"
#define XML_LEVEL_NAME_KEY "DisplayableName"

#define XML_STEP_START_KEY "STEP"
#define XML_STEP_PLUGIN_NAME_KEY "PluginName"
#define XML_STEP_KEY_KEY "Key"

CDM_StepsMenuManager::CDM_StepsMenuManager()
{
    m_menuOfSteps = new CT_StepsMenu();
    m_pluginManager = NULL;
}

CDM_StepsMenuManager::~CDM_StepsMenuManager()
{
    delete m_menuOfSteps;
}

void CDM_StepsMenuManager::setPluginManager(CDM_PluginManager *pluginManager)
{
    m_pluginManager = pluginManager;
}

CT_StepsMenu *CDM_StepsMenuManager::stepsMenu() const
{
    return m_menuOfSteps;
}

void CDM_StepsMenuManager::clear()
{
    delete m_menuOfSteps;
    m_menuOfSteps = new CT_StepsMenu();
}

void CDM_StepsMenuManager::saveFavoritesTo(const QString &filepath)
{
    CT_MenuLevel *favorites = m_menuOfSteps->levelFromOperation(CT_StepsMenu::LO_Favorites);

    if(favorites != NULL) {
        QFile xmlDoc(filepath);

        if(xmlDoc.open(QIODevice::WriteOnly)) {
            QXmlStreamWriter xmlWriter(&xmlDoc);
            xmlWriter.setAutoFormatting(true);
            xmlWriter.writeStartDocument();
            xmlWriter.writeStartElement(XML_FAVORITES_START_KEY);

            QList<CT_MenuLevel*> levels = favorites->levels();
            QListIterator<CT_MenuLevel*> it(levels);

            while(it.hasNext()) {
                writeLevelInformationToFavoritesFileRecursively(it.next(), xmlWriter);
            }

            xmlWriter.writeEndElement();

            xmlDoc.close();
        }
    }
}

void CDM_StepsMenuManager::writeLevelInformationToFavoritesFileRecursively(const CT_MenuLevel *level, QXmlStreamWriter &xmlWriter)
{
    xmlWriter.writeStartElement(XML_LEVEL_START_KEY);
    xmlWriter.writeTextElement(XML_LEVEL_NAME_KEY, level->displayableName());

    QList<CT_VirtualAbstractStep*> steps = level->steps();
    QListIterator<CT_VirtualAbstractStep*> itS(steps);

    while(itS.hasNext()) {
        writeStepInformationToFavoritesFileRecursively(itS.next(), xmlWriter);
    }

    QList<CT_MenuLevel*> levels = level->levels();
    QListIterator<CT_MenuLevel*> it(levels);

    while(it.hasNext()) {
        writeLevelInformationToFavoritesFileRecursively(it.next(), xmlWriter);
    }

    xmlWriter.writeEndElement();
}

void CDM_StepsMenuManager::writeStepInformationToFavoritesFileRecursively(const CT_VirtualAbstractStep *step, QXmlStreamWriter &xmlWriter)
{
    xmlWriter.writeStartElement(XML_STEP_START_KEY);

    xmlWriter.writeTextElement(XML_STEP_PLUGIN_NAME_KEY, m_pluginManager->getPluginName(step->getPlugin()));
    xmlWriter.writeTextElement(XML_STEP_KEY_KEY, step->getPlugin()->getKeyForStep(*step));

    xmlWriter.writeEndElement();
}

void CDM_StepsMenuManager::loadFavoritesFrom(const QString &filepath)
{
    CT_MenuLevel *favorites = m_menuOfSteps->getOrCreateRootLevel(CT_StepsMenu::LO_Favorites);

    QFile xmlDoc(filepath);

    if(xmlDoc.open(QIODevice::ReadOnly)) {
        QXmlStreamReader xmlReader(&xmlDoc);

        while(!xmlReader.atEnd()) {
            xmlReader.readNext();

            if(xmlReader.isStartElement()) {
                if(xmlReader.name() == XML_LEVEL_START_KEY) {
                    readLevelAndAddItToFavoritesRecursively(favorites, xmlReader);
                }
            }
        }

        xmlDoc.close();
    }
}

void CDM_StepsMenuManager::readLevelAndAddItToFavoritesRecursively(CT_MenuLevel *parentLevel, QXmlStreamReader &xmlReader)
{
    if(parentLevel != NULL) {
        CT_MenuLevel *thisLevel = NULL;

        while(!xmlReader.atEnd()) {

            xmlReader.readNext();

            if(xmlReader.isStartElement()) {
                if(xmlReader.name() == XML_LEVEL_START_KEY) {
                    readLevelAndAddItToFavoritesRecursively(thisLevel, xmlReader);
                } else if(xmlReader.name() == XML_LEVEL_NAME_KEY) {
                    thisLevel = CT_MenuLevel::getOrCreateLevel(xmlReader.readElementText(), parentLevel);
                } else if(xmlReader.name() == XML_STEP_START_KEY) {
                    readStepAndAddItToLevel(thisLevel, xmlReader);
                }
            } else if(xmlReader.isEndElement()) {
                return;
            }
        }
    }
}

void CDM_StepsMenuManager::readStepAndAddItToLevel(CT_MenuLevel *parentLevel, QXmlStreamReader &xmlReader)
{
    if(parentLevel != NULL) {
        QString pluginName;
        QString stepKey;

        while(!xmlReader.atEnd()) {

            xmlReader.readNext();

            if(xmlReader.isStartElement()) {
                if(xmlReader.name() == XML_STEP_KEY_KEY) {
                    stepKey = xmlReader.readElementText();
                } else if(xmlReader.name() == XML_STEP_PLUGIN_NAME_KEY) {
                    pluginName = xmlReader.readElementText();
                }
            } else if(xmlReader.isEndElement()) {
                return;
            }

            if(!pluginName.isEmpty() && !stepKey.isEmpty()) {
                CT_AbstractStepPlugin *plugin = m_pluginManager->getPlugin(pluginName);

                if(plugin != NULL) {
                    CT_VirtualAbstractStep *step = plugin->getStepFromKey(stepKey);

                    if(step != NULL) {
                        parentLevel->addStep(plugin->createNewInstanceOfStep(*step, NULL));
                    }
                }

                while(!xmlReader.atEnd()) {
                    xmlReader.readNext();
                    if(xmlReader.isEndElement()) {
                        return;
                    }
                }
            }
        }
    }
}
