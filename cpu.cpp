// 10927217 張維娜 10927219 林明憲 

#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <iomanip>

using namespace std;

int panel() {
	cout << "**** Simulate FIFO Queues by SQF *****" << endl;
	cout << "* 0. Quit                            *" << endl;
	cout << "* 1. Sort a file                     *" << endl;
	cout << "* 2. Simulate one FIFO queue         *" << endl;
	cout << "**************************************" << endl;
	
	int command = 0;
	cout << "Input a command(0, 1, 2): ";
	cin >> command;
	return command;
}

struct Process {
	int oid;
	int arrival;
	int duration;
	int timeout;
};

// 11/28
class QueueBase {
	public:
		struct QueueLink {
			int oid;
			int arrival;
			int duration;
			int timeout;
			struct QueueLink *next;
		};
		
		struct QueueLink *head;
		struct QueueLink *tail;
		int size;
		
		QueueBase() {
			head = NULL;
			tail = NULL;
			size = 0;
		}
		
		void push(struct Process &in) {
			if ( queueSize() == 0 ) {
				head = new struct QueueLink();
				head->oid = in.oid;
				head->arrival = in.oid;
				head->duration = in.duration;
				head->timeout = in.timeout;
				head->next = NULL;
				tail = head;
			}
			else {
				tail->next = new struct QueueLink();
				tail = tail->next;
				tail->oid = in.oid;
				tail->arrival = in.oid;
				tail->duration = in.duration;
				tail->timeout = in.timeout;
				tail->next = NULL;
			}
			
			size += 1; 
		}
		 
		struct Process pop() { 
			struct QueueLink *temp = head;
			Process result;
			
			result.oid = head->oid;
			result.arrival = head->arrival;
			result.duration = head->duration;
			result.timeout = head->timeout;
			head = head->next;
			delete temp;
			temp = NULL;
			if ( head == NULL ) {
				tail = NULL;
			}
			
			size -= 1;
			return result;
		}
		
		int queueSize() {
			return size;	
		}
};
// 11/28

class DataSort {
	private:
		double timeStrt;
		double timeRd;
		double timeWrt;
		double timeSrt;
	public:
		vector<Process> data;
		string fileName;
		
		bool loadData() {
			char temp = '\0';
			struct Process push;
			FILE *openFile;
			
			cout << endl << "Input a file number (e.g., 401, 402, 403, ...):";
			cin >> fileName;
			
			openFile = fopen(("input" + fileName + ".txt").c_str(), "r");
			if (openFile == NULL) {
    		    cout << "\n### input" << fileName << ".txt does not exist! ###\n\n";
    		    return false;
    		}
			else {
				int e = 0;
				
				timeStrt = (double)clock();
				while ( e != -1 && temp != '\n' ) {  ///////////////////////////////////////////// 讀掉標題  
					e = fscanf(openFile, "%c", &temp); 
				}                                      
    	    	
				while ( fscanf(openFile, "%d", &push.oid) != -1 ) {  ////////////////////////////  讀檔 
					fscanf(openFile, "%d %d %d", &push.arrival, &push.duration, &push.timeout);
					data.push_back(push);
				}
    	    	
				fclose(openFile);
				timeRd = (double)clock() - timeStrt;
				
				return true;
    		}
		}
		
		void printData() {
			cout << endl << "\tOID\tArrival\tDuration\tTimeOut" << endl;
			for ( int i = 0 ; i <= data.size() - 1 ; i++ ) {
				cout << "(" << i + 1 << ")\t" << data.at(i).oid << '\t' << data.at(i).arrival << '\t' 
					 << data.at(i).duration << '\t' << data.at(i).timeout << endl;
			}
		} 
		
		void writeData() {
			FILE *openFile;
			
			timeStrt = (double)clock();
			openFile = fopen(("sorted" + fileName + ".txt").c_str(), "w");
			fprintf(openFile, "OID\tArrival\tDuration\tTimeOut\n");
			for ( int i = 0 ; i <= data.size() - 1 ; i++ ) {
				fprintf(openFile, "%d\t%d\t%d\t%d\n", data.at(i).oid, data.at(i).arrival, data.at(i).duration, data.at(i).timeout);
			}
			
			fclose(openFile);
			timeWrt = (double)clock() - timeStrt;
		}
		
		void printTime() {
			int rd = timeRd, srt = timeSrt, wrt = timeWrt;
			cout << endl << "Reading data: " << rd << " clocks (" 
				 << setiosflags(ios::fixed) << setprecision(2) << timeRd << " ms)." << endl;
			cout << "Sorting data: " << srt << " clocks ("
				 << setiosflags(ios::fixed) << setprecision(2) << timeSrt << " ms)." << endl;
			cout << "Writing data: " << wrt << " clocks ("
				 << setiosflags(ios::fixed) << setprecision(2) << timeWrt << " ms)." << endl;
		}
		
		void sort() {
			Process temp;
			
			timeStrt = (double)clock();
			for ( int step = data.size() / 2 ; step > 0 ; step /= 2 ) {
				for ( int i = step ; i <= data.size() - 1 ; i += 1 ) {
					temp.oid = data.at(i).oid;
					temp.arrival = data.at(i).arrival;
					temp.duration = data.at(i).duration;
					temp.timeout = data.at(i).timeout;
					
					int j = i;
					for ( bool keep = true ; keep && j >= step && temp.arrival <= data.at(j - step).arrival ; j -= step ) {
						if ( temp.arrival == data.at(j - step).arrival ) {
							if ( temp.oid < data.at(j - step).oid ) {
								data.at(j).oid = data.at(j - step).oid;
								data.at(j).arrival = data.at(j - step).arrival;
								data.at(j).duration = data.at(j - step).duration;
								data.at(j).timeout = data.at(j - step).timeout;
							}
							else {
								keep = false;
								j += step;
							}
						}
						else {
							data.at(j).oid = data.at(j - step).oid;
							data.at(j).arrival = data.at(j - step).arrival;
							data.at(j).duration = data.at(j - step).duration;
							data.at(j).timeout = data.at(j - step).timeout;
						}
					}
					
					data.at(j).oid = temp.oid;
					data.at(j).arrival = temp.arrival;
					data.at(j).duration = temp.duration;
					data.at(j).timeout = temp.timeout;
				}
			}
			
			timeSrt = (double)clock() - timeStrt;
		}
};

class CPUSim {
	public:
		vector<Process> sortedData;
		vector<Process> finishList;
		vector<Process> cancelList;
		string fileName;
		
		bool loadData(string name) {
			char temp = '\0';
			struct Process push;
			FILE *openFile; 
			
			fileName = name;
			if ( fileName.empty() ) {
				cout << endl << "Input a file number (e.g., 401, 402, 403, ...):";
				cin >> fileName;
			}
			
			openFile = fopen(("sorted" + fileName + ".txt").c_str(), "r");
			if (openFile == NULL) {
    		    cout << "\n### sorted" << fileName << ".txt does not exist! ###\n\n";
    		    return false;
    		}
			else {
				int e = 0;
				
				while ( e != -1 && temp != '\n' ) {
					e = fscanf(openFile, "%c", &temp);
				}
    	    	
				while ( fscanf(openFile, "%d", &push.oid) != -1 ) {
					fscanf(openFile, "%d %d %d", &push.arrival, &push.duration, &push.timeout);
					sortedData.push_back(push);
				}
    	    	
				fclose(openFile);
				
				return true;
    		}
		}
		
		void simulate() {
			/*
OID	Arrival	Duration	TimeOut
101	3	9	12
112	5	6	13
103	6	7	15
106	6	9	17
108	6	8	18
105	10	6	20
104	11	9	22

cpu cancel
cpu finish
now cancel
queue cancel
cpu empty 
	queue not empty
		use queue
		now into queue
	use now
cpu full
	queue full cancel
	queue emptu into queue
	
	
check per sec
	cpu cancel
	queue cancel
	now cancel
	
	arrive
	finish = arrive + duration
	
sep to
	cpu
	queue
	vector
			*/
		}
};

int main(int argc, char** argv) {
	int command = 0;
	string fileName = "";
				
	do {
		DataSort sort;
		CPUSim sim;
		command = panel();
		bool open = false;
		
		if ( command == 1 ) {
			open = sort.loadData();
			fileName = sort.fileName;
			if ( open ) {
				if ( sort.data.empty() ) {
					cout << endl << "### Get nothing from the file input400.txt ! ###" << endl << endl;
				}
				else {
					sort.printData();
					sort.sort();
					sort.writeData();
					
					system("pause");
					sort.printTime();
					cout << endl << "See sorted" << sort.fileName << ".txt" << endl << endl;
				}
			}
		}
    	else if ( command == 2 ) {
    		open = sim.loadData(fileName);
    		if ( open ) {
				cout << endl << "The simulation is running..." << endl
					 << "See output" << sim.fileName << ".txt" << endl << endl;
			}
		}
		else if ( command == 0 ) {
			cout << endl << "Quit!" << endl;
		}
		else {
			cout << endl << "Command does not exist!" << endl << endl;
		}
	} while ( command != 0 );

	system("pause");
	return 0;
}
