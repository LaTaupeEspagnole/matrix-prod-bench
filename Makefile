CC= gcc
CFLAGS= -std=c99 -Wall -Wextra -Werror -pedantic -Iincludes
LDFLAGS= -lpthread -lm

VPATH= src tests

OBJS_LIB= libmatrix.o multmat.o
LIB= libmatrix.a
OBJS_CHK= check.o
CHK= check
OBJS_BCH= bench.o
BCH= bench

.PHONY: clean debug clean_lib clean_chk


# --- Main targets ---

lib: $(LIB)
check: $(CHK)
bench: $(BCH)
lib_debug: clean_lib
lib_debug: CFLAGS += -g
lib_debug: lib
chk_debug: clean lib_debug
chk_debug: CFLAGS += -g
chk_debug: $(CHK)


# --- Sub-targets ---

$(LIB): $(OBJS_LIB) $(LIB)($(OBJS_LIB)) clean_after_lib
$(CHK): CFLAGS += -O3
$(CHK): $(OBJS_CHK) $(LIB)
$(BCH): CFLAGS += -O3
$(BCH): $(OBJS_BCH) $(LIB)


# --- Clean targets ---

clean: clean_lib clean_chk clean_bch
clean_after_lib:
	$(RM) $(OBJS_LIB)
clean_bch:
	$(RM) $(OBJS_BCH)
	$(RM) $(BCH)
clean_lib:
	$(RM) $(OBJS_LIB)
	$(RM) $(LIB)
clean_chk:
	$(RM) $(OBJS_CHK)
	$(RM) $(CHK)
