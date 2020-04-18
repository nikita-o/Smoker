#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

#define Need_Sigaretes 100	//Число итераций (сколько сигарет сделают и выкурят)
int iteration = 0;
bool run = true;
//atomic<bool> run = true;

bool tabak = false;
bool paper = false;
bool matches = false;
int statistik_paper = 0;
int statistik_tabak = 0;
int statistik_matches = 0;
int iterT = 0;
int iterP = 0;
int iterM = 0;
std::mutex mtx;	// мьютекс стола, стол свободен или нет

int main()
{
	setlocale(LC_ALL, "");
	srand(time(0));

	thread smoker_tabak([]()
		{
			for (; run; iterT++)
			{
				lock_guard<mutex> lock(mtx);
				if (paper && matches)
				{
					matches = false;
					paper = false;
					cout << "ТАБАК  выкурил\n";
					statistik_tabak++;
				}
			}
		});

	thread smoker_paper([]()
		{
			for (; run; iterP++)
			{
				lock_guard<mutex> lock(mtx);
				if (tabak && matches)
				{
					matches = false;
					tabak = false;

					cout << "БУМАГА выкурил\n";
					statistik_paper++;
				}
			}
		});

	thread smoker_matches([]() 
		{
			for (; run; iterM++)
			{
				lock_guard<mutex> lock(mtx);
				if (paper && tabak)
				{
					paper = false;
					tabak = false;
					cout << "СПИЧКИ выкурил\n";
					statistik_matches++;
				}
			}
		});

	for (;;)
	{
		lock_guard<mutex> lock(mtx);
		if (statistik_tabak + statistik_paper + statistik_matches >= Need_Sigaretes)
			break;
		if (!paper && !tabak && !matches)
		{
			switch (rand() % 3)
			{
			case 0:
				matches = true;
				tabak = true;
				break;
			case 1:
				paper = true;
				tabak = true;
				break;
			case 2:
				matches = true;
				paper = true;
				break;
			}
		}
	}
	run = false;
	smoker_tabak.join();
	smoker_paper.join();
	smoker_matches.join();
	cout << endl << statistik_tabak << " - Табак - Итераций: " << iterT << endl;
	cout << endl << statistik_paper << " - Бумаги - Итераций: " << iterP << endl;
	cout << endl << statistik_matches << " - Спичек - Итераций: " << iterM << endl;
	cout << endl << Need_Sigaretes << " - Сигарет всего выкурено\n\n";
	system("pause");
	return 0;
}
