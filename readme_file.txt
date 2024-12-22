https://askubuntu.com/questions/396654/how-to-run-a-python-program-in-the-background-even-after-closing-the-terminal

Use the shebang line in your python script. Make it executable using the command,

chmod +x test.py
Use no hangup to run the program in the background even if you close your terminal,

nohup /path/to/test.py &
or simply (without making any change in your program)

nohup python /path/to/test.py &
Do not forget to use & to put it in the background.

Role of nohup: nohup makes your script ignore SIGHUP, and redirects stdout/stderr to a file nohup.out, so that the command can continue running in the background after you log out. If you close the shell/terminal or log off, your command is no longer a child of that shell. It belongs to init process. If you search in pstree you'll see it is now owned by process 1 (init).

To see the process again, use in terminal,

ps ax | grep test.py
That cannot be brought back to the foreground because the foreground (as the terminal already closed) no longer exists. So there is no way to get that terminal back again once it is closed.
