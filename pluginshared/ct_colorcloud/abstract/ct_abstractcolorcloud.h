#ifndef CT_ABSTRACTCOLORCLOUD_H
#define CT_ABSTRACTCOLORCLOUD_H

#include "ct_cloud/abstract/ct_abstractcloud.h"

#include "interfaces.h"
#include "ct_color.h"

#ifdef USE_PCL
#include "pcl/point_cloud.h"
#endif

/**
 * A color cloud
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractColorCloud : virtual public CT_AbstractCloud
{
public:
    CT_AbstractColorCloud();
    virtual ~CT_AbstractColorCloud() {}

    /**
     * @brief Returns the number of colors
     */
    virtual size_t size() const = 0;

    /**
     * @brief Returns a reference to the color at index 'index'
     */
    virtual CT_Color& colorAt(const size_t &index) = 0;

    /**
     * @brief Returns a const reference to the color at index 'index'
     */
    virtual const CT_Color& constColorAt(const size_t &index) const = 0;

    /**
     * @brief Returns a reference to the color at index 'index'
     */
    virtual CT_Color& operator[](const size_t &index) = 0;

    /**
     * @brief Returns a const reference to the color at index 'index'
     */
    virtual const CT_Color& operator[](const size_t &index) const = 0;

#ifdef USE_PCL
    /**
     * @brief Returns the pcl cloud of colors
     */
    virtual boost::shared_ptr< pcl::PointCloud<CT_Color> > getPCLCloud() const = 0;
#endif
};

#endif // CT_ABSTRACTCOLORCLOUD_H
