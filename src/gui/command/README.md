# Note of add a new command

1. add class of command, public CmdBase, define command type
2. go to cmddefine.h, add command string and command enum
2. go to cmddefine.cpp, able to get command string and command enum each other
3. add an icon for the command in cmdicon.cpp
4. Now, take your way to the file cmdregister.cpp, assign the group of command(the command is visiblable by user is also defined here), then create command