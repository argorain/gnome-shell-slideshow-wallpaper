#include <gtk/gtk.h>
#include <stdio.h>

#include "debug.h"
#include "slideshow.h"
#include "slideshowwin.h" /*TODO dodelat toto */
//#include "slideshowappprefs.h" /*TODO dodelat toto */

//**************** STRUCTS & ARRAYS *****************

struct _SlideshowApp
{
  GtkApplication parent;
};

struct _SlideshowAppClass
{
  GtkApplicationClass parent_class;
};


//****************** FUNCTIONS *************************

G_DEFINE_TYPE(SlideshowApp, slideshow_app, GTK_TYPE_APPLICATION);

static void slideshow_app_init (SlideshowApp *app)
{
  DBG("app init");
}

/*static void preferences_activated (GSimpleAction *action, GVariant *parameter, gpointer app)
{
  SlideshowAppPrefs *prefs;
  GtkWindow *win;

  win = gtk_application_get_active_window (GTK_APPLICATION (app));
  prefs = slideshow_app_prefs_new (slideshow_APP_WINDOW (win));
  gtk_window_present (GTK_WINDOW (prefs));
}*/

static void quit_activated (GSimpleAction *action, GVariant *parameter, gpointer app)
{
  DBG("app quit");
  g_application_quit (G_APPLICATION (app));
}


static GActionEntry app_entries[] =
{
  //{ "preferences", preferences_activated, NULL, NULL, NULL },
  { "quit", quit_activated, NULL, NULL, NULL }
};

static void slideshow_app_startup (GApplication *app) 
{
  GtkBuilder *builder;
  GMenuModel *app_menu;
  const gchar *quit_accels[2] = { "<Ctrl>Q", NULL };

  G_APPLICATION_CLASS (slideshow_app_parent_class)->startup (app);

  g_action_map_add_action_entries (G_ACTION_MAP (app),
                                   app_entries, G_N_ELEMENTS (app_entries),
                                   app);
  gtk_application_set_accels_for_action (GTK_APPLICATION (app),
                                         "app.quit",
                                         quit_accels);

  //builder = gtk_builder_new_from_resource ("/org/gtk/slideshow/menu.ui");
  //app_menu = G_MENU_MODEL (gtk_builder_get_object (builder, "appmenu"));
  //gtk_application_set_app_menu (GTK_APPLICATION (app), app_menu);
  //g_object_unref (builder);
  DBG("startup");
}

static void slideshow_app_activate (GApplication *app)
{
  SlideshowAppWindow *win;

  DBG("activate");

  win = slideshow_app_window_new (SLIDESHOW_APP (app));
  gtk_window_present (GTK_WINDOW (win));
}

static void slideshow_app_open (GApplication *app, GFile **files, gint n_files, const gchar *hint)
{
  GList *windows;
  SlideshowAppWindow *win;
  int i;

  DBG("app open");

  windows = gtk_application_get_windows (GTK_APPLICATION (app));
  if (windows)
    win = SLIDESHOW_APP_WINDOW (windows->data);
  else
    win = slideshow_app_window_new (SLIDESHOW_APP (app));

  for (i = 0; i < n_files; i++)
    slideshow_app_window_open (win, files[i]);

  gtk_window_present (GTK_WINDOW (win));
}

static void slideshow_app_class_init (SlideshowAppClass *class)
{
  DBG("class init");

  G_APPLICATION_CLASS (class)->startup = slideshow_app_startup;
  G_APPLICATION_CLASS (class)->activate = slideshow_app_activate;
  G_APPLICATION_CLASS (class)->open = slideshow_app_open;
}

SlideshowApp * slideshow_app_new (void)
{
  DBG("app new");

  return g_object_new (SLIDESHOW_APP_TYPE,
                       "application-id", "org.gtk.slideshow",
                       "flags", G_APPLICATION_HANDLES_OPEN,
                       NULL);
}
