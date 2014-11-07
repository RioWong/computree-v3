#include "ct_standardbox2ddrawmanager.h"
#include "ct_box2d.h"

#include "ct_global/ct_context.h"


const QString CT_StandardBox2DDrawManager::INDEX_CONFIG_DRAW_CENTER = CT_StandardBox2DDrawManager::staticInitConfigDrawCenter();
const QString CT_StandardBox2DDrawManager::INDEX_CONFIG_DRAW_BOX = CT_StandardBox2DDrawManager::staticInitConfigDrawBox();

CT_StandardBox2DDrawManager::CT_StandardBox2DDrawManager(QString drawConfigurationName) : CT_StandardAbstractShape2DDrawManager(drawConfigurationName.isEmpty() ? "CT_Box2D" : drawConfigurationName)
{
    
}

CT_StandardBox2DDrawManager::~CT_StandardBox2DDrawManager()
{
}

void CT_StandardBox2DDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    CT_StandardAbstractShape2DDrawManager::draw(view, painter, itemDrawable);

    const CT_Box2D &item = dynamic_cast<const CT_Box2D&>(itemDrawable);

    bool drawCenter = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_DRAW_CENTER).toBool();
    bool drawBox = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_DRAW_BOX).toBool();
    bool useAltZVal = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_USE_ALTERNATIVE_ZVALUE).toBool();
    float zVal = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_Z_VALUE).toFloat();

    float zPlane = CT_Context::staticInstance()->getZPlaneFor2DShapes();
    if (useAltZVal) {zPlane = zVal;}

    if (drawCenter)
    {
        painter.drawPoint(item.getCenterX(), item.getCenterY(), zPlane);
    }

    if (drawBox)
    {
        QVector2D min, max;
        item.getData().getBoundingBox(min, max);

        painter.drawRectXY(QRectF(min.x(), min.y(), item.getWidth(), item.getHeight()), zPlane);
    }
}

CT_ItemDrawableConfiguration CT_StandardBox2DDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addAllConfigurationOf(CT_StandardAbstractShape2DDrawManager::createDrawConfiguration(drawConfigurationName));
    item.addNewConfiguration(CT_StandardBox2DDrawManager::staticInitConfigDrawCenter() ,"Dessiner le centre", CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(CT_StandardBox2DDrawManager::staticInitConfigDrawBox() ,"Dessiner le rectangle", CT_ItemDrawableConfiguration::Bool, true);

    return item;
}

// PROTECTED //

QString CT_StandardBox2DDrawManager::staticInitConfigDrawCenter()
{
    return "B2D_CT";
}

QString CT_StandardBox2DDrawManager::staticInitConfigDrawBox()
{
    return "B2D_BX";
}
