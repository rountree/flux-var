#!/bin/bash
export PATH=/g/g24/rountree/FLUX/install/bin:$PATH
echo `date` The flux we will be using is: `which flux`

echo `date` starting flux
srun --pty --mpi=none --ntasks=4 --nodes=4 -ppdebug flux start	

#srun --pty --mpi=none 		\
#	--ntasks=144		\
#	--nodes=4		\
#	--sockets-per-node=2	\
#	--cores-per-socket=18	\
#	--threads-per-core=1	\
#	-ppdebug 		\
#	flux start
#

# srun --pty --mpi=none --ntasks=144 --nodes=4 --sockets-per-node=2 --cores-per-socket=18 --threads-per-core=1 -ppdebug flux start	# Works  0/8
#srun --pty --mpi=none --ntasks=8   --nodes=4 --sockets-per-node=2 --cores-per-socket=1  --threads-per-core=1 -ppdebug flux start	# Works  2/2
# srun --pty --mpi=none --ntasks=16   --nodes=4 --sockets-per-node=2 --cores-per-socket=2  --threads-per-core=1 -ppdebug flux start	# Works  2/2
# srun --pty --mpi=none --ntasks=16   --nodes=4 --sockets-per-node=1 --cores-per-socket=4  --threads-per-core=1 -ppdebug flux start	# Works  6/6
# srun --pty --mpi=none --ntasks=32   --nodes=4 --sockets-per-node=1 --cores-per-socket=8  --threads-per-core=1 -ppdebug flux start	# Works  5/7
# srun --pty --mpi=none --ntasks=32   --nodes=4 --sockets-per-node=2 --cores-per-socket=4  --threads-per-core=1 -ppdebug flux start	# Works  1/4
