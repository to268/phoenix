HOSTCC?=gcc
override CFLAGS=-Wall -O2 -I include/
override LDFLAGS=

C_FILES=$(shell find -name "*.c")
OBJS=$(C_FILES:.c=.o)
PREFIX=utils/genconfig

BIN=genconfig

all: $(BIN)

$(BIN): $(OBJS)
	echo "   HOSTLD    $(PREFIX)/$(BIN)"
	$(HOSTCC) $(LDFLAGS) -o $(BIN) $(OBJS) $(LIBS)

$(OBJS): $(C_FILES)
	echo "   HOSTCC    $(PREFIX)/$*.o"
	$(HOSTCC) $(CFLAGS) -c $*.c -o $@

clean:
	echo "   RM        $(PREFIX)/*.o"
	rm -f $(ASM_OBJS) $(OBJS)

mrproper: clean
	rm -f $(BIN)

.SILENT:
.PHONY: all clean mrproper
