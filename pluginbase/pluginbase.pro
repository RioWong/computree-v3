COMPUTREE += ctlibio ctlibmetrics ctlibfilters ctliblas

include(../shared.pri)
include($${PLUGIN_SHARED_DIR}/include.pri)
include(../include_ct_library.pri)

DEFINES += _CRT_SECURE_NO_WARNINGS

TARGET = plug_base

greaterThan(QT_MAJOR_VERSION, 4): QT += concurrent

HEADERS += $${PLUGIN_SHARED_INTERFACE_DIR}/interfaces.h \
    pb_pluginentry.h \
    pb_steppluginmanager.h \
    actions/pb_actionselectitemdrawablegv.h \
    actions/pb_actionshowitemdatagv.h \
    exporters/ascid/pb_ascidexporter.h \
    exporters/ascrgb/pb_ascrgbexporter.h \
    exporters/csv/pb_csvexporter.h \
    exporters/csv/pb_csvexportercolumn.h \
    exporters/csv/pb_csvexporterconfiguration.h \
    exporters/gdal/pb_gdalexporter.h \
    exporters/grid2d/pb_grid2dexporter.h \
    exporters/grid3d/pb_grid3dastableexporter.h \
    exporters/grid3d/pb_grid3dexporter.h \
    exporters/groupdata/pb_groupdataexporter.h \
    exporters/mesh/pb_meshobjexporter.h \
    exporters/pbm/pb_pbmexporter.h \
    exporters/pgm/pb_pgmexporter.h \
    exporters/polygon2d/pb_polygon2dexporter.h \
    exporters/profile/pb_profileexporter.h \
    exporters/topology/pb_opfexporter.h \
    exporters/xyb/pb_multixybexporter.h \
    exporters/xyb/pb_xybexporter.h \
    step/pb_stepapplypointfilters.h \
    step/pb_stepbeginloopthroughgroups.h \
    step/pb_stepcomputepointmetrics.h \
    step/pb_stepcomputerastermetrics.h \
    step/pb_stepexportitemlist.h \
    step/pb_stepgenericexporter.h \
    step/pb_stepgenericloadfile.h \
    step/pb_stepuseritemselection.h \
    views/actions/pb_actionselectitemdrawablegvoptions.h \
    views/exporters/csv/pbg_csvconfigurationdialog.h \
    views/exporters/csv/pbg_csvdatareflistwidget.h \
    views/exporters/csv/pbg_csvpreviewwidget.h \
    actions/pb_actionpickitemsinlist.h \
    views/actions/pb_actionpickitemsinlistoptions.h \
    step/pb_stepcreatereaderlist.h \
    step/pb_stepusereadertoloadfiles.h \
    step/pb_stepexportattributesinloop.h \
    step/pb_stepexportpointsbyxyarea.h

SOURCES += \
    pb_pluginentry.cpp \
    pb_steppluginmanager.cpp \
    actions/pb_actionselectitemdrawablegv.cpp \
    actions/pb_actionshowitemdatagv.cpp \
    exporters/ascid/pb_ascidexporter.cpp \
    exporters/ascrgb/pb_ascrgbexporter.cpp \
    exporters/csv/pb_csvexporter.cpp \
    exporters/csv/pb_csvexportercolumn.cpp \
    exporters/csv/pb_csvexporterconfiguration.cpp \
    exporters/gdal/pb_gdalexporter.cpp \
    exporters/grid2d/pb_grid2dexporter.cpp \
    exporters/grid3d/pb_grid3dastableexporter.cpp \
    exporters/grid3d/pb_grid3dexporter.cpp \
    exporters/groupdata/pb_groupdataexporter.cpp \
    exporters/mesh/pb_meshobjexporter.cpp \
    exporters/pbm/pb_pbmexporter.cpp \
    exporters/pgm/pb_pgmexporter.cpp \
    exporters/polygon2d/pb_polygon2dexporter.cpp \
    exporters/profile/pb_profileexporter.cpp \
    exporters/topology/pb_opfexporter.cpp \
    exporters/xyb/pb_multixybexporter.cpp \
    exporters/xyb/pb_xybexporter.cpp \
    step/pb_stepapplypointfilters.cpp \
    step/pb_stepbeginloopthroughgroups.cpp \
    step/pb_stepcomputepointmetrics.cpp \
    step/pb_stepcomputerastermetrics.cpp \
    step/pb_stepexportitemlist.cpp \
    step/pb_stepgenericexporter.cpp \
    step/pb_stepgenericloadfile.cpp \
    step/pb_stepuseritemselection.cpp \
    views/actions/pb_actionselectitemdrawablegvoptions.cpp \
    views/exporters/csv/pbg_csvconfigurationdialog.cpp \
    views/exporters/csv/pbg_csvdatareflistwidget.cpp \
    views/exporters/csv/pbg_csvpreviewwidget.cpp \
    actions/pb_actionpickitemsinlist.cpp \
    views/actions/pb_actionpickitemsinlistoptions.cpp \
    step/pb_stepcreatereaderlist.cpp \
    step/pb_stepusereadertoloadfiles.cpp \
    step/pb_stepexportattributesinloop.cpp \
    step/pb_stepexportpointsbyxyarea.cpp

INCLUDEPATH += .
INCLUDEPATH += ./actions
INCLUDEPATH += ./views
INCLUDEPATH += ./step
INCLUDEPATH += ./views/actions

RESOURCES += resource.qrc

FORMS += \
    views/actions/pb_actionselectitemdrawablegvoptions.ui \
    views/exporters/csv/pbg_csvconfigurationdialog.ui \
    views/exporters/csv/pbg_csvdatareflistwidget.ui \
    views/exporters/csv/pbg_csvpreviewwidget.ui \
    views/actions/pb_actionpickitemsinlistoptions.ui

TRANSLATIONS += languages/pluginbase_fr.ts \
                languages/pluginbase_en.ts
