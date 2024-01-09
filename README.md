# AIFIRA2023Sim for AIFIRA2023 Experiment [huber@cenbg.in2p3.fr, huberarnaud@gmail.com]

# INSTRUCTIONS TO USE THE SIMULATION
#- Download the VMWare [Geant4.11.0.2](#){https://heberge.lp2ib.in2p3.fr/G4VM/index.html}
#- Copy all the files from AIFIRA2023_Simulation folder [ici] (https://github.com/ahuber33/AIFIRA2023_Simulation) !
#- Create a build folder in the AIFIRA2023_Simulation folder and go inside.
#- Use this command : '''cmake -DGeant4_DIR=$G4COMP ../''' and compile it with make
#- The executable AIFIRA2023Sim will be add to your bin folder
#- If you want to have a visualization, launch this command : '''./AIFIRA2023Sim [name of ROOT file ]'''. It will generate 1 particle according to the vis.mac with QT and you will have a ROOT file with the name you gave in response located in the Resultats folder.
#- If you want to have statistics without the visualization, use this command : '''./AIFIRA2023Sim [name of ROOT file] [number of events generated] [name of macro]'''. Personnaly, I used the vrml.mac but you can create another one. Just to remember that you need to write the name of your macro when you launch the simulation.
#- An AIFIRA2023Sim.cfg file is located in bin directory. All the dimensions necessary are in this file to avoid recompilation when you want to change some parameters. If you add some other dimensions, don't forget to add the variables in Geometry.cc.
#- Based on G4EmStandardPhysics_option3 with additional Optical part.
#- DO NOT HESITATE TO REPORT BUGS OR ANY IDEAS THAT WILL IMPROVE THE SIMULATION !!!!


# Commit #1 le 09/01/2024 [AIFIRA2023Sim.0.1.0]
## - Simulation issue de la version TP_Simulation.0.9.0 -> Commit initial avec changement des noms des fichiers et des variables
