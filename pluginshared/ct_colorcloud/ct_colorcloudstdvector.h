#ifndef CT_COLORCLOUDSTDVECTOR_H
#define CT_COLORCLOUDSTDVECTOR_H

#include "ct_colorcloud/abstract/ct_abstractcolorcloud.h"
#include "ct_cloud/ct_standardcloudstdvectort.h"

/**
 * A color cloud that use a std::vector
 */
class PLUGINSHAREDSHARED_EXPORT CT_ColorCloudStdVector : public CT_StandardCloudStdVectorT<CT_Color>, public CT_AbstractColorCloud
{
public:
    CT_ColorCloudStdVector(bool withAlphaInformation);
    CT_ColorCloudStdVector(size_t size, bool withAlphaInformation);
    CT_ColorCloudStdVector(size_t size, bool *withAlphaInformation);

    /**
     * @brief Returns the number of colors
     */
    size_t size() const;

    /**
     * @brief Add the normal color in parameter to the cloud
     */
    void addColor(const CT_Color &color);

    /**
     * @brief Add a default color to the cloud and return a reference to it
     */
    CT_Color& addColor();

    /**
     * @brief Returns true if must use the alpha information of colors
     */
    bool hasAlphaInformation() const;

    /**
     * @brief Returns a reference to the color at index 'index'
     */
    CT_Color& colorAt(const size_t &index);

    /**
     * @brief Returns a const reference to the color at index 'index'
     */
    const CT_Color& constColorAt(const size_t &index) const;

    /**
     * @brief Returns a reference to the color at index 'index'
     */
    CT_Color& operator[](const size_t &index);

    /**
     * @brief Returns a const reference to the color at index 'index'
     */
    const CT_Color& operator[](const size_t &index) const;

    /**
     * @brief Returns a copy of this cloud
     */
    CT_AbstractCloud* copy() const;

#ifdef USE_PCL
    /**
     * @brief Returns the pcl cloud of colors
     */
    boost::shared_ptr< pcl::PointCloud<CT_Color> > getPCLCloud() const;
#endif

private:
    bool                    m_withAlphaInformation;
};

#endif // CT_COLORCLOUDSTDVECTOR_H
