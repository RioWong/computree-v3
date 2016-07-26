##### OSG ####
include(osg_default_path.pri)
exists(osg_user_path.pri) {
    include(osg_user_path.pri)
    OSG_USER_MODE = "osg_user_path.pri"
} else {
    OSG_USER_MODE = "osg_default_path.pri"
}
include(osg_check.pri)

#if OSG can be used
isEmpty(USE_OSG_ERROR_MSG) {
    OSG_LIB_ADD = osg
    OSG_LIB_ADD += OpenThreads
    OSG_LIB_ADD += osgGA
    OSG_LIB_ADD += osgViewer
    OSG_LIB_ADD += osgWidget
    OSG_LIB_ADD += osgManipulator
    OSG_LIB_ADD += osgSim
    OSG_LIB_ADD += osgText
    OSG_LIB_ADD += osgUtil
    OSG_LIB_ADD += osgDB
    OSG_LIB_ADD += osgFX

    unix {
        for(a, OSG_LIB_ADD) {
            CONFIG(debug, debug|release) {
                !exists($$OSG_LIBS_PATH/$${a}d*) {
                    USE_OSG_ERROR_MSG += "Library $$OSG_LIBS_PATH/$${a}d was not found"
                }
            } else {
                !exists($$OSG_LIBS_PATH/$${a}*) {
                    USE_OSG_ERROR_MSG += "Library $$OSG_LIBS_PATH/$${a} was not found"
                }
            }
        }
    }

    windows {
        for(a, OSG_LIB_ADD) {
            CONFIG(debug, debug|release) {
                !exists($$OSG_LIBS_PATH/$${a}d*) {
                    USE_OSG_ERROR_MSG += "Library $$OSG_LIBS_PATH/$${a}d was not found"
                }
            } else {
                !exists($$OSG_LIBS_PATH/$${a}*) {
                    USE_OSG_ERROR_MSG += "Library $$OSG_LIBS_PATH/$${a} was not found"
                }
            }
        }
    }
    isEmpty(USE_OSG_ERROR_MSG) {
        warning("DEPENDENCY CHECK - OSG    - OK ($$OSG_USER_MODE)")
    } else {
        warning("DEPENDENCY CHECK - OSG    - Libraries not found in specified path ($$OSG_USER_MODE)")
    }
} else {
        warning("DEPENDENCY CHECK - OSG    - invalid path specified ($$OSG_USER_MODE)")
}


##### GDAL ####
include(gdal_default_path.pri)
exists(gdal_user_path.pri) {
    include(gdal_user_path.pri)
    GDAL_USER_MODE = "gdal_user_path.pri"
} else {
    GDAL_USER_MODE = "gdal_default_path.pri"
}
include(gdal_check.pri)

#if GDAL can be used
isEmpty(USE_GDAL_ERROR_MSG) {
    GDAL_LIB_ADD = gdal

    unix {
        for(a, GDAL_LIB_ADD) {
            CONFIG(debug, debug|release) {
                !exists($$GDAL_LIBS_PATH/lib$${a}_i*) {
                    USE_GDAL_ERROR_MSG += "Library $$GDAL_LIBS_PATH/lib$${a}_i was not found"
                }
            } else {
                !exists($$GDAL_LIBS_PATH/lib$${a}_i*) {
                    USE_GDAL_ERROR_MSG += "Library $$GDAL_LIBS_PATH/lib$${a}_i was not found"
                }
            }
        }
    }

    windows {
        for(a, GDAL_LIB_ADD) {
            CONFIG(debug, debug|release) {
                !exists($$GDAL_LIBS_PATH/$${a}_i*) {
                    USE_GDAL_ERROR_MSG += "Library $$GDAL_LIBS_PATH/$${a}_i was not found"
                }
            } else {

                !exists($$GDAL_LIBS_PATH/$${a}_i*) {
                    USE_GDAL_ERROR_MSG += "Library $$GDAL_LIBS_PATH/$${a}_i was not found"
                }
            }
        }
    }

    isEmpty(USE_GDAL_ERROR_MSG) {
        warning("DEPENDENCY CHECK - GDAL   - OK ($$GDAL_USER_MODE)")
    } else {
        warning("DEPENDENCY CHECK - GDAL   - Libraries not found in specified path ($$GDAL_USER_MODE)")
    }
} else {
        warning("DEPENDENCY CHECK - GDAL   - invalid path specified ($$GDAL_USER_MODE)")
}


##### GSL ####
include(gsl_default_path.pri)
exists(gsl_user_path.pri) {
    include(gsl_user_path.pri)
    GSL_USER_MODE = "gsl_user_path.pri"
} else {
    GSL_USER_MODE = "gsl_default_path.pri"
}
include(gsl_check.pri)

#if GSL can be used
isEmpty(USE_GSL_ERROR_MSG) {
    GSL_LIB_ADD = libgsl

    unix {
        for(a, GSL_LIB_ADD) {
            CONFIG(debug, debug|release) {
                !exists($$GSL_LIBS_PATH/lib$${a}_debug*) {
                    USE_GSL_ERROR_MSG += "Library $$GSL_LIBS_PATH/lib$${a}_debug was not found"
                }
            } else {
                !exists($$GSL_LIBS_PATH/lib$${a}*) {
                    USE_GSL_ERROR_MSG += "Library $$GSL_LIBS_PATH/lib$${a} was not found"
                }
            }
        }
    }

    windows {
        for(a, GSL_LIB_ADD) {
            CONFIG(debug, debug|release) {
                !exists($$GSL_LIBS_PATH/$${a}_d*) {
                    USE_GSL_ERROR_MSG += "Library $$GSL_LIBS_PATH/$${a}_d was not found"
                }
            } else {

                !exists($$GSL_LIBS_PATH/$${a}*) {
                    USE_GSL_ERROR_MSG += "Library $$GSL_LIBS_PATH/$${a} was not found"
                }
            }
        }
    }

    isEmpty(USE_GSL_ERROR_MSG) {
        warning("DEPENDENCY CHECK - GSL    - OK ($$GSL_USER_MODE)")
    } else {
        warning("DEPENDENCY CHECK - GSL    - Libraries not found in specified path ($$GSL_USER_MODE)")
    }
} else {
        warning("DEPENDENCY CHECK - GSL    - invalid path specified ($$GSL_USER_MODE)")
}


##### PCL ####
include(pcl_default_path.pri)
exists(pcl_user_path.pri) {
    include(pcl_user_path.pri)
    PCL_USER_MODE = "pcl_user_path.pri"
} else {
    PCL_USER_MODE = "pcl_default_path.pri"
}
include(pcl_check.pri)

#if PCL can be used
isEmpty(USE_PCL_ERROR_MSG) {
    PCL_LIB_ADD =
    PCL_LIB_ADD += pcl_common
    PCL_LIB_ADD += pcl_features
    PCL_LIB_ADD += pcl_filters
    PCL_LIB_ADD += pcl_io_ply
    PCL_LIB_ADD += pcl_io
    PCL_LIB_ADD += pcl_kdtree
    PCL_LIB_ADD += pcl_keypoints
    PCL_LIB_ADD += pcl_octree
    PCL_LIB_ADD += pcl_outofcore
    PCL_LIB_ADD += pcl_people
    PCL_LIB_ADD += pcl_recognition
    PCL_LIB_ADD += pcl_registration
    PCL_LIB_ADD += pcl_sample_consensus
    PCL_LIB_ADD += pcl_search
    PCL_LIB_ADD += pcl_segmentation
    PCL_LIB_ADD += pcl_surface
    PCL_LIB_ADD += pcl_tracking
    PCL_LIB_ADD += pcl_visualization

    unix {
        for(a, PCL_LIB_ADD) {
            CONFIG(debug, debug|release) {
                !exists($$PCL_LIBS_PATH/lib$${a}_debug*) {
                    USE_PCL_ERROR_MSG += "Library $$PCL_LIBS_PATH/lib$${a}_debug was not found"
                }
            } else {
                !exists($$PCL_LIBS_PATH/lib$${a}*) {
                    USE_PCL_ERROR_MSG += "Library $$PCL_LIBS_PATH/lib$${a} was not found"
                }
            }
        }
    }
    windows {
        for(a, PCL_LIB_ADD) {
            CONFIG(debug, debug|release) {
                !exists($$PCL_LIBS_PATH/$${a}_debug*) {
                    USE_PCL_ERROR_MSG += "Library $$PCL_LIBS_PATH/$${a}_debug was not found"
                }
            } else {

                !exists($$PCL_LIBS_PATH/$${a}_release*) {
                    USE_PCL_ERROR_MSG += "Library $$PCL_LIBS_PATH/$${a}_release was not found"
                }
            }
        }
    }

    isEmpty(USE_PCL_ERROR_MSG) {
        warning("DEPENDENCY CHECK - PCL    - OK ($$PCL_USER_MODE)")
    } else {
        warning("DEPENDENCY CHECK - PCL    - Libraries not found in specified path ($$PCL_USER_MODE)")
    }
} else {
        warning("DEPENDENCY CHECK - PCL    - invalid path specified ($$PCL_USER_MODE)")
}


##### OPENCV ####
include(opencv_default_path.pri)
exists(opencv_user_path.pri) {
    include(opencv_user_path.pri)
    OPENCV_USER_MODE = "opencv_user_path.pri"
} else {
    OPENCV_USER_MODE = "opencv_default_path.pri"
}
include(opencv_check.pri)

#if OPENCV can be used
isEmpty(USE_OPENCV_ERROR_MSG) {
    OPENCV_LIB_ADD =
    OPENCV_LIB_ADD += opencv_ts
    OPENCV_LIB_ADD += opencv_world

    unix {
        for(a, OPENCV_LIB_ADD) {
            CONFIG(debug, debug|release) {
                !exists($$OPENCV_LIBS_PATH/lib$${a}$${OPENCV_VERSION}d*) {
                    USE_OPENCV_ERROR_MSG += "Library $$OPENCV_LIBS_PATH/lib$${a}$${OPENCV_VERSION}d was not found"
                }
            } else {
                !exists($$OPENCV_LIBS_PATH/lib$${a}$${OPENCV_VERSION}*) {
                    USE_OPENCV_ERROR_MSG += "Library $$OPENCV_LIBS_PATH/lib$${a}$${OPENCV_VERSION} was not found"
                }
            }
        }
    }

    windows {
        for(a, OPENCV_LIB_ADD) {
            CONFIG(debug, debug|release) {
                !exists($$OPENCV_LIBS_PATH/$${a}$${OPENCV_VERSION}d*) {
                    USE_OPENCV_ERROR_MSG += "Library $$OPENCV_LIBS_PATH/$${a}$${OPENCV_VERSION}d was not found"
                }
            } else {
                !exists($$OPENCV_LIBS_PATH/$${a}$${OPENCV_VERSION}*) {
                    USE_OPENCV_ERROR_MSG += "Library $$OPENCV_LIBS_PATH/$${a}$${OPENCV_VERSION} was not found"
                }
            }
        }
    }

    isEmpty(USE_OPENCV_ERROR_MSG) {
        warning("DEPENDENCY CHECK - OPENCV - OK ($$OPENCV_USER_MODE)")
    } else {
        warning("DEPENDENCY CHECK - OPENCV - Libraries not found in specified path ($$OPENCV_USER_MODE)")
    }
} else {
        warning("DEPENDENCY CHECK - OPENCV - invalid path specified ($$OPENCV_USER_MODE)")
}
