CXX = g++
CXXFLAG = -fPIC -g -O2
FIRST_PROTOCOL = -I../first_protocol/
FIRST_CLIENT = libfirstclient.a
OBJS = first_client.o

$(FIRST_CLIENT): $(OBJS)
	rm -rf $(FIRST_CLIENT)
	ar cru $(FIRST_CLIENT) $(OBJS)
	ranlib $(FIRST_CLIENT)

$(OBJS): %.o: %.c
	$(CXX) -c $(CXXFLAG) ${FIRST_PROTOCOL} $< -o $@

clean:
	rm -rf *.a *.o .*.d $(OBJS) $(FIRST_CLIENT) core *~
