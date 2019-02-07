# ManaOS

Proyecto ManaOS de Federico Badaloni e Ivo Elbert.
Hecho en base a NachOS, como un trabajo para Sistemas Operativos II, LCC, FCEIA, UNR.

---------------------------------------
UserProg puede ser un miemnbro de thread.
UserProgMgr en si no esta aportando nada, ni parece poder aportarlo.
El AddrSpace puede pasar a UserProg. userRegisters capaz tambien. Hay que cambiar algunas cosas, pero lo vale.

StartProcess arranca en un kernel, todo lo que genera lo guarda en un nuevo UserProcess y empieza la ejecucion en usuario.

Exec entonces seria simplemente hacer un buen fork con StartProcess.



--------------------------------------
Cuando se queda sin threads se queda en un loop infinito. Capaz hay que arreglarlo. Capaz deja de existir el problema cuando haya una shell permanente...



-------------------------------------
sudo apt-get install libc6:i386
