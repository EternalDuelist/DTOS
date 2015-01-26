This the README for Justin Barros (jbarros) and David Carranza's(dcarran2) 
Assignment 1 Submission

This shell is capable of executing simple commands and can only execute one
redirect or pipe per line.

To compile our program just enter "make" and an executable called
"myshell" will be made.

To run our program you must type "./myshell". 
You will know our shell is running because all lines are prefixed with "SwagShell>"

Available make commands:

	make all	Compiles the executable myshell file

	make clean 	Removes all generated files except for source files in the
			current directory.

Known errors with our program:

	After succesfully doing a piping job our shell will inexplicably exit

	If the files we submit have "^M" as new lines, then typing in vi
	":%s/^M//g" (Where "^M" is: ctrl v + ctrl m) will remove them.

Special Notes:

	Justin will be submiting the entire project and David will submit a
	README

	In order to do this project we used Darrell's starter code, looked at lots
	of different man pages (both on minix and found through google searches) for
	the various system calls, piazza, and looked at a few tutorials on how to do
	certain things listed here:

		> http://www.tldp.org/LDP/lpg/node11.html
		> http://en.wikipedia.org/wiki/File_descriptor
		> http://stackoverflow.com/questions/503878/how-to-know-what-the-errno-means
		> https://www.youtube.com/watch?v=ZAc3q2jzXbI

	Hope you don't mind the swag, it was all for comedic effect.