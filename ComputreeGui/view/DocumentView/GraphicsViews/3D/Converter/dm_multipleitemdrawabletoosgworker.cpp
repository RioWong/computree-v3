#include "dm_multipleitemdrawabletoosgworker.h"

#include "dm_guimanager.h"
#include "view/DocumentView/GraphicsViews/3D/Picking/dm_osgpicker.h"

#include <osg/ShadeModel>

#include <QThreadPool>
#include <QMutexLocker>

#define SHADER_INFO_LOCATION 6
//#define SHADER_COLOR_LOCATION 7

/*static const char *vertexShader = {
    "#version 120\n"
    "// selection color of points\n"
    "uniform vec4 selectionColor;\n"
    "// to check if it is selected\n"
    "uniform int checkSelected;\n"
    "// info of the point (is selected ?)\n"
    "attribute float info;\n"
    "// vertex color\n"
    "attribute vec4 myColor;\n"
    "void main(void)\n"
    "{\n"
    "    int infoInt = int(info);\n"
    "    if(infoInt == checkSelected)\n"
    "    {\n"
    "         gl_FrontColor = selectionColor;\n"
    "    }\n"
    "    else\n"
    "    {\n"
    "         gl_FrontColor = myColor;\n"
    "    }\n"
    "    gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "}\n"
};*/


static const char *vertexShader = {
    "#version 120\n"
    "// selection color of points\n"
    "uniform vec4 selectionColor;\n"
    "// to check if it is selected\n"
    "uniform int checkSelected;\n"
    "// info of the point (is selected ?)\n"
    "attribute float info;\n"
    "void main(void)\n"
    "{\n"
    "    int infoInt = int(info);\n"
    "    if(infoInt == checkSelected)\n"
    "    {\n"
    "         gl_FrontColor = selectionColor;\n"
    "    }\n"
    "    else\n"
    "    {\n"
    "         gl_FrontColor = gl_Color;\n"
    "    }\n"
    "    gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "}\n"
};


DM_MultipleItemDrawableToOsgWorker::DM_MultipleItemDrawableToOsgWorker(GOsgGraphicsView &view) : m_view(view)
{
    m_mutex = new QMutex(QMutex::Recursive);
    m_stop = false;

    if(m_view.getDocumentView() != NULL) {
        m_geometriesConfiguration.setGlobalColorArray(dynamic_cast<GDocumentViewForGraphics*>(m_view.document())->getOrCreateGlobalColorArrayForPoints());
        m_geometriesConfiguration.setGlobalVertexAttribArray(SHADER_INFO_LOCATION, dynamic_cast<GDocumentViewForGraphics*>(m_view.document())->getOrCreateGlobalAttribArrayForPoints());
    }

    //m_geometriesConfiguration.setGlobalColorVertexAttribArrayLocationIndex(SHADER_COLOR_LOCATION);
    //m_geometriesConfiguration.setLocalColorVertexAttribArrayLocationIndex(SHADER_COLOR_LOCATION);

    createGlobalShader();
    createLocalShader();
}

DM_MultipleItemDrawableToOsgWorker::~DM_MultipleItemDrawableToOsgWorker()
{
    m_stop = true;

    while(isRunning());

    delete m_mutex;
}

QHash<CT_AbstractItemDrawable *, DM_PainterToOsgElementsResult> DM_MultipleItemDrawableToOsgWorker::takeResults()
{
    QMutexLocker locker(m_mutex);

    QHash< CT_AbstractItemDrawable*, DM_PainterToOsgElementsResult > l = m_results;
    m_results.clear();

    return l;
}

uint DM_MultipleItemDrawableToOsgWorker::staticGlobalVertexAttribArrayLocationIndex()
{
    return SHADER_INFO_LOCATION;
}

uint DM_MultipleItemDrawableToOsgWorker::staticLocalVertexAttribArrayLocationIndex()
{
    return SHADER_INFO_LOCATION;
}

void DM_MultipleItemDrawableToOsgWorker::createGlobalShader()
{
    osg::ref_ptr<osg::Program> program = new osg::Program;
    osg::ref_ptr<osg::Shader> shader = new osg::Shader( osg::Shader::VERTEX, vertexShader);

    if(program->addShader(shader)) {
        program->addBindAttribLocation("info", SHADER_INFO_LOCATION);
        //program->addBindAttribLocation("myColor", SHADER_COLOR_LOCATION);

        osg::ref_ptr<osg::StateSet> ss = new osg::StateSet;
        ss->setAttributeAndModes(program.get(), osg::StateAttribute::ON);

        QColor color = m_view.getOptions().getSelectedColor();

        ss->addUniform(new osg::Uniform("selectionColor", osg::Vec4(color.redF(), color.greenF(), color.blueF(), color.alphaF())));
        ss->addUniform(new osg::Uniform("checkSelected", (GLbyte)1));

        m_geometriesConfiguration.setGlobalGeometriesStateSet(ss);

    }
}

void DM_MultipleItemDrawableToOsgWorker::createLocalShader()
{
    if(m_geometriesConfiguration.globalStateSet() != NULL) {

        osg::Program *program = (osg::Program*)m_geometriesConfiguration.globalStateSet()->getAttribute(osg::StateAttribute::PROGRAM);

        if(program != NULL) {
            osg::StateSet *ss = new osg::StateSet;
            ss->setAttributeAndModes(program, osg::StateAttribute::ON);

            QColor color = m_view.getOptions().getSelectedColor();

            ss->addUniform(new osg::Uniform("selectionColor", osg::Vec4(color.redF(), color.greenF(), color.blueF(), color.alphaF())));
            ss->addUniform(new osg::Uniform("checkSelected", (GLbyte)1));

            m_geometriesConfiguration.setLocalGeometriesStateSet(ss);
        }
    }
}

void DM_MultipleItemDrawableToOsgWorker::run()
{
    while(!m_stop) {
        int ideal = QThread::idealThreadCount();

        if(ideal < 2)
            ideal = 2;

        while((QThreadPool::globalInstance()->activeThreadCount() >= ideal) && !m_stop) {
            // wait loop
            msleep(10);
        }

        if(!m_stop)
        {
            QMutexLocker locker(m_mutex);

            while(!m_queue.isEmpty() && (QThreadPool::globalInstance()->activeThreadCount() < ideal)) {
                DM_SingleItemDrawableToOsgWorker *worker = new DM_SingleItemDrawableToOsgWorker(m_view);
                CT_AbstractItemDrawable *item = m_queue.dequeue();
                worker->setColor(m_infos.take(item));
                worker->setItemDrawable(item);

                DM_GeometriesConfiguration newConfig = m_geometriesConfiguration;
                newConfig.setLocalVertexAttribArray(SHADER_INFO_LOCATION, new DM_OsgPicker::LocalVertexAttribArray(1));

                worker->setGeometriesConfiguration(newConfig);

                connect(worker, SIGNAL(finished(DM_SingleItemDrawableToOsgWorker*)), this, SLOT(workerFinished(DM_SingleItemDrawableToOsgWorker*)), Qt::DirectConnection);

                QThreadPool::globalInstance()->start(worker);
            }
        }
    }
}

void DM_MultipleItemDrawableToOsgWorker::addItemDrawable(CT_AbstractItemDrawable &item, const QColor &defaultColor)
{
    QMutexLocker locker(m_mutex);

    if(!m_queue.contains(&item)) {
        m_queue.enqueue(&item);
        m_infos.insert(&item, defaultColor);
        connect(&item, SIGNAL(destroyed(QObject*)), this, SLOT(itemDrawableDestroyed(QObject*)), Qt::DirectConnection);
    } else {
        m_infos.insert(&item, defaultColor);
    }
}

void DM_MultipleItemDrawableToOsgWorker::removeItemDrawable(CT_AbstractItemDrawable &item)
{
    removeItemDrawable(&item);
}

void DM_MultipleItemDrawableToOsgWorker::removeItemDrawable(CT_AbstractItemDrawable *item)
{
    QMutexLocker locker(m_mutex);

    m_queue.removeOne(item);
    m_infos.remove(item);

    disconnect(item, NULL, this, NULL);
}

void DM_MultipleItemDrawableToOsgWorker::itemDrawableDestroyed(QObject *o)
{
    removeItemDrawable((CT_AbstractItemDrawable*)o);
}

void DM_MultipleItemDrawableToOsgWorker::workerFinished(DM_SingleItemDrawableToOsgWorker *w)
{
    QMutexLocker locker(m_mutex);

    disconnect(w->itemDrawable(), NULL, this, NULL);

    m_results.insert(w->itemDrawable(), w->result());

    emit newResultAvailable();
}
