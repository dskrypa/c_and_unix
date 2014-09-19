/**
	Homework 2
	Vending Machine
	Author: Douglas Skrypa
	Date: 2014.02.19
*/

#include <stdio.h>

// Function declarations
int validCoin(int);

/**
	Main loop for the program
*/
int main(void)
{
	int balance = 0;	// Balance in cents
	int inputVal;		// Value to be provided via user input

	printf("Welcome to the Soda Vending Machine\n");
	printf("===================================\n");
	printf("All sodas cost $1.50 each\n");
	printf("This machine accepts the following coins:\n");
	printf("nickel = 5, dime = 10, quarter = 25, half-dollar = 50, dollar = 100\n");
	printf("Please use the numeric value corresponding to each coin\n");

	// Ask user for input until the balance is 150 cents or more
	while(balance < 150){
		printf("Deposit a coin: ");
		scanf("%d", &inputVal);		// Accept integer values for cent value

		if(validCoin(inputVal)==1){
			// Add the value to the total balance if it is valid
			balance += inputVal;
			printf("%s $%1.2f\n", "Current balance: ", balance/100.0);
		} else {
			printf("Invalid coin.  This machine only accepts dollars, half-dollars, quarters, dimes, and nickels.\n");
		}
	}

	balance -= 150;
	printf("Dispensing soda...\n");
	printf("Your change:");

	if(balance > 0){
		int firstRun = 1;

		while(balance > 0){
			if(firstRun != 1){
				printf(",");	// Print a ',' between each coin in change
			}
			firstRun = 0;

			// Give change in the largest coin denominations possible
			if(balance >= 50){
				balance = balance - 50;
				printf(" half-dollar");
			} else if(balance >= 25){
				balance = balance - 25;
				printf(" quarter");
			} else if(balance >= 10){
				balance = balance - 10;
				printf(" dime");
			} else if(balance >= 5){
				balance = balance - 5;
				printf(" nickel");
			}
		}
	} else {
		printf(" none");	// No change if exact amount was provided
	}
	printf("\n");
	return 0;
}

/**
	Determines whether the given value corresponds with a valid type of coin
	@param val integer value of a potential coin
	@return 1 if it is a valid coin, 0 otherwise
*/
int validCoin(int val)
{
	switch(val){
		// Valid coin types are dollar, half-dollar, quarter, dime, and nickel
		case 100:
		case 50:
		case 25:
		case 10:
		case 5:
			return 1;
		default:
			return 0;
	}
}
