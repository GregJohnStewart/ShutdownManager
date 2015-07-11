/*
	Shutdown Manager
	V1.0

	Author: Greg Stewart
	Started: 9/12/14

	Simple utility, can shutdown the computer according to different scenarios

	Code present for different operating systems (windows/linux)

	Uses system calls to the operating systeem, same as if you were in command prompt or terminal

	Uses preprocessors to deal with the differences in operating systems
	#ifdef _WIN32//define something for Windows
	#else//define it for a Unix machine
	#endif

	TODO:
	figure out how to to the at a certain time bit
*/

/*
	Includes
*/
//c++ libraries
#include <iostream>//to do console stuff
#include <cstdlib>//the c standard library
#include <string>//for strings
//my custom functions
#include "shutdownWorkers.h"//worker classes for this program

/*
	usings
*/
using namespace std;//for std stuff

/*
	Global Variables
*/


/*
	Program start
*/
int main(){
	/*
		Instance variables
	*/
	
	bool run = true;//LCV (loop control variable, for main running loop)
	char choice;//the choice the user makes in the main loop
	string destState = "Shutdown";//the state that the user wants to get to
	bool worked = true;//variable to catch results from shutdown attempts


	/*
		Print intro
	*/
	cout << "ShutdownManager" <<endl<<
		"V1.0" << endl << endl <<
		"Simple, quick automatic shutdown utility" << endl <<
		"Author: Greg Stewart" << endl <<
		"Copyriight 2014 Greg Stewart" << endl << endl;

	/*
		Print Notes
	*/

	cout << "Notes:" << endl <<
#ifdef _WIN32//define something for Windows
		//"\t-windows-specific note"<<endl<<
#else//define it for a Unix machine
		"\t-Must be run as sudo user on Linux/Unix (mac)" << endl <<
		"\t-Due to the many different user session managers used in different"<<endl<<
		"\t\tdistrobutions of Linux/Unix, Logging out automatically is not"<<endl<<
		"\t\tsupported."<<endl<<
		//"\t-For hibernating on Linux/Unix (mac), make sure the command 'pm-hibernate' is available" << endl <<
#endif
		"\t-Simply choose the option you want and minimize this window"<<endl<<
		"\t-You can continue to do work after you have set this program to go"<<endl<<
		endl;


	/*
		Main running loop
	*/
	while (run){
		//reset needed variables
		choice = ' ';

		//prompt for input
		cout << "What would you like to do?" << endl << endl <<
			"\t1- " << destState << " now" << endl <<
			"\t2- " << destState << " after a certain amount of time" << endl <<
			"\t3- Switch intended power state (currently "<<destState<<")"<< endl <<
			"\t0- Exit the program"<< endl << endl <<
			"Choice: ";
		//get the input from the user
		choice = getCharFromUser();
		//spacers
		cout << endl << endl;
		
		// OS- specific modifiers
		#ifdef _WIN32//define something for Windows
		#else//define it for a Unix machine
		#endif

		/*
			Main Switch:
		*/
		switch (choice){
		/*
			Shutdown Now
			main switch
		*/
		case '1':{
			cout << destState << " now..." << endl << endl;
			//do the thing immediately
			worked = activateState(destState);
			break;

		/*
			Shutdown after n amount of time
			main switch
		*/
		}case '2':{
			cout << destState << " after an amount of time..." << endl << endl;
			/*
			Instance Variables
			*/
			unsigned int hours = 0;//# of hours to wait to shutdown
			unsigned int minutes = 0;//# of minutes to wait to shutdown
			unsigned int seconds = 0;//# of seconds to wait to shutdown

			bool madeSelection = false;//if the user made a decision
			bool gopherGo = false;//if user is ready to set process to go (go-for-go)
			while (!madeSelection){
				cout << "Currently timer set to " << destState << " after:" << endl <<
					"\tHours:   " << hours << endl <<
					"\tMinutes: " << minutes << endl <<
					"\tSeconds: " << seconds << endl << endl;

				cout << "Make a selection:" << endl <<
					"\t1- Set Hours" << endl <<
					"\t2- Set Minutes" << endl <<
					"\t3- Set Seconds" << endl <<
					"\t4- Go" << endl <<
					"\t0- Skip" << endl <<
					"Choice: ";
				//get choice
				choice = getCharFromUser();
				//spacers
				cout << endl << endl;

				switch (choice){
				case '1':{
					//get input from user
					cout << "Enter the number of desired hours:" << endl;
					hours = getUIntFromUser();
					break;
				}case '2':{
					//get input from user
					cout << "Enter the number of desired minutes:" << endl;
					minutes = getUIntFromUser();

					break;
				}case '3':{
					//get inputfrom user
					cout << "Enter the number of desired seconds:" << endl;
					seconds = getUIntFromUser();
					break;
				}case '4':{
					madeSelection = true;
					gopherGo = true;
					break;
				}case '0':{
					madeSelection = true;
					gopherGo = false;
					break;
				}default:{
					cout << "Invalid input." << endl;
				}
				}//timed switch
				//spacer
				cout << endl;

				/*
				validity evaluation
				*/

				//check if over 60 seconds, if so, put those extra into minutes
				if (seconds >= 60){
					//cout << "Seconds over 60. Fixing..." << endl;//debugging
					//cout << "60 % seconds = " << (60 % seconds)<<endl;//debugging
					while (60 % seconds >= 1){
						minutes++;
						seconds -= 60;
					}
				}
				//check if over 60 minutes, if so, put those extra into hours
				if (minutes >= 60){
					//cout << "minutes over 60. Fixing..." << endl;//debugging
					//cout << "60 % minutes = " << (60 % seconds) << endl;//debugging
					while (60 % minutes >= 1){
						hours++;
						minutes -= 60;
					}
				}
			}//while(!madeSelection)

			//if going for the thing
			if (gopherGo){
				cout << "Going to " << destState << " after:" << endl <<
					"Hours:   " << hours << endl <<
					"Minutes: " << minutes << endl <<
					"Seconds: " << seconds << endl << endl;

				//setup needed variables
				unsigned int timeToWait = ((hours * 3600) + (minutes * 60) + seconds);//time, in millisecinds, to wait

				// wait for the specified time
				bool interrupted = waitForX(timeToWait);

				cout << endl;
				//if interrupted, do nothing
				if (interrupted){
					cout << "Countdown cancelled.";
				}
				else{
					cout << destState << " now...";
					//do the thing immediately
					//worked = activateState(destState);
				}
				cout << endl << endl;

			}//if good to countdown
			//not good to go
			else{
				cout << destState << " skipped." << endl << endl;
			}

			break;
		/*
			Shutdown at a certain time
			main switch
		*/
		
		/*
			Change the desired state
			main switch
		*/
		}case '3':{
			cout << "Change desired state" << endl << endl <<
				"Make a choice:" << endl <<
				"\t1- Shutdown" << endl <<
				"\t2- Restart" << endl <<
				"\t3- Hibernate" << endl <<
				"\t4- Sleep" << endl <<
				#ifdef _WIN32//define something for Windows
				"\t5- Logout" << endl <<
				#else//define it for a Unix machine
				#endif
				"\t0- Skip" << endl <<
				"Choice: ";
			//get input from user
			choice = getCharFromUser();
			//spacers
			cout << endl;
			
			// OS- specific modifiers
			#ifdef _WIN32//define something for Windows
			#else//define it for a Unix machine
			if(choice == '5'){
				choice = '9';
			}
			#endif

			switch (choice){
			case '1':{
				destState = "Shutdown";
				cout << "Program set to " << destState;
				break;
			}case '2':{
				destState = "Restart";
				cout << "Program set to " << destState;
				break;
			}case '3':{
				destState = "Hibernate";
				cout << "Program set to " << destState;
				break;
			}case '4':{
				destState = "Sleep";
				cout << "Program set to " << destState;
				break;
			}case '5':{
				destState = "Logout";
				cout << "Program set to " << destState;
				break;
			}case '0':{
				cout << "Desired state was not changed.";
				break;
			}default:{
				cout << "Invalid Input. Nothing changed.";
			}
			}//state switch
			cout << endl << endl;
			break;

		/*
			exit the program
			main switch
		*/
		}case '0':{
			cout << "Exiting the program..." << endl << endl;
			run = false;
			break;

		/*
			Invalid input
			main switch
		*/
		}default:{
			cout << "Invalid input." << endl << endl;
			break;
		}//cases
		}//main switch
	}//while(run)
	return 0;
}//main()
