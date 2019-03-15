CFLAGS =

all: brx btx wrx

wrx: WSJTXreceiver.c DieWithError.c
	gcc $(CFLAGS) -D_GNU_SOURCE -o $@ $^

btx: BroadcastSender.c DieWithError.c
	gcc $(CFLAGS) -D_GNU_SOURCE -o $@ $^

brx: BroadcastReceiver.c DieWithError.c
	gcc $(CFLAGS) -D_GNU_SOURCE -o $@ $^


clean:
	rm -rf btx brx
