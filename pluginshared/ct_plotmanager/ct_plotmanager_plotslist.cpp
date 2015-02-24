#include "ct_plotmanager_plotslist.h"

CT_PlotManager_plotsList::CT_PlotManager_plotsList() : CT_AbstractPlotManager()
{

}

CT_PlotManager_plotsList::~CT_PlotManager_plotsList()
{
    qDeleteAll(_plotList.values());
    _plotList.clear();
    _alreadyReturned.clear();
}

QMap<QString, CT_AreaShape2DData *> CT_PlotManager_plotsList::getPlotsInBoundingBox(const Eigen::Vector3d &min, const Eigen::Vector3d &max) const
{
    QMap<QString, CT_AreaShape2DData*> map;

    QMapIterator<QString, CT_AreaShape2DData*> it(_plotList);
    while (it.hasNext())
    {
        it.next();
        const QString &idVal = it.key();
        CT_AreaShape2DData* shape = it.value();

        if (!_alreadyReturned.contains(idVal))
        {
            Eigen::Vector3d minSh, maxSh;
            shape->getBoundingBox(minSh, maxSh);

            if (maxSh(0) > min(0) &&
                maxSh(1) > min(1) &&
                minSh(0) < max(0) &&
                minSh(2) < max(1))
            {
                map.insert(idVal, shape);
                _alreadyReturned.append(idVal);
            }
        }
    }

    return map;
}

void CT_PlotManager_plotsList::addPlot(const QString &idVal, CT_AreaShape2DData *shape)
{
    _plotList.insert(idVal, shape);
}

