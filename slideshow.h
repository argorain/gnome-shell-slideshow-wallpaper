#ifndef __SLIDESHOW_H__
#define __SLIDESHOW_H__

#include <gtk/gtk.h>


#define SLIDESHOW_APP_TYPE (slideshow_app_get_type ())
#define SLIDESHOW_APP(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), SLIDESHOW_APP_TYPE, SlideshowApp))


typedef struct _SlideshowApp       SlideshowApp;
typedef struct _SlideshowAppClass  SlideshowAppClass;


GType           slideshow_app_get_type    (void);
SlideshowApp     *slideshow_app_new         (void);


#endif /* __SLIDESHOW_H__ */
