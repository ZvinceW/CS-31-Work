#include <iostream>
#include <string>
using namespace std;

int main()
{
	string id;
	double revenue;
	string country;
	double licenseFee;

	cout << "Identification: ";
	getline(cin, id);
	cout << "Expected revenue (in millions): ";
	cin >> revenue;
	cin.ignore(10000, '\n');
	cout << "Country: ";
	getline(cin, country);
	
	cout << "---" << endl;

	if (id == "")
	{
		cout << "You must enter a property identification." << endl;
		return 1;
	}

	if (country == "")
	{
		cout << "You must enter a country." << endl;
		return 1;
	}

	if (revenue < 0)
	{
		cout << "The expected revenue must be nonnegative. " << endl;
		return 1;
	}
	else if (revenue <= 20)
	{
		licenseFee = revenue * 0.181;
	}
	else if (revenue > 20 && revenue <= 50)
	{
		if (country == "Turkey" || country == "UAE")
		{
			licenseFee = (revenue - 20) * 0.217 + 20 * 0.181;
		}
		else
		{
			licenseFee = (revenue - 20) * 0.203 + 20 * 0.181;
		}
	}
	else 
	{
		if (country == "Turkey" || country == "UAE")
		{
			licenseFee = (revenue - 50) * 0.23 + 20 * 0.181 + 30 * 0.217;
		}
		else
		{
			licenseFee = (revenue - 50) * 0.23 + 20 * 0.181 + 30 * 0.203;
		}
	}

	cout.setf(ios::fixed);
	cout.precision(3);
	cout << "The license fee for " << id << " is $" << licenseFee << " million." << endl;
	
	


}