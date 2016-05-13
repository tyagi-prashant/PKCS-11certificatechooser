CFLAGS := $(shell pkg-config --cflags --libs gtk+-3.0 gcr-3) -g
certificate_chooser: certificate_chooser.c certificate_renderer.c
	$(CC) $(CFLAGS) -o $@ $<
