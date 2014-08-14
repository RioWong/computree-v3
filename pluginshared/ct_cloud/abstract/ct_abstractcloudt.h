#ifndef CT_ABSTRACTCLOUDT_H
#define CT_ABSTRACTCLOUDT_H

#include "ct_abstractcloud.h"

#ifdef USE_PCL
#include "pcl/point_cloud.h"
#endif

/**
 * @brief A cloud of T
 */
template<typename T>
class CT_AbstractCloudT : virtual public CT_AbstractCloud
{
public:
    CT_AbstractCloudT() {}

    inline size_t indexOf(const T *pointer) const
    {
        if((pointer == NULL)
                || (size() == 0))
            return 0;

        return (pointer - (&constTAt(0)));
    }

    inline T& tAt(const size_t &index) { return (*this)[index]; }
    inline const T& constTAt(const size_t &index) const { return (*this)[index]; }

    virtual T& operator[](const size_t &index) = 0;
    virtual const T& operator[](const size_t &index) const = 0;

#ifdef USE_PCL
    virtual boost::shared_ptr< pcl::PointCloud<T> > getPCLCloud() const = 0;
#endif
};

#endif // CT_ABSTRACTCLOUDT_H
