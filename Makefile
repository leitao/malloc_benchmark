CC	:= gcc
CFLAGS	:= -W -Wall -O3 
LD	:= $(CC)
LDFLAGS	:=
PROGS	:= benchmark

.PHONY: all clean run

all: clean run

clean:
	rm -f *.o $(PROGS)

: %.o: %.c
	$(CC) $(CFLAGS) $^

benchmark: allocs.o timing.o main.o
	$(LD) $(LDFLAGS) $^ -o $@

run: $(PROGS)
	./benchmark

	@echo
	@./benchmark malloc         5 10000000 16 2>/dev/null
	@./benchmark memalign       5 10000000 16 2>/dev/null
	@./benchmark posix_memalign 5 10000000 16 2>/dev/null

	@echo
	@./benchmark malloc         5 10000000 24 2>/dev/null
	@./benchmark memalign       5 10000000 24 2>/dev/null
	@./benchmark posix_memalign 5 10000000 24 2>/dev/null

	@echo
	@./benchmark malloc         50 1000000 128 2>/dev/null
	@./benchmark memalign       50 1000000 128 2>/dev/null
	@./benchmark posix_memalign 50 1000000 128 2>/dev/null

	@echo
	@./benchmark malloc         50 100000 1024 2>/dev/null
	@./benchmark memalign       50 100000 1024 2>/dev/null
	@./benchmark posix_memalign 50 100000 1024 2>/dev/null

	@echo
	@./benchmark malloc         100 100000 16384 2>/dev/null
	@./benchmark memalign       100 100000 16384 2>/dev/null
	@./benchmark posix_memalign 100 100000 16384 2>/dev/null

	@echo
	@./benchmark malloc         400 40000 65536 2>/dev/null
	@./benchmark memalign       400 40000 65536 2>/dev/null
	@./benchmark posix_memalign 400 40000 65536 2>/dev/null

	@echo
	@./benchmark malloc         100 100000 1 1024 2>/dev/null
	@./benchmark memalign       100 100000 1 1024 2>/dev/null
	@./benchmark posix_memalign 100 100000 1 1024 2>/dev/null

