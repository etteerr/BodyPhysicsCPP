#include <iostream>
#include <string>




using namespace std;

void prthelp() {
	cout << "GravitySim1-multi usage:"
			<< "	"
			<< endl;
}

int main(int nargs, char * args[]) {
	for (int i = 0; i < nargs; i++)
		cout << "input " << i << ": " << args[i] << endl;
	if (nargs < 2) {
		cout << "Incorrect usage" << endl;
		return 1;
	}

	for (int i = 0; i < nargs; i++) {
		//Print help and quit
		if (string(args[i]) == "--help" || string(args[i]) == "-h") {
			prthelp();
			return 0;
		}

	}

	return 0;
}
