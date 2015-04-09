CC = gcc
PKGCONFIG = $(shell which pkg-config)
CFLAGS = `$(PKGCONFIG) --cflags gtk+-3.0`
LIBS = `$(PKGCONFIG) --libs gtk+-3.0`
GLIB_COMPILE_RESOURCES = `$(PKGCONFIG) --variable=glib_compile_resources gio-2.0`
GLIB_COMPILE_SCHEMAS = `$(PKGCONFIG) --variable=glib_compile_schemas gio-2.0`

SRC = resources.c slideshow.c slideshowwin.c main.c

OBJS = $(SRC:.c=.o)

all: slideshow

org.gtk.slideshow.gschema.valid: org.gtk.slideshow.gschema.xml
	$(GLIB_COMPILE_SCHEMAS) --strict --dry-run --schema-file=$< && mkdir -p $(@D) && touch $@

gschemas.compiled: org.gtk.slideshow.gschema.valid
	$(GLIB_COMPILE_SCHEMAS) .

resources.c: slideshow.gresource.xml $(shell $(GLIB_COMPILE_RESOURCES) --sourcedir=. --generate-dependencies slideshow.gresource.xml)
	$(GLIB_COMPILE_RESOURCES) slideshow.gresource.xml --target=$@ --sourcedir=. --generate-source

%.o: %.c
	$(CC) -c -o $(@F) $(CFLAGS) $<

slideshow: $(OBJS) gschemas.compiled
	$(CC) -o $(@F) $(LIBS) $(OBJS)

clean:
	rm -f org.gtk.slideshow.gschema.valid
	rm -f gschemas.compiled
	rm -f resources.c
	rm -f $(OBJS)
	rm -f slideshow
