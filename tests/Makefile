
CC = gcc
LIBS = -lev3dev-c
CFLAGS = -c -g
WARNINGS = -Wextra
TARGET_EV3 = light sensor motor uart client
EXECS= $(TARGET_EV3: *.*=.exe)

all: $(EXECS)
	
$(EXECS): $(TARGET_EV3).o
	$(CC) $< $(LIBS) -o $@

$(TARGET_EV3).o: $(TARGET_EV3).c
	$(CC) $(CFLAGS) $< -o $@

serve: server.o
	$(CC) $< $(LIBS) -o $@

server.o: server.c
	$(CC) $(CFLAGS) $< -o $@

client: client.o
	$(CC) $< $(LIBS) -o $@

client_test.o: client_test.c
	$(CC) $(CFLAGS) $< -o $@

endian: endian_test.o
	$(CC) $< $(LIBS) -o $@

endian_test.o: endian_test.c
	$(CC) $(CFLAGS) $< -o $@
	
uart: uart.o	
	$(CC) $< $(LIBS) -o $@

uart.o: uart.c
	$(CC) $(CFLAGS) $< -o $@

motor: motor.o
	$(CC) $< $(LIBS) -o $@

motor.o: motor.c
	$(CC) $(CFLAGS) $< -o $@
	
sensor: sensor.o
	$(CC) $< $(LIBS) -o $@
	
sensor.o: sensor.c
	$(CC) $(CFLAGS) $< -o $@

light: light.o
	$(CC) $< $(LIBS) -o $@

light.o: light.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: ssh clean

ssh:
	ssh robot@ev3dev.local

clean:
	rm -r sensor sensor.o light light.o
