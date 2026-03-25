#include <iostream>
#include <iomanip>
#include <string>
#include <pthread.h>
#include <semaphore.h>
#include <unordered_map>
#include <list>
#include <cctype>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <fstream>

using namespace std;

const int DISK_SIZE = 100;
const int MAX_ACCOUNTS = 100;
const int MAX_FILES = 100;
const int MAX_BLOCKS_PER_FILE = 20;
const int MAX_IO_REQUESTS = 50;
const int PAGE_SIZE = 256;      
const int NUM_PAGES = 10;      
const int MAX_TRANSACTIONS = 100;   

struct MsgBuffer 
{
    long msg_type;
    char msg_text[100];
} message;

class MessageQueue 
{
    key_t key;
    int msgid;

public:
    MessageQueue(const char* file, int id) 
    {
        key = ftok(file, id);
        if (key == -1) 
        {
            perror("ftok failed");
            msgid = -1;
            return;
        }
        msgid = msgget(key, 0666 | IPC_CREAT);
        if (msgid == -1) 
        {
            perror("msgget failed");
        }
    }

    ~MessageQueue() 
    {
        if (msgid != -1) 
        {
            msgctl(msgid, IPC_RMID, NULL);
        }
    }

    void sendMessage(const char* text) 
    {
        if (msgid == -1) return;
        message.msg_type = 1;
        strncpy(message.msg_text, text, sizeof(message.msg_text) - 1);
        message.msg_text[sizeof(message.msg_text) - 1] = '\0';
        if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) 
        {
            perror("msgsnd failed");
        }
    }

    void receiveMessage() 
    {
        if (msgid == -1) return;
        if (msgrcv(msgid, &message, sizeof(message.msg_text), 1, 0) == -1) 
        {
            perror("msgrcv failed");
            return;
        }
        cout << "Received message: " << message.msg_text << endl;
    }
};

class DiskManager 
{
    string simulatedDisk[DISK_SIZE];
    bool blockStatus[DISK_SIZE];
    struct FATEntry 
    {
        string filename;
        int startBlock;
        int size;
        int allocatedBlocks[MAX_BLOCKS_PER_FILE];
        int blockCount;
    };
    FATEntry FAT[MAX_FILES];
    int FATCount;
    string ioRequests[MAX_IO_REQUESTS];
    int ioRequestCount;

public:
    DiskManager() : FATCount(0), ioRequestCount(0) 
    {
        for (int i = 0; i < DISK_SIZE; i++) 
        {
            simulatedDisk[i] = "";
            blockStatus[i] = false;
        }
    }

    int allocateBlocks(int size, int allocatedBlocks[]);
    void deallocateBlocks(int allocatedBlocks[], int count);
    void updateFAT(string filename, int size);
    void displayFATTable();
    void addIORequest(const string &request);
    void processIORequests();
};

int DiskManager::allocateBlocks(int size, int allocatedBlocks[]) 
{
    int count = 0;
    for (int i = 0; i < DISK_SIZE && count < size; i++) 
    {
        if (!blockStatus[i]) 
        {
            allocatedBlocks[count++] = i;
            blockStatus[i] = true;
        }
    }
    if (count < size) 
    {
        for (int i = 0; i < count; i++) 
        {
            blockStatus[allocatedBlocks[i]] = false;
        }
        count = 0;
    }
    return count;
}

void DiskManager::deallocateBlocks(int allocatedBlocks[], int count) 
{
    for (int i = 0; i < count; i++) 
    {
        blockStatus[allocatedBlocks[i]] = false;
        simulatedDisk[allocatedBlocks[i]] = "";
    }
}

void DiskManager::updateFAT(string filename, int size) 
{
    if (FATCount < MAX_FILES) 
    {
        int allocatedBlocks[MAX_BLOCKS_PER_FILE];
        int blockCount = allocateBlocks(size, allocatedBlocks);
        if (blockCount > 0) 
        {
            FAT[FATCount] = {filename, allocatedBlocks[0], size, {}, blockCount};
            for (int i = 0; i < blockCount; i++) 
            {
                FAT[FATCount].allocatedBlocks[i] = allocatedBlocks[i];
                simulatedDisk[allocatedBlocks[i]] = filename;
            }
            FATCount++;
            cout << "\nFile '" << filename << "' stored successfully.\n";
        } else {
            cout << "\nError: Not enough disk space available.\n";
        }
    } else {
        cout << "\nError: FAT table is full.\n";
    }
}

void DiskManager::displayFATTable() 
{
    cout << "\nFILE ALLOCATION TABLE (FAT):\n";
    cout << "==========================================================\n";
    cout << setw(15) << "Filename" << setw(15) << "Start Block" << setw(10) << "Size" << setw(25) << "Allocated Blocks\n";
    cout << "==========================================================\n";
    if (FATCount == 0) 
    {
        cout << "No files are currently allocated in the FAT table.\n";
    } else {
        for (int i = 0; i < FATCount; i++) 
        {
            cout << setw(15) << FAT[i].filename 
                 << setw(15) << FAT[i].startBlock 
                 << setw(10) << FAT[i].size << " ";
            for (int j = 0; j < FAT[i].blockCount; j++) 
            {
                cout << FAT[i].allocatedBlocks[j] << " ";
            }
            cout << endl;
        }
    }
    cout << "==========================================================\n";
}

void DiskManager::addIORequest(const string &request) 
{
    if (ioRequestCount < MAX_IO_REQUESTS) 
    {
        ioRequests[ioRequestCount++] = request;
    } else {
        cout << "\nError: I/O request queue is full.\n";
    }
}

void DiskManager::processIORequests() 
{
    cout << "\nProcessing I/O Requests...\n";
    for (int i = 0; i < ioRequestCount; i++) 
    {
        cout << "\nProcessing: " << ioRequests[i];
        updateFAT("transaction.log", 1);
    }
    ioRequestCount = 0;
    cout << "\nAll I/O Requests Processed.\n";
}

class Account 
{
    int acno;
    char name[50];
    int deposit;
    char type;

public:
    bool create_account();
    void show_account() const;
    void dep(int);
    void draw(int);
    void report() const;
    int retacno() const;
    int retdeposit() const;
    char rettype() const;
    const char* getName() const { return name; }
};

Account accounts[MAX_ACCOUNTS];
int accountCount = 0;

sem_t semaphore;
pthread_mutex_t mutex;

struct TransactionProcess 
{
    int transactionID;
    int accountNumber;
    char type;  
    int amount;
    string status;  
};

TransactionProcess processTable[MAX_TRANSACTIONS];
int processCount = 0;

struct Process 
{
    int id;           
    int burstTime;      
    int remainingTime;   
    int waitingTime;     
    int turnaroundTime;  
};

Process processes[MAX_TRANSACTIONS];

class Page 
{
    int id;   
    char data[PAGE_SIZE]; 

public:
    Page() : id(-1) {}   
    int getPageID() const { return id; }
    void setPageID(int pageID) { id = pageID; }
};

class MemoryManager 
{
private:
    Page pages[NUM_PAGES];
    unordered_map<int, list<int>::iterator> pageTable;  
    list<int> lruList;                                  

public:
    void loadPage(int pageID) 
    {
        if (pageTable.find(pageID) != pageTable.end()) 
        {
            lruList.erase(pageTable[pageID]);
            lruList.push_front(pageID);
            pageTable[pageID] = lruList.begin();
        } else {
            if (lruList.size() == NUM_PAGES) 
            {
                int lruPage = lruList.back();
                lruList.pop_back();
                pageTable.erase(lruPage);
                for (int i = 0; i < NUM_PAGES; ++i) 
                {
                    if (pages[i].getPageID() == lruPage) 
                    {
                        pages[i].setPageID(-1);
                        break;
                    }
                }
            }
            for (int i = 0; i < NUM_PAGES; ++i) 
            {        
                if (pages[i].getPageID() == -1) 
                {              
                    pages[i].setPageID(pageID);                
                    lruList.push_front(pageID);                
                    pageTable[pageID] = lruList.begin();       
                    cout << "Loaded page " << pageID << " into memory.\n";  
                    break;                                    
                }
            }
        }
    }

    void displayMemoryMap(const Account accounts[], int accountCount) const 
    {
        cout << "\nMemory Map:\n";
        cout << "=========================================================\n";
        cout << "Page Number   Account Number   Account Holder Name   Account Type   Balance\n";
        cout << "=========================================================\n";

        for (int i = 0; i < NUM_PAGES; ++i) 
        {
            if (pages[i].getPageID() != -1) 
            {
                int pageID = pages[i].getPageID();
                cout << setw(12) << i << setw(17) << pageID;

                bool found = false;
                for (int j = 0; j < accountCount; j++) 
                {
                    if (accounts[j].retacno() == pageID) 
                    {
                        found = true;
                        cout << setw(25) << accounts[j].getName()
                             << setw(15) << accounts[j].rettype()
                             << setw(10) << accounts[j].retdeposit();
                        break;
                    }
                }

                if (!found) 
                {
                    cout << setw(25) << "No Account"
                         << setw(15) << "N/A"
                         << setw(10) << "N/A";
                }

                cout << "\n";
            } else {
                cout << setw(12) << i << setw(17) << "Empty" << "\n";
            }
        }
        cout << "=========================================================\n";
    }
};

MemoryManager memoryManager;
DiskManager diskManager;

int findAccountIndexByNumber(int accountNumber)
{
    for (int i = 0; i < accountCount; i++)
    {
        if (accounts[i].retacno() == accountNumber)
        {
            return i;
        }
    }
    return -1;
}

void initSyncTools() 
{
    sem_init(&semaphore, 0, 1);
    pthread_mutex_init(&mutex, NULL);
}

void destroySyncTools() 
{
    sem_destroy(&semaphore);
    pthread_mutex_destroy(&mutex);
}

bool Account::create_account() 
{
    sem_wait(&semaphore);
    cout << "\nEnter the Account No.: ";
    cin >> acno;
    cout << "\nPlease Enter the Name of the Account holder: ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "\nEnter Type of the Account (C/S): ";
    cin >> type;
    type = toupper(type);
    cout << "\nEnter The Initial amount: ";
    cin >> deposit;

    if (findAccountIndexByNumber(acno) != -1)
    {
        cout << "\nAccount number already exists. Please use a unique account number.\n";
        sem_post(&semaphore);
        return false;
    }

    if (acno > 0 && (type == 'C' || type == 'S') && deposit >= 0) 
    {
        cout << "\nAccount Created.\n";
        memoryManager.loadPage(acno);
        diskManager.updateFAT(to_string(acno), 1);  
        sem_post(&semaphore);
        return true;
    } else {
        cout << "\nInvalid account details. Please try again.\n";
        sem_post(&semaphore);
        return false;
    }
}

void Account::show_account() const 
{
    pthread_mutex_lock(&mutex);
    cout << "\nAccount No.: " << acno;
    cout << "\nAccount Holder Name: " << name;
    cout << "\nType of Account: " << type;
    cout << "\nBalance amount: " << deposit << "\n";
    pthread_mutex_unlock(&mutex);
}

void Account::dep(int x) 
{
    pthread_mutex_lock(&mutex);
    deposit += x;
    pthread_mutex_unlock(&mutex);
}

void Account::draw(int x) 
{
    pthread_mutex_lock(&mutex);
    deposit -= x;
    pthread_mutex_unlock(&mutex);
}

void Account::report() const 
{
    pthread_mutex_lock(&mutex);
    cout << setw(10) << left << acno << setw(20) << left << name << setw(10) << left << type << setw(10) << left << deposit << endl;
    pthread_mutex_unlock(&mutex);
}

int Account::retacno() const 
{
    return acno;
}

int Account::retdeposit() const 
{
    return deposit;
}

char Account::rettype() const 
{
    return type;
}

void write_account() 
{
    if (accountCount < MAX_ACCOUNTS) 
    {
        if (accounts[accountCount].create_account())
        {
            accountCount++;
        }
    } else {
        cout << "\nAccount list is full. Cannot create new account.";
    }
}

void display_sp(int n) 
{
    bool found = false;
    for (int i = 0; i < accountCount; i++) 
    {
        if (accounts[i].retacno() == n) 
        {
            accounts[i].show_account();
            found = true;
            break;
        }
    }
    if (!found) 
    {
        cout << "\nAccount number does not exist.";
    }
}

void display_all() 
{
    cout << "\nACCOUNT HOLDER LIST\n\n";
    cout << "====================================================\n";
    cout << "A/c No.    NAME                Type     Balance\n";
    cout << "====================================================\n";
    for (int i = 0; i < accountCount; i++) 
    {
        accounts[i].report();
    }
}

void* processTransaction(void* arg) 
{
    TransactionProcess* tp = (TransactionProcess*)arg;

    sem_wait(&semaphore);
    int idx = findAccountIndexByNumber(tp->accountNumber);
    if (idx == -1)
    {
        tp->status = "Failed";
        sem_post(&semaphore);
        return NULL;
    }

    if (tp->type == 'D') 
    {
        accounts[idx].dep(tp->amount);
    } else if (tp->type == 'W') 
    {
        if (accounts[idx].retdeposit() >= tp->amount) 
        {
            accounts[idx].draw(tp->amount);
        } else {
            tp->status = "Failed";
            sem_post(&semaphore);
            return NULL;
        }
    }
    tp->status = "Completed";
    sem_post(&semaphore);
    return NULL;
}

void createTransaction(int accountNumber, char type, int amount, int burstTime) 
{
    if (findAccountIndexByNumber(accountNumber) == -1)
    {
        cout << "Account Number " << accountNumber << " not found. Transaction rejected.\n";
        return;
    }

    if (processCount < MAX_TRANSACTIONS) 
    {
        TransactionProcess &tp = processTable[processCount++];
        tp.transactionID = processCount;
        tp.accountNumber = accountNumber;
        tp.type = type;
        tp.amount = amount;
        tp.status = "Pending";

        Process process;
        process.id = tp.transactionID;
        process.burstTime = burstTime;
        process.remainingTime = burstTime;
        process.waitingTime = 0;
        process.turnaroundTime = 0;

        processes[process.id - 1] = process;
    } else {
        cout << "Process table is full.\n";
    }
}

void executeTransactionsWithScheduling(int n, int quantum) 
{
    if (n <= 0)
    {
        cout << "\nNo transactions to execute.\n";
        return;
    }

    if (quantum <= 0)
    {
        cout << "\nInvalid quantum time. It must be greater than 0.\n";
        return;
    }

    int time = 0;
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;
    bool done;

    cout << "\nExecuting Transactions using Round Robin Scheduling:\n";
    cout << "--------------------------------------------------\n";

    do {
        done = true;
        for (int i = 0; i < n; i++) 
        {
            Process &p = processes[i];
            if (p.remainingTime > 0) 
            {
                done = false;
                cout << "| Transaction " << p.id << " ";

                if (p.remainingTime > quantum) 
                {
                    time += quantum;
                    p.remainingTime -= quantum;
                } else {
                    time += p.remainingTime;
                    p.waitingTime = time - p.burstTime;
                    p.remainingTime = 0;
 
                    TransactionProcess &tp = processTable[p.id - 1];
                    int idx = findAccountIndexByNumber(tp.accountNumber);
                    if (idx == -1)
                    {
                        tp.status = "Failed";
                        continue;
                    }

                    if (tp.type == 'D') 
                    {
                        accounts[idx].dep(tp.amount);
                        tp.status = "Completed";
                    } else if (tp.type == 'W') {
                        if (accounts[idx].retdeposit() >= tp.amount) 
                        {
                            accounts[idx].draw(tp.amount);
                            tp.status = "Completed";
                        } else {
                            tp.status = "Failed";
                        }
                    }
                }
            }
        }
    } while (!done);

    cout << "|\n--------------------------------------------------\n";

    for (int i = 0; i < n; i++) 
    {
        Process &p = processes[i];
           p.turnaroundTime = p.burstTime + p.waitingTime;
    totalWaitingTime += p.waitingTime;
    totalTurnaroundTime += p.turnaroundTime;
    }

    double avgWaitingTime = static_cast<double>(totalWaitingTime) / n;
    double cpuUtilization = (time + totalWaitingTime > 0)
        ? (static_cast<double>(time) / (time + totalWaitingTime)) * 100
        : 0.0;

    cout << "\nTransaction Metrics:\n";
    cout << "--------------------------------------------------\n";
    cout << "Average Waiting Time: " << fixed << setprecision(2) << avgWaitingTime << " units\n";
    cout << "CPU Utilization: " << fixed << setprecision(2) << cpuUtilization << "%\n";
    cout << "--------------------------------------------------\n";

    cout << "\nTransactions executed successfully!\n";
}

void displayTransactions() 
{
    cout << "====================================================\n";
    cout << "Transaction ID\tAccount Number\tType\tAmount\tStatus\n";
    cout << "====================================================\n";
    for (int i = 0; i < processCount; ++i) 
    {
        cout << setw(14) << left << processTable[i].transactionID
             << setw(16) << left << processTable[i].accountNumber
             << setw(8) << left << processTable[i].type
             << setw(8) << left << processTable[i].amount
             << processTable[i].status << "\n";
    }
}

void deposit_multiple_transactions(int numTransactions) 
{
    for (int i = 0; i < numTransactions; i++) 
    {
        int accountNumber, amount;
        cout << "\nEnter Account Number for Deposit Transaction " << i + 1 << ": ";
        cin >> accountNumber;
        cout << "Enter Amount for Deposit: ";
        cin >> amount;
        createTransaction(accountNumber, 'D', amount, 5);
        cout << "Deposit transaction queued for Account No. " << accountNumber << ".\n";
    }
}

void withdraw_multiple_transactions(int numTransactions) 
{
    for (int i = 0; i < numTransactions; i++) 
    {
        int accountNumber, amount;
        cout << "\nEnter Account Number for Withdraw Transaction " << i + 1 << ": ";
        cin >> accountNumber;
        cout << "Enter Amount for Withdraw: ";
        cin >> amount;
        createTransaction(accountNumber, 'W', amount, 5);
        cout << "Withdrawal transaction queued for Account No. " << accountNumber << ".\n";
    }
}

int main() 
{
    int choice;
    initSyncTools();

    // Ensure ftok input file exists for message queue key generation.
    ofstream keyFile("progfile", ios::app);
    keyFile.close();
    
    MessageQueue msgQueue("progfile", 65);
    
    do {
        cout << "\n\nMain Menu";
        cout << "\n1. New Account";
        cout << "\n2. Deposit Amount";
        cout << "\n3. Withdraw Amount";
        cout << "\n4. Balance Enquiry";
        cout << "\n5. Display All Accounts";
        cout << "\n6. Display All Transactions";
        cout << "\n7. Execute Transactions with Round Robin Scheduling";
        cout << "\n8. Display Memory Map";
        cout << "\n9. Display FAT Table";
        cout << "\n10. Add I/O Request";
        cout << "\n11. Process I/O Requests";
        cout << "\n12. Exit";
        cout << "\nSelect Your Option (1-12): ";
        cin >> choice;

        switch (choice) 
        {
            case 1:
                write_account();
                msgQueue.sendMessage("New account created"); 
                break;
            case 2: 
            {
                int numTransactions;
                cout << "Enter number of deposit transactions: ";
                cin >> numTransactions;
                deposit_multiple_transactions(numTransactions);
                msgQueue.sendMessage("Amount deposited"); 
                break;
            }
            case 3: 
            {
                int numTransactions;
                cout << "Enter number of withdrawal transactions: ";
                cin >> numTransactions;
                withdraw_multiple_transactions(numTransactions);
                msgQueue.sendMessage("Amount withdrawn");
                break;
            }
            case 4: 
            {
                int num;
                cout << "\nEnter The account No.: ";
                cin >> num;
                display_sp(num);
                msgQueue.sendMessage("Balance enquiry made"); 
                break;
            }
            case 5:
                display_all();
                msgQueue.sendMessage("Display All Account Holder"); 
                break;
            case 6:
                displayTransactions();
                msgQueue.sendMessage("Display All Transactions"); 
                break;
            case 7: 
            {
                int quantum;
                cout << "Enter the quantum time for Round Robin Scheduling: ";
                cin >> quantum;
                executeTransactionsWithScheduling(processCount, quantum);
                msgQueue.sendMessage("CPU Scheduling Complete");
                break;
            }
            case 8:
                memoryManager.displayMemoryMap(accounts, accountCount);
                msgQueue.sendMessage("Display Memory Map"); 
                break;
            case 9:
                diskManager.displayFATTable();
                msgQueue.sendMessage("Display FAT Table"); 
                break;
            case 10: 
            {
                string request;
                cout << "Enter I/O request: ";
                cin.ignore();
                getline(cin, request);
                diskManager.addIORequest(request);
                msgQueue.sendMessage("ADD IO Request Successfully"); 
                break;
            }
            case 11:
                diskManager.processIORequests();
                msgQueue.sendMessage("Process IO Request Successfully");
                break;
            case 12:
                destroySyncTools();
                cout << "\nThank you for using the banking system!";
                break;
            default:
                cout << "\aInvalid option! Please enter a valid option (1-12).";
        }
 
        if (choice != 12) 
        {
            msgQueue.receiveMessage();
        }
    } while (choice != 12);

    return 0;
}

