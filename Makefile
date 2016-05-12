CFLAGS := $(shell pkg-config --cflags --libs gtk+-3.0 gcr-3)
certificate_chooser: certificate_chooser.c
	$(CC) $(CFLAGS) -o $@ $<
