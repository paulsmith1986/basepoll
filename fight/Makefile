CXX = gcc
CXXFLAG = -fPIC -g -O2

OBJS =  struct_pool.o first_fight.o skill_effect.o fight_formula.o
FIRST_FIGHT = libfirstfight.a
FIRST_FIGHT_INCLUDE = -I. -L.
FIRST_FIGHT_LIB = -lfirstfight

FORMULA_OBJS =  skill_formula.o
FIRST_FIGHT_FORMULA = libfirstfightformula.a
FIRST_FIGHT_FORMULA_LIB = -lfirstfightformula

$(FIRST_FIGHT): $(OBJS)
	rm -rf $(FIRST_FIGHT)
	ar cru $(FIRST_FIGHT) $(OBJS)
	ranlib $(FIRST_FIGHT)

$(FIRST_FIGHT_FORMULA): $(FORMULA_OBJS)
	rm -rf $(FIRST_FIGHT_FORMULA)
	ar cru $(FIRST_FIGHT_FORMULA) $(FORMULA_OBJS)
	ranlib $(FIRST_FIGHT_FORMULA)

$(OBJS): %.o: %.c
	$(CXX) -c $(CXXFLAG) $< -o $@

$(FORMULA_OBJS): %.o: %.c
	$(CXX) -c $(CXXFLAG) $< -o $@

fight: fight.o
	$(CXX) $(CXXFLAG) fight.o -o fight ${FIRST_FIGHT_INCLUDE} ${FIRST_FIGHT_FORMULA_LIB} ${FIRST_FIGHT_LIB} -lm

fight.o: fight.c
	$(CXX) $(CXXFLAG) -c fight.c

clean:
	rm -rf *.a *.o .*.d $(OBJS) $(FIRST_FIGHT) core *~
