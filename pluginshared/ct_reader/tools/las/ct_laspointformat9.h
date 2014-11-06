#ifndef CT_LASPOINTFORMAT9_H
#define CT_LASPOINTFORMAT9_H

#include "ct_reader/tools/las/ct_laspointformat6.h"
#include "ct_reader/tools/las/ct_laswavepacket.h"

class PLUGINSHAREDSHARED_EXPORT CT_LASPointFormat9 : public CT_LASPointFormat6, public CT_LASWavePacket
{
public:
    CT_LASPointFormat9();

    /**
     * @brief Return the size of the points in byte
     */
    virtual size_t sizeInBytes() const;

    /**
     * @brief Write the point 'globalIndex' (index in the global pointCloud)
     */
    virtual CT_LasPointInfo* write(QDataStream &stream, const size_t &globalIndex);

protected:
    /**
     * @brief Derivated class must return attribute types that they need to write a point
     */
    virtual QList<CT_LasDefine::LASPointAttributesType> typesToSearch();

    /**
     * @brief Write only the information of the point format 9
     */
    void writeInfoFormat9(QDataStream &stream, CT_LasPointInfo *info);
};

#endif // CT_LASPOINTFORMAT9_H
