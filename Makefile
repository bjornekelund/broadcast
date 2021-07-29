CFLAGS =

all: brx btx wrx enc

enc: encodeWSJTX.c 
	gcc $(CFLAGS) -D_GNU_SOURCE -o $@ $^ -lm

wrx: WSJTXreceiver.c DieWithError.c
	gcc $(CFLAGS) -D_GNU_SOURCE -o $@ $^ -lm

btx: BroadcastSender.c DieWithError.c
	gcc $(CFLAGS) -D_GNU_SOURCE -o $@ $^

brx: BroadcastReceiver.c DieWithError.c
	gcc $(CFLAGS) -D_GNU_SOURCE -o $@ $^


clean:
	rm -rf btx brx enc
