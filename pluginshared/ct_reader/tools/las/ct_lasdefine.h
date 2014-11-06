#ifndef CT_LASDEFINE_H
#define CT_LASDEFINE_H

#include "pluginShared_global.h"

class PLUGINSHAREDSHARED_EXPORT CT_LasDefine {
public:
    /**
     * @brief Use this type to set the type of the LAS point attributes
     */
    enum LASPointAttributesType {
        Intensity = 0,
        Return_Number,
        Number_of_Returns,
        Classification_Flags,
        Scanner_Channel,
        Scan_Direction_Flag,
        Edge_of_Flight_Line,
        Classification,
        Scan_Angle_Rank,
        User_Data,
        Point_Source_ID,
        GPS_Time,
        Red,
        Green,
        Blue,
        NIR,
        Wave_Packet_Descriptor_Index,
        Byte_offset_to_waveform_data,
        Waveform_packet_size_in_bytes,
        Return_Point_Waveform_Location
    }; // if you add another attributes don't forget to add it in the CT_LasPointInfo class and CT_LasPointFormatX to write it
};

#endif // CT_LASDEFINE_H
