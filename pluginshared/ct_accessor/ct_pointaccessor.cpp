#include "ct_pointaccessor.h"

#include "ct_global/ct_context.h"
#include "ct_pointcloud/abstract/ct_abstractpointcloud.h"
#include "ct_coordinates/tools/ct_coordinatesystemmanager.h"

CT_PointAccessor::CT_PointAccessor()
{
    m_pCloud = PS_REPOSITORY->globalPointCloud();
}

CT_Point CT_PointAccessor::pointAt(const size_t &globalIndex) const
{
    CT_Point p;

    m_pCloud->constTAt(globalIndex).realPoint(p, globalIndex);
    return p;
}

void CT_PointAccessor::pointAt(const size_t &globalIndex, CT_Point &point) const
{
    m_pCloud->constTAt(globalIndex).realPoint(point, globalIndex);
}

const CT_Point& CT_PointAccessor::constPointAt(const size_t &globalIndex) const
{
    m_pCloud->constTAt(globalIndex).realPoint(m_p, globalIndex);
    return m_p;
}

void CT_PointAccessor::replacePointAt(const size_t &globalIndex, const CT_Point &p, CT_AbstractCoordinateSystem *sys)
{
    CT_PointData &pData = internalPointAt(globalIndex);

    if(sys == NULL) {
        PS_COORDINATES_SYS_MANAGER->coordinateSystemAt(0)->convertImport(p(CT_Point::X), p(CT_Point::Y), p(CT_Point::Z), pData(CT_PointData::X), pData(CT_PointData::Y), pData(CT_PointData::Z));
        PS_COORDINATES_SYS_MANAGER->setCoordinateSystemForPointAt(globalIndex, 0);
    } else {
        sys->convertImport(p(CT_Point::X), p(CT_Point::Y), p(CT_Point::Z), pData(CT_PointData::X), pData(CT_PointData::Y), pData(CT_PointData::Z));
        PS_COORDINATES_SYS_MANAGER->setCoordinateSystemForPointAt(globalIndex, PS_COORDINATES_SYS_MANAGER->indexOfCoordinateSystem(sys));
    }
}

CT_PointData& CT_PointAccessor::internalPointAt(const size_t &globalIndex)
{
    return m_pCloud->tAt(globalIndex);
}

const CT_PointData& CT_PointAccessor::constInternalPointAt(const size_t &globalIndex) const
{
    return m_pCloud->constTAt(globalIndex);
}

size_t CT_PointAccessor::size() const
{
    return m_pCloud->size();
}
