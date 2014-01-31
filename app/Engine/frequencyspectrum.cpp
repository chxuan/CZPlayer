#include "stdafx.h"
#include "frequencyspectrum.h"

FrequencySpectrum::FrequencySpectrum(int numPoints) : m_elements(numPoints)
{
}

void FrequencySpectrum::reset()
{
    iterator i = begin();
    for ( ; i != end(); ++i)
	{
        *i = Element();
	}
}

int FrequencySpectrum::count() const
{
    return m_elements.count();
}

FrequencySpectrum::Element& FrequencySpectrum::operator[](int index)
{
    return m_elements[index];
}

const FrequencySpectrum::Element& FrequencySpectrum::operator[](int index) const
{
    return m_elements[index];
}

FrequencySpectrum::iterator FrequencySpectrum::begin()
{
    return m_elements.begin();
}

FrequencySpectrum::iterator FrequencySpectrum::end()
{
    return m_elements.end();
}

FrequencySpectrum::const_iterator FrequencySpectrum::begin() const
{
    return m_elements.begin();
}

FrequencySpectrum::const_iterator FrequencySpectrum::end() const
{
    return m_elements.end();
}
