// NeuralNet.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <thread>

#include "Network.h"
#include "Layer.h"
#include "Neuron.h"

using namespace std;



std::ofstream::_Openmode eSavingMode = ios::binary;// std::ofstream::binary; //std::ofstream::out


vector<Network*> networks;
int nbNetwork = 100;
int nbThread = 1;
std::thread* pThreads = NULL;

double inputs[4][2]={{0.0,0.0}, {0.0, 1.0}, {1.0, 0.0}, {1.0,1.0}};
double outputs[4][1]={{0.0}, {0.0}, {0.0}, {1.0}};

void RunThread(int iStartNetwork, int iCountNetwork)
{
	for(int iTest=0; iTest<4; ++iTest)
	{
		for( int i=0; i < iCountNetwork; ++i)
		{
			networks[i+iStartNetwork]->Feed(inputs[iTest]);
			networks[i+iStartNetwork]->ComputeError(outputs[iTest]);
		}
	}
}

void Run()
{
	int nbStep = 3;
	cout << "Nb steps ";
	cin >> nbStep;
	if(nbStep < 0)
	{
		nbStep = INT_MAX;
	}
	system("cls");

	int iStep=0, iLastStepChange=0;
	double dBestError = 1;



	time_t start, end;
	time(&start);
	for( ; iStep < nbStep; ++iStep)
	{
		int nbNetworkPerThread = nbNetwork / nbThread;
		for(int iThread=0; iThread < nbThread-1; ++iThread)
		{
			pThreads[iThread] = std::thread(RunThread, nbNetworkPerThread * iThread, nbNetworkPerThread);
		}
		int nbNetworkRemaining = nbNetwork - nbNetworkPerThread * (nbThread-1);
		RunThread(nbNetworkPerThread * (nbThread-1), nbNetworkRemaining);

		for(int iThread=0; iThread < nbThread-1; ++iThread)
		{
			pThreads[iThread].join();
		}


		std::sort(networks.begin(), networks.end(), [](const Network* lhs, const Network* rhs){return lhs->GetError() < rhs->GetError();});

		if(networks[0]->GetError() < dBestError)
		{
			dBestError = networks[0]->GetError();
			iLastStepChange = 0;
		}
		else
		{
			++iLastStepChange;
		}
		cout << "Best network at step " << iStep << " error: " << networks[0]->GetError() << " last change since " << iLastStepChange << " steps" << endl;


		for( int i=0; i < nbNetwork; ++i)
		{
			networks[i]->ResetError();
		}

		for( int i=1; i < nbNetwork ; ++i)
		{
			networks[i]->CopyAndMutateFrom(*networks[0]);
		}
	}

	time(&end);
	cout << difftime(end, start) << " seconds" << std::endl;
}


int _tmain(int argc, _TCHAR* argv[])
{
	if(Debug::s_uDebugSeed != 0) srand(Debug::s_uDebugSeed); 



	cout << "Number of thread available: " << std::thread::hardware_concurrency() << endl;
	cout << "Number of thread to use ? ";
	cin >> nbThread;

	nbThread = min((unsigned int)max(nbThread, 1), std::thread::hardware_concurrency());
	pThreads = new std::thread[nbThread];



	
	
	int iMode = 0;
	cout << "Create & Teach > 1" << endl
		 << "Load & Teach   > 2" << endl
		 << "Load & Test    > 3" << endl;
	cin >> iMode;

	switch( iMode )
	{
		case 2:
		{
			nbNetwork = 1;
			networks.push_back( new Network() );

			std::ifstream loadStream("test.nnet", eSavingMode);
			networks[0]->Load(loadStream);
			loadStream.close();
		}
		break;
		case 3:
		{
			nbNetwork = 1;
			networks.push_back( new Network() );

			std::ifstream loadStream("test.nnet", eSavingMode);
			networks[0]->Load(loadStream);
			loadStream.close();
		}
		break;

		default:  // default to new one
		{
			cout << "Nb network" << endl;
			cin >> nbNetwork;
			for(int i=0; i < nbNetwork; ++i)
				networks.push_back( new Network() );

			std::vector<int> layers;
			layers.push_back(2);
			layers.push_back(15);
			layers.push_back(15);
			layers.push_back(1);

			for( int i=0; i < nbNetwork; ++i)
			{
				networks[i]->Create(layers);
			}

			Run();
		}
	}
	

	if( iMode != 2)
	{
		const char* sFileName = "test.nnet";
		cout << "Saving to " << sFileName << "...";
		std::ofstream saveStream(sFileName, eSavingMode);
		bool bBinary = ((eSavingMode & std::ofstream::binary) != 0);
		networks[0]->Save(saveStream, bBinary);
		saveStream.close();
		cout << "done" << endl;
	}

	system("pause");

	return 0;
}

