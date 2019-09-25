Author: Tim Lindquist (Tim.Lindquist@asu.edu)
        Software Engineering, CIDSE, IAFSE, Arizona State University Polytechnic
Version: January 2019

See http://pooh.poly.asu.edu/Ser321/Assigns/Assign4/assign4.html

Purpose: Sample C++ program demonstrating Ant builds and an FLTK
 GUI for Messages.

This program is has been tested on Linux and MacOS (build.xml only works on
Linux).

To execute Ant using the build.xml in this directory, you will need to
copy the file: antlibs.jar from the lib directory to your home directory:
cp lib/antlibs.jar ~
or
cp lib/antlibs.jar $HOME/
Note that ~ (tilde) is a shortcut for $HOME
then extract the antlibs.jar file:
pushd ~
jar xvf antlibs.jar
pushd -0
The pushd commands manipulate a stack of directories for switching your
bash's current directory. The first pushd pushes home onto the stack and
switches the current directory to home. The second pushd takes you
back to whatever directory you were in before the first.

To run the example GUI, from a bash shell in the project directory, execute the
command:
ant execute.sample

To clean the project (remove the executable files) execute:
ant clean


