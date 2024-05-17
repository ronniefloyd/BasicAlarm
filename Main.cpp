/*-------------------------------------------------
 * Author:          Ronnie Floyd
 * Written:         05/17/2024
 * Last Update:     05/17/2024
 *
 * Compilation:
 *     Open the solution in Visual Studio and build the project.
 *     Alternatively, use the Developer Command Prompt for Visual Studio:
 *     devenv YourSolution.sln /Build
 * Execution:
 *     Run the executable generated in the Debug or Release folder.
 *     Alternatively, use the Developer Command Prompt for Visual Studio:
 *     cd path\to\output\directory
 *     YourExecutable.exe
 * 
 * About:
 *	Basic alarm for windows. Written in C++ 23 utilizes modules, ensure word wrap on. Allows user to set a time limit for task.  * Plays alarm sounds once timer has depleted. 
 *
 *
 *
 *
 */
#include <windows.h>

import std;

/**
* set_final_time function passed two arguments system time and user defined time. Adds attributes hours and minutes seprately to create a future time. 
* 
*
*
*
*/
SYSTEMTIME set_final_time(SYSTEMTIME start, SYSTEMTIME user)
{
	//return variable
	SYSTEMTIME final_time;
	//set hours
	final_time.wHour = start.wHour + user.wHour;
	//edge case if time becomes greater than or equal to 24 hours put timer to next day.
	if (final_time.wHour >= 24)
	{
		final_time.wHour = final_time.wHour - 24;
	}
	//set minutes
	final_time.wMinute = start.wMinute + user.wMinute;
	//edge case minutes above 60 just subtract could also use modulo
	if (final_time.wMinute >= 60)
	{
		final_time.wMinute = final_time.wMinute - 60;
	}


	return final_time;


}
/**
* count_down passsed the parameter alarm_time, when alarm is set to go off. Utilized in thread as to keep active until alarm goes off. 
*
*
*
*
*/
void count_down(SYSTEMTIME alarm_time)
{
	//variable for while state
	bool counter_state = true;
	//used to exit
	char end;
	//SYSTEMTIME variable.
	SYSTEMTIME current;
	//Set local time. 
	GetLocalTime(&current);
	//While counter_state true 
	while (counter_state)
	{
		//continue to get local time
		GetLocalTime(&current);
		//if alarm time is equal to current time then play sound. 
		if (alarm_time.wHour == current.wHour && alarm_time.wMinute == current.wMinute)
		{
			//Play sound
			PlaySound(TEXT("emergency_alarm.wav"), NULL, SND_FILENAME | SND_ASYNC);
			//Allow user to exit
			std::cout << "Press e to exit and enter :";
			std::cin >> end;
			counter_state = false;

		}
		//else sleep
		else
		{
			std::cout << "Alarm is active waiting for countdown to complete: \n";
			std::this_thread::sleep_for(std::chrono::seconds(15));
		}

	}

}

int main()
{

	//SYSTEMTIME Variables
	SYSTEMTIME st, lt, set_time, final_time;
	// Using a char to ignore ':' input for the HH:MM format.
	// Consider using std::getline() in the future to read the input as a string and parse.
	char ignore;
	//Get system and local times pass address
	GetSystemTime(&st);
	GetLocalTime(&lt);
	//Show current system time and local time. 
	std::cout << "The current system time is: " << st.wHour << ':' << st.wMinute << '\n';
	std::cout << "The current local time is: " << lt.wHour << ':' << lt.wMinute << '\n';
	//Get time in hours and minutes from user. 
	std::cout << "Please enter a time limit in HH:MM format :\n";
	std::cin >> set_time.wHour >> ignore >> set_time.wMinute;
	//Call to method final_time passing two arguments system and user time limit
	final_time = set_final_time(lt, set_time);
	std::cout << "Alarm set for: " << final_time.wHour << final_time.wMinute << '\n';
	//thread
	std::thread counter(count_down, final_time);
	counter.join();
	return 0;
}
