CC= gcc
CFLAGS= -std=c99 -Wall -Wextra -Werror -pedantic -Iincludes -mavx -mavx2 -mfma
# CFLAGS+= -fopt-info-all -fopt-info-loop # Control loop vectorization
LDFLAGS= -L. -lpthread -lm -lmatrix

VPATH= src tests

OBJS_LIB= libmatrix.o multmat.o serialize.o buffer.o
LIB= libmatrix.a
OBJS_CHK= check.o
OBJS_BCH= bench.o
OBJS_TST= test.o
OBJS_DEV= dev.o

.PHONY: clean clean_lib clean_chk clean_bch clean_tst


# --- Main targets ---

lib: $(LIB)
check: $(LIB) $(OBJS_CHK)
bench: $(LIB) $(OBJS_BCH)
test: CFLAGS += -O3
test: $(LIB)
test: $(OBJS_TST)
lib_debug: clean_lib
lib_debug: CFLAGS += -g
lib_debug: $(LIB)
chk_debug: clean
chk_debug: CFLAGS += -g
chk_debug: $(LIB)
chk_debug: check
check_opti: CFLAGS += -O3
check_opti: check
bench_opti: CFLAGS += -O3
bench_opti: bench
test-suite: test
test-suite:
	tests/test-suite.py tests/json-tests/ ./test
test-suite-control:
	tests/test-suite.py tests/json-tests/ ./numpy-test.py
dev: $(LIB) $(OBJS_DEV)
dev_opti: CFLAGS += -O3
dev_opti: dev
dev_debug: CFLAGS += -g
dev_debug: dev

$(LIB): $(OBJS_LIB)
	ar r $@ $^
	ranlib $@

# --- Cleanng targets ---

clean: clean_lib clean_chk clean_bch clean_tst clean_py clean_dev
clean_bch:
	$(RM) $(OBJS_BCH)
	$(RM) bench
clean_lib:
	$(RM) $(OBJS_LIB)
	$(RM) $(LIB)
clean_chk:
	$(RM) $(OBJS_CHK)
	$(RM) check
clean_tst:
	$(RM) $(OBJS_TST)
	$(RM) test
clean_dev:
	$(RM) $(OBJS_DEV)
	$(RM) dev

clean_py:
	$(RM) -r tests/__pycache__
