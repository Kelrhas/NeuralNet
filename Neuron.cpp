#include "Neuron.h"

#include <cstdlib>
#include <math.h>

#include "Layer.h"


Neuron::Neuron(Layer* pPreviousLayer)
: m_dValue(0)
, m_pPreviousLayer(pPreviousLayer)
, m_pWeights(nullptr)
{
	if(pPreviousLayer != nullptr)
	{
		const int& nbPreviousNeurons = pPreviousLayer->GetNbNeurons();
		if(Debug::s_bOutput) std::cout << "Creating Neuron with " << nbPreviousNeurons << " previous neurons" << std::endl;
		m_pWeights = new double[nbPreviousNeurons];

		for(int i=0; i < nbPreviousNeurons; ++i)
		{
			m_pWeights[i] = GetRandomWeight();
		}
	}
}

void Neuron::Activate()
{
	if(m_pPreviousLayer != nullptr)
	{
		// get the value from the previous layer and the weight
		double dValue = 0;
		int nbPreviousNeurons = m_pPreviousLayer->GetNbNeurons();
		Neuron** pNeurons = m_pPreviousLayer->GetNeurons();

		for(int i=0; i <nbPreviousNeurons; ++i)
		{
			dValue += pNeurons[i]->GetValue() * m_pWeights[i];
		}
		this->m_dValue = dValue;
	}

	this->m_dValue = this->m_dValue / (1 + fabs(this->m_dValue));
}

void Neuron::Derive()
{
	this->m_dValue = this->m_dValue * (1 - this->m_dValue);
}

void Neuron::CopyAndMutateFrom(const Neuron& other)
{
	if(this->m_pPreviousLayer == nullptr)
		return;

	int iNbWeights = this->m_pPreviousLayer->GetNbNeurons();
	assert( iNbWeights == other.m_pPreviousLayer->GetNbNeurons() );

	for(int i=0; i < iNbWeights; ++i)
	{
		this->m_pWeights[i] = other.m_pWeights[i];
	}

	Mutate();
}

void Neuron::Save(std::ostream& oStream, bool bBinary /*= true*/)
{
	if(m_pPreviousLayer != nullptr)
	{
		if(!bBinary)
			oStream << std::endl;
		int iNbPreviousNeurons = m_pPreviousLayer->GetNbNeurons();
		for(int i=0; i < iNbPreviousNeurons; ++i)
		{
			if(bBinary)
			{
				oStream.write((char*)&m_pWeights[i], sizeof(m_pWeights[i]));
			}
			else
			{
				oStream << m_pWeights[i];
				if(i < m_pPreviousLayer->GetNbNeurons() -1)
					oStream << " ";
			}
		}
	}
}

void Neuron::Load(std::istream& oStream, bool bBinary /*= true*/)
{
	if(m_pPreviousLayer != nullptr)
	{
		int iNbPreviousNeurons = m_pPreviousLayer->GetNbNeurons();
		for(int i=0; i < iNbPreviousNeurons; ++i)
		{
			if(bBinary)
			{
				oStream.read((char*)&m_pWeights[i], sizeof(m_pWeights[i]));
			}
			else
			{
				oStream >> m_pWeights[i];
			}
		}
	}
}

void Neuron::Mutate()
{
	int nbPreviousNeurons = m_pPreviousLayer->GetNbNeurons();

	for(int i=0; i <nbPreviousNeurons; ++i)
	{
		double dRand = (rand() / ((double)RAND_MAX) / 10);
		if( dRand < 0.1 ) // 10% chance mutation
		{
			if( dRand < 0.03 )
			{
				m_pWeights[i] *= (rand() / ((double)RAND_MAX)); // decrease
			}
			else if( dRand < 0.06 )
			{
				m_pWeights[i] *= 1 + (rand() / ((double)RAND_MAX)); // increase
			}
			else
			{
				m_pWeights[i] *= -1; // invert
			}
		}
	}
}

double Neuron::GetRandomWeight()
{
	return (rand() / ((double)RAND_MAX)) *2 -1; // between -1 and 1
}
