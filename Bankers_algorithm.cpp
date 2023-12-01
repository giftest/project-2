#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
using namespace std;

 // Global variables

int numProc;    // # of processes 
int numResType; // # of resource types 
std::vector<int> E_exist; // # of resources of each type in existence
std::vector<std::vector<int>> C_alloc; // the current allocation matrix C
std::vector<std::vector<int>> R_request_need; // the request matrix R
std::vector<int> A_work;  // the available resources matrix A
std::vector<bool> finish; // the finish vector
std::queue<int> sequence; // safe sequence
bool init; // set to true if constructor reads the input file, false otherwise

void DeadlockDetector(string filename)
{
    // try opening the file
    ifstream infile(filename);
    if (!infile.is_open()) {
        cout << "Cannot open file\n";
        init = false;
    }

    string line;
    int num;

    // read line 1
    getline(infile, line);
    istringstream ss(line);
    ss >> numProc >> numResType;

    // read line 2
    getline(infile, line);
    ss.str(line); // give line to ss
    ss.seekg(0);  // set ss to begining of line
    for (int i = 0; i < numResType; i++)
    {
        ss >> num;
        E_exist.push_back(num);
    }

    // read allocation matrix C
    for (int i = 0; i < numProc; i++)
    {
        getline(infile, line);
        ss.str(line);
        ss.seekg(0);
        vector<int> procAlloc;

        for (int j = 0; j < numResType; j++)
        {
            ss >> num;
            procAlloc.push_back(num);
        }

        C_alloc.push_back(procAlloc);
    }

    // read request matrix R
    for (int i = 0; i < numProc; i++)
    {
        getline(infile, line);
        ss.str(line);
        ss.seekg(0);
        vector<int> procReq;

        for (int j = 0; j < numResType; j++)
        {
            ss >> num;
            procReq.push_back(num);
        }

        R_request_need.push_back(procReq);
    }

    // initialise remaining members
    // available vector
    for (int i = 0; i < numResType; i++)
    {
        A_work.push_back(0);
    }

    // finish vector
    for (int i = 0; i < numProc; i++)
    {
        finish.push_back(false);
    }

    // init
    init = true;

    infile.close();
}

/*
 print the contents of the vector v
*/
void printVector(std::vector<int> v)
 {
     for (int i = 0; i < v.size(); i++)
     {
         cout << v[i] << ' ';
     }
     cout << '\n';
}

/*
 * (re)calculate the available resources matrix A
 * adds freed resources to available
 */
void updateAvailableResources()
{
    // store column-wise sum of C_alloc
    vector<int> colSum;

    for (int col = 0; col < numResType; col++)
    {
        int sum = 0;
        for (int row = 0; row < numProc; row++)
        {
            sum += C_alloc[row][col];
        }
        colSum.push_back(sum);
    }

    // update currently available resources
    for (int i = 0; i < numResType; i++)
    {
        A_work[i] = E_exist[i] - colSum[i];
    }
}


/*
 * determine whether all processes are finished
 */
bool isDone()
{
    for (int i = 0; i < numProc; i++)
    {
        if (!finish[i]) {
            return false;
        }
    }

    return true;
}

/*
 * determine whether enough resources are avaliable
 * for process #idx to execute
 */
bool enoughResourcesAvailable(int idx)
{
    vector<int> req_need = R_request_need[idx];

    for (int i = 0; i < numResType; i++)
    {
        if (req_need[i] > A_work[i]) {
            return false;
        }
    }

    return true;
}

/*
 * print the contents of the matrix v
 */
void printMatrix(vector<vector<int>> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        vector<int> row = v[i];

        for (int j = 0; j < row.size(); j++)
        {
            cout << row[j] << ' ';
        }
        cout << '\n';
    }
}

/*
 * print the safe sequence
 */
void printSafeSequence()
{
    while (!sequence.empty()) {
        cout <<"P" << sequence.front() << "-> ";
        sequence.pop();
    }
    cout << endl;
}



/*
 * run the simulation
 */
void run()
{
    if (!init) {
        return;
    }

    cout << "Initial Values:\n";
    cout<<"================\n";

    cout << "1. Number of processes: " << numProc << '\n';
    cout << "2. Number of resource types: " << numResType << '\n';

    cout << "3. Existing Resource Vector\n\n";
    printVector(E_exist);

    cout << "\nCurrent Allocation Matrix\n";
    cout<<"===========================\n\n";
    printMatrix(C_alloc);

    cout << "Request Matrix\n";
    cout <<"=============\n\n";
    printMatrix(R_request_need);

    // (re)calculate available matrix A
    updateAvailableResources();

    cout << "Available Resource Vector\n";
    cout <<"=======================\n\n";
    printVector(A_work);

    int idx = 0;      // the current process number
    int idxCount = 0; // # of processes that can't execute

    // loop until all processes are finished
    while (!isDone()) {
        // Find a process that can execute
        bool procFoundFlag = false;
        if (!finish[idx] && enoughResourcesAvailable(idx)) {
            procFoundFlag = true;
            idxCount = 0;
            // simulate execution of the process
            for (int i = 0; i < numResType; i++)
            {
                // allocated resources are freed
                C_alloc[idx][i] = 0;
            }

            // add freed resources to available
            updateAvailableResources();

            // mark process
            finish[idx] = true;
            cout << "\nProcess " << idx << " marked\n";
            sequence.push(idx);

            cout << "Available Resource Vector\n";
            cout <<"=======================\n\n";
            printVector(A_work);
        }

        idx = (idx + 1) % numProc; // so idx loops back to 0

        // if current process can't be executed, increment the count
        if (!procFoundFlag) {
            idxCount++;
        }

        // if the count of processes that can't execute reaches total processes,
        // we have detected a deadlock
        if (idxCount == numProc) {
            break;
        }
    }

    if (!isDone()) {
        // system is in deadlock
        cout << "\nSystem is deadlocked\n";
        cout << "Deadlocked processes: ";
        for (int i = 0; i < numProc; i++)
        {
            if (!finish[i]) {
                cout << i << ' ';
            }
        }
        cout << '\n';
    } else {
        // system is not in deadlock
        cout << "\nSystem is in safe state\n";
        cout << "The safe sequence is:\n";
        printSafeSequence();
    }
}

// Main class 

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        cout << "Invalid number of arguments!\n";
        cout << "Usage: " << argv[0] << " <input_file>\n";
        return -1;
    }

    DeadlockDetector(argv[1]);
    run();

    return 0;
}
