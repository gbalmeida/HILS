HILS algorithm code for Single Source Capacitated Location Problem

It was shown that the algorithm generates quality solutions for very large-scale SSCFLP instances

Instructions to run on a linux operating system:

CPLEX (any version) must be installed on the machine.

Copy the files "hils_sscflp.cpp'', "funcoes_hils.cpp", "funcoes.h" and "makefile" to a directory

In the "makefile" file check the CPLEX path

In a terminal go to the directory and use the "make" command to create the sscflp executable

To run on an "instance name" file use the command ./sscflp "instance name"
