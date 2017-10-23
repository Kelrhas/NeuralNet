#include "Layer.h"

#include "Neuron.h"

Layer::Layer(int nbNeuron, Layer* pPreviousLayer)
: m_iNbNeurons(nbNeuron)
{
	if(Debug::s_bOutput) std::cout << "Creating Layer with " << nbNeuron << std::endl;
	m_pNeurons = new Neuron*[nbNeuron];
	
	for(int i=0; i < nbNeuron; ++i)
	{
		m_pNeurons[i] = new Neuron(pPreviousLayer);
	}
}

void Layer::FeedForward()
{
	for(int i=0; i < m_iNbNeurons; ++i)
	{
		m_pNeurons[i]->Activate();
	}
}

void Layer::FeedInputs(const double* pInputs)
{
	assert(pInputs != nullptr);

	for(int i=0; i < m_iNbNeurons; ++i)
	{
		m_pNeurons[i]->SetValue(pInputs[i]);
	}
}

void Layer::CopyAndMutateFrom(const Layer& other)
{
	assert( this->m_iNbNeurons == other.m_iNbNeurons );

	for(int i=0; i < m_iNbNeurons; ++i)
	{
		m_pNeurons[i]->CopyAndMutateFrom(*other.m_pNeurons[i]);
	}

}

void Layer::Save(std::ostream& oStream, bool bBinary /*= true*/)
{
	if(bBinary)
		oStream.write((char*)&m_iNbNeurons, sizeof(m_iNbNeurons));
	else
		oStream << m_iNbNeurons;

	for(int i=0; i< m_iNbNeurons; ++i)
	{
		m_pNeurons[i]->Save(oStream, bBinary);
	}
}

void Layer::Load(std::istream& oStream, bool bBinary /*= true*/)
{
	assert( m_pNeurons != nullptr && m_iNbNeurons > 0 );
	for(int i=0; i <m_iNbNeurons; ++i)
	{
		m_pNeurons[i]->Load(oStream, bBinary);
	}
}
