#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <iterator>
#include <fstream> 
#include <map>
#include <fstream>
#include "json.hpp"//needed for file formatting 

using namespace std;
using json = nlohmann::json; //needed for file formatting 

class Date{  //class for date uses atributes for day/month/year
	public:
		int day, month, year;
		void setDay(int d) { day = d; };
		void setMonth(int m) { month = m; };
		void setYear(int y) { year = y; };
		
		int getDay() { return day; };
		int getMonth() { return month; };
		int getYear() {return year; };
		
		Date(){day = 1; month = 1; year = 2022;};
		Date(int d, int m, int y){
			setDay(d);
			setMonth(m);
			setYear(y);
		}; 
		
		void showDate(){ // prints the date
			cout << "Patient's appointment: " << getDay() << "/" << getMonth() << "/" << getYear() << endl; 
		};		
};

class Patient { //class for patients, uses name, egn, date for visit, diegnose, worries, old worries
	public:
		int EGN; 
		string name; 
		Date dateOfVisit;
		string diagnose; 
		vector <string> worries;
		vector <string>  oldWorries;
		string medicine[5];
			
		friend ostream & operator << (ostream &os, Patient &p){ // operator << is overload to print formated data for patient
			os << "Patient name: " << p.name << "\nEGN: " << p.EGN << "\nDiagnose: " << p.diagnose << endl;  
		
			os << "Worries: "<< endl;
			for(auto& w : p.worries){
				os << w << endl;
			}
			
			os << "Old worries: "<< endl;
			for(auto& ow : p.oldWorries){
				os << ow << endl;
			}
			
			os<< "Medine: "<< endl;
			for(auto& m : p.medicine){
				if(m == "") break;
				os << m << endl;
			}
			os << "Patient's appointment: " << p.dateOfVisit.getDay() << "/" << p.dateOfVisit.getMonth() << "/" << p.dateOfVisit.getYear() << "\n" << endl; 		
			return os;
		}
			
		Patient() { };// empty construcktor 
				
		Patient(int egn, string n, int d, int m, int y, string di, string w, string ow, string med ) { //class constructor
		 EGN = egn; 
		 name = n; 
		 dateOfVisit.setDay(d);
		 dateOfVisit.setMonth(m);
		 dateOfVisit.setYear(y); 
		 diagnose = di; 
		 addWorries(w);
		 addOldWorries(ow);
		 addMedicine(med);
		};
			
		void addWorries(string w){	//used to add new worries to patiens data, it goes to last element and there it is saved		
			auto it = worries.begin();
			if(!(worries.empty())){ // This function returns true if the container size is zero and false if the container size is not zero.
				for(it; it != worries.end(); ++it){
					worries.insert(it, w);
				}
			}else{
				worries.insert(it, w);// if empty it inserts directly 
			}
		}
		
		void addOldWorries(string w){ // same funtion like addWorries
			auto it = oldWorries.begin();
			
			if(!(oldWorries.empty())){ // This function returns true if the container size is zero and false if the container size is not zero.
				for(it; it != oldWorries.end(); ++it){
					oldWorries.insert(it, w);
				}
			}else{
				oldWorries.insert(it, w);
			}
		}
		
		void addMedicine(string m){ // same funtion like addWorries but for arrays; m for medicine
			int i;
			bool f = true;
			for(i = 0; i < 5; i++){
				if(medicine[i] == ""){ //searches for last empty space 
					medicine[i] = m;
					f = false; //if there is a space it inserts it in there
					break;
				}
			}
			//if there is no more space for the new medicine the next line ask what medicine to be replaced with the new one 
			if(f){				
				cout << "Patient can't have more medication!\nDo you want to chage one? [y/n] ";
				string yn;
				cin >> yn;
				f = !f; // must be false
				if(yn == "y" || yn == "Y"){	
					yn.clear();
					cout << "Medicine used: " << endl; //for user-frendly the medinice used is dispayed 
					for(i = 0; i < 5; i++){	//showing medication
						if(f) cout <<", ";							
						cout << medicine[i];
						f = true;						
					}
					f = !f; 
					cout << "\nWitch one to replace? ";
					cin >> yn;
					
					for(i = 0; i < 5; i++){ //if the input finds the medicine it is replaysed
						if(medicine[i] == yn){
							medicine[i] = m;
							f = true; //and the flag isnt activated and the next if isnt activated 
							break;
						}
					}
					if(!f){ //if there is input error the prosider is to ask if the user wants to continue and do it again or to cancel 
						cout << "No such medicine found!\nDo you want to try again? [y/n] ";
						yn.clear();
						cin >> yn;
						if(yn == "y" || yn == "Y") addMedicine(m);	//if yes I use recuntion to do it again
					}
					
					f = false;
					cout << "\nMedicine used: " << endl;
					for(i = 0; i < 5; i++){	//dispays the medications to see changes
						if(f) cout <<", ";													
						cout << medicine[i];
						f = true;						
					}
					cout << "" << endl;				
				}		
			}
		}	
};


json to_jason(list<Patient> &l){// this funtion seriallizes the list of patiens so that it is formated inside of the json file
	json j, jarr;
	
	for(auto p : l) //goes to every element
	{
		j = json{
			{"name", p.name}, // "Name" : "Joe"/ atribute : value
			{"EGN", p.EGN},
			{"Day", p.dateOfVisit.day},
			{"Month", p.dateOfVisit.month},
			{"Year", p.dateOfVisit.year},
			{"Diagnose", p.diagnose},
			{"Worries", p.worries},		
			{"Old Worries", p.oldWorries},
			{"Medication", p.medicine}					
		};
		jarr.emplace_back(j); // creates json array;
	}
	return jarr; 
}


Patient patient_fromj(json &j) //this function creates obj of class Patient with the input data from file
{	
	Patient p;

	j.at("name").get_to(p.name); // at the atribute "name" it gets the value and stores it in p.name
	j.at("EGN").get_to(p.EGN);  
	j.at("Day").get_to(p.dateOfVisit.day);	
	j.at("Month").get_to(p.dateOfVisit.month);
	j.at("Year").get_to(p.dateOfVisit.year);
	j.at("Diagnose").get_to(p.diagnose);
	j.at("Worries").get_to(p.worries);
	j.at("Old Worries").get_to(p.oldWorries);	
	j.at("Medication").get_to(p.medicine);
	
	return p; //returns the new obj that was read
}

list<Patient> from_jason(json &j){ //this func separate the objs that are stored in the json file 
	Patient p;
	list<Patient> lp;	

	for(auto it = j.begin(); it != j.end(); ++it){		
		p = patient_fromj(it.value());						
		
		lp.emplace_back(p); 
	}
	return lp; //returns the newly formed list of data/obj class patients 
}


list<Patient> readFile(){ //  this func opens the json file and reads the data in one-go

	list<Patient> lp;
	ifstream file_read;
	
	file_read.open("PatientData.json"); // opening json file name PatientData.json 
	
	if(!file_read){
		cerr << "Error: file could not be opened!" << endl;
		exit(1);
	}else{	
		json j; //here we will store all the data like in array
		file_read >> j; // and saves it to file with operator << 
		lp = from_jason(j);
	}
	file_read.close(); // closing file
	return lp; //returns the newly formed list of patients that were in the json file
}



list<Patient> addPatients(list<Patient> &l){ // used to add new patients 
	string name, diagnose, worrie, oldworrie, meds;
	int egn, day, month, year;
	cout << "New patiend name: ";
	cin >> name;
	cout << "New patiend EGN: ";
	cin >> egn;
	cout << "New patiend worrie: ";
	cin >> worrie;
	cout << "New patiend old worries: ";
	cin >> oldworrie;
	cout << "New patiend day of visit: ";
	cin >> day;
	if(day <= 0 || day > 31){
			cout << "Invalid day!Try again:"<<endl;
			cin >> day;
	}
	cout << "New patiend month of visit: ";
	cin >>month;
	if(month <= 0 || month > 12){
			cout << "Invalid month!Try again:"<<endl;
			cin >> month;
	}
	cout << "New patiend year of visit: ";
	cin >>year;
	cout << "New patiend diagnose: ";
	cin >> diagnose;
	cout << "New patiend medication: ";
	cin >> meds;
	
	Patient newP(egn, name, day, month, year, diagnose, worrie, oldworrie, meds); // creating new patien/ obj
	
	l.emplace_back(newP);// adds new patien to the back of list 
		
	return l;	
}

list<Patient> removalOfPatients(list<Patient> &l){ // removind elements
	int egn;
	cout << "Remove patient by EGN: "; // by egn
	cin >> egn;
	list<Patient> :: iterator itp;
	bool flag = false;
	
	for(itp = l.begin(); itp != l.end(); itp++){ // starts from the first element and goes to the end, one by one 	
		if(itp->EGN == egn){
			l.erase(itp); // erases the element from the list 
			flag = true; 
			break;
		}
	}
	if(!flag){ 
		cout << "No such patient!\nWant to try again?[y/n] ";// if wrong input
		string yn;
		cin >> yn;
		if(yn == "y" || yn == "Y"){
			l =  removalOfPatients(l); // recuntion
		}
	}	
	cout << "Patient is removed!" << endl;
	
	return l; // returns list 	
}

list<Patient> updateData(list<Patient> &l){//updating patien's data 
	int indicator = 0;
	int newEgn;
	bool flag = false;
	cout<< "Who's data do you want to update? Write the EGN: " << endl;//searches by egn
	cin >> newEgn; // add check for name
	list<Patient> :: iterator it = l.begin();
	for(it; it != l.end(); ++it){
			if(it->EGN == newEgn){ // if patient found 
				flag = true;	
			    break;
			}
	}
	
	if(flag){ // we come here  
		cout << "What do you want to update?\nName = 1\nEGN = 2\nDate of visit = 3\nDiagnose = 4\nWorries = 5\nMedicine = 6" << endl; // dispays what can be changed 
		cin >> indicator;
	
		if(indicator == 1){ 
			string newname;
			cout << "With what name do you want to update it with? " << endl;
			cin >> newname;
			it->name = newname;
			return l;	
		}
		
		else if(indicator == 2){
			int newEGN;
			cout << "With what EGN do you want to update it with? " << endl;
			cin >> newEGN;
			it->EGN = newEGN;	
		return l;
		}
		
		else if(indicator == 3){
			int d,m,y;
			cout << "New date of visit: " << endl;
			cin >> d >> m >> y; 
			Date newDate(d,m,y);
			it->dateOfVisit = newDate;
		return l;
		}
		
		else if(indicator == 4){
			string newDiagnose;
			cout << "Update Diagnose to: " << endl;
			cin >> newDiagnose;
			it->diagnose = newDiagnose;
		return l;
		}
		else if(indicator == 5){
			string newWorries;
			cout << "Update worries: " << endl;
			cin >> newWorries;
			it->addWorries(newWorries);
		return l;
		}
		
		else if(indicator == 6){
			string newMeds;
			cout << "New madicine: " << endl;
			cin >> newMeds;
			it->addMedicine(newMeds);
		return l;
		}
		else { // if wrong choice 
			cout << "Wrong input!\nTry again:" << endl;
			l = updateData(l); //recuntion
		}
	cout << "Changes are made!" << endl;
	
	}else{ //if no patient found 
		cout << "No such name found in data!\nTry again: " << endl; 
		l = updateData(l); 
	}
	
	return l;
}

void showAll(list<Patient> &l){ // to display all patients 
	for(auto itp: l){
	 	cout << itp;
	}
}

void saveToFile(list<Patient> &patients){ // saving to file 
	
	ofstream file_in;
	
	file_in.open("PatientData.json"); // opening json file name PatientData.json 
	
	if(!file_in){
		cerr << "Error: file could not be opened!" << endl;
		exit(1);
	}else{	
	
	json j = to_jason(patients); // uses to_jason juntion to get the patiens in json format
	file_in << j; // and saves it to file with operator << 
	
	cout << "Data was successfully saved!" << endl;
	}
	file_in.close(); // closing file 
}

void lastMonthVisits(list<Patient> &p){ // patiens who visited last month 
	int lastMonth = 0, lastYear = 0;
	for(auto itp: p){
		if(itp.dateOfVisit.getMonth() > lastMonth || itp.dateOfVisit.getYear() > lastYear ){ //loops to find the monst early month and year  
			lastMonth = itp.dateOfVisit.getMonth();// saves it 
			lastYear = itp.dateOfVisit.getYear();  
		}
	}
	cout << "Patients that visited last month were:" << endl;
	for(auto itp: p){
		if(itp.dateOfVisit.getMonth() == lastMonth && itp.dateOfVisit.getYear() == lastYear ){ // and checks patiens if it maches 
	 		cout << itp;
		}
	}
}

void patientWithWories(list<Patient> &p){ // displays patients who have worries 		
	cout << "Patients with worries:" << endl;
	for(auto itp: p){
		if(!itp.worries.empty()){ //if it is not empty the we dispay patiens 
			cout << itp;
		}
	}
}

void mostCommanDiagnostic(list<Patient> &p){ //searches for most comman diagnose 	
	string s;
	
	for(auto itp : p){	//loops through the list 
		s.append(itp.diagnose); // puts all diagnoses in one string with append and " " for space inbetween them
		s.append(" ");
	}	
	
	istringstream my_stream(s); // creating stream with class std::istringstream and in here I save all diagnostincs 
	
	map<string, int> count; // using map class to map diagnostics to repetition
	string diagnose;// empty string
	decltype(count)::iterator most_common; //iterator from the same type like count 
	
	while(my_stream >> diagnose){ // 
		auto iterator = count.emplace(diagnose, 0).first; // emplace returns a pair<iterator,bool> indicating where the word and its count are in the map, and whether its newly inserted; but we care where it is: capture emplace(...).first  
		++iterator->second; //As we update the count, we check if that makes the word the most-common word seen so far. If so we copy the iterator to the local variable most_common, so we have a record of both the most commonly seen word so far and its count.
		if(count.size() == 1 || iterator->second > most_common->second){//checks for most common 
			most_common = iterator; //saves most common diagnostic 
		}
	}
	
	cout << "Most comman diagnose is " << most_common->first << "." << endl;
}


int main(int argc, char** argv) { 

	cout << "################===Starting program!===################"<< endl;
	int input;//for navication in program
	list<Patient> patients;//list of our objects
	bool end = true; //for ending the program	
	
	cout << "To show all data of patients press 0.\nTo add patiend press 1.\nTo update data press 2.\nTo remove patient press 3.\nTo save data to file press 4.\nTo show patient who visited last month press 5.\nTo show patient with worries press 6.\nTo show most commond diagnose press 7.\nTo show options press 8.\nTo end press 9." << endl;
	patients = readFile();//creating list of obj/ patients by reading json file
	do{
		cout << "Your input: ";
		cin >> input; 
		cout << endl;	
	
		if(input == 0){ showAll(patients); }		
		
		if(input == 1){ patients = addPatients(patients); } //adding Patient 
		
		if(input == 2){ patients = updateData(patients); }// updating data
    
		if(input == 3){ patients = removalOfPatients(patients); }// removing objs
				
		if(input == 4){ saveToFile(patients); } //saving in file
		
		if(input == 5){ lastMonthVisits(patients); } //patients who visited last month
		
		if(input == 6){ patientWithWories(patients); }//patient with worries
			
		if(input == 7){ mostCommanDiagnostic(patients); }// most comman diagnose	
		
		if(input == 8){	cout << "To show all data of patients press 0.\nTo add patiend press 1.\nTo update data press 2.\nTo remove patient press 3.\nTo save data to file press 4.\nTo show patient who visited last month press 5.\nTo show patient with worries press 6.\nTo show most commond diagnose press 7.\nTo show options press 8.\nTo end press 9." << endl; }
						
		if(input == 9){ end = false; } //to end the program
	 	
	}while(end);
	
	cout << "#####################################+End of program!+#####################################"<< endl;
	return 0;
}

