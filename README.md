# My project's README

Build steps:

1. Navigate to the parent main folder with cmake lists file
2. run cmake commands:
   a. to Generate make file and eclipse project:
cmake -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_ECLIPSE_MAKE_ARGUMENTS=-j8
   b. To only Generate make file
cmake .
3. To build the demo version of the applicaion, set the define AGENDA_DEMO_MODE inside /src/agenda/config/agenda_config.h to 1
4. to build --> make
5. Output will be in /bin directory 
# edu_projects
