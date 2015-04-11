#include <gtk/gtk.h>

#include "debug.h"
#include "slideshow.h"
#include "slideshowwin.h"


struct _SlideshowAppWindow
{
  GtkApplicationWindow parent;
};

struct _SlideshowAppWindowClass
{
  GtkApplicationWindowClass parent_class;
};

typedef struct _SlideshowAppWindowPrivate SlideshowAppWindowPrivate;

struct _SlideshowAppWindowPrivate
{
  GSettings *settings;
  GtkWidget *stack;
  //GtkWidget *search;
  //GtkWidget *searchbar;
  //GtkWidget *searchentry;
  GtkWidget *gears;
  GtkWidget *sidebar;
  GtkWidget *words;
  //GtkWidget *lines;
  //GtkWidget *lines_label;
};

G_DEFINE_TYPE_WITH_PRIVATE(SlideshowAppWindow, slideshow_app_window, GTK_TYPE_APPLICATION_WINDOW);

static void visible_child_changed (GObject *stack, GParamSpec *pspec)
{
  SlideshowAppWindow *win;
  SlideshowAppWindowPrivate *priv;

  if (gtk_widget_in_destruction (GTK_WIDGET (stack)))
    return;

  win = SLIDESHOW_APP_WINDOW (gtk_widget_get_toplevel (GTK_WIDGET (stack)));
  priv = slideshow_app_window_get_instance_private (win);

  DBG("child changed");
}

static void slideshow_app_window_init (SlideshowAppWindow *win)
{
  SlideshowAppWindowPrivate *priv;
  GtkBuilder *builder;
  GMenuModel *menu;
  GAction *action;

  priv = slideshow_app_window_get_instance_private (win);
  gtk_widget_init_template (GTK_WIDGET (win));
  priv->settings = g_settings_new ("org.gtk.slideshow");

  g_settings_bind (priv->settings, "transition",
                   priv->stack, "transition-type",
                   G_SETTINGS_BIND_DEFAULT);

  g_settings_bind (priv->settings, "show-words",
                   priv->sidebar, "reveal-child",
                   G_SETTINGS_BIND_DEFAULT);

  //g_object_bind_property (priv->search, "active",
  //                        priv->searchbar, "search-mode-enabled",
  //                        G_BINDING_BIDIRECTIONAL);

  //g_signal_connect (priv->sidebar, "notify::reveal-child",
  //                  G_CALLBACK (words_changed), win);

  builder = gtk_builder_new_from_resource ("/org/gtk/slideshow/gears-menu.ui");
  menu = G_MENU_MODEL (gtk_builder_get_object (builder, "menu"));
  gtk_menu_button_set_menu_model (GTK_MENU_BUTTON (priv->gears), menu);
  g_object_unref (builder);

  action = g_settings_create_action (priv->settings, "show-words");
  g_action_map_add_action (G_ACTION_MAP (win), action);
  g_object_unref (action);

  //action = (GAction*) g_property_action_new ("show-lines", priv->lines, "visible");
  //g_action_map_add_action (G_ACTION_MAP (win), action);
  //g_object_unref (action);

  //g_object_bind_property (priv->lines, "visible",
  //                        priv->lines_label, "visible",
  //                        G_BINDING_DEFAULT);

  g_object_set (gtk_settings_get_default (), "gtk-shell-shows-app-menu", FALSE, NULL);
  gtk_application_window_set_show_menubar (GTK_APPLICATION_WINDOW (win), TRUE);

  DBG("window init");
}

static void slideshow_app_window_dispose (GObject *object)
{
  SlideshowAppWindow *win;
  SlideshowAppWindowPrivate *priv;

  win = SLIDESHOW_APP_WINDOW (object);
  priv = slideshow_app_window_get_instance_private (win);

  g_clear_object (&priv->settings);

  G_OBJECT_CLASS (slideshow_app_window_parent_class)->dispose (object);

  DBG("window dispose");
}

static void slideshow_app_window_class_init (SlideshowAppWindowClass *class)
{
  G_OBJECT_CLASS (class)->dispose = slideshow_app_window_dispose;

  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (class),
                                               "/org/gtk/slideshow/window.ui");

  gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (class), SlideshowAppWindow, stack);
  //gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (class), SlideshowAppWindow, search);
  //gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (class), SlideshowAppWindow, searchbar);
  //gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (class), SlideshowAppWindow, searchentry);
  gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (class), SlideshowAppWindow, gears);
  gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (class), SlideshowAppWindow, words);
  gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (class), SlideshowAppWindow, sidebar);
  //gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (class), SlideshowAppWindow, lines);
  //gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (class), SlideshowAppWindow, lines_label);

  //gtk_widget_class_bind_template_callback (GTK_WIDGET_CLASS (class), search_text_changed);
  gtk_widget_class_bind_template_callback (GTK_WIDGET_CLASS (class), visible_child_changed);

  DBG("window class init");
}

SlideshowAppWindow* slideshow_app_window_new (SlideshowApp *app)
{
  DBG("window ctor");
  return g_object_new (SLIDESHOW_APP_WINDOW_TYPE, "application", app, NULL);
}

void slideshow_app_window_open (SlideshowAppWindow *win, GFile *file)
{
  SlideshowAppWindowPrivate *priv;
  gchar *basename;
  GtkWidget *scrolled, *view;
  gchar *contents;
  gsize length;
  GtkTextBuffer *buffer;
  GtkTextTag *tag;
  GtkTextIter start_iter, end_iter;

  priv = slideshow_app_window_get_instance_private (win);
  basename = g_file_get_basename (file);

  scrolled = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolled);
  gtk_widget_set_hexpand (scrolled, TRUE);
  gtk_widget_set_vexpand (scrolled, TRUE);
  view = gtk_text_view_new ();
  gtk_text_view_set_editable (GTK_TEXT_VIEW (view), FALSE);
  gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (view), FALSE);
  gtk_widget_show (view);
  gtk_container_add (GTK_CONTAINER (scrolled), view);
  gtk_stack_add_titled (GTK_STACK (priv->stack), scrolled, basename, basename);

  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

  if (g_file_load_contents (file, NULL, &contents, &length, NULL, NULL))
    {
      gtk_text_buffer_set_text (buffer, contents, length);
      g_free (contents);
    }

  tag = gtk_text_buffer_create_tag (buffer, NULL, NULL);
  g_settings_bind (priv->settings, "font",
                   tag, "font",
                   G_SETTINGS_BIND_DEFAULT);

  gtk_text_buffer_get_start_iter (buffer, &start_iter);
  gtk_text_buffer_get_end_iter (buffer, &end_iter);
  gtk_text_buffer_apply_tag (buffer, tag, &start_iter, &end_iter);

  g_free (basename);
  DBG("window open");
}
