
CC = gcc
EV3_LIBS = -lev3dev-c
PTHREAD_LIB = -lpthread
CFLAGS = -c -g
WARNINGS = -Wextra
EXECS_EV3 = light.exe sensor.exe motor.exe uart.exe client_test.exe PID.exe

# ------------------------------------------------

ev3: $(EXECS_EV3)
	
%.exe: %.o
	$(CC) $< $(EV3_LIBS) -o $@

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

# ------------------------------------------------

main_client: main_client.o client.o PID.o
	$(CC) $^ $(EV3_LIBS) -o $@

main_client.o: main_client.c client.h PID.h
	$(CC) $(CFLAGS) $< -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@

# ------------------------------------------------

main_server: main_server.o server.o

main_server.o: main_server.c server.h 
	$(CC) $(CFLAGS) $< -o $@ $(PTHREAD_LIB) 

server.o: server.c server.h

# ----------------------------------------------------
.PHONY: ssh clean

ssh:
	ssh robot@ev3dev.local

clean:
	rm -rf *.exe *.o main_client main_server
