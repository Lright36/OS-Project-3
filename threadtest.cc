// threadtest.cc
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield,
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

//Begin code changes by Luke Thibodeaux
#include "copyright.h"
#include "system.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <synch.h>

using namespace std;


  Thread* philo[1000];
  bool chopstick[1000];
  bool leaveCheck[1000];
  int current[1000];
  int philosopherCount;
  int mealCount;
  int mealsEaten;
  int deadLockCount[1000];
  int deadlockCurrent;
  int count=0;
  int breakCount=0;
  //End code changes by Luke Thibodeaux

//Begin code changes by Jude Thompson
  //global variable for shoutNum to be used, unable to pass with thread -> fork
  int globalShoutNum;
  int globalPeopleNum;
  int globalMailboxSize;
  int globalMessageNum;
  Semaphore **taskThreeSemaphores;
  char *messagesArray[6] = {"Ey bah!", "I done spunt that money!", "Ey girl, you like brass bands?", "Mais bruh!", "Nah son!", "How you got here?"} ;
  string **globalMailbox;

  void randomShout (char *message) {
  	//choose between five hard-coded messages with a random number 1-5
  	switch(Random() % 5 + 1) {
        case 1 :
           strcpy(message, messagesArray[0]);
           break;
        case 2 :
           strcpy(message ,messagesArray[1]);
           break;
        case 3 :
           strcpy(message, messagesArray[2]);
           break;
        case 4 :
           strcpy(message ,messagesArray[3]);
           break;
        case 5 :
           strcpy(message, messagesArray[4]);
           break;
        default :
           strcpy(message, messagesArray[5]);
     }
  }

  void
  TaskTwoThread(int threadID)
  {
  	char message[100];
  	for (int i = 0; i < globalShoutNum; i++) {
  		randomShout(message);
  		cout << "My name is thread " << threadID << " and I say: " << message << "\n";
  		//yield the cpu between 3 and 7 times
  		for (int j = 0; j < (Random() % 7 + 3);j++) {
  			currentThread->Yield();
  		}
          }
  }



  bool isValidNumber (char *input) {
  	bool validNumber = true;
  	int size = 0;
  	bool isInteger = true, isNegative = false, isDecimal = false, isString = false, isCharacter = false;
  	//check for negative dash or decimal in first character
  	int i = 0;
  	if (input[0] == '-') {
  		isNegative = true;
  		i++;
  		size++;
  	} else if (input[0] == '.') {
  		isDecimal=true;
  		i++;
  		size++;
  	}
  	//check for multiple decimal points
  	bool decimalFound = false;
  	//check if the input is decimal, integer, string etc.
  	for(i; input[i] != '\0'; i++) {
  		size++;
  		if (!(input[i] >= '0' && input[i] <= '9')) {
  			if (input[i] == '.' && decimalFound == false) {
  				isDecimal = true;
  				decimalFound = true;
  			} else {
  				isInteger = false;
  				isNegative = false;
  				isDecimal = false;
  				isString = true;
  				isCharacter = true;
  				break;
  			}
  		}

  	}
  	if (isNegative == true || isString == true || isCharacter == true || isDecimal == true) {
  		validNumber = false;
  	}
  	return validNumber;
  }



  void
  TaskOneThread(int which)
  {

      bool entering = true;
      while (entering)
      {
          char input[100];
          cout << "Please enter a string of at most 100 characters:";
          cin >> input;
  	//check for multiple decimal points
  	bool decimalFound = false;
  	int size = 0;
  	bool isInteger = true, isNegative = false, isDecimal = false, isString = false, isCharacter = false;
  	//check for negative dash or decimal in first character
  	int i = 0;
  	if (input[0] == '-') {
  		isNegative = true;
  		i++;
  		size++;
  	} else if (input[0] == '.') {
  		isDecimal=true;
  		decimalFound = true;
  		i++;
  		size++;
  	}

  	//check if the input is decimal, integer, string etc.
  	for(i; input[i] != '\0'; i++) {
  		size++;
  		if (!(input[i] >= '0' && input[i] <= '9')) {
  			if (input[i] == '.' && decimalFound == false && isCharacter == false && isString == false) {
  				isDecimal = true;
  				decimalFound = true;
  				cout << decimalFound;
  			} else {
  				isInteger = false;
  				isNegative = false;
  				isDecimal = false;
  				isString = true;
  				isCharacter = true;

  			}
  		}




  	}
  	//check if string is only a character
  	if (size == 1) {
  		//check for single decimal and dash
  		if (*input == '.' || *input == '-') {
  			isNegative = false;
  			isInteger = false;
  			isDecimal = false;
  			isCharacter = true;
  		}
  		isString = false;
  	} else {
  		isCharacter = false;
  	}

  	//if decimal, is not an integer
  	if (isDecimal == true) {
  		isInteger = false;
  	}
  	cout << "\n\ninteger: " << isInteger << "\nnegative: " << isNegative << "\ndecimal: " << isDecimal << "\nstring: " << isString << "\ncharacter: " << isCharacter << "\n";
  	cout << "\n\nDo you want to continue? (y/n):";
  	cin >> input;
  	if (*input == 'n')
  	{
  		entering = false;
  	}
      }
      //currentThread->Yield();

      /*
      int num;

      for (num = 0; num < 5; num++) {
  	printf("*** threa %d looped %d times\n", which, num);
          currentThread->Yield();
      }*/
  }

  void TaskFiveThread (int person)
  {
  	while (globalMessageNum > 0)
  	{

  		cout << "Hi, I'm person " << person << " and I am entering the post office!\n";
  		readMessages:
  		bool foundMessage = false;
  		int allFull = 0;
  		//go through own mailbox spot-by-spot and read a message if it is there
  		for (int i = (globalMailboxSize - 1); i > -1; i--)
  		{
  			if (globalMailbox[person][i] != "")
  			{
  				cout << "I am person " << person << " and I am reading a message in slot " << i << " of my mailbox!\n";

  				globalMailbox[person][i] = "";
  				taskThreeSemaphores[person]->V();
  				foundMessage = true;
  				break;
  			}
  		}


  		if (foundMessage == true)
  		{
  			goto readMessages;
  		}
  		//put messages into othe rrandom person's mailbox
  		int thisRandomNumber = -1;

  		while (thisRandomNumber == -1 || thisRandomNumber == person)
  		{
  			thisRandomNumber = Random() % (globalPeopleNum - 1) + 0;
  		}
  		string randomMessage = "hey";
  		//randomShout(randomMessage);

  		taskThreeSemaphores[thisRandomNumber]->P();
  		for (int j = 0; j < globalMailboxSize; j++)
  		{
  			if (globalMailbox[thisRandomNumber][j] == "")
  			{
  				cout << "--I am person " << person << " and I am putting a message into person " << thisRandomNumber << "'s mailbox in slot " << j << ".\n";
  				globalMailbox[thisRandomNumber][j] = randomMessage;

  				break;
  			}
  		}
  		globalMessageNum--;
  		cout << "I am person " << person << " and I am leaving the post office!\n";
  		for (int j = 0; j < (Random() % 7 + 3);j++) {
  			currentThread->Yield();
  		}

  	}
  	//final check of each person's mailbox
  	for (int k = 0; k < globalMailboxSize; k++)
  	{
  		if (globalMailbox[person][k] != "")
  			{
  				cout << "I am person " << person << " and I am reading a message in slot " << k << " of my mailbox!\n";
  				globalMailbox[person][k] = "";

  			}

  	} cout << "I am person " << person << " and I am leaving the post office!\n";
  }
//End code changes by Jude Thompson
//Begin code changes by Luke Thibodeaux
//Gets call to signify when a philosoper is ready to leave after all emals are eaten and refreshes if needed
  void leave(int philosopherNumber){
    if(count > -1){
    cout << "------------Philosopher " << philosopherNumber << " is waiting to leave" << endl;
    count --;
      currentThread->Yield();
    }
  }

//Performs an eat action and yields during the eating. When finished, makes the philosopher drop his chopstick
void eat(int philosopherNumber){
  if (mealCount != 0){
  mealCount--;
  mealsEaten++;
  cout << "-------Philosopher " << philosopherNumber << " begins to eat (" << mealsEaten << " meals have been eaten so far)" << endl;
  currentThread->Yield();
  cout << "--------Philosopher " << philosopherNumber << " finishes eating" << endl;
  if (mealCount == 0){
    cout << "----------All meals have been eaten!" << endl;
  }
  cout << "---------Philosopher " << philosopherNumber << " drops chopstick [" << philosopherNumber << "]" << endl;
  chopstick[philosopherNumber] = false;
  if (chopstick[0] == true && current[philosopherNumber] == (philosopherCount-1)){
    cout << "----------Philosopher " << philosopherNumber << " drops chopstick [0]" << endl;
    chopstick[0] = false;
  }
  else{
    cout << "----------Philosopher " << philosopherNumber << " drops chopstick [" << philosopherNumber+1 << "]" << endl;
    chopstick[philosopherNumber+1] = false;
  }
  }
}

//Does all the math to determine deadlock, chopstick interactions and calls the eating function, while leaving and ending the thread at it's exit

void chopPickup(int (philosopherNumber)){
  if (mealCount != 0){
  if(current[philosopherNumber] != philosopherNumber){
      cout << "----Philosopher " << philosopherNumber << " is trying to pick up chopstick [" << philosopherNumber << "]" << endl;
    }
  if (deadLockCount[philosopherNumber] == 5){
    cout << "-----Philosopher " << philosopherNumber << " abort picking up chopstick [" << philosopherNumber << "] after 5 trials to prevent deadlock!"<< endl;
    deadLockCount[philosopherNumber] = 0;
    currentThread->Yield();
  }
  else if (deadLockCount[philosopherNumber+1] == 5 && philosopherNumber == (philosopherCount-1)){
    cout << "-----Philosopher " << philosopherNumber << " abort picking up chopstick [0] after 5 trials to prevent deadlock!"<< endl;
    deadLockCount[philosopherNumber+1] = 0;
    chopstick[philosopherNumber] = false;
    currentThread->Yield();
  }
  else if (deadLockCount[philosopherNumber+1] == 5){
    cout << "-----Philosopher " << philosopherNumber << " abort picking up chopstick [" << philosopherNumber+1 << "] after 5 trials to prevent deadlock!"<< endl;
    deadLockCount[philosopherNumber+1] = 0;
    chopstick[philosopherNumber-1] = false;
    currentThread->Yield();
  }
  else{
    if (chopstick[philosopherNumber] == false){
      cout << "-----Philosopher " << philosopherNumber << " has picked up chopstick [" << philosopherNumber << "]" << endl;
      chopstick[philosopherNumber] = true;
      current[philosopherNumber] = philosopherNumber;
      if (philosopherNumber == (philosopherCount-1)){
        cout << "----Philosopher " << philosopherNumber << " is trying to pick up chopstick [0]" << endl;
      }
      else{
        cout << "----Philosopher " << philosopherNumber << " is trying to pick up chopstick [" << philosopherNumber+1 << "]" << endl;
      }
    }
    else if (chopstick[philosopherNumber] == true){
      deadLockCount[philosopherNumber]++;
        currentThread->Yield();
      }
    if(philosopherNumber == (philosopherCount-1)){
      if(chopstick[0] == false){
        cout << "-----Philosopher " << philosopherNumber << " has picked up chopstick [0]" << endl;
        chopstick[0] = true;
        current[0] = philosopherNumber;
      }
      else if (chopstick[0] == true){
        deadLockCount[philosopherNumber]++;
        currentThread->Yield();
      }
    }
    else if (chopstick[philosopherNumber+1] == false){
      cout << "-----Philosopher " << philosopherNumber << " has picked up chopstick [" << philosopherNumber+1 << "]" << endl;
      chopstick[philosopherNumber+1] = true;
      current[philosopherNumber+1] = philosopherNumber;
    }
    else if (chopstick[philosopherNumber] == true){
      deadLockCount[philosopherNumber]++;
      currentThread->Yield();
    }
    if (philosopherNumber == (philosopherCount-1) && current[philosopherNumber] == current[0] || current[philosopherNumber] == current[philosopherNumber+1] && chopstick[philosopherNumber] == true && (chopstick[philosopherNumber+1] == true || chopstick[0] == true)){
      eat(philosopherNumber);
      cout << "-----------Philosopher " << philosopherNumber << " Thinking" << endl;
      currentThread->Yield();
    }
  while (mealCount < 1){
  leave(philosopherNumber);
  if (count == 0){
    cout << "$$$$$ All Philosophers leave the table together. $$$$$ " << endl;
    cout << "$$$$$World Peace! $$$$$ " << endl;
    count--;
  }
    currentThread->Yield();
    if (mealCount < 1){
    currentThread->Finish();
    }
      }
    }
  }
}

//Peforms yields forcing each philosopeher to enter and wait until they are all present to sit

void busyWaiting(int philosopherNumber){
  cout << "-Philosopher " << philosopherNumber << " has joined the room" << endl;
  cout << "--Philosopher " << philosopherNumber << " is waiting to sit" << endl;
  count++;
    if (count == philosopherCount){
      cout << "---All philosophers sit down at the table" << endl;
    }
    currentThread->Yield();
    while (mealCount > 0){;
      chopPickup(philosopherNumber);
    }
  }

//Does a check to ensure the input given from main is a valid positive integer

bool isValidNumberLuke (char *input) {
	bool validNumber = true;
	int size = 0;
	bool isInteger = true, isNegative = false, isDecimal = false, isString = false, isCharacter = false;
	//check for negative dash or decimal in first character
	int i = 0;
	if (input[0] == '-') {
		isNegative = true;
		i++;
		size++;
	} else if (input[0] == '.') {
		isDecimal=true;
		i++;
		size++;
	}
	//check for multiple decimal points
	bool decimalFound = false;
	//check if the input is decimal, integer, string etc.
	for(i; input[i] != '\0'; i++) {
		size++;
		if (!(input[i] >= '0' && input[i] <= '9')) {
			if (input[i] == '.' && decimalFound == false) {
				isDecimal = true;
				decimalFound = true;
			} else {
				isInteger = false;
				isNegative = false;
				isDecimal = false;
				isString = true;
				isCharacter = true;
				break;
			}
		}

	}
	if (isNegative == true || isString == true || isCharacter == true || isDecimal == true) {
		validNumber = false;
	}
	return validNumber;
}
//End code changes by Luke Thibodeaux
//Main Function for Task 3 which propts for amount of philosophers and meals to pass to other functions
//Begin code changes by Jude Thompson
void
ThreadTest()
{
  if (task_number == 1) {
    DEBUG('t', "Entering ThreadTest");

        Thread *t = new Thread("forked thread");

      t->Fork(TaskOneThread, 1);
  } else if (task_number == 2) {
  DEBUG('t', "Entering ThreadTest");
  char strThreadNum[100];
  char strShoutNum[100];
  bool entering = true;
  while (entering == true) {
    cout << "Please enter a postive number of threads to be in the shouting match:";
    cin >> strThreadNum;
    cout << "Please enter a positive number of shouts that the threads should complete: ";
    cin >> strShoutNum;
    if (isValidNumber(strThreadNum) == true && isValidNumber(strShoutNum) == true) {
      entering = false;
    } else {
      cout << "\n\nInput is invalid. Please enter positive whole numbers for your input. \n\n";
    }
  }

  //change input into integers
  int threadNum = atoi(strThreadNum);
  globalShoutNum = atoi(strShoutNum);
  //create new threads
  for (int i = 0; i < globalShoutNum; i++) {
    Thread *t = new Thread("forked thread");
    t ->Fork(TaskTwoThread, i);
  }
} else if (task_number == 5) {

  DEBUG('t', "Entering ThreadTest");
  char peopleNum[100];
  char mailboxSize[100];
  char messageNum[100];
  bool entering = true;
  while (entering == true) {

    cout << "Please enter a positive number (greater than two) of people to be in the post office: ";
    cin >> peopleNum;
    cout << "Please enter a positive number of slots (greater than two) to be in each mailbox: ";
    cin >> mailboxSize;
    cout << "Please enter a positive number of total messages to be sent: ";

    cin >> messageNum;

    if (isValidNumber(messageNum) == true && isValidNumber(mailboxSize) == true && isValidNumber(peopleNum) == true && atoi(peopleNum) > 2 && atoi(messageNum) > 1) {
      entering = false;
    } else {
      cout << "\n\nInput is invalid. Please enter positive whole numbers for your input. \n\n";
    }
  }

  //change input into integers
  globalPeopleNum = atoi(peopleNum);
  globalMailboxSize = atoi(mailboxSize);
  globalMessageNum = atoi(messageNum);
  //establish semaphore number

  taskThreeSemaphores = new Semaphore*[globalPeopleNum];

  for (int j = 0; j < globalPeopleNum; j++)
  {
    taskThreeSemaphores[j] = new Semaphore("this semaphore",globalMailboxSize);
  }
  //create mailbox
  globalMailbox = new string*[globalPeopleNum];
  for (int k = 0; k < globalPeopleNum; k++)
  {
    globalMailbox[k] = new string[globalMailboxSize];
    for (int l = 0; l < globalMailboxSize;  l++)
    {
      globalMailbox[k][l] = "";
    }

  }
  //create new threads
  for (int i = 0; i < globalPeopleNum; i++) {

    Thread *t = new Thread("forked thread");
    t ->Fork(TaskFiveThread, i);
  }
}

//End code changes by Jude Thompson
//Begin code changes by Luke Thibodeaux
  else if (task_number == 3) {
  DEBUG('t', "Entering ThreadTest");
  char strPhiloNum[100];
  char strMealNum[100];
  bool entering = true;
  while (entering == true) {
    cout << "Enter number of philosophers (under 1000): ";
    cin >> strPhiloNum;
    cout << "Enter the total number of meals to be eaten (under 1000): ";
    cin >> strMealNum;
    if (isValidNumberLuke(strPhiloNum) == true && isValidNumberLuke(strMealNum) == true && atoi(strPhiloNum) < 1001 && atoi(strMealNum) < 1001) {
        entering = false;
    }
    else {
      cout << "\n\nInput is invalid. Please enter positive whole numbers under 1000 for your input. \n\n";
      }
      }
      philosopherCount = atoi(strPhiloNum);
      mealCount = atoi(strMealNum);
    for(int i = 0; i<philosopherCount ;i++) {
      Thread *t = new Thread("person");
			t ->Fork(busyWaiting, i);
    }
  }
}
//End code changes by Luke Thibodeaux
