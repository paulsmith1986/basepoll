#export LD_LIBRARY_PATH = /usr/lib64/

OBJ = task_server.o demo_handle.o task.o proto_bin.o demo.o

FLAGS= -Wall -g -O2
CC = g++
FIRST_INCLUDE = -I../../first_base/ -I../../first_protocol/protocol/include -L../../first_protocol/protocol/lib -I../../first_ini -L../../first_ini -I../../first_log -L../../first_log -I../../first_poll -L../../first_poll
FIRST_ARG = -lfirstprotocol -lfirstini -lfirstlog -lfirstpoll

demo: $(OBJ) 
	$(CC) $(FLAGS) -g -o demo $(OBJ) $(FIRST_INCLUDE) $(FIRST_ARG) 
task_server.o: task_server.h task_server.c
	$(CC) $(FLAGS) -c task_server.c $(FIRST_INCLUDE)
task.o: task.c
	$(CC) $(FLAGS) -c task.c $(FIRST_INCLUDE)
demo.o: demo.h demo.c 
	$(CC) $(FLAGS) -c demo.c $(FIRST_INCLUDE)
demo_handle.o :demo_handle.h demo_handle.c
	$(CC) $(FLAGS) -c demo_handle.c $(FIRST_INCLUDE) 
proto_bin.o :proto_bin.h proto_bin.c
	$(CC) $(FLAGS) -c proto_bin.c $(FIRST_INCLUDE)
clean:
	rm -f *.o demo
