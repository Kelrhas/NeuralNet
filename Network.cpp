#include "Network.h"

#include "Layer.h"
#include "Neuron.h"

Network::Network()
: m_iNbLayer(0)
, m_pLayers(nullptr)
, m_dError(0.0)
{
	
}

void Network::Create(const std::vector<int>& vLayer)
{
	assert(m_pLayers == nullptr && m_iNbLayer == 0);
	
	m_iNbLayer = vLayer.size();

	if(Debug::s_bOutput) std::cout << "Creating Network with " << m_iNbLayer << " layers" << std::endl;

	m_pLayers = new Layer*[m_iNbLayer];
	for(int i=0; i < m_iNbLayer; ++i)
	{
		m_pLayers[i] = new Layer(vLayer[i], i == 0 ? nullptr : m_pLayers[i-1]);
	}
}

void Network::Feed(const double* pInputs)
{
	m_pLayers[0]->FeedInputs(pInputs);

	for(int i=0; i < m_iNbLayer; ++i)
	{
		m_pLayers[i]->FeedForward();
	}
}

double Network::ComputeError(double* wantedOutput)
{
	Layer* pOutputLayer = m_pLayers[m_iNbLayer-1];
	int iNbOutput = pOutputLayer->GetNbNeurons();
	Neuron** pOutputNeurons = pOutputLayer->GetNeurons();

	for(int i=0; i < iNbOutput; ++i)
	{
		m_dError += fabs(wantedOutput[i] - pOutputNeurons[i]->GetValue());
	}

	return m_dError;
}

void Network::CopyAndMutateFrom(const Network& other)
{
	if(this->m_iNbLayer != other.m_iNbLayer)
	{
		for(int i=0; i < this->m_iNbLayer; ++i)
		{
			delete this->m_pLayers[i];
		}
		delete[] this->m_pLayers;

		this->m_iNbLayer = 0;
		std::vector<int> vLayers;

		for(int i=0; i < other.m_iNbLayer; ++i)
		{
			vLayers.push_back(other.GetLayer(i)->GetNbNeurons());
		}

		this->Create(vLayers);
	}


	for(int iLayer=0; iLayer < this->m_iNbLayer; ++iLayer)
	{
		m_pLayers[iLayer]->CopyAndMutateFrom(*other.GetLayer(iLayer));
	}
}

void Network::Save(std::ostream& oStream, bool bBinary /*= true*/)
{
	if(bBinary)
	{
		oStream.write((char*)&bBinary, sizeof(bBinary));
		oStream.write((char*)&m_iNbLayer, sizeof(m_iNbLayer));
	}
	else
	{
		oStream << bBinary << std::endl << m_iNbLayer << std::endl;
	}

	for(int i=0; i< m_iNbLayer; ++i)
	{
		m_pLayers[i]->Save(oStream, bBinary);
		if(!bBinary && i < m_iNbLayer -1)
			oStream << std::endl;
	}
}

void Network::Load(std::istream& oStream)
{
	char cBinary;
	oStream.read(&cBinary, sizeof(cBinary));
	if(cBinary == 1) // every other value could be false or a character that would mean not binary
	{
		oStream.read((char*)&m_iNbLayer, sizeof(m_iNbLayer));
		m_pLayers = new Layer*[m_iNbLayer];
		unsigned int iNbNeuron = 0;
		for(int i=0; i < m_iNbLayer; ++i)
		{
			oStream.read((char*)&iNbNeuron, sizeof(iNbNeuron));
			m_pLayers[i] = new Layer(iNbNeuron, i > 0 ? m_pLayers[i-1]: nullptr);
			m_pLayers[i]->Load(oStream, true);
		}
	}
	else
	{
		oStream >> m_iNbLayer;
		m_pLayers = new Layer*[m_iNbLayer];
		int iNbNeuron = 0;
		for(int i=0; i < m_iNbLayer; ++m_iNbLayer)
		{
			oStream >> iNbNeuron;
			m_pLayers[i] = new Layer(iNbNeuron, i > 0 ? m_pLayers[i-1]: nullptr);
			m_pLayers[i]->Load(oStream, false);
		}
	}
}
