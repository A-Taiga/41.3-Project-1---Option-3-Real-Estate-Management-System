
//TODO
// clean up functions


#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

struct Property {
    string address;
    double price;
    int numTenants;
    int bookingDays[31][4];
	bool year[373];
	double monthlyTotals[13];
};

Property setYear(Property property);
void printCalendar(Property property, int month);
Property addBookings (Property Property, string start, string end);
double getYearlyTotal(Property& Property); // fix this
void printCalendarForFile(Property property, double total);


int main() {

    vector<Property> properties;
    ifstream file("properties.txt");
    if(!file.is_open()) {
        cout << "ERROR FILE properties.txt COULD NOT OPEN" << endl;
        exit(-1);
    }
    Property temp;
    istringstream ss;
    string date;
    while(getline(file,temp.address)) {
        file >> temp.price;
        file >> temp.numTenants;
        file.ignore();
        file.ignore(256,'\n');
        for(int i = 0; i < temp.numTenants; i++) {
            getline(file,date);
            ss.str(date);
            for(int j = 0; j < 4; j++) {
                ss >> temp.bookingDays[i][j];
            }
        }
        file.ignore();
        file.ignore();
		temp = setYear(temp);


        properties.push_back(temp);
    }


    string addressEntered;
	string optionEntered;
	int selectionIndex;
	Property selectedProperty;

	bool propertyCheck = true;
	cout << "Enter the address of the property you want to book or exit to exit:" << endl;
		getline(cin,addressEntered);

	while(addressEntered != "exit") {


		for(size_t i = 0; i < properties.size(); i++) {
			if(properties.at(i).address.find(addressEntered) != string::npos) {
				selectionIndex = i;
				selectedProperty = properties.at(i);
				propertyCheck = true;
				break;
			} else {
				propertyCheck = false;
			}
		}
		if(!propertyCheck) {
			cout << "Address not found" << endl;
			cout << "Enter the address of the property you want to book or exit to exit:" << endl;
			getline(cin,addressEntered);
		} else {
	

			cout << "Enter an option:" << endl;
			cout << "1. Show the calendar of the property on a given month" << endl;
			cout << "2. Book a period of time for a given property" << endl;        
			cout << "q. Quit" << endl;
				cin >> optionEntered;
				cin.ignore();

		if(optionEntered == "1") { // show selected month
			int month;
			cout << "Enter the month number:" << endl;
			cin >> month;
			cin.ignore();
			printCalendar(properties.at(selectionIndex),month);

		} else if (optionEntered == "2") { // book date

			string start;
			string end;
			cout << "Enter the start month and day:" << endl;
				getline(cin,start);
			cout << "Enter the end month and day:" << endl;
				getline(cin,end);
			properties.at(selectionIndex) = addBookings(properties.at(selectionIndex),start,end);
			
	
		} else if (optionEntered == "q") { // quit
			cout << "Enter the address of the property you want to book or exit to exit:" << endl;
				getline(cin,addressEntered);

		} else { // invalid option entered
			cout << "Invalid option" << endl;

			}
		}
	}

	ofstream fileOut("yearly.txt");
	fileOut.clear();
	
	for(size_t i = 0; i < properties.size(); i++) {
		getYearlyTotal(properties.at(i));
		
		printCalendarForFile(properties.at(i),getYearlyTotal(properties.at(i)));
		
	}
    return 0;
}

Property setYear(Property property) {
	
	for(int i = 1; i < 373; i++) {
		property.year[i] = false;
	}


	for(int i = 0; i < property.numTenants; i++) {

	

		int startMonth = (property.bookingDays[i][0] * 31) - 31;
		int startDay = property.bookingDays[i][1];
		int range = 31 * (property.bookingDays[i][2] - property.bookingDays[i][0]) + (property.bookingDays[i][3] - property.bookingDays[i][1]);
		if(range == 0) {
			property.year[startDay+startMonth] = true;
		} else {
			for(int j = startMonth+startDay; j < startMonth+range+startDay+1; j++) { // must be +1 becasue day cannot be 0
				property.year[j] = true;
				
			}
		}
	}
	return property;
}

void printCalendar(Property property, int month) {

	if(month > 0 && month <= 12){
		int day = 1;
		for(int i = month*31-30; i <= month*31; i++) {
			if(day % 31 == 0) {
				if(property.year[i]) {
					cout << "B" << endl;
					day = 1;
					cout << endl;
				} else {
					cout << "*" << endl;
					day = 1;
					cout << endl;
				}
			} else if(day % 7 == 1) {
				if(property.year[i]) {
					cout << "D ";
				} else {
					cout << "* ";
				}
				day++;
			} else if(day % 7 == 0) {
				if(property.year[i]) {
					cout << "D " << endl;
				} else {
					cout << "* " << endl;
				}
				day++;
			} else {
				if(property.year[i]) {
					cout << "B ";
				} else {
					cout << "* ";
				}
				day++;
			}
		}
	} else {
		cout << "Invalid month number" << endl;
	}
}

Property addBookings (Property property, string start, string end) {

	istringstream ss;
	int newDate[4];
	ss.str(start);
	ss >> newDate[0] >> newDate[1];
	ss.clear();
	ss.str(end);
	ss >> newDate[2] >> newDate[3];

	if(newDate[0] <= 0 || newDate[0] > 12 || newDate[2] <= 0 || newDate[2] > 12) {
		cout << "Invalid month number" << endl; 
		return property;
	} else if (newDate[1] <= 0 || newDate[1] > 31 || newDate[3] <= 0 || newDate[3] > 31) {
		cout << "Invalid day number" << endl; 
		return property;
	}
		int startMonth = (newDate[0] * 31) - 31;
		int startDay = newDate[1];
		int range = 31 * (newDate[2] - newDate[0]) + (newDate[3] - newDate[1]);
		
		if(range == 0) {
			if(property.year[startMonth+startDay]) {
				cout << "Date range has dates that are already booked!" << endl;
				return property;
			}
		} else {
			for(int j = startMonth+startDay; j < startMonth+startDay+range; j++) {
				if(property.year[j]) {
					cout << "Date range has dates that are already booked!" << endl;
					return property;
				}
			}
		}
	property.numTenants++;
	for(int i = 0; i < 4; i++) {
		property.bookingDays[property.numTenants-1][i] = newDate[i];
	}
	cout << "Booked!" << endl;
	return setYear(property);
}

double getYearlyTotal(Property& property) {

	double profit = 0;
	double totalProfit = 0;
	
	int month = 1;
	int day = 1;
	for(int i = 1; i < 373; i++) {
		if(property.year[i]) {
			if(day % 7 == 1) {
				profit += property.price * 2;
				totalProfit += property.price * 2;
			} else if (day % 7 == 0) {
				profit += property.price * 2;
				totalProfit += property.price * 2;
			} else {
				profit += property.price;
				totalProfit += property.price;
			}
		}
		day++;
		if(day == 32) {
			property.monthlyTotals[month] = profit;
			month++;
			day = 1;
			profit = 0;
			
		}
	}

	return totalProfit;
}

void printCalendarForFile(Property property, double total) {

		ofstream fileOut("yearly.txt", ios::app);
		string monthNames[] = {"January","February","March","April","May","June","July","August","September","October","November","December"};

		fileOut << "Address: " << property.address << endl;
		fileOut << fixed << setprecision(2);

		for(int j = 1; j <= 12; j++) {

		if(property.monthlyTotals[j] != 0) {

			
		fileOut << fixed << setprecision(2);
		fileOut << monthNames[j-1] << ": " << property.monthlyTotals[j] << endl;
		int month = j;
		int day = 1;
		for(int i = month*31-30; i <= month*31; i++) {
			if(day % 31 == 0) {
				if(property.year[i]) {
					fileOut << "B" << endl;
					day = 1;
					fileOut << endl;
				} else {
					fileOut << "*" << endl;
					day = 1;
					fileOut << endl;
				}
			} else if(day % 7 == 1) {
				if(property.year[i]) {
					fileOut << "D ";
				} else {
					fileOut << "* ";
				}
				day++;
			} else if(day % 7 == 0) {
				if(property.year[i]) {
					fileOut << "D " << endl;
				} else {
					fileOut << "* " << endl;
				}
				day++;
			} else {
				if(property.year[i]) {
					fileOut << "B ";
				} else {
					fileOut << "* ";
				}
				day++;
			}
		}
		
			}		
		}
		
		fileOut << "Total: $" << total << endl;
		fileOut << endl;
}