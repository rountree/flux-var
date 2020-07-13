#!/bin/bash

# Set up to run on Quartz.

export PATH=/g/g24/rountree/FLUX/install/bin:$PATH
echo `date` The flux we will be using is: `which flux`

echo `date` running salloc
#salloc --nodes=4 --sockets-per-node=2 --cores-per-socket=18 --threads-per-core=1 --time=12 --partition=pdebug 	
salloc --nodes=4 --time=12 --partition=pdebug 	

