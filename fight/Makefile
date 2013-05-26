CXX = gcc
CXXFLAG = -fPIC -g -O2

OBJS =  struct_pool.o yile_fight.o skill_effect.o
YILE_FIGHT = libyilefight.a
YILE_FIGHT_INCLUDE = -I. -L.
YILE_FIGHT_LIB = -lyilefight

FORMULA_OBJS =  skill_formula.o
YILE_FIGHT_FORMULA = libyilefightformula.a
YILE_FIGHT_FORMULA_LIB = -lyilefightformula

$(YILE_FIGHT): $(OBJS)
	rm -rf $(YILE_FIGHT)
	ar cru $(YILE_FIGHT) $(OBJS)
	ranlib $(YILE_FIGHT)

$(YILE_FIGHT_FORMULA): $(FORMULA_OBJS)
	rm -rf $(YILE_FIGHT_FORMULA)
	ar cru $(YILE_FIGHT_FORMULA) $(FORMULA_OBJS)
	ranlib $(YILE_FIGHT_FORMULA)

$(OBJS): %.o: %.c
	$(CXX) -c $(CXXFLAG) $< -o $@

$(FORMULA_OBJS): %.o: %.c
	$(CXX) -c $(CXXFLAG) $< -o $@

fight: fight.o
	$(CXX) $(CXXFLAG) fight.o -o fight ${YILE_FIGHT_INCLUDE} ${YILE_FIGHT_FORMULA_LIB} ${YILE_FIGHT_LIB} -lm

fight.o: fight.c
	$(CXX) $(CXXFLAG) -c fight.c

clean:
	rm -rf *.a *.o .*.d $(OBJS) $(YILE_FIGHT) core *~
