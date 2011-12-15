Star

1) upload:
scp * skvarjak@star.fit.cvut.cz:/home/skvarjak

2) Make
make

3) Run:
./main

4) Run star:
qsub serial_job.sh

5) run local
mpiexec -n 2 main


Parallel
========

run:
qsub -pe ompi 5 -q 4c_long.q -l plq parallel_job.sh
qsub -pe ompi 2 -q 4c_short.q -l psq parallel_job.sh
qstat


