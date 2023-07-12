HILS algorithm code for the Single Source Capacitated Location Problem:

HILS combines the neighborhood-based heuristic procedures Iterated Local Search and Variable Neighborhood Descent with the solution of two binary linear programming sub-problems through a general-purpose solver. The results showed that it generates high-quality solutions for very large-scale SSCFLP instances.

Instructions to run on a Linux operating system:
1) CPLEX (any version) must be installed on the machine;
2) Copy the files "hils_sscflp.cpp'', "funcoes_hils.cpp", "funcoes.h" and "makefile" to a directory;
3) In the "makefile" file, check the CPLEX path;
4) In a terminal, go to the directory and use the "make" command to create the sscflp executable;
5) Use the command ./sscflp "instance name" to run on an "instance name" file.
