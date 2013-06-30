#export LD_LIBRARY_PATH = /usr/lib64/

OBJ = task_server.o im_handle.o task.o role_task.o php_task.o proto_bin.o im.o im_proxy.o

FLAGS= -Wall -g -O2
CC = g++
FIRST_INCLUDE = -I../first_base/ -I../first_protocol/protocol/include -L../first_protocol/protocol/lib -I../first_ini -L../first_ini -I../first_log -L../first_log -I../first_poll -L../first_poll
FIRST_ARG = -lfirstprotocol -lfirstini -lfirstlog -lfirstpoll -lcrypto

im: $(OBJ) 
	$(CC) $(FLAGS) -g -o im $(OBJ) $(FIRST_INCLUDE) $(FIRST_ARG) 
task_server.o: task_server.h task_server.c
	$(CC) $(FLAGS) -c task_server.c $(FIRST_INCLUDE)
task.o: task.h task.c
	$(CC) $(FLAGS) -c task.c $(FIRST_INCLUDE)
role_task.o: task.h role_task.c 
	$(CC) $(FLAGS) -c role_task.c $(FIRST_INCLUDE)
php_task.o: task.h php_task.c 
	$(CC) $(FLAGS) -c php_task.c $(FIRST_INCLUDE)
im.o: im.h im.c 
	$(CC) $(FLAGS) -c im.c $(FIRST_INCLUDE)
im_handle.o :im_handle.h im_handle.c
	$(CC) $(FLAGS) -c im_handle.c $(FIRST_INCLUDE) 
proto_bin.o :proto_bin.h proto_bin.c
	$(CC) $(FLAGS) -c proto_bin.c $(FIRST_INCLUDE)
im_proxy.o :im_proxy.h im_proxy.c
	$(CC) $(FLAGS) -c im_proxy.c $(FIRST_INCLUDE)
clean:
	rm -f *.o im
