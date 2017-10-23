#pragma once
#include "stdafx.h"

class Layer;

class Neuron
{
public:
	Neuron(Layer* pPreviousLayer);

	void Activate(); //   x / (1 + abs(x))     tanh ?
	void Derive(); //  x * (1-x)              pas sûr
	void CopyAndMutateFrom(const Neuron& other);


	void Save(std::ostream& oStream, bool bBinary = true);
	void Load(std::istream& oStream, bool bBinary = true);

	void SetValue(double dValue) { m_dValue = dValue; }
	double GetValue() const { return m_dValue; }


	static double GetRandomWeight();

protected:
	Neuron(Neuron&){assert(false);}

	void Mutate();


	double m_dValue;
	double* m_pWeights; // synapses with previous layer
	Layer* m_pPreviousLayer;

};