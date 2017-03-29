#include "InputManager.h"
#include "Problem.h"

#include "base.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <cstring>

using namespace std;

InputManager::~InputManager()
{
	if (m_problem)
	{
		delete m_problem;
	}
}

void InputManager::printHelp() const
{
    cout << "Run program: ./bw.out -f FILE_NAME -a NATURAL_NUMBER -t NUMBER_OF_THREADS" << endl;
}

bool InputManager::parse(int argc, char* argv[])
{
	if (argc == ARGUMENT_END)
	{
		m_problem = new Problem();

        string fileName;
		if (strcmp("-f", argv[ARGUMENT_F_TAG]) == 0)
		{
			fileName = argv[ARGUMENT_FILE_NAME];
		}
		else
		{
            cerr << "Argument " << argv[ARGUMENT_F_TAG] << " unknown" << endl;
            printHelp();
			return false;
		}

		if (strcmp("-a", argv[ARGUMENT_A_TAG]) == 0)
		{
            m_problem->a = stoi(argv[ARGUMENT_A]);
		}
		else
		{
            cerr << "Argument " << argv[ARGUMENT_A_TAG] << " unknown" << endl;
            printHelp();
			return false;
		}

        if (strcmp("-t", argv[ARGUMENT_T_TAG]) == 0)
        {
            m_problem->threads = stoi(argv[ARGUMENT_T]);
        }
        else
        {
            cerr << "Argument " << argv[ARGUMENT_T_TAG] << " unknown" << endl;
            printHelp();
            return false;
        }

        ifstream in(fileName);
		if (in.is_open())
		{
            string data;
            getline(in, data);

            istringstream iss(data);
			iss >> m_problem->n;

            if (m_problem->a < 5 || m_problem->a > (m_problem->n / 2))
			{
                cerr << "a must be in range [5, n/2]" << endl;
				return false;
            }
			
            while (getline(in, data))
			{
                vector<bool> row;
				if (data.size() != m_problem->n)
				{
                    cerr << "Incorrect file: " << fileName << endl;
					return false;
				}

				for (unsigned int i = 0; i < m_problem->n; ++i)
				{
					row.push_back(data[i] != '0');
				}
				m_problem->graph.push_back(row);
			}

            cout << m_problem->graph;
			in.close();
			return true;
		}
		else
		{
            cerr << "Unable to open file: " + fileName << endl;
		}
	}
	else
	{
        cerr << "Number of arguments must be " << ARGUMENT_END - 2 << endl;
        printHelp();
	}
	return false;
}
