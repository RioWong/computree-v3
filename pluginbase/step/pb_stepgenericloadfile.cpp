#include "pb_stepgenericloadfile.h"

#include "ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstdsingularitemmodel.h"
#include "ct_result/model/inModel/ct_inresultmodelnotneedinputresult.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_itemdrawable/ct_standarditemgroup.h"
#include "ct_model/tools/ct_modelsearchhelper.h"

#define DEF_SearchResult                "PB_SGLF_result"
#define DEF_SearchGroup                 "PB_SGLF_group"

PB_StepGenericLoadFile::PB_StepGenericLoadFile(CT_StepInitializeData &dataInit, CT_AbstractReader *reader) : CT_AbstractStepLoadFile(dataInit)
{
    m_reader = reader;

    connect(m_reader, SIGNAL(progressChanged(int)), this, SLOT(readerProgressChanged(int)), Qt::DirectConnection);
    connect(m_reader, SIGNAL(filePathModified()), this, SLOT(readerFilePathModified()), Qt::DirectConnection);
    connect(this, SIGNAL(stopped()), m_reader, SLOT(cancel()), Qt::DirectConnection);
}

PB_StepGenericLoadFile::~PB_StepGenericLoadFile()
{
    delete m_reader;
}

void PB_StepGenericLoadFile::init()
{
    m_reader->init(false);

    CT_AbstractStepLoadFile::init();
}

QString PB_StepGenericLoadFile::getStepName() const
{
    return m_reader->GetReaderClassName();
}

QString PB_StepGenericLoadFile::getStepDisplayableName() const
{
    return m_reader->GetReaderName();
}

QString PB_StepGenericLoadFile::getStepDescription() const
{
//    const QList<FileFormat>& formats = m_reader->readableFormats();

//    if (formats.size() > 0)
//    {
//        return formats.first().description();
//    }

    return m_reader->GetReaderName();

}

QString PB_StepGenericLoadFile::getStepDetailledDescription() const
{
    if(m_reader != NULL)
    {
        QString t = m_reader->toolTip();

        if(!t.isEmpty())
            return t;
    }

    QString ext = createAcceptedExtensionString(" ");

    return tr("Charge un fichier du type") + ext;
}

QList<FileFormat> PB_StepGenericLoadFile::getFileExtensionAccepted() const
{
    return m_reader->readableFormats();
}

bool PB_StepGenericLoadFile::setAllSettings(const SettingsNodeGroup *settings)
{
    if(!CT_AbstractStepLoadFile::setAllSettings(settings))
        return false;

    QList<SettingsNodeGroup*> listG = settings->groupsByTagName(m_reader->GetReaderClassName());

    if(listG.isEmpty())
        return false;

    listG = listG.first()->groups();

    if(listG.isEmpty())
        return false;

    return m_reader->setAllSettings(listG.first());
}

SettingsNodeGroup *PB_StepGenericLoadFile::getAllSettings() const
{
    SettingsNodeGroup *root = CT_AbstractStepLoadFile::getAllSettings();

    SettingsNodeGroup *readerGroup = m_reader->getAllSettings();

    SettingsNodeGroup *group = new SettingsNodeGroup(m_reader->GetReaderClassName());

    if(readerGroup != NULL)
        group->addGroup(readerGroup);

    root->addGroup(group);

    return root;
}

bool PB_StepGenericLoadFile::setFilePath(QString filePath)
{
    QString fp = getFilePath();

    if(CT_AbstractStepLoadFile::setFilePath(filePath))
    {
        if(!m_reader->setFilePath(getFilePath()))
        {
            setFilePath(fp);
            setSettingsModified(false);

            return false;
        }

        return true;
    }

    return false;
}

CT_VirtualAbstractStep* PB_StepGenericLoadFile::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepGenericLoadFile(dataInit, m_reader->copy());
}

bool PB_StepGenericLoadFile::preConfigure()
{
    if(CT_AbstractStepLoadFile::preConfigure())
    {
        if(m_reader->configure()) {
            setSettingsModified(true);
            return true;
        }
    }

    return false;
}

bool PB_StepGenericLoadFile::postConfigure()
{
    if(CT_AbstractStepLoadFile::postConfigure())
    {
        if(m_reader->configure()) {
            setSettingsModified(true);
            return true;
        }
    }

    return false;
}

bool PB_StepGenericLoadFile::protectedInitAfterConfiguration()
{
    if(getStepChildList().isEmpty())
        m_reader->createOutItemDrawableModelList();

    return true;
}

void PB_StepGenericLoadFile::createInResultModelListProtected()
{
    // No in result is needed
    setNotNeedInputResult();
}

// Creation and affiliation of OUT models
void PB_StepGenericLoadFile::createOutResultModelListProtected()
{
    CT_OutStdGroupModel *root = new CT_OutStdGroupModel(DEF_SearchGroup);

    QListIterator<CT_OutStdSingularItemModel*> it(m_reader->outItemDrawableModels());

    while(it.hasNext())
    {
        CT_OutStdSingularItemModel *model = it.next();
        root->addItem((CT_OutStdSingularItemModel*)model->copy());
    }

    QListIterator<CT_OutStdGroupModel*> itG(m_reader->outGroupsModel());

    while(itG.hasNext())
    {
        CT_OutStdGroupModel *model = itG.next();
        root->addGroup((CT_OutStdGroupModel*)model->copy());
    }

    addOutResultModel(new CT_OutResultModelGroup(DEF_SearchResult, root, "Result"));
}

void PB_StepGenericLoadFile::compute()
{
    if(m_reader->readFile())
    {
        CT_ResultGroup *out_res = getOutResultList().first();
        CT_StandardItemGroup *group = new CT_StandardItemGroup(DEF_SearchGroup, out_res);

        QListIterator<CT_OutStdSingularItemModel*> it(m_reader->outItemDrawableModels());

        while(it.hasNext())
        {
            CT_OutStdSingularItemModel *model = it.next();
            CT_OutAbstractItemModel *modelCreation = (CT_OutAbstractItemModel*)PS_MODELS->searchModelForCreation(model->uniqueName(), out_res);

            QList<CT_AbstractSingularItemDrawable*> items = m_reader->takeItemDrawableOfModel(model->uniqueName(), out_res, modelCreation);
            QListIterator<CT_AbstractSingularItemDrawable*> itI(items);

            while(itI.hasNext())
                group->addItemDrawable(itI.next());
        }

        QListIterator<CT_OutStdGroupModel*> itG(m_reader->outGroupsModel());

        while(itG.hasNext())
        {
            CT_OutStdGroupModel *model = itG.next();
            CT_OutAbstractItemModel *modelCreation = (CT_OutAbstractItemModel*)PS_MODELS->searchModelForCreation(model->uniqueName(), out_res);

            QList<CT_AbstractItemGroup*> groups = m_reader->takeGroupOfModel(model->uniqueName(), out_res, modelCreation);
            QListIterator<CT_AbstractItemGroup*> itI(groups);

            while(itI.hasNext())
                group->addGroup(itI.next());
        }

        out_res->addGroup(group);
    }
}

void PB_StepGenericLoadFile::readerProgressChanged(int progress)
{
    setProgress(progress);
}

void PB_StepGenericLoadFile::readerFilePathModified()
{
    CT_AbstractStepLoadFile::setFilePath(m_reader->filepath());
}
