
CC = gcc
LIBS = -lev3dev-c
CFLAGS = -c -g
WARNINGS = -Wextra
EXECS_EV3 = light.exe sensor.exe motor.exe uart.exe client.exe PID.exe

ev3: $(EXECS_EV3)
	
%.exe: %.o
	$(CC) $< $(LIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

server.exe: server.o
	$(CC) $< -o $@

server.o: server.c
	$(CC) $(CFLAGS) $< -o $@

endian.exe: endian_test.o
	$(CC) $< -o $@

endian_test.o: endian_test.c
	$(CC) $(CFLAGS) $< -o $@
	
.PHONY: ssh clean

ssh:
	ssh robot@ev3dev.local

clean:
	rm -rf *.exe *.o
