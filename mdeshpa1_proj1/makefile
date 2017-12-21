

# Specify the compiler options or flags, linking libraries
# and any debugging information flags as a variables
CC = gcc

#
# Specify the target
all: process_hierarchy_main


# Specify how the executables should be created from source files
process_hierarchy_main: process_hierarchy_main.c
			  $(CC) process_hierarchy_main.c -o process_hierarchy_main

# Specify the object files and executables that are generated
# and need to be removed to re-compile the whole thing
clean:
	rm -f process_hierarchy_main




