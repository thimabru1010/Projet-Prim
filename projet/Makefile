
CC = gcc
EV3_LIBS = -lev3dev-c
PTHREAD_LIB = -lpthread
CFLAGS = -c -g
WARNINGS = -Wextra
EXECS_EV3 = light.exe sensor.exe motor.exe uart.exe client1_test.exe client2_test.exe stop_motor.exe PID_test.exe

# ------------------------------------------------

ev3: $(EXECS_EV3)
	
%.exe: %.o
	$(CC) $< $(EV3_LIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

server_test.exe: server_test.o
	$(CC) $< -o $@

server_test.o: server_test.c
	$(CC) $(CFLAGS) $< -o $@

client2_test.exe: client2_test.o
	$(CC) $< -o $@

client2_test.o: client2_test.c
	$(CC) $(CFLAGS) $< -o $@

client1_test.exe: client1_test.o
	$(CC) $< -o $@

client1_test.o: client1_test.c
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
	$(CC) $^ $(PTHREAD_LIB) -o $@

main_server.o: main_server.c server.h 

server.o: server.c server.h

# ------------------------------------------------

thread_test: thread_test.o
	$(CC) $^ $(PTHREAD_LIB) -o $@

thread_test.o: thread_test.c


# ------------------------------------------------

server_test: server_test.o

server_test.o: server_test.c

# ----------------------------------------------------
.PHONY: ssh clean

ssh:
	ssh robot@ev3dev.local

clean:
	rm -rf *.exe *.o main_client main_server
