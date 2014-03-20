CC = gcc

TARGET = send_pkt

SRCS=$(TARGET).c
OBJS=$(SRCS:.c=.o)

all: $(TARGET)

debug: CC += -DDEBUG
debug:	$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

$(TARGET).o: Makefile

clean:
	rm -rf *.o

distclean: clean
	rm -f $(TARGET)
