#ifndef CT_ABSTRACTMODIFIABLECLOUDINDEX_H
#define CT_ABSTRACTMODIFIABLECLOUDINDEX_H

#include "ct_cloudindex/abstract/ct_abstractcloudindex.h"

/**
 * @brief Represent a cloud index that can be modifiable
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractModifiableCloudIndex : virtual public CT_AbstractCloudIndex
{
public:
    virtual ~CT_AbstractModifiableCloudIndex() {}

    /**
     * @brief Add a new index in the cloud
     * @param newIndex : index to add
     */
    virtual void addIndex(const size_t &newIndex) = 0;

    /**
     * @brief Remove the index from the cloud
     * @param index : index to remove
     */
    virtual void removeIndex(const size_t &index) = 0;

    /**
     * @brief Replace the index at the location 'i' in the cloud with the 'newIndex'
     *
     * @warning CAUTION ! If you use this method and sortType() != NotSorted, the sortType will be changed to NotSorted if the index
     * passed in parameter don't respect the sortType. If the class don't accept to be NotSorted the sortType will be corrupted !
     *
     * @param i : location of the index
     * @param newIndex : the newIndex
     * @param verifyRespectSort : set to false if you want to accelerate the process and you are VERY sure to respect the sort
     */
    virtual void replaceIndex(const size_t &i, const ct_index_type &newIndex, const bool &verifyRespectSort = true) = 0;

    /**
     * @brief Removes all indexes in the cloud index
     */
    virtual void clear() = 0;
};

#endif // CT_ABSTRACTMODIFIABLECLOUDINDEX_H
