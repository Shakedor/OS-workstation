KERNELDIR = /usr/src/linux-2.4.18-14custom
include $(KERNELDIR)/.config
CFLAGS = -D__KERNEL__ -DMODULE -I$(KERNELDIR)/include -O -Wall
all: srandom.o

srandom.o: mix.o sha1.o my_module.o
	$(LD) -r $^ -o $@


my_module.o: my_module.c 
	gcc $(CFLAGS) -c $^ -o $@

mix.o: mix.c 
	gcc $(CFLAGS) -c $^ -o $@

sha1.o: sha1.c
	gcc $(CFLAGS) -c $^ -o $@
