//============================================================================
// Name        : RPS.cpp
// Author      : anirudh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include<iostream>
#include<cstdlib>
#include<ctime>
#include<string>
#include<iomanip>

using namespace std;

const int MAX_ROUNDS = 20;

int main()
{
	char humanInput;
	int count = 1;
	int human;
	int comp;
	int hwc = 0, cwc = 0, tie = 0;
	do
	{
		human = -1;
		cout << "Round " << count << endl;


		do
		{
			cout << "  Your choice? ";
			cin >> humanInput;
			if(humanInput=='R'||humanInput== 'r')
					{

						human = 0;
					}
					else if(humanInput=='P'||humanInput=='p')
					{

						human = 1;
					}
					else if(humanInput=='S'||humanInput=='s')
					{

						human = 2;
					}
					else
					{
						cout << "  *** ERROR: Valid choices are R, P, or S" << endl;
					}

		} while(!(human == 0 || human == 1 || human == 2));



	    srand (time(0));

	    comp = rand() % 3;


	    if (human == 0 || human == 1 || human == 2)
	    {

				if (comp == 0)
				{
					if (human == 1)
					{
						cout << "  You chose PAPER" << endl;
						cout << "  The computer chose ROCK" << endl;
						cout << "  The winner is you" << endl;
						hwc += 1;
					}
					else if (human == 2)
					{
						cout << "  You chose SCISSORS" << endl;
						cout << "  The computer chose ROCK" << endl;
						cout << "  The winner is the computer" << endl;
						cwc += 1;
					}
					else
					{
						cout << "  You chose ROCK" << endl;
						cout << "  The computer chose ROCK" << endl;
						cout << "  Its a tie." << endl;
						tie += 1;
					}

				}
				if (comp == 1)
				{
					if (human == 0)
					{
						cout << "  You chose ROCK" << endl;
						cout << "  The computer chose PAPER" << endl;
						cout << "  The winner is the computer" << endl;
						cwc += 1;
					}
					else if (human == 2)
					{
						cout << "  You chose SCISSORS" << endl;
						cout << "  The computer chose PAPER" << endl;
						cout << "  The winner is you" << endl;
						hwc += 1;
					}
					else
					{
						cout << "  You chose PAPER" << endl;
						cout << "  The computer chose PAPER" << endl;
						cout << "  Its a tie." << endl;
						tie += 1;
					}
				}
				if (comp == 2)
				{
					if (human == 0)
					{
						cout << "  You chose ROCK" << endl;
						cout << "  The computer chose SCISSORS" << endl;
						cout << "  The winner is you" << endl;
						hwc += 1;
					}
					else if (human == 1)
					{
						cout << "  You chose ROCK" << endl;
						cout << "  The computer chose SCISSORS" << endl;
						cout << "  The winner is the computer" << endl;
						cwc += 1;
					}
					else
					{
						cout << "  You chose SCISSORS" << endl;
						cout << "  The computer chose SCISSORS" << endl;
						cout << "  Its a tie" << endl;
						tie += 1;
				}
			}
	     }
	    else
	    {
	    	cout << "  *** ERROR: Valid choices are R, P, or S";
	    	count--;
	    }
	    cout << "\n";

	    count++;

		} while (count <= MAX_ROUNDS);

    cout << "Summary" << endl;
    cout << "-------" << endl;
	cout << setw(15) << right << "Human wins:" << hwc << endl;
	cout << setw(15) << right << "Computer wins: " << cwc << endl;
	cout << setw(15) << right << "Ties: " << tie << endl;


	return 0;
}
