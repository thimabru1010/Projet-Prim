
CC = gcc
LIBS = -lev3dev-c
CFLAGS = -c -g
WARNINGS = -Wextra

client: client_test.o
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

ssh:
	ssh robot@ev3dev.local

clean:
	rm -r sensor sensor.o light light.o
