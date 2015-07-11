#ifndef SHUTDOWNWORKERS_H
#define SHUTDOWNWORKERS_H
/*
	shutdownWorkers.h
	V1.0
	worker classes for the shutdownManager.cpp program

	Author: Greg Stewart
	Copyright 2014 Greg Stewart
*/

/*
	Includes
*/
//c++ libraries
#include <iostream>//to do console stuff
#include <cstdlib>//the c standard library
#include <string>//for strings
#include <time.h>//for time measurement
#include <thread>//for multithreading
#include <mutex>//for handling resource management between threads
//for checking for keyboard interrupts
#ifdef _WIN32//code for windows
#include <conio.h>
#else//code for UNIX
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#endif
//my custom files

//usings
using namespace std;

#ifdef _WIN32
#else
//unix-system specific function to equate to getch()
int _getch( ) {
  struct termios oldt, newt;
  int ch;
  tcgetattr( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
  return ch;
}
#endif

/*
set mutex
*/
mutex m;

/*
set globals available in mutex 
*/
bool interrupted = false;//if the wait has been interrupted

////////////////////////////////
////	Functions start		////
////////////////////////////////
/*
	To get an input (char) fron the user

	if an invalid thing entered, simply returns a space
*/
char getCharFromUser(){
	char input = ' ';
	//get input (in the form of an int)
	try{//get the input
		cin >> input;
	}
	catch (char* error){
		input = ' ';
	}//try/catch
	cin.clear(); //clear stream
	cin.ignore(numeric_limits<streamsize>::max(), '\n');//ignore leftover data
	//cout << "Input: " << choice;//debugging
	
	return input;
}

/*
To get an input fron the user

if an invalid thing entered, simply returns 0
*/
int getIntFromUser(){
	int input;
	//get input (in the form of an int)
	try{//get the input
		cin >> input;
	}
	catch (int* error){
		input = 0;
	}//try/catch
	cin.clear(); //clear stream
	cin.ignore(numeric_limits<streamsize>::max(), '\n');//ignore leftover data
	//cout << "Input: " << choice;//debugging

	return input;
}

/*
To get an unsigned input fron the user

if an invalid thing entered, simply returns 0
*/
unsigned int getUIntFromUser(){
	int input;//input from the user
	unsigned int uInput;//what we will pass back after making sure validity
	//get input (in the form of an int)
	try{//get the input
		cin >> input;
	}
	catch (int* error){
		input = 0;
	}//try/catch
	cin.clear(); //clear stream
	cin.ignore(numeric_limits<streamsize>::max(), '\n');//ignore leftover data
	//cout << "Input: " << choice;//debugging

	//get absolute value, to eliminate negatives
	uInput = abs(input);

	return uInput;
}

/*
	to shutdown the computer
*/
bool shutdown(){

	#ifdef _WIN32//code for windows
	system("shutdown /s");

	#else//code for UNIX
	system("shutdown -h now");
	#endif
	return true;
}//shutdown()

/*
	to restart the computer
*/
bool restart(){
	#ifdef _WIN32//code for windows
	system("shutdown /r");

	#else//code for UNIX
	system("shutdown -r now");
	#endif

	return true;
}//restart()

/*
	to hibernate
*/
bool hibernate(){

	#ifdef _WIN32//code for windows
	system("shutdown /h /f");

	#else//code for UNIX
	system("pm-hibernate");
	#endif
	return true;
}//hibernate()

bool compSleep(){

#ifdef _WIN32//code for windows
	system("shutdown /s /hybrid /f");

#else//code for UNIX
	system("pm-suspend-hybrid");
#endif
	return true;
}//hybrid()

/*
to logout
*/
bool logout(){

#ifdef _WIN32//code for windows
	system("shutdown /l /f");

#else//code for UNIX
	//system("gnome-session-save --force-logout");
	cout<<"Logout not supported for Linux/Unix"<<endl;
#endif
	return true;
}//logout()

/*
	to activate the appropriate power state function
*/
bool activateState(string &stateToGoTo){
	char state = ' ';
	bool result = true;
	/*
		do the right thing
	*/
	if (stateToGoTo == "Shutdown"){
		result = shutdown();
	}
	else if (stateToGoTo == "Restart"){
		result = restart();
	}
	else if (stateToGoTo == "Hibernate"){
		result = hibernate();
	}
	else if (stateToGoTo == "Sleep"){
		result = compSleep();
	}
	else if (stateToGoTo == "Logout"){
		result = logout();
	}
	else{
		//cout << "Invalid option entered" << endl;//debugging
		result = false;
		return false;
	}
	//return appropriately
	return result;
}//activateState(string)

/*
Thread to run to check for interrupts
*/
void checkInterrupt(){
	//prompt
	cout << "Press any key to cancel...";
	//check for interrupt
	char foo = _getch();
	//cout << endl << "input recieved." << endl;//debug
	//set flag
	m.lock();
	interrupted = true;
	m.unlock();
}//checkInterrupt

/*
	waitForX(int time)
*/
bool waitForX(int timeToWait){
	bool finished = false;//if we have finished waiting
	interrupted = false;//if the wait has been interrupted
	time_t curTime = time(NULL);
	time_t timeToBe = curTime + timeToWait;
	//set the other thread to watch for interrupts
	thread check(checkInterrupt);
	check.detach();
	//cout << "Countdown finishes on: " << "/";
	do{
		//are we done waiting?
		curTime = time(NULL);
		if (timeToBe - curTime < 0){
			finished = true;
			//cout << "finished set to true." << endl;
		}
		else{
			//cout << "Time Left: " << (timeToBe - curTime) << endl;
		}
		//cout << "Interrupted: " << interrupted << endl;
	} while (!interrupted & !finished);

	return interrupted;
}//waitForX(int)


#endif
