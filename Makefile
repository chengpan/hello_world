SENDER=file_sender
RECEIVER=file_receiver
HELLO_WORLD=hello_world
CFLAGS=-Wall

ALL=${SENDER} ${RECEIVER} ${HELLO_WORLD}

all: ${ALL}

${SENDER}: file_sender.o

${RECEIVER}: file_receiver.o

${HELLO_WORLD}: hello_world.o

.PHONY: clean
clean:
	rm -f *.o ${ALL}

