#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

#define Need_Sigaretes 10	// Number of iterations (how many cigarettes)
int iteration = 0;
bool run = true;

// Ingredients:
bool tabak = false;		
bool paper = false;
bool matches = false;
// Statistik ingredients:
int statistik_paper = 0;
int statistik_tabak = 0;
int statistik_matches = 0;
// number of interations smokers:
int iterT = 0;
int iterP = 0;
int iterM = 0;
mutex mtxT, mtxP, mtxM;	

int main()
{
	setlocale(LC_ALL, "");
	srand(time(0));

	thread smoker_tabak([]()
		{
			for (; run; iterT++)
			{
				lock_guard<mutex> lockP(mtxP);
				lock_guard<mutex> lockM(mtxM);
				if (paper && matches)
				{
					paper = false;
					matches = false;
					cout << "TABAK   smoke\n";
					statistik_tabak++;
				}
			}
		});
	thread smoker_paper([]()
		{
			for (; run; iterP++)
			{
				lock_guard<mutex> lockT(mtxT);
				lock_guard<mutex> lockM(mtxM);
				if (tabak && matches)
				{
					tabak = false;
					matches = false;
					cout << "PAPER   smoke\n";
					statistik_paper++;
				}
			}
		});
	thread smoker_matches([]()
		{
			for (; run; iterM++)
			{
				lock_guard<mutex> lockT(mtxT);
				lock_guard<mutex> lockP(mtxP);
				if (paper && tabak)
				{
					tabak = false;
					paper = false;
					cout << "MATCHEs smoke\n";
					statistik_matches++;
				}
			}
		});

	for (; run;)
	{
		switch (rand() % 3)
		{
		case 0:
		{
			lock_guard<mutex> lockT(mtxT);
			lock_guard<mutex> lockM(mtxM);
			if (statistik_tabak + statistik_paper + statistik_matches >= Need_Sigaretes)
			{
				run = false;
				break;
			}
			tabak = true;
			matches = true;
		}
		break;
		case 1:
		{
			lock_guard<mutex> lockT(mtxT);
			lock_guard<mutex> lockP(mtxP);
			if (statistik_tabak + statistik_paper + statistik_matches >= Need_Sigaretes)
			{
				run = false;
				break;
			}
			tabak = true;
			paper = true;
		}
		break;
		case 2:
		{
			lock_guard<mutex> lockP(mtxP);
			lock_guard<mutex> lockM(mtxM);
			if (statistik_tabak + statistik_paper + statistik_matches >= Need_Sigaretes)
			{
				run = false;
				break;
			}
			paper = true;
			matches = true;
		}
		break;
		}
	}
	//stop = false;
	smoker_tabak.join();
	smoker_paper.join();
	smoker_matches.join();
	cout << endl << statistik_tabak << " - tabak - iterations: " << iterT << endl;
	cout << endl << statistik_paper << " - paper - iterations: " << iterP << endl;
	cout << endl << statistik_matches << " - matches - iterations: " << iterM << endl;
	cout << endl << Need_Sigaretes << " - Cigarettes smoked\n\n";
	system("pause");
	return 0;
}
