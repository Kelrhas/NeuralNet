#pragma once
#include "stdafx.h"

class Neuron;

class Layer
{
public:
	Layer(int nbNeuron, Layer* pPreviousLayer);

	void FeedForward();
	void FeedInputs(const double* pInputs);

	void CopyAndMutateFrom(const Layer& other);

	void Save(std::ostream& oStream, bool bBinary = true);
	void Load(std::istream& oStream, bool bBinary = true);
	

	const int& GetNbNeurons() const { return m_iNbNeurons; }
	Neuron** GetNeurons() const { return m_pNeurons; }


protected:
	int m_iNbNeurons;
	Neuron** m_pNeurons;
};