# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Users/orahmoun/.brew/Cellar/cmake/3.25.0/bin/cmake

# The command to remove a file.
RM = /Users/orahmoun/.brew/Cellar/cmake/3.25.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/orahmoun/Webserv

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/orahmoun/Webserv

# Include any dependencies generated for this target.
include CMakeFiles/Webserv.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Webserv.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Webserv.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Webserv.dir/flags.make

CMakeFiles/Webserv.dir/main.cpp.o: CMakeFiles/Webserv.dir/flags.make
CMakeFiles/Webserv.dir/main.cpp.o: main.cpp
CMakeFiles/Webserv.dir/main.cpp.o: CMakeFiles/Webserv.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/orahmoun/Webserv/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Webserv.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Webserv.dir/main.cpp.o -MF CMakeFiles/Webserv.dir/main.cpp.o.d -o CMakeFiles/Webserv.dir/main.cpp.o -c /Users/orahmoun/Webserv/main.cpp

CMakeFiles/Webserv.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Webserv.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/orahmoun/Webserv/main.cpp > CMakeFiles/Webserv.dir/main.cpp.i

CMakeFiles/Webserv.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Webserv.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/orahmoun/Webserv/main.cpp -o CMakeFiles/Webserv.dir/main.cpp.s

CMakeFiles/Webserv.dir/Request.cpp.o: CMakeFiles/Webserv.dir/flags.make
CMakeFiles/Webserv.dir/Request.cpp.o: Request.cpp
CMakeFiles/Webserv.dir/Request.cpp.o: CMakeFiles/Webserv.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/orahmoun/Webserv/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Webserv.dir/Request.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Webserv.dir/Request.cpp.o -MF CMakeFiles/Webserv.dir/Request.cpp.o.d -o CMakeFiles/Webserv.dir/Request.cpp.o -c /Users/orahmoun/Webserv/Request.cpp

CMakeFiles/Webserv.dir/Request.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Webserv.dir/Request.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/orahmoun/Webserv/Request.cpp > CMakeFiles/Webserv.dir/Request.cpp.i

CMakeFiles/Webserv.dir/Request.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Webserv.dir/Request.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/orahmoun/Webserv/Request.cpp -o CMakeFiles/Webserv.dir/Request.cpp.s

CMakeFiles/Webserv.dir/Route.cpp.o: CMakeFiles/Webserv.dir/flags.make
CMakeFiles/Webserv.dir/Route.cpp.o: Route.cpp
CMakeFiles/Webserv.dir/Route.cpp.o: CMakeFiles/Webserv.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/orahmoun/Webserv/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Webserv.dir/Route.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Webserv.dir/Route.cpp.o -MF CMakeFiles/Webserv.dir/Route.cpp.o.d -o CMakeFiles/Webserv.dir/Route.cpp.o -c /Users/orahmoun/Webserv/Route.cpp

CMakeFiles/Webserv.dir/Route.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Webserv.dir/Route.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/orahmoun/Webserv/Route.cpp > CMakeFiles/Webserv.dir/Route.cpp.i

CMakeFiles/Webserv.dir/Route.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Webserv.dir/Route.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/orahmoun/Webserv/Route.cpp -o CMakeFiles/Webserv.dir/Route.cpp.s

CMakeFiles/Webserv.dir/RouterBuilder.cpp.o: CMakeFiles/Webserv.dir/flags.make
CMakeFiles/Webserv.dir/RouterBuilder.cpp.o: RouterBuilder.cpp
CMakeFiles/Webserv.dir/RouterBuilder.cpp.o: CMakeFiles/Webserv.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/orahmoun/Webserv/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Webserv.dir/RouterBuilder.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Webserv.dir/RouterBuilder.cpp.o -MF CMakeFiles/Webserv.dir/RouterBuilder.cpp.o.d -o CMakeFiles/Webserv.dir/RouterBuilder.cpp.o -c /Users/orahmoun/Webserv/RouterBuilder.cpp

CMakeFiles/Webserv.dir/RouterBuilder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Webserv.dir/RouterBuilder.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/orahmoun/Webserv/RouterBuilder.cpp > CMakeFiles/Webserv.dir/RouterBuilder.cpp.i

CMakeFiles/Webserv.dir/RouterBuilder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Webserv.dir/RouterBuilder.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/orahmoun/Webserv/RouterBuilder.cpp -o CMakeFiles/Webserv.dir/RouterBuilder.cpp.s

CMakeFiles/Webserv.dir/Server.cpp.o: CMakeFiles/Webserv.dir/flags.make
CMakeFiles/Webserv.dir/Server.cpp.o: Server.cpp
CMakeFiles/Webserv.dir/Server.cpp.o: CMakeFiles/Webserv.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/orahmoun/Webserv/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Webserv.dir/Server.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Webserv.dir/Server.cpp.o -MF CMakeFiles/Webserv.dir/Server.cpp.o.d -o CMakeFiles/Webserv.dir/Server.cpp.o -c /Users/orahmoun/Webserv/Server.cpp

CMakeFiles/Webserv.dir/Server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Webserv.dir/Server.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/orahmoun/Webserv/Server.cpp > CMakeFiles/Webserv.dir/Server.cpp.i

CMakeFiles/Webserv.dir/Server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Webserv.dir/Server.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/orahmoun/Webserv/Server.cpp -o CMakeFiles/Webserv.dir/Server.cpp.s

CMakeFiles/Webserv.dir/ServerBuilder.cpp.o: CMakeFiles/Webserv.dir/flags.make
CMakeFiles/Webserv.dir/ServerBuilder.cpp.o: ServerBuilder.cpp
CMakeFiles/Webserv.dir/ServerBuilder.cpp.o: CMakeFiles/Webserv.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/orahmoun/Webserv/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Webserv.dir/ServerBuilder.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Webserv.dir/ServerBuilder.cpp.o -MF CMakeFiles/Webserv.dir/ServerBuilder.cpp.o.d -o CMakeFiles/Webserv.dir/ServerBuilder.cpp.o -c /Users/orahmoun/Webserv/ServerBuilder.cpp

CMakeFiles/Webserv.dir/ServerBuilder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Webserv.dir/ServerBuilder.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/orahmoun/Webserv/ServerBuilder.cpp > CMakeFiles/Webserv.dir/ServerBuilder.cpp.i

CMakeFiles/Webserv.dir/ServerBuilder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Webserv.dir/ServerBuilder.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/orahmoun/Webserv/ServerBuilder.cpp -o CMakeFiles/Webserv.dir/ServerBuilder.cpp.s

CMakeFiles/Webserv.dir/ServerPoll.cpp.o: CMakeFiles/Webserv.dir/flags.make
CMakeFiles/Webserv.dir/ServerPoll.cpp.o: ServerPoll.cpp
CMakeFiles/Webserv.dir/ServerPoll.cpp.o: CMakeFiles/Webserv.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/orahmoun/Webserv/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/Webserv.dir/ServerPoll.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Webserv.dir/ServerPoll.cpp.o -MF CMakeFiles/Webserv.dir/ServerPoll.cpp.o.d -o CMakeFiles/Webserv.dir/ServerPoll.cpp.o -c /Users/orahmoun/Webserv/ServerPoll.cpp

CMakeFiles/Webserv.dir/ServerPoll.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Webserv.dir/ServerPoll.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/orahmoun/Webserv/ServerPoll.cpp > CMakeFiles/Webserv.dir/ServerPoll.cpp.i

CMakeFiles/Webserv.dir/ServerPoll.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Webserv.dir/ServerPoll.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/orahmoun/Webserv/ServerPoll.cpp -o CMakeFiles/Webserv.dir/ServerPoll.cpp.s

CMakeFiles/Webserv.dir/tools.cpp.o: CMakeFiles/Webserv.dir/flags.make
CMakeFiles/Webserv.dir/tools.cpp.o: tools.cpp
CMakeFiles/Webserv.dir/tools.cpp.o: CMakeFiles/Webserv.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/orahmoun/Webserv/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/Webserv.dir/tools.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Webserv.dir/tools.cpp.o -MF CMakeFiles/Webserv.dir/tools.cpp.o.d -o CMakeFiles/Webserv.dir/tools.cpp.o -c /Users/orahmoun/Webserv/tools.cpp

CMakeFiles/Webserv.dir/tools.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Webserv.dir/tools.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/orahmoun/Webserv/tools.cpp > CMakeFiles/Webserv.dir/tools.cpp.i

CMakeFiles/Webserv.dir/tools.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Webserv.dir/tools.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/orahmoun/Webserv/tools.cpp -o CMakeFiles/Webserv.dir/tools.cpp.s

CMakeFiles/Webserv.dir/HttpResponse.cpp.o: CMakeFiles/Webserv.dir/flags.make
CMakeFiles/Webserv.dir/HttpResponse.cpp.o: HttpResponse.cpp
CMakeFiles/Webserv.dir/HttpResponse.cpp.o: CMakeFiles/Webserv.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/orahmoun/Webserv/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/Webserv.dir/HttpResponse.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Webserv.dir/HttpResponse.cpp.o -MF CMakeFiles/Webserv.dir/HttpResponse.cpp.o.d -o CMakeFiles/Webserv.dir/HttpResponse.cpp.o -c /Users/orahmoun/Webserv/HttpResponse.cpp

CMakeFiles/Webserv.dir/HttpResponse.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Webserv.dir/HttpResponse.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/orahmoun/Webserv/HttpResponse.cpp > CMakeFiles/Webserv.dir/HttpResponse.cpp.i

CMakeFiles/Webserv.dir/HttpResponse.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Webserv.dir/HttpResponse.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/orahmoun/Webserv/HttpResponse.cpp -o CMakeFiles/Webserv.dir/HttpResponse.cpp.s

# Object files for target Webserv
Webserv_OBJECTS = \
"CMakeFiles/Webserv.dir/main.cpp.o" \
"CMakeFiles/Webserv.dir/Request.cpp.o" \
"CMakeFiles/Webserv.dir/Route.cpp.o" \
"CMakeFiles/Webserv.dir/RouterBuilder.cpp.o" \
"CMakeFiles/Webserv.dir/Server.cpp.o" \
"CMakeFiles/Webserv.dir/ServerBuilder.cpp.o" \
"CMakeFiles/Webserv.dir/ServerPoll.cpp.o" \
"CMakeFiles/Webserv.dir/tools.cpp.o" \
"CMakeFiles/Webserv.dir/HttpResponse.cpp.o"

# External object files for target Webserv
Webserv_EXTERNAL_OBJECTS =

Webserv: CMakeFiles/Webserv.dir/main.cpp.o
Webserv: CMakeFiles/Webserv.dir/Request.cpp.o
Webserv: CMakeFiles/Webserv.dir/Route.cpp.o
Webserv: CMakeFiles/Webserv.dir/RouterBuilder.cpp.o
Webserv: CMakeFiles/Webserv.dir/Server.cpp.o
Webserv: CMakeFiles/Webserv.dir/ServerBuilder.cpp.o
Webserv: CMakeFiles/Webserv.dir/ServerPoll.cpp.o
Webserv: CMakeFiles/Webserv.dir/tools.cpp.o
Webserv: CMakeFiles/Webserv.dir/HttpResponse.cpp.o
Webserv: CMakeFiles/Webserv.dir/build.make
Webserv: CMakeFiles/Webserv.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/orahmoun/Webserv/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable Webserv"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Webserv.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Webserv.dir/build: Webserv
.PHONY : CMakeFiles/Webserv.dir/build

CMakeFiles/Webserv.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Webserv.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Webserv.dir/clean

CMakeFiles/Webserv.dir/depend:
	cd /Users/orahmoun/Webserv && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/orahmoun/Webserv /Users/orahmoun/Webserv /Users/orahmoun/Webserv /Users/orahmoun/Webserv /Users/orahmoun/Webserv/CMakeFiles/Webserv.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Webserv.dir/depend

