CXX = g++
CXXFLAG = -Wall -g -O2

FIRST_BASE_INCLUDE = -I../first_base/
FIRST_LOG = libfirstlog.a
OBJS = first_stream.o first_log.o

$(FIRST_LOG): $(OBJS)
	rm -rf $(FIRST_LOG)
	ar cru $(FIRST_LOG) $(OBJS)
	ranlib $(FIRST_LOG)

$(OBJS): %.o: %.c
	$(CXX) $(CXXFLAG) -c $(CXXFLAG) ${FIRST_BASE_INCLUDE} $< -o $@

test: $(OBJS) test.o
	$(CXX) $(CXXFLAG) -o test test.o ${OBJS} ${FIRST_BASE_INCLUDE}

test.o: test.c
	$(CXX) $(CXXFLAG) -c ${FIRST_BASE_INCLUDE} test.c


clean:
	rm -rf *.o .*.d $(OBJS) $(FIRST_LOG) core *~
