SENDER=file_sender
RECEIVER=file_receiver
CFLAGS=-Wall

ALL=${SENDER} ${RECEIVER}

all: ${ALL}

${SENDER}: file_sender.o

${RECEIVER}: file_receiver.o

.PHONY: clean
clean:
	rm -f *.o ${ALL}

