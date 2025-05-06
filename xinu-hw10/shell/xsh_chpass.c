/**
 * @file     xsh_chpass.c
 * @provides xsh_chpass
 *
 */
/* Embedded XINU, Copyright (C) 2024.  All rights reserved. */

#include <xinu.h>

/**
 * Shell command (chpass) changes an existing user password.
 * @param args array of arguments
 * @return OK for success, SYSERR for errors.
 */
command xsh_chpass(int nargs, char *args[])
{
/**
 * TODO:
 * This function creates a new password for an existing user.
 * You may break this task down into any number of new helper
 * functions within this file, and also may rely on helper functions
 * that already exist, such as getusername(), hasspassword(), and
 * passwdFileWrite().
 *
 * Steps:
 * 1) If no user name was provided to the shell command, superusr should
 *    be prompted for one.  Normal users are not prompted, because we
 *    default to changing their own password.
 * 2) Search for the user name in the usertab.
 * 3) If the current user is not superusr, prompt for the previous password.
 *    Prompt text = "Enter previous password for user %s: ".
 * 4) If the hash of the previous password matched what is on record,
 *    prompt for new password.
 *    Prompt test = "Enter new password for user %s: ".
 * 5) Place the new password hash into the user entry, and commit to disk.
 * 6) Printf "Successfully changed password for user ID %d\n" with user ID.
 *
 * Errors to watch for:
 * 1) There is not already a user logged in.
 *    Error text = "Must login first\n".
 * 2) The logged in userid is not SUPERUID, but is trying to change someone
 *    else's password.
 *    Error text = "ERROR: Only superusr can change other passwords!\n".
 * 3) The given user name cannot be found in the existing user table.
 *    Error text = "User name %s not found.\n".   
 * 4) The password change failed.  (i.e., passwords didn't match.)
 *    Error text = "Password for user %s does not match!\n".
 */

	//Make sure we are logged in
	if (userid < SUPERUID) {
		printf("Must login first\n");
		return SYSERR;
	}

	//printf("//we are logged in\n");

	char username[MAXUSERLEN + 1];
	bzero(username, MAXUSERLEN+1);

	if (userid == SUPERUID) {
		//Get username from argument, or prompt them
		if (nargs < 2) {
			getusername(username, MAXUSERLEN);
		} else {
			strncpy(username, args[1], MAXUSERLEN); //copy from second argument
		}
		//printf("//we are superuser, and got this username:");
		//int i;
		//for (i = 0; i < MAXUSERLEN; i++) printf(" %02X", username[i]);
		//printf(" which is %s\n", username);
	} else {
		//Get username from argument, or just infer
		if (nargs < 2) {
			strncpy(username, usertab[userid].username, MAXUSERLEN);
		} else {
			strncpy(username, args[1], MAXUSERLEN); //copy from second argument
			//are we trying to change someone else's password?
			if (strncmp(username, usertab[userid].username, MAXUSERLEN) != 0) {
				printf("ERROR: Only superusr can change other passwords!\n");
				return SYSERR;
			}
		}
		//printf("//we are not superuser, and got the username %s\n", username);
	}

	int id = searchname(username);
	//printf("//id = %d\n", id);
	if (id < SUPERUID || id > MAXUSERS) {
		printf("User name %s not found.\n", username);
		return SYSERR;	
	}

	if (userid != SUPERUID) {
		printf("Enter previous password for user %s: ");
		ulong passhash = hashpassword(usertab[id].salt);	
		if (passhash != usertab[id].passhash) {
			printf("Password for user %s does not match!\n", username);
			return SYSERR;
		}
	}

	printf("Enter new password for user %s: ");
	usertab[id].passhash = hashpassword(usertab[id].salt);

	passwdFileWrite();

	printf("Successfully changed password for user ID %d\n", id);

	
    return OK;
}
