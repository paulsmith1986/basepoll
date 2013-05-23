OBJ = task_client.o client.o proto_c.o task.o

FLAGS= -Wall -g -O2
CC = g++
FIRST_INCLUDE = -I../../first_base/ -I../../first_client/firstclient/include -L../../first_client/firstclient/lib -I../../first_protocol/protocol/include -L../../first_protocol/protocol/lib  -I../../first_log -L../../first_log
FIRST_ARG = -lfirstclient -lfirstprotocol -lfirstlog

client: $(OBJ) 
	$(CC) $(FLAGS) -g -o client $(OBJ) $(FIRST_INCLUDE) $(FIRST_ARG) 
task_client.o: task_client.h task_client.c
	$(CC) $(FLAGS) -c task_client.c $(FIRST_INCLUDE)
task.o: task.h task.c
	$(CC) $(FLAGS) -c task.c $(FIRST_INCLUDE)
client.o: client.h client.c 
	$(CC) $(FLAGS) -c client.c $(FIRST_INCLUDE)
proto_c.o :proto_c.h proto_c.c
	$(CC) $(FLAGS) -c proto_c.c $(FIRST_INCLUDE)
clean:
	rm -f *.o client
