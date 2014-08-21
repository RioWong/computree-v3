#ifndef CT_CLOUDINDEXLESSMEMORYT_HPP
#define CT_CLOUDINDEXLESSMEMORYT_HPP

#include "ct_cloudindex/ct_cloudindexlessmemoryt.h"

template<typename T>
CT_CloudIndexLessMemoryT<T>::CT_CloudIndexLessMemoryT(const size_t &begin, const size_t &size) : CT_AbstractCloudIndexT<T>()
{
    internalClear();
    this->internalSetSortType(CT_AbstractCloudIndex::SortedInAscendingOrder);

    m_begin = begin;
    m_size = size;
}

template<typename T>
size_t CT_CloudIndexLessMemoryT<T>::size() const
{
    return m_size;
}

template<typename T>
size_t CT_CloudIndexLessMemoryT<T>::indexAt(const size_t &i) const
{
    return i+m_begin;
}

template<typename T>
const typename CT_AbstractCloudIndex::size_type& CT_CloudIndexLessMemoryT<T>::constIndexAt(const size_t &i) const
{
    m_temporary = i+m_begin;
    return m_temporary;
}

template<typename T>
size_t CT_CloudIndexLessMemoryT<T>::operator[](const size_t &i) const
{
    return i+m_begin;
}

template<typename T>
void CT_CloudIndexLessMemoryT<T>::indexAt(const size_t &i, size_t &index) const
{
    index = i+m_begin;
}

template<typename T>
size_t CT_CloudIndexLessMemoryT<T>::first() const
{
    return m_begin;
}

template<typename T>
size_t CT_CloudIndexLessMemoryT<T>::last() const
{
    return m_begin + (m_size-1);
}

template<typename T>
bool CT_CloudIndexLessMemoryT<T>::contains(const size_t &index) const
{
    if(size() == 0)
        return false;

    return ((index >= m_begin) && (index <= last()));
}

template<typename T>
size_t CT_CloudIndexLessMemoryT<T>::lowerBound(const size_t &value) const
{
    if((value >= m_begin) && (value <= last()))
        return value-m_begin;

    return -1;
}

template<typename T>
size_t CT_CloudIndexLessMemoryT<T>::upperBound(const size_t &value) const
{
    if((value >= m_begin) && (value <= last()))
        return (value-m_begin)+1;

    return -1;
}

template<typename T>
void CT_CloudIndexLessMemoryT<T>::shiftAll(const size_t &offset, const bool &negativeOffset)
{
   internalShiftAll(offset, negativeOffset);
}

template<typename T>
bool CT_CloudIndexLessMemoryT<T>::cut(const size_t &begin, const size_t &end)
{
    if((begin >= first())
            && (end <= last()))
    {
        m_size = m_size - ((end - begin)+1);

        return true;
    }

    return false;
}

template<typename T>
void CT_CloudIndexLessMemoryT<T>::clear()
{
    internalClear();
}

#ifdef USE_PCL
template<typename T>
boost::shared_ptr< std::vector<int> > CT_CloudIndexLessMemoryT<T>::getPCLIndices() const
{
    boost::shared_ptr< std::vector<int> > indices = boost::shared_ptr< std::vector<int> >(new std::vector<int>(m_size));

    size_t a = m_begin;

    for(size_t i=0; i<m_size; ++i)
    {
        (*indices.get())[i] = a;
        ++a;
    }

    return indices;
}
#endif

template<typename T>
CT_AbstractCloudIndex* CT_CloudIndexLessMemoryT<T>::copy() const
{
    return new CT_CloudIndexLessMemoryT<T>(m_begin, m_size);
}

template<typename T>
bool CT_CloudIndexLessMemoryT<T>::internalResize(const size_t &newSize)
{
    m_size = newSize;

    return true;
}

template<typename T>
void CT_CloudIndexLessMemoryT<T>::internalShiftAll(const size_t &offset, const bool &negativeOffset)
{
    if(negativeOffset)
        m_begin -= offset;
    else
        m_begin += offset;
}

template<typename T>
void CT_CloudIndexLessMemoryT<T>::internalClear()
{
    m_begin = 0;
    m_size = 0;
}

#endif // CT_CLOUDINDEXLESSMEMORYT_HPP
