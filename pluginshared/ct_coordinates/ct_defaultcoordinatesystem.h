#ifndef CT_DEFAULTCOORDINATESYSTEM_H
#define CT_DEFAULTCOORDINATESYSTEM_H

#include "ct_coordinates/abstract/ct_abstractcoordinatesystem.h"

/**
 * @brief The default coordinate system just translate coordinates
 */
class PLUGINSHAREDSHARED_EXPORT CT_DefaultCoordinateSystem : public CT_AbstractCoordinateSystem
{
public:
    CT_DefaultCoordinateSystem();

    /**
     * @brief Returns true if coordinates passed in parameter can be converted to realIm (import). False
     *        if the capacity of a realIm is not sufficient.
     */
    bool canConvertImport(CT_AbstractCoordinateSystem::realEx x,
                          CT_AbstractCoordinateSystem::realEx y,
                          CT_AbstractCoordinateSystem::realEx z) const;

    /**
     * @brief Convert a coordinate of type realEx to realIm. This method don't test if the realIm
     *        can accept the capacity of the converted value (if you want to know if its possible call
     *        the method "canConvertedForImport")
     */
    void convertImport(CT_AbstractCoordinateSystem::realEx x,
                       CT_AbstractCoordinateSystem::realEx y,
                       CT_AbstractCoordinateSystem::realEx z,
                       CT_AbstractCoordinateSystem::realIm &xOut,
                       CT_AbstractCoordinateSystem::realIm &yOut,
                       CT_AbstractCoordinateSystem::realIm &zOut,
                       CT_AbstractCoordinateSystem::DatasToConvert convert = CT_AbstractCoordinateSystem::CONVERT_ALL) const;

    /**
     * @brief Convert a coordinate of type realIm to realEx.
     */
    void convertExport(CT_AbstractCoordinateSystem::realIm x,
                       CT_AbstractCoordinateSystem::realIm y,
                       CT_AbstractCoordinateSystem::realIm z,
                       CT_AbstractCoordinateSystem::realEx &xOut,
                       CT_AbstractCoordinateSystem::realEx &yOut,
                       CT_AbstractCoordinateSystem::realEx &zOut,
                       CT_AbstractCoordinateSystem::DatasToConvert convert = CT_AbstractCoordinateSystem::CONVERT_ALL) const;

    /**
     * @brief Configure the coordinate system.
     * @return false if the configuration was canceled.
     */
    bool configure();

    /**
     * @brief Modify offset of this coordinate system
     */
    bool setOffset(CT_AbstractCoordinateSystem::realEx x, CT_AbstractCoordinateSystem::realEx y, CT_AbstractCoordinateSystem::realEx z);

    /**
     * @brief X Offset
     */
    inline CT_AbstractCoordinateSystem::realEx xOffset() const { return m_xOffset; }

    /**
     * @brief Y Offset
     */
    inline CT_AbstractCoordinateSystem::realEx yOffset() const { return m_yOffset; }

    /**
     * @brief Z Offset
     */
    inline CT_AbstractCoordinateSystem::realEx zOffset() const { return m_zOffset; }

private:
    CT_AbstractCoordinateSystem::realEx  m_xOffset;
    CT_AbstractCoordinateSystem::realEx  m_yOffset;
    CT_AbstractCoordinateSystem::realEx  m_zOffset;
};

#endif // CT_DEFAULTCOORDINATESYSTEM_H
