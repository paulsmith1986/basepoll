CXX = g++
CXXFLAG = -Wall -g -O2

FIRST_INI_READ = libfirstini.a
OBJS = first_ini.o

$(FIRST_INI_READ): $(OBJS)
	rm -rf $(FIRST_INI_READ)
	ar cru $(FIRST_INI_READ) $(OBJS)
	ranlib $(FIRST_INI_READ)

$(OBJS): %.o: %.c
	$(CXX) $(CXXFLAG) -c  $< -o $@

test: $(OBJS) test.o
	$(CXX) $(CXXFLAG) -o test test.o ${OBJS}

test.o: test.c
	$(CXX) $(CXXFLAG) -c test.c

clean:
	rm -rf *.o .*.d $(OBJS) $(FIRST_INI_READ) core *~
