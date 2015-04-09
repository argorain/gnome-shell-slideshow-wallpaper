#ifndef __SLIDESHOWAPPWIN_H__
#define __SLIDESHOWAPPWIN_H__

#include <gtk/gtk.h>
#include "slideshow.h"


#define SLIDESHOW_APP_WINDOW_TYPE (slideshow_app_window_get_type ())
#define SLIDESHOW_APP_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), SLIDESHOW_APP_WINDOW_TYPE, SlideshowAppWindow))


typedef struct _SlideshowAppWindow         SlideshowAppWindow;
typedef struct _SlideshowAppWindowClass    SlideshowAppWindowClass;


GType                   slideshow_app_window_get_type     (void);
SlideshowAppWindow       *slideshow_app_window_new          (SlideshowApp *app);
void                    slideshow_app_window_open         (SlideshowAppWindow *win,
                                                         GFile            *file);


#endif /* __SLIDESHOWAPPWIN_H__ */
