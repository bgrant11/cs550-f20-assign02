Brian Grant
cs550 assign02

I believe everything works as it should. 

The process list is obtained upon a device open. Each process info is
held in a linked list.

I left in some extra code I used for debugging. Such as test prints
I can examine using dmesg. sand.c is a "sandbox" where I could debug
some code related to my linked list without crashing the vm.

to run the code use the following commands:

cd proc-list-dev
make
make in
cd ..
make
make test

then remove the module:
cd proc-list-dev
make out



