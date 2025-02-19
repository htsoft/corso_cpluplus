// Command Injection
#include <stdio.h>
main()
{
	char user_input[100];
	int int_in;
	char password[10] = "Password1";

	printf("Enter an integer\n");
	scanf("%d", &int_in);
	printf("Please enter a string\n");
	fgets(user_input, sizeof(user_input), stdin);

	printf(user_input); // Safe version is: printf("%s", user_input);
	printf("\n");

	return 0;
}

// SQL Injection
/*
SELECT User.UserID
FROM User
WHERE User.UserID = ' " + UserID + " '
AND User.Pwd = ' " + Password + " '

SELECT User.UserID
FROM User
WHERE User.UserID = ''; DROP TABLE User; --'AND Pwd = ''OR"='
*/