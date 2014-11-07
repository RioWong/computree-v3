#include "ct_standardabstractshape2ddrawmanager.h"


const QString CT_StandardAbstractShape2DDrawManager::INDEX_CONFIG_USE_ALTERNATIVE_ZVALUE = CT_StandardAbstractShape2DDrawManager::staticInitConfigUseAlternativeZValue();
const QString CT_StandardAbstractShape2DDrawManager::INDEX_CONFIG_Z_VALUE = CT_StandardAbstractShape2DDrawManager::staticInitConfigZValue();


CT_StandardAbstractShape2DDrawManager::CT_StandardAbstractShape2DDrawManager(QString drawConfigurationName) : CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager(drawConfigurationName.isEmpty() ? "CT_AbstractShape" : drawConfigurationName)
{
}

CT_StandardAbstractShape2DDrawManager::~CT_StandardAbstractShape2DDrawManager()
{
}


CT_ItemDrawableConfiguration CT_StandardAbstractShape2DDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addAllConfigurationOf(CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::createDrawConfiguration(drawConfigurationName));
    item.addNewConfiguration(CT_StandardAbstractShape2DDrawManager::staticInitConfigUseAlternativeZValue(), QObject::tr("Utiliser une valeur Z différente"), CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(CT_StandardAbstractShape2DDrawManager::staticInitConfigZValue(), QObject::tr("Valeur Z"), CT_ItemDrawableConfiguration::Double, 0);

    return item;
}

// PROTECTED //

QString CT_StandardAbstractShape2DDrawManager::staticInitConfigUseAlternativeZValue()
{
    return "USE_ALT_ZVAL";
}

QString CT_StandardAbstractShape2DDrawManager::staticInitConfigZValue()
{
    return "ZVAL";
}
