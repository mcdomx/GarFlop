# GarFlop
Garmin File Operations Utility

This UNIX utlity will copy or move file from your computer to your Garmin device.

User settings are stored in a settings.txt file.
1 - folder_search - Specify folders to search (1-current, 2-downloads, 3-entire drive)
2 - units - display route distances in 1-metric or 2-imperial.
3 - path_to_garmin - fulle path to garmin folder to copy/move files to.
4 - move_copy_ask - 1-always move files (deleting source after confirmed copy), 2-always copy, 3-ask the user.


Execute file with no options: garflop
Execute using terminal from directory that garflop is located.
(optionally, put the garflop directory in the PATH statement)

Program will search for gpx and fit files and present them in a list to the user.

User enters the number of the desired item in the list to initiate move/copy.

Once file is moved/copied, a confirmation message is displayed and the user is presented with a new list of file based on a new search.

User enters Q to exit program. (E, EXIT, exit, Exit, ^c, ^d will also work).
