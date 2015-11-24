#include "dm_attributesnormalt.h"

#include "ct_global/ct_context.h"
#include "ct_iterator/ct_pointiterator.h"

template<>
bool DM_AttributesNormalT<CT_AbstractPointsAttributes>::process(GDocumentViewForGraphics *doc)
{
    if(m_an == NULL)
        return false;

    CT_PointIterator it(abstractTypeAttributes()->abstractCloudIndex());

    size_t size = it.size();

    osg::ref_ptr<GOsgGraphicsView::NormalArrayType> normalArray = doc->getOrCreateGlobalNormalArrayForPoints();

    if(normalArray.valid())
    {
        size_t i = 0;
        while(it.hasNext() && !isCanceled())
        {
            size_t indexP = it.next().cIndex();

            const CT_Normal &nxnynz_pa = m_an->constNormalAt(i);

            // set the normal of the point at this document
            (*normalArray)[indexP] = nxnynz_pa;

            setProgress((i*100)/size);
            ++i;
        }

        doc->dirtyNormalsOfPoints();

        return true;
    }

    return false;
}

template<>
bool DM_AttributesNormalT<CT_AbstractFaceAttributes>::process(GDocumentViewForGraphics *doc)
{
    if(m_an == NULL)
        return false;

    const CT_AbstractCloudIndex *index = abstractTypeAttributes()->abstractCloudIndex();

    size_t size = index->size();
    size_t indexF;

    // TODO
    /*
    QSharedPointer<CT_StandardNormalCloudRegistered> spn = doc->normalCloudRegistered<CT_AbstractFaceAttributes>();

    if(spn.data() != NULL)
    {
        CT_AbstractNormalCloud *nn = spn->abstractNormalCloud();

        for(size_t i=0; i<size && !isCanceled(); ++i)
        {
            index->indexAt(i, indexF);
            const CT_Normal &nxnynz_pa = m_an->constNormalAt(i);

            // set the normal of the face at this document
            CT_Normal &nxnynz = nn->normalAt(indexF);
            nxnynz.setNormal(nxnynz_pa);

            setProgress((i*100)/size);
        }

        doc->dirtyNormalsOfPoints();

        return true;
    }
*/
    return false;
}

template class DM_AttributesNormalT<CT_AbstractPointsAttributes>;
template class DM_AttributesNormalT<CT_AbstractFaceAttributes>;
