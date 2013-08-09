QT       += core gui

TARGET = stepcreatorv2
TEMPLATE = app

OBJECTS_DIR = .obj
MOC_DIR = .moc
UI_DIR = .ui
RCC_DIR = .rcc

SOURCES += main.cpp\
        mainwindow.cpp \
    inmodeldialog.cpp \
    outmodeldialog.cpp \
    widgets/inresultwidget.cpp \
    widgets/ingroupwidget.cpp \
    models/inresultmodel.cpp \
    models/ingroupmodel.cpp \
    models/initemmodel.cpp \
    widgets/initemwidget.cpp \
    tools.cpp \
    models/abstractinmodel.cpp \
    widgets/abstractinwidget.cpp \
    models/outresultmodel.cpp \
    models/outitemmodel.cpp \
    models/outgroupmodel.cpp \
    models/abstractoutmodel.cpp \
    widgets/outresultwidget.cpp \
    widgets/outitemwidget.cpp \
    widgets/outgroupwidget.cpp \
    widgets/abstractoutwidget.cpp \
    copymodeldialog.cpp \
    widgets/copyresultwidget.cpp \
    widgets/copyitemwidget.cpp \
    widgets/copygroupwidget.cpp \
    widgets/abstractcopywidget.cpp \
    models/copyresultmodel.cpp \
    models/copyitemmodel.cpp \
    models/copygroupmodel.cpp \
    models/abstractcopymodel.cpp \
    parameters/abstractparameter.cpp \
    parameters/widgets/abstractparameterwidget.cpp \
    parametermodeldialog.cpp \
    parameters/parameterbool.cpp \
    parameters/widgets/widgetbool.cpp \
    parameters/parametertextline.cpp \
    parameters/parameterstringchoice.cpp \
    parameters/parameterint.cpp \
    parameters/parameterfilechoice.cpp \
    parameters/parameterexcludevalue.cpp \
    parameters/parameteremptyline.cpp \
    parameters/parameterdouble.cpp \
    parameters/widgets/widgettextline.cpp \
    parameters/widgets/widgetstringchoice.cpp \
    parameters/widgets/widgetint.cpp \
    parameters/widgets/widgetfilechoice.cpp \
    parameters/widgets/widgetexcludevalue.cpp \
    parameters/widgets/widgetemptyline.cpp \
    parameters/widgets/widgetdouble.cpp \
    parameters/parameterstring.cpp \
    parameters/widgets/widgetstring.cpp

HEADERS  += mainwindow.h \
    inmodeldialog.h \
    outmodeldialog.h \
    widgets/inresultwidget.h \
    widgets/ingroupwidget.h \
    models/inresultmodel.h \
    models/ingroupmodel.h \
    models/initemmodel.h \
    widgets/initemwidget.h \
    tools.h \
    models/abstractinmodel.h \
    widgets/abstractinwidget.h \
    models/outresultmodel.h \
    models/outitemmodel.h \
    models/outgroupmodel.h \
    models/abstractoutmodel.h \
    widgets/outresultwidget.h \
    widgets/outitemwidget.h \
    widgets/outgroupwidget.h \
    widgets/abstractoutwidget.h \
    copymodeldialog.h \
    widgets/copyresultwidget.h \
    widgets/copyitemwidget.h \
    widgets/copygroupwidget.h \
    widgets/abstractcopywidget.h \
    models/copyresultmodel.h \
    models/copyitemmodel.h \
    models/copygroupmodel.h \
    models/abstractcopymodel.h \
    parameters/abstractparameter.h \
    parameters/widgets/abstractparameterwidget.h \
    parametermodeldialog.h \
    parameters/parameterbool.h \
    parameters/widgets/widgetbool.h \
    parameters/parametertextline.h \
    parameters/parameterstringchoice.h \
    parameters/parameterint.h \
    parameters/parameterfilechoice.h \
    parameters/parameterexcludevalue.h \
    parameters/parameteremptyline.h \
    parameters/parameterdouble.h \
    parameters/widgets/widgettextline.h \
    parameters/widgets/widgetstringchoice.h \
    parameters/widgets/widgetint.h \
    parameters/widgets/widgetfilechoice.h \
    parameters/widgets/widgetexcludevalue.h \
    parameters/widgets/widgetemptyline.h \
    parameters/widgets/widgetdouble.h \
    parameters/parameterstring.h \
    parameters/widgets/widgetstring.h

FORMS    += mainwindow.ui \
    inmodeldialog.ui \
    outmodeldialog.ui \
    widgets/inresultwidget.ui \
    widgets/ingroupwidget.ui \
    widgets/initemwidget.ui \
    widgets/outresultwidget.ui \
    widgets/outitemwidget.ui \
    widgets/outgroupwidget.ui \
    copymodeldialog.ui \
    widgets/copyresultwidget.ui \
    widgets/copyitemwidget.ui \
    widgets/copygroupwidget.ui \
    parametermodeldialog.ui \
    parameters/widgets/widgetbool.ui \
    parameters/widgets/widgettextline.ui \
    parameters/widgets/widgetstringchoice.ui \
    parameters/widgets/widgetint.ui \
    parameters/widgets/widgetfilechoice.ui \
    parameters/widgets/widgetexcludevalue.ui \
    parameters/widgets/widgetemptyline.ui \
    parameters/widgets/widgetdouble.ui \
    parameters/widgets/widgetstring.ui


INCLUDEPATH += ./models
INCLUDEPATH += ./widgets



















