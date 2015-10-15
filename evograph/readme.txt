EvoGraph 0.30

Copyright (C) 2003
by Jianjun Hu(hujianju@cse.msu.edu)
www.egr.msu.edu/~hujianju

Genetic Algorithm Research & Application Group (GARAGe Group)
Department of Computer Science & Engineering
Michigan State University
East Lansing, MI, 48823
USA. 



ABOUT
=====

EvoGraph is a package for graph synthesis by genetic programming. It is developed based on strongly-typed lilgp, originally developed at GARAGe, MSU and later patched by Sean Luke with the strongly-typed feature. Basically,EvoGraph provides a set of GP functions and terminals along with the modifiable site concept for evolving an arbitrary graph. The same technique is widely used in developmental genetic programming for evolving electric circuits, bond graphs, neural networks. 


GETTING STARTED
===============
1. Install

Currently, only Microsoft Visual.net and Visual C++ 6.0 project files are provided, though the code is also compatible with linux and unix. To start, one simply open the EvoGraph.sln (with Visual C++.net) or EvoGraph.dsw (with Visual C++ 6.0). 

There are two projects in the Visual C++ solutions. One is graphlib, which is a program used to generate a random graph with a specified number of nodes and edges. The location of all nodes are constrained in a rectanglar area specified by the users. The second project is EvoGraph, which is the graph evolving engine. After one compile these two projects, two exe files (graphlib.exe and HFCGP.exe) will be generated in the \evograph\Bin directory. 




2. How to run

To start your experiment of graph evolving, one needs an parameter input file used to specify the running parameters of Genetic programming. Two such input files can be found in \evograph\Inputfile, one is for single population GP, another is for multi-subpopulation GP. 

For generic graph evolving, to invoke the experiment, one can simply
    go to \evograph\Bin directory
    >HFCGP -f .evographMulPop.file

For wireless access piont cofiguration problem included  here, one can simply run evograph.bat, which will use a default client distribution map and multi-population GP to evolve a solution.  

To generate a random client distribution graph using the graphlib:
    graphLib FileName VertexNo [EdgeNo MaxXCoordinate MaxYCoordinate]
    e.g:  graphLib client 20 10 1000 1000
You can also edit the generated map and modify those locations of clients. 

After you generate the client map, you need to tell the evolution engine which map you want to use. This can be achieved by changing one parameter in your input file.

app.clientGraphFile=client.txt
app.watchInterval=1
app.maxServiceRadius=100
output.basename = BestGraph




3. How to view the evolved graphs

The evolved graphs are exported to a series of  BestGraph?.gw. Graphs in this format can be read by LEDA (http://www.algorithmic-solutions.com/). For convenience, one compiled LEDA program (early version) can be found in \evograph\Tools4Animation. This program simplying continuously loads BestGraph.gw, BestGraph1.gw,...etc. So you can view an animation of how your graph gets evolved generation after generation. To load a single graph, one can download a trai version from www.algorithmic-solutions.com and can get the graph by its exporting function. 

To see one run example, go to \evograph\Tools4Animation, uncompress the animation.zip and run LEDA.exe.


Another option to view the evolved graph is to use the aiSee program from (www.aisee.com)
You only need to comment out 
	#define EXPORT_GW_LEDA in the App.c

One good thing of viewing graphs with LEDA or aiSee is that they provide some nice layout algorithms which enable make very cool evolved graphs




4. How to hack (evolve your own graph)

To evolve graphs for your own problem, you only need to change the fitness definition. This function is named
	void app_eval_fitness_parameter ( individual *ind )
and is located in file App.c in directory \evograph\AppGraph

Since the graph library provides a bunch of useful algorithms, you can specify many kinds of fitness functions.For example, try to evolve a graph with 8 nodes and 5 equal-length edges, or evolve a complete graph, or....whatever you want to define. 

One useful feature of Evograph is that, it can evolve both the topology and the locations of the nodes simultanously! It shares the same underlying principles used for circuit evolving by Professor. John, Koza at Stanford University. 


comment: there are many other unused gp functions in this package, some of which may even not thoroughly tested. So use it at your own risk. The example here use hybrid encoding, both edge operators and node operators are included. 



DOCUMENTATIONS
==============

Oops! we don't have complete document available now. We assume you are familar with the basic genetic programming and know how to use lil-gp, whose document can found at (http://garage.cps.msu.edu/software/software-index.html#lilgp)
Descriptions of GP functions for graph evolving can be found in the source code (function.c file)

There are some legacy codes in the evoGraph which are not used but remain there. 



REFERENCE
==============
Luke, S. and L. Spector. 1996. Evolving Graphs and Networks with Edge encoding: Preliminary Report. In Late Breaking 	
	Papers at the Genetic Programming 1996 Conference (GP96). J. Koza, ed. Stanford: Stanford Bookstore. 117-124.
F. Gruau. Neural network synthesis using cellular encoding and the genetic algorithm. PhD thesis, Laboratoire de 	
	L'informatique     du Paralllisme, Ecole Normale Supriere de Lyon, Lyon, France, 1994. 

Koza, John R. 2002. Automatic synthesis of both the topology and numerical parameters for complex structures using 	
	genetic programming. In Chakrabarti, Amaresh (editor). Engineering Design Synthesis. London: Springer 2002. Pages 319
More related references can be found at www.egr.msu.edu/~hujianju/evograph/evograph.htm



AKNOWLEDGEMENTS
==============
This package is developed during our NSF project "Automated System Design by Bond graph and Genetic Programming". Some of the results were discuessed among our group members: Erik. D. Goodman, Kisung. Seo, Jianjun. Hu,  Zhun. Fan,Ronald C. Rosenberg. I especially thank my advisor Erik,Goodman for his support and advice.
 


