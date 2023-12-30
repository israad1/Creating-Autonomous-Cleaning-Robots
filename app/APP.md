# Applications

## Input File

input.csv needs to be the name of the input file.  It contains the information that the application will use to build robots and rooms.

inputtemplate.csv contains a template for how to write the input file, but here are further specifications about each property in the file:

robots:

[robot name] - must be contained all in one word, no spaces, should contain at least 1 letter

[robot size] - must be "small" or "large"

[robot type] - must be "sweeper", "mopper", or "scrubber"

rooms:

[room name] - must be contained all in one word, no spaces

[room length] - must be a positive float

[room width] - must be a positive float

[room sweepable] - must be "true" if the room is sweepable, "false" if the room is not

[room moppable] - must be "true" if the room is moppable, "false" if the room is not

[room scrubbable] - must be "true" if the room is scrubbable, "false" if the room is not

The robot fleet management system will be a high level application.
It will need to communicate with a simulator and a back end storage system.
Any other apps you require may be included in here.

By convention, these applications use the libaries you create.
So maybe you have a library of robots, then the application should include the header for it like `say_hello` does with `libhello`.