#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
using namespace std;

struct Property {
    string address;
    double price;
    int numTenants;
    vector<vector<int> > dates;
    char year[372];
    double monthlyTotals[12];
    double total;
};

void setYear(Property &property);
void printMonth(Property property,int month);
void addDate(Property &Property, string startDate, string endDate);
void getTotals(vector<Property> &properties);
void printToFile(vector<Property> properties);

int main() {

    vector<Property> properties;
    ifstream file("properties.txt");

    if(!file.is_open()) {
        cout << "ERROR: FILE DID NOT OPEN" << endl;
        exit(-1);
    }
    
    Property* temp;
	temp = new Property;
	
	string line;

	while(getline(file,temp->address)) {
		file >> temp->price;
		file >> temp->numTenants;
		file.ignore(); 
		file.ignore(256,'\n');
		for(int i = 0; i < temp->numTenants; i++) {
			vector<int> tempVect;
			int tempInt;
			getline(file,line);
			istringstream ss(line);
			ss >> tempInt;
			tempVect.push_back(tempInt);
			ss >> tempInt;
			tempVect.push_back(tempInt);
			ss >> tempInt;
			tempVect.push_back(tempInt);
			ss >> tempInt;
			tempVect.push_back(tempInt);
			temp->dates.push_back(tempVect);
			tempVect.clear();
		}
		for(int i = 0; i < 372; i++) {
			temp->year[i] = '*';
		}
		properties.push_back(*temp);
		file.ignore();
		file.ignore();
		delete temp;
		temp = new Property;
	}
	delete temp;

	for(size_t i = 0; i < properties.size(); i++) {
		setYear(properties.at(i));
	}
	string addressEntered;
	char option;
	Property* selectedProperty;
	bool pass = true;

	cout << "Enter the address of the property you want to book or exit to exit:" << endl;
	getline(cin,addressEntered);

	while(addressEntered != "exit") {
		for(size_t i = 0; i < properties.size(); i++) {
			if(properties.at(i).address.find(addressEntered) != string::npos) {
				selectedProperty = &properties.at(i);
				pass = true;
				break;
			} else {
				pass = false;
			}
		}
		if(!pass) {
			cout << "Address not found" << endl;
			cout << "Enter the address of the property you want to book or exit to exit:" << endl;
			getline(cin,addressEntered);
			pass = true;
		} else {

			cout << "Enter an option:" << endl;
			cout << "1. Show the calendar of the property on a given month" << endl;
			cout << "2. Book a period of time for a given property" << endl;        
			cout << "q. Quit" << endl;
			cin >> option;
			cin.ignore();

			if(option == '1') {
				int month;
				cout << "Enter the month number:" << endl;
				cin >> month;
				cin.ignore();
				if( 0 < month && month <= 12) {
					printMonth(*selectedProperty,month);
				} else {
					cout << "Invalid month number" << endl;
				}

			} else if(option == '2') {
				string startDate;
				string endDate;
				cout << "Enter the start month and day:" << endl;
				getline(cin,startDate);
				cout << "Enter the end month and day:" << endl;
				getline(cin,endDate);
				addDate(*selectedProperty, startDate, endDate);	

			} else if(option == 'q') {
				cout << "Enter the address of the property you want to book or exit to exit:" << endl;
				getline(cin,addressEntered);
			
			} else {
				cout << "Invalid option" << endl;

				}
			}
		}
		getTotals(properties);
		printToFile(properties);


	

    return 0;
}
void setYear(Property &property) {
	int range;
	int start;
	for(int i = 0; i < property.numTenants; i++) {
		start = ((property.dates[i][0] * 31 - 31) + property.dates[i][1]);
		range = 31 * (property.dates[i][2] - property.dates[i][0]) + (property.dates[i][3]-property.dates[i][1]);
		int day = property.dates[i][1];
		for(int j = start-1; j < start+range; j++) {
			if(day % 7 == 1 || day % 7 == 0) {
				property.year[j] = 'D';
				day++;
			} else {
				property.year[j] = 'B';
				day++;
			}
			if (day == 32) {
				day = 1;
			}
		}
	}
}
void addDate(Property &property, string startDate, string endDate) {
	
	vector<int> date(4);
	istringstream ss;
	ss.str(startDate);
	ss >> date[0];
	ss >> date[1];
	ss.clear();
	ss.str(endDate);
	ss >> date[2];
	ss >> date[3];

	if(0 < date[0] && date[0] <= 12 && 0 < date[2] && date[2] <= 12) { // check month
		if(0 < date[1] && date[1] <= 31 && 0 < date[3] && date[3] <= 31) { // check day

			int start = date[0] * 31 - 31;
			int range = 31 * (date[2] - date[0]) + (date[3] - date[1]);
			int day = date[1];
			for(int i = start+day; i < start+range; i++) {
				if(property.year[i] != '*') {
					cout << "Date range has dates that are already booked!" << endl;
					return;
				}
			}
			property.numTenants++;
			property.dates.push_back(date);
			setYear(property);
			cout << "Booked!" << endl;
		} else {
			cout << "Invalid day number" << endl;
		}
	} else {
		cout << "Invalid month number" << endl;
	}
}
void printMonth(Property property,int month) {
	int start = month * 31 - 31;
	int day = 1; 
	for(int i = start; i <= start+31-1; i++) {
		if(day % 7 == 0) {
			cout << property.year[i] << " " << endl;
		} else {
			if(day != 31) { // stupid formating requirement
				cout << property.year[i] << " ";
			} else {
				cout << property.year[i];
			}
		}
		day++;
	}
	cout << endl;
	cout << endl;
}
void getTotals(vector<Property> &properties) {

	for(size_t i = 0; i < properties.size(); i++) {
	
		int day = 1;
		int month = 0;
		double monthTotal = 0;
		double yearlyTotal = 0;

		for(int j = 0; j < 372; j++) {
			if(properties[i].year[j] == 'D') {
				monthTotal += properties[i].price * 2;
				day++;
			} else if(properties[i].year[j] == 'B') {
				monthTotal += properties[i].price;
				day++;
			} else {
				day++;
			}
			if(day == 32) {
				properties[i].monthlyTotals[month] = monthTotal;
				yearlyTotal += monthTotal;
				monthTotal = 0;
				month++;
				day = 1;
			}
		}
		properties[i].total = yearlyTotal;
	}
}
void printToFile(vector<Property> properties) {

	string monthNames[] = {"January","February","March","April","May","June","July","August","September","October","November","December"};
	ofstream fileOut("yearly.txt");
	
	for(size_t i = 0; i < properties.size(); i++) {
		fileOut << "Address: " << properties[i].address << endl;
		for(int j = 1; j <= 12; j++) {
			if(properties[i].monthlyTotals[j-1] != 0) {


				int start = j * 31 - 31;
				int end =  j * 31;
				int day = 1;

				fileOut << fixed << setprecision(2);
				fileOut << monthNames[j-1] << ": " << properties[i].monthlyTotals[j-1] << endl;

				for(int k = start; k < end; k++) {
					
					if(day % 7 == 0) {
						fileOut << properties[i].year[k] << " " << endl;
						day++;
					}else if (day == 31) {
						fileOut << properties[i].year[k] << endl;
						fileOut << endl;
						day = 1;
					} else {
						fileOut << properties[i].year[k] << " ";
						day++;
					}
				}
			}
		}
		fileOut << fixed << setprecision(2);
		fileOut << "Total: $" << properties[i].total << endl;
		fileOut << endl;
	}
}
