#include "ct_standardpointclusterdrawmanager.h"
#include "ct_itemdrawable/ct_pointcluster.h"

const QString CT_StandardPointClusterDrawManager::INDEX_CONFIG_BARYCENTER_VISIBLE = CT_StandardPointClusterDrawManager::staticInitConfigBarycenterVisible();
const QString CT_StandardPointClusterDrawManager::INDEX_CONFIG_LINES_VISIBLE = CT_StandardPointClusterDrawManager::staticInitConfigLinesVisible();

CT_StandardPointClusterDrawManager::CT_StandardPointClusterDrawManager(QString drawConfigurationName) : CT_StandardAbstractItemDrawableWithPointCloudDrawManager(drawConfigurationName.isEmpty() ? "CT_PointCluster" : drawConfigurationName)
{
}

CT_StandardPointClusterDrawManager::~CT_StandardPointClusterDrawManager()
{
}

void CT_StandardPointClusterDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    const CT_PointCluster &item = dynamic_cast<const CT_PointCluster&>(itemDrawable);

    CT_StandardAbstractItemDrawableWithPointCloudDrawManager::draw(view, painter, itemDrawable);

    if(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_BARYCENTER_VISIBLE).toBool())
    {

        const CT_PointClusterBarycenter &barycenter = item.getBarycenter();
        painter.drawPoint(barycenter.x(), barycenter.y(), barycenter.z());
    }

    if(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_LINES_VISIBLE).toBool())
    {
        const CT_AbstractCloudIndexT<CT_Point>* cloudIndex = (const CT_AbstractCloudIndexT<CT_Point>*)item.getPointCloudIndex();

        if(cloudIndex != NULL)
        {
            for ( size_t i = 0 ; i < cloudIndex->size()-1 ; ++i )
            {
                const CT_Point& p1 = cloudIndex->constTAt(i);
                const CT_Point& p2 = cloudIndex->constTAt(i+1);

                painter.drawLine(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
            }
        }
    }

}

CT_ItemDrawableConfiguration CT_StandardPointClusterDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addAllConfigurationOf(CT_StandardAbstractItemDrawableWithPointCloudDrawManager::createDrawConfiguration(drawConfigurationName));
    item.addNewConfiguration(CT_StandardPointClusterDrawManager::staticInitConfigBarycenterVisible(),"Barycentre", CT_ItemDrawableConfiguration::Bool, false );
    item.addNewConfiguration(CT_StandardPointClusterDrawManager::staticInitConfigLinesVisible(),"Polyligne", CT_ItemDrawableConfiguration::Bool, false );

    return item;
}

// PROTECTED //

QString CT_StandardPointClusterDrawManager::staticInitConfigBarycenterVisible()
{
    return "PTCL_BA";
}

QString CT_StandardPointClusterDrawManager::staticInitConfigLinesVisible()
{
    return "PTCL_LI";
}
