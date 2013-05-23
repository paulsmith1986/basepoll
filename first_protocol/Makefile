CXX = g++
CXXFLAG = -fPIC -g -O2
FIRST_PROTOCOL = libfirstprotocol.a
OBJS = first_protocol.o

$(FIRST_PROTOCOL): $(OBJS)
	rm -rf $(FIRST_PROTOCOL)
	ar cru $(FIRST_PROTOCOL) $(OBJS)
	ranlib $(FIRST_PROTOCOL)

$(OBJS): %.o: %.c
	$(CXX) -c $(CXXFLAG) $< -o $@

clean:
	rm -rf *.a *.o .*.d $(OBJS) $(FIRST_PROTOCOL) core *~
