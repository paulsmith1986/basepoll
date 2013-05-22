CXX = g++
CXXFLAG = -Wall -g -O2

LIB_FIRST_POLL = libfirstpoll.a
OBJS = first_poll.o poller.o
FIRST_INCLUDE = -I../first_log/ -I../first_base/ -I../first_protocol/ -L../first_log/
$(LIB_FIRST_POLL): $(OBJS)
	rm -rf $(LIB_FIRST_POLL)
	ar cru $(LIB_FIRST_POLL) $(OBJS)
	ranlib $(LIB_FIRST_POLL)

$(OBJS): %.o: %.c
	$(CXX) ${CXXFLAG} -c ${FIRST_INCLUDE} $< -o $@

clean:
	rm -rf *.o .*.d $(OBJS) $(LIB_FIRST_POLL) core *~
