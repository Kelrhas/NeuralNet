#pragma once
#include "stdafx.h"

class Layer;

class Network
{
public:
	Network();
	
	void Create(const std::vector<int>& vLayers);
	void Feed(const double* pInputs);
	double ComputeError(double* wantedOutput);

	void CopyAndMutateFrom(const Network& other);

	void Save(std::ostream& oStream, bool bBinary = true);
	void Load(std::istream& oStream);
	
	bool operator<(const Network& other) const { return this->m_dError < other.m_dError; }

	double GetError() const { return m_dError; }
	void ResetError() { m_dError = 0; }
	const Layer* const GetLayer(int iLayer) const { assert(iLayer < m_iNbLayer); return m_pLayers[iLayer]; }
	
protected:
	int m_iNbLayer;
	Layer** m_pLayers;
	double m_dError;
};