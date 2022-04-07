#1. (2pts) Use the command chmod to modify the permissions to files and directories for
#owner, group and others. Create a shell script and give the current user full permission
#(read, write, and execution) and everyone else read and execute permission only.

chmod 755 myfile.txt

#2. (2pts) Create a FAT, NTFS, and EXT3 filesystem in your Linux box. Mount it in three
#separate subdirectories inside /mnt/ (say, myFat, myNTFS, and myEXT3).



#3. (2pts) Use ‘df’ command to test it.

df -h # -h flag gives human-readable output for filesystem sizes

#4. (2pts) Now create three different users called ‘testuser1’, ‘testuser2’, and
#‘testuser3’.

adduser testuser1
adduser testuser2
adduser testuser3

#5. (2pts) Create a group called ‘myfsuser’



#6. (2pts) Add all those three users to ‘myfsuser’ group



#7. (2pts) Set the permission of the ‘myFat’ directory so that only the members of
#‘myfsuser’ group can access it.



#8. (4pts) Now write a program (shell script) that will check if the current user has write
#permission to myFat, myNTFS and myEXT3. In three separate print statements let the
#user know about it. For example, if the user has write permission to ‘myFat’ print a
#message ‘you have permission to write in myFat. Otherwise, print a
#message ‘you do not have permission to write in myFat. And so on



#9. (2pts) Now modify the ~./bashrc file so that the user will automatically get the
#message right after login.
