//
//  project.cpp
//  payroll project
//
//  Created by Justin Bushue on 12/4/19.
//  Copyright © 2019 Justin Bushue. All rights reserved.
//

//This section has all the header files that are needed for the program

#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <ios>
#include <sstream>

//In this program I decided to use global variables for the majority of variables needed
//This decision was made due to the fact that nearly every function will be manipulating
//these variables and having to pass them to each function would be very time consuming
//and redundant.

std::string firstName, lastName, employeeID, payTo = "Justin Bushue";
int checkNumber=0, decimalsPay=0, hours=0, hold=0, holdDeci=0;
bool justHours = false, appendFlag = false;
float netPay=0, grossPay=0, sTax=0, fTax=0, ficaTax=0, sdiTax=0, medicareTax=0, ytdGrossPayHold = 0, ytdGrossPay=0, ytdNetPay=0, hourlyRate=0, totalTax=0, ytdTotalTax=0, overtimePay = 0.0, ytdNetPayHold =0, ytdTotalTaxHold = 0;
std::vector<std::string> employeeFirstNames, employeeLastNames, empID;
std::vector<long> payRate;
std::vector<int> hoursWorked;

//This function is called when printing the check to convert the numerical value of the net pay to the written dollar amount
//This function uses recursion to figure it out.

std::string inttostr(const unsigned int number)
{
	const std::vector<std::string> first14 = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen" };
	const std::vector<std::string> prefixes = {"twen", "thir", "for", "fif", "six", "seven", "eigh", "nine"};

	if( number <= 14 )
		return first14.at(number);
	if( number < 20 )
		return prefixes.at(number-12) + "teen";
	if( number < 100 )
		{
			unsigned int remainder = number - (static_cast<int>(number/10)*10);
			return prefixes.at(number/10-2) + (0 != remainder ? "ty " + inttostr(remainder) : "ty");
		}
	if( number < 1000 )
		{
			unsigned int remainder = number - (static_cast<int>(number/100)*100);
			return first14.at(number/100) + (0 != remainder ? " hundred " + inttostr(remainder) : " hundred");
		}
	if( number < 1000000 )
		{
			unsigned int thousands = static_cast<int>(number/1000);
			unsigned int remainder = number - (thousands*1000);
			return inttostr(thousands) + (0 != remainder ? " thousand " + inttostr(remainder) : " thousand");
		}
	if( number < 1000000000 )
		{
			unsigned int millions = static_cast<int>(number/1000000);
			unsigned int remainder = number - (millions*1000000);
			return inttostr(millions) + (0 != remainder ? " million " + inttostr(remainder) : " million");
		}
	throw std::out_of_range("inttostr() value too large");
}

//This function adds the employees information to the corrosponding vectors.
//I've made this into a function so I wouldn't have to write this out each time
//I need to add to the vectors.

void addToVectors()
{
	empID.push_back(employeeID);
	employeeFirstNames.push_back(firstName);
	employeeLastNames.push_back(lastName);
	payRate.push_back(hourlyRate);
	hoursWorked.push_back(hours);
}

//This function formats the employee id to have leading '0's after the 'A' and before the id number.

std::string format_empid_number(int id_no)

{
  std::ostringstream out;
  
  out << std::internal << "A" << std::setfill('0') << std::setw(6) << id_no;
  
  return out.str();
}

//This function does the calculation for overtime if overtime is worked based on
//The employees hours.

float calcOverTime()
{
	overtimePay = 0.0;
	if(hours > 40)
		{
			int otHours = 0;
			float otRate = 0;

			otHours = hours - 40;
			otRate = hourlyRate * 1.5;
			overtimePay = otRate * otHours;
		}
	return overtimePay;
}

//This is the general calculations functions that will calculate any of the required
//values for the check.

void doCalculations(int index)
{
	const float stateTax = .05, federalTax = .09, fica = .06, medicare = .01, sdi = .02;

	srand(static_cast<unsigned int>(time(NULL)));

	checkNumber = rand() % 1000 + 1;
	grossPay = (hoursWorked.at(index) * payRate.at(index)) + calcOverTime();
	sTax = grossPay * stateTax;
	fTax = grossPay * federalTax;
	ficaTax = grossPay * fica;
	medicareTax = grossPay * medicare;
	sdiTax = grossPay * sdi;
	netPay = grossPay - (sTax - fTax - ficaTax - sdiTax - medicareTax);

	totalTax = sTax + fTax + ficaTax + medicareTax + sdiTax;

	payTo = employeeFirstNames.at(index) + " " + employeeLastNames.at(index);
	hold = netPay * 100;
	holdDeci = (hold % 100);
}

//This function is designed to draw the check to the console

void drawCheckToConsole(int index)
{
	firstName = employeeFirstNames.at(index);
	lastName = employeeLastNames.at(index);
	hours = hoursWorked.at(index);
	hourlyRate = payRate.at(index);

	std::string firmName = "LSP Inc.", bankName = "US National Bank", departmentName = "Payroll Account", date = "September 18, 2019", CFOName = "John Hanaghin", payPeriod = "September 2-13, 2019", memo = "PPE September 13, 2019";

	doCalculations(index);

	std::cout.precision(2);
	std::cout << std::right << firmName << std::setw(79) << "Check # " << checkNumber << std::endl;
	std::cout << std::right << departmentName << std::setw(70) << "Date: " << date << std::endl;
	std::cout << "\n\n";
	std::cout << std::fixed << std::right << "Pay to the order of " << payTo << std::setw(47) << "$" << netPay << std::endl;
	std::cout << "\n";
	std::cout << std::right << inttostr(netPay) << " dollars " << inttostr(holdDeci) << "/100" << std::endl;
	std::cout << "\n\n";
	std::cout << bankName << std::endl;
	std::cout << std::right << "Memo: " << memo << std::setw(64) << CFOName << std::endl;
	std::cout << std::setw(102) << "Cheif Financial Officer" << std::endl;
	std::cout << "\n~~~~~~~~~~~~Detach check above before depositing and save checkstub below for your records~~~~~~~~~~~~" << std::endl << std::endl;
	std::cout << std::right << firmName << std::setw(79) << "Check # " << checkNumber << std::endl;
	std::cout << std::right << "Employee: " <<  payTo << std::setw(62) << "Date: " << date << std::endl;
	std::cout << std::right << "Pay Period: " << payPeriod << std::setw(59) << "Current\t\tYTD" << std::endl;
	std::cout << std::right << std::setw(50) << "Gross Earnings" << std::setw(29) << "$" << grossPay << std::setw(10) << "$" << ytdGrossPay << std::endl;
	std::cout << std::endl;
	std::cout << std::setw(49) << "Deductions:" << std::endl;
	std::cout << std::setw(49) << "Federal Income Tax" << std::setw(20) << fTax << std::endl;
	std::cout << std::setw(49) << "Social Security (FICA)" << std::setw(20) << ficaTax << std::endl;
	std::cout << std::setw(49) << "Medicare" << std::setw(20) << medicareTax << std::endl;
	std::cout << std::setw(49) << "State (AL) Income Tax" << std::setw(20) << sTax << std::endl;
	std::cout << std::setw(49) << "State Disability Insurance (SDI)" << std::setw(20) << sdiTax << std::setw(16) << totalTax << std::setw(17) << ytdTotalTax << std::endl;
	std::cout << std::setw(49) << "Net Pay" << std::setw(31) << "$" << netPay << std::setw(11) << "$" << ytdNetPay<<std::endl<<std::endl;
}

//This function is designed to draw the check to a file. The file is named according to
//The employees first and last name.

void drawCheckToFile(int index)
{
	std::ofstream outFile;
	std::string fileName;

	firstName = employeeFirstNames.at(index);
	lastName = employeeLastNames.at(index);
	hours = hoursWorked.at(index);
	hourlyRate = payRate.at(index);

	std::string firmName = "LSP Inc.", bankName = "US National Bank", departmentName = "Payroll Account", date = "September 18, 2019", CFOName = "John Hanaghin", payPeriod = "September 2-13, 2019", memo = "PPE September 13, 2019";

	fileName = employeeFirstNames.at(index) + "_" + employeeLastNames.at(index) + "Check.txt";

	outFile.open(fileName);

	doCalculations(index);

	outFile.precision(2);
	outFile << std::right << firmName << std::setw(79) << "Check # " << checkNumber << std::endl;
	outFile << std::right << departmentName << std::setw(70) << "Date: " << date << std::endl;
	outFile << "\n\n";
	outFile << std::fixed << std::right << "Pay to the order of " << payTo << std::setw(47) << "$" << netPay << std::endl;
	outFile << "\n";
	outFile << std::right << inttostr(netPay) << " dollars " << inttostr(holdDeci) << "/100" << std::endl;
	outFile << "\n\n";
	outFile << bankName << std::endl;
	outFile << std::right << "Memo: " << memo << std::setw(64) << CFOName << std::endl;
	outFile << std::setw(102) << "Cheif Financial Officer" << std::endl;
	outFile << "\n~~~~~~~~~~~~Detach check above before depositing and save checkstub below for your records~~~~~~~~~~~~" << std::endl << std::endl;
	outFile << std::right << firmName << std::setw(79) << "Check # " << checkNumber << std::endl;
	outFile << std::right << "Employee: " <<  payTo << std::setw(62) << "Date: " << date << std::endl;
	outFile << std::right << "Pay Period: " << payPeriod << std::setw(59) << "Current\t\tYTD" << std::endl;
	outFile << std::right << std::setw(50) << "Gross Earnings" << std::setw(29) << "$" << grossPay << std::setw(10) << "$" << ytdGrossPayHold << std::endl;
	outFile << std::endl;
	outFile << std::setw(49) << "Deductions:" << std::endl;
	outFile << std::setw(49) << "Federal Income Tax" << std::setw(20) << fTax << std::endl;
	outFile << std::setw(49) << "Social Security (FICA)" << std::setw(20) << ficaTax << std::endl;
	outFile << std::setw(49) << "Medicare" << std::setw(20) << medicareTax << std::endl;
	outFile << std::setw(49) << "State (AL) Income Tax" << std::setw(20) << sTax << std::endl;
	outFile << std::setw(49) << "State Disability Insurance (SDI)" << std::setw(20) << sdiTax << std::setw(16) << totalTax << std::setw(17) << ytdTotalTaxHold << std::endl;
	outFile << std::setw(49) << "Net Pay" << std::setw(31) << "$" << netPay << std::setw(11) << "$" << ytdNetPayHold <<std::endl<<std::endl;
	outFile.close();
}

//This function is called when I am needing to list out the employees that are in the employe file.
//It uses the information in the vectors to display the list.

void listEmployees()
{
	int i = 0;
	unsigned long size = employeeFirstNames.size();
	if(size == 0)
		{
			std::cout << "There are no employees to list. Enter -1 to abort this operation." << std::endl;
		}
	else
		{
			std::cout << std::left << std::setw(5) << "   #" << std::setw(16) << "First Name" << std::setw(12) << "Last Name" << std::setw(13) << "Employee ID" << std::endl;
			std::cout << "---------------------------------------------" << std::endl;
			while (i <= size -1)
				{
					std::cout << std::right << std::setw(4) << i+1;
					std::cout << std::left << std::setw(2) << ". " << std::setw(15)<< employeeFirstNames.at(i) << " " << std::setw(12) << employeeLastNames.at(i) << " (" << empID.at(i) << ")" << std::endl;
					i++;
				}
		}
}

//This function is used to return if the user would like to draw the check to the console or to a file

std::string toConsoleOrFile()
{
	std::string which;
	do
		{
			std::cout << "Would you like to print the pay check to Console (c) or File (f): ";
			while(!(std::cin >> which))
				{
					std::cout << "You need to enter a character value!" << std::endl;
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
		}
	while(which != "c" and which != "f");
	if(which == "c")
		{
			return "c";
		}
	return "f";
}

//This function determines if there are any employees in the employee file.
//Since with my program you are able to delete employees I have checks in
//Place to ensure the user doesn't do anything stupid.

bool isEmpty(std::ifstream& pFile)
{
	return pFile.peek() == std::ifstream::traits_type::eof();
}

//This is the function that does the initial reading of the employee file that
//Stores to the employees information into the vectors.

void getFromFile()
{
	std::ifstream inFile;
	inFile.open("employeeFile.txt");

	if(isEmpty(inFile) and inFile)
		{
			std::cout << "The employee file is empty." << std::endl;
			return;
		}

	if (!inFile)
		{
			std::cout << "Employee File is not found. Please select option 3 or 7 in the main menu." << std::endl << std::endl;
		}
	if(inFile.is_open())
		{
			while(inFile >> employeeID >> firstName >> lastName >> hours >> hourlyRate)
				{
					addToVectors();
				}
			std::cout << "Information from the employee file has been retrieved.";
			inFile.close();
			std::cout << std::endl;
		}
}

//This function is used in several different ways, however, it's functionality
//is pretty straight forward. It simply asks in the console for employees
//information whether it before starting a new employee file, or appending.

void getFromConsole()
{
	if(!justHours)
		{
			int ID = 0;
			std::cout << "Please enter the empoyee ID (without the 'A'): ";
			while(!(std::cin >> ID))
				{
					std::cout << "You need to enter a string value!" << std::endl;
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
			employeeID = format_empid_number(ID);
			std::cout << "Please enter the employees first and last name: ";
			while(!(std::cin >> firstName >> lastName))
				{
					std::cout << "You need to enter a string value!" << std::endl;
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}

			do
				{
					std::cout << "Please enter the employees hourly rate: $";
					while(!(std::cin >> hourlyRate))
						{
							std::cout << "You need to enter a float value!" << std::endl;
							std::cin.clear();
							std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						}

				}
			while(hourlyRate < 7.25);
		}
	do
		{
			std::cout << "Please enter the employees hours for the week: ";
			while(!(std::cin >> hours))
				{
					std::cout << "You need to enter an integer value!" << std::endl;
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
		}
	while(hours <= 0);

	addToVectors();
}

//This function is similar to the listEmployees function but is displays the totality
//Of the employees information.

void displayEmployeeFile()
{
	try
		{
			int i = 0;

			std::cout << "Employee ID    First Name    Last Name    Hours Worked    PayRate" << std::endl;
			std::cout << "-----------------------------------------------------------------" << std::endl;
			while (i <= employeeLastNames.size()-1)
				{
					std::cout << std::left << std::setw(15) << empID.at(i) << std::setw(14) << employeeFirstNames.at(i) << std::setw(13) << employeeLastNames.at(i) << std::setw(16) << hoursWorked.at(i) << std::setw(8) << payRate.at(i) << std::endl;
					i++;
				}
		}
	catch(...)
		{
			std::cout << "There are no employees in the file or the employee file is missing." << std::endl;
		}
}

//This function is also a dual purpose function. It handles starting a new employe file
//As well as when the user is wanting to append to the employee file. This is done
//By having an append flag that is set when either of the options in the menu
//Are selected.

void newEmployeeFile()
{
	char doAnother;
	std::ofstream outFile;
	std::string fileName;
	fileName = "employeeFile.txt";
	if(!appendFlag)
		{
			outFile.open(fileName);
			empID.clear();
			employeeFirstNames.clear();
			employeeLastNames.clear();
			payRate.clear();
			hoursWorked.clear();
		}
	else
		{
			outFile.open(fileName, std::ios::app);
		}
	do
		{
			justHours = false;
			getFromConsole();

			outFile << employeeID << " " << firstName << " " << lastName << " " << hours << " " << hourlyRate << std::endl;

			std::cout << firstName << " " << lastName << " was added to the employee file." << std::endl;
			system("pause");
			system("CLS");

			do
				{
					std::cout << "Enter another entry? (y/n): ";
					while(!(std::cin >> doAnother))
						{
							std::cout << "You need to enter a character value!" << std::endl;
							std::cin.clear();
							std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						}
				}
			while(doAnother != 'y' and doAnother != 'n');
		}
	while(doAnother == 'y');

	std::cout << "Employee File has been created with information you provided from the console." << std::endl;
}

//This function is to delete an employe from the employee file as well as the employee vectors.
//It used a temp file to store the employees information that you are keeping and bypasses
//The employees information you are deleting. It then renames the temp file to the
//Original employee file. To delete from the vectors it finds the location in memory
//Of each vector and deletes the portion that corresponds to the index of the selected employee.

void eraseFileLine()
{
	std::string line, eraseLine, path = "employeeFile.txt";
	std::ifstream fin;
	int index;

	listEmployees();


	std::cout << "What employee would you like to erase: ";
	while(!(std::cin >> index))
		{
			std::cout << "You need to enter an integer value!" << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	if(index < 0)
		{
			return;
		}
	try
		{
			eraseLine = empID.at(index-1);

			fin.open(path);
			std::fstream temp; // contents of path must be copied to a temp file then renamed back to the path file
			temp.open("temp.txt", std::ios::app);

			std::cin.ignore();
			while (std::getline(fin,line))
				{
					if (!((line.rfind(eraseLine, 0)) == 0))
						temp << line << std::endl;
				}

			std::cout << employeeFirstNames.at(index-1) << " " << employeeLastNames.at(index-1) << " was erased from the employee file." << std::endl;

			empID.erase(empID.begin() + (index - 1));
			employeeFirstNames.erase(employeeFirstNames.begin() + (index - 1));
			employeeLastNames.erase(employeeLastNames.begin() + (index - 1));
			payRate.erase(payRate.begin() + (index - 1));
			hoursWorked.erase(hoursWorked.begin() + (index - 1));


			temp.close();
			fin.close();

			const char * p = path.c_str(); // required conversion for remove and rename functions
			remove(p);
			rename("temp.txt", p);
		}
	catch(...)
		{
			std::cout << "You entered an invalid selection. Aborting to the menu..." << std::endl;
			return;
		}

}

//This function is used as a decision for which employee to print a chck for. It returns
//The index of the employee the user is wanting to print for.

int printWhichEmployee()
{
	int index;

	listEmployees();

	std::cout << "Which employee would you like to print a check for: ";
	while(!(std::cin >> index))
		{
			std::cout << "You need to enter an integer value!" << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	try
		{
			empID.at(index-1);
			return index-1;
		}
	catch(...)
		{
			return -1;
		}
}

//This function is used to generate a psuedo-random employee file of
//Any given size (maximum value is 18,446,744,073,709,551,615)
//There are theoretically 833,333 unique combinations of first names,
//Last names, hours worked, and pay rate that this function can generate.

void generateEmpFile()
{
	const std::vector<std::string> firstNames = {"Maricela","Delaine","Sharlene","Doris","Jesusita","Julieta","Florence","Filiberto","Harriett","Obdulia","Felicidad","Dominica","Leena","Verna","Kimber","Adolph","Casandra","Micheline","Emma","Jasmin","Kimberlee","Danny","Nola","Neomi","Jenell","Ozell","Marilu","Nancy","Modesto","Dannielle","Jannet","Madalene","Charlena","Kenna","Danilo","Jamel","Fabiola","Louis","Josette","Eric","Manda","Edna","Nan","Mattie","Susana","Adrianna","Cira","Kimbra","Necole","Natalya"};
	const std::vector<std::string> lastNames = {"Davis","Henry","Lewis","Mann","Ford","Schmidt","O'Reilly","Jacobs","Sherman","Howells","Walters","Warner","Schroeder","King","Lucas","Bright","Perry","Goodwin","Walton","Warren","Lamb","Estrada","Christensen","Flynn","Schultz","Bowen","Andrews","Rice","Dean","Silva","Torres","Garza","Murray","Chen","Hill","Avila","Chandler","Perkins","Miles","Bennett","Dixon","Flowers","Jackson","Frank","Fischer","Logan","Nelson","Barnes","Hughes"};
	
	srand(static_cast<unsigned int>(time(NULL)));
	
	int hours = 0, id = 1, x = 1, indexFirst = 0, indexLast = 0;
	long long howMany = 0;
	float rate;
	std::string strID;
	
	std::ofstream outFile;
	outFile.open("employeeFile.txt");
	
	std::cout << "How many employees would you like to generate: ";
	while(!(std::cin >> howMany))
		{
			std::cout << "You need to enter an integer value!" << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	if(howMany <= 0)
	{
		std::cout << "You entered an invalid number. Aborting to the main menu." << std::endl;
		return;	
	}
	
	while(x <= howMany)
	{
		indexFirst = rand() % 50;
		indexLast = rand() % 49;
		hours = rand() % 80;
		rate = rand() % 100 + 8;
		strID = format_empid_number(id);
		outFile << strID << " " << firstNames.at(indexFirst) << " " << lastNames.at(indexLast) << " " << hours << " " << rate << std::endl;
		std::cout << strID << " " << firstNames.at(indexFirst) << " " << lastNames.at(indexLast) << " " << hours << " " << rate << std::endl;
		id++;
		x++;
	}
}

//This is the function used as the menu page. It returns true as long as option 8
//Is not selected. It uses a switch control structure to determine which option was
//Selected and calls the corresponding functions.

bool menuPageMain()
{
	int switchOption;
	std::cout << "Welcome to the Main Menu!" << std::endl;
	std::cout << "You have " << employeeFirstNames.size() << " employees in your payroll." << std::endl << std::endl;
	std::cout << "1. Print checks from current Employee file." << std::endl;
	std::cout << "2. View Current Employee File." << std::endl;
	std::cout << "3. Start a new Employee file." << std::endl;
	std::cout << "4. Append employee to current Employee file." << std::endl;
	std::cout << "5. Erase current Employee file." << std::endl;
	std::cout << "6. Erase specific employees record from file." << std::endl;
	std::cout << "7. Generate a random employee file." << std::endl;
	std::cout << "8. Exit from program." << std::endl;
	std::cout << "Please select the option you would like to do: ";
	while(!(std::cin >> switchOption))
		{
			std::cout << "You need to enter an integer value!" << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

	switch(switchOption)
		{
			case 1:
			{
				int index = 0;
				std::string whichOne;
				std::cout <<  "Printing checks from current file..." << std::endl;
				system("pause");
				system("CLS");
				index = printWhichEmployee();
				if(index < 0)
					break;
				whichOne = toConsoleOrFile();
				if(whichOne == "f")
					{
						drawCheckToFile(index);
					}
				else
					{
						drawCheckToConsole(index);
					}
				break;
			};
			case 2:
			{
				std::cout << "Dispaying current Employee File..." << std::endl;
				system("pause");
				system("CLS");
				displayEmployeeFile();
				break;
			};
			case 3:
			{
				appendFlag = false;
				std::cout << "Creating New Employee File..." << std::endl;
				system("pause");
				system("CLS");
				newEmployeeFile();
				break;
			};
			case 4:
			{
				appendFlag = true;
				std::cout << "Appending records to Employee File..." << std::endl;
				system("pause");
				system("CLS");
				newEmployeeFile();
				break;
			};
			case 5:
			{
				char sureEnough;
				std::cout << "Erasing Employee File..." << std::endl;
				do
					{
						std::cout << "Are you sure? (y/n): ";
						while(!(std::cin >> sureEnough))
							{
								std::cout << "You need to enter a character value!" << std::endl;
								std::cin.clear();
								std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
							}
					}
				while(sureEnough != 'y' and sureEnough != 'n');

				if(sureEnough == 'n')
					break;
				else
					{
						remove("employeeFile.txt");
						empID.clear();
						employeeFirstNames.clear();
						employeeLastNames.clear();
						payRate.clear();
						hoursWorked.clear();
						std::cout << "Employee File has been erased." << std::endl;
						break;
					}
				
			};
			case 6:
			{
				std::cout <<  "Erase specific employees record from file..." << std::endl;
				system("pause");
				system("CLS");
				eraseFileLine();
				break;
			};
			case 7:
			{
				std::cout << "Generating new employee file..." << std::endl;
				empID.clear();
				employeeFirstNames.clear();
				employeeLastNames.clear();
				payRate.clear();
				hoursWorked.clear();
				generateEmpFile();
				getFromFile();
				break;
			}
			case 8:
			{
				return false;
			};
			default:
			{
				system("CLS");
				std::cout << "You have entered an invalid choice. Please choose a number from the list." << std::endl;
				system("pause");
				system("CLS");
				return true;
			};
		};
	std::cout << std::endl << "Task Completed!" << std::endl;
	system("pause");
	system("CLS");
	return true;
}

//The main function initialy calles the function getFromFile to do the initial
//Read of the employee file. This is to ensure the employee file gets read.
//Then we have a do-while loop that calls the funtion menuPageMain and stores
//It's returned boolean value on a variable to check whether to loop again or end.

int main()
{
	bool continueProgram;
	getFromFile();
	do
		{
			continueProgram = menuPageMain();
		}
	while(continueProgram);
	return 0;
}
