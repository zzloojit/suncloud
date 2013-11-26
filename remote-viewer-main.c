/*
 * Remote Viewer: A spice/vnc client based on virt-viewer
 *
 * Copyright (C) 2011-2012 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Marc-André Lureau <marcandre.lureau@redhat.com>
 */

#include <config.h>
#include <locale.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <stdlib.h>
#ifdef G_OS_WIN32
#include <windows.h>
#include <io.h>
#endif

#ifdef HAVE_GTK_VNC
#include <vncdisplay.h>
#endif
#ifdef HAVE_SPICE_GTK
#include <spice-option.h>
#endif

#include "remote-viewer.h"
#include "virt-viewer-app.h"
#include "virt-viewer-session.h"

static void
remote_viewer_version(void)
{
    g_print(_("remote-viewer version %s\n"), VERSION BUILDID);
    exit(EXIT_SUCCESS);
}

gboolean fullscreen = FALSE;
gboolean fullscreen_auto_conf = FALSE;

static gboolean
option_fullscreen(G_GNUC_UNUSED const gchar *option_name,
                  const gchar *value,
                  G_GNUC_UNUSED gpointer data, GError **error)
{
    fullscreen = TRUE;

    if (value == NULL)
        return TRUE;

    if (g_str_equal(value, "auto-conf")) {
        fullscreen_auto_conf = TRUE;
        return TRUE;
    }

    g_set_error(error, G_OPTION_ERROR, G_OPTION_ERROR_FAILED, _("Invalid full-screen argument: %s"), value);
    return FALSE;
}

static void
recent_add(gchar *uri, const gchar *mime_type)
{
    GtkRecentManager *recent;
    GtkRecentData meta = {
        .app_name     = (char*)"remote-viewer",
        .app_exec     = (char*)"remote-viewer %u",
        .mime_type    = (char*)mime_type,
    };

    if (uri == NULL)
        return;

    recent = gtk_recent_manager_get_default();
    meta.display_name = uri;
    if (!gtk_recent_manager_add_full(recent, uri, &meta))
        g_warning("Recent item couldn't be added");
}

static void connected(VirtViewerSession *session,
                      VirtViewerApp *self G_GNUC_UNUSED)
{
    gchar *uri = virt_viewer_session_get_uri(session);
    const gchar *mime = virt_viewer_session_mime_type(session);

    recent_add(uri, mime);
    g_free(uri);
}


static void rdp_exec(char* cmd) {
    if (strncmp(cmd, "rdp", 3) == 0) {
        const char* prog = "C:/windows/system32/mstsc.exe";
        int i = 0, j = 1;
        char* p = cmd + strlen("rdp");
        char* argv[20] = {NULL};
        while (*p == ' ') 
            p++;
        /* FILE* log = fopen("log.txt", "w"); */
        /* if (cmd == NULL) { */
        /*     fprintf(log, "cmd == NULL\n"); */
        /*     fflush(log); */
        /* } */
        /* fprintf(log,"%s\n", cmd); */
        /* fflush(log); */
        argv[0] = prog;

        while (*p != '\0') {
            if (j == 20) {
                exit(0);
            }
            if (p[i] == ' ') {
                p[i] = '\0';
                argv[j] = p;
                p = p + i + 1;
                i = 0;
                j++;
                while (*p == ' ')
                    p++;
                continue;
            }
            if (p[i] == '\0') {
                argv[j] = p;
                break;
            }
            i++;
        }
        /* for (i = 0; argv[i] != NULL; i++) { */
        /*     fprintf(log, "%d %s\n", i, argv[i]); */
        /*     fflush(log); */
        /* }  */
        /* fclose(log); */
        execv(argv[0], argv);
    }
}
static const char base[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="; 
/* */ 
static char find_pos(char ch)   
{ 
    char *ptr = (char*)strrchr(base, ch);//the last position (the only) in base[] 
    return (ptr - base); 
} 
static char *base64_decode(const char *data, int data_len) 
{ 
    int ret_len = (data_len / 4) * 3; 
    int equal_count = 0; 
    char *ret = NULL; 
    char *f = NULL; 
    int tmp = 0; 
    int temp = 0; 
    char need[3]; 
    int prepare = 0; 
    int i = 0; 
    if (*(data + data_len - 1) == '=') 
    { 
        equal_count += 1; 
    } 
    if (*(data + data_len - 2) == '=') 
    { 
        equal_count += 1; 
    } 
    if (*(data + data_len - 3) == '=') 
    {//seems impossible 
        equal_count += 1; 
    } 
    switch (equal_count) 
    { 
    case 0: 
        ret_len += 4;//3 + 1 [1 for NULL] 
        break; 
    case 1: 
        ret_len += 4;//Ceil((6*3)/8)+1 
        break; 
    case 2: 
        ret_len += 3;//Ceil((6*2)/8)+1 
        break; 
    case 3: 
        ret_len += 2;//Ceil((6*1)/8)+1 
        break; 
    } 
    ret = (char *)malloc(ret_len); 
    if (ret == NULL) 
    { 
        printf("No enough memory.\n"); 
        exit(0); 
    } 
    memset(ret, 0, ret_len); 
    f = ret; 
    while (tmp < (data_len - equal_count)) 
    { 
        temp = 0; 
        prepare = 0; 
        memset(need, 0, 4); 
        while (temp < 4) 
        { 
            if (tmp >= (data_len - equal_count)) 
            { 
                break; 
            } 
            prepare = (prepare << 6) | (find_pos(data[tmp])); 
            temp++; 
            tmp++; 
        } 
        prepare = prepare << ((4-temp) * 6); 
        for (i=0; i<3 ;i++ ) 
        { 
            if (i == temp) 
            { 
                break; 
            } 
            *f = (char)((prepare>>((2-i)*8)) & 0xFF); 
            f++; 
        } 
    } 
    *f = '\0'; 
    return ret; 
}

typedef struct args{
    int argc;
    char** argv;
} args;

args* decode_args(int argc, char**argv)
{
    static args ret;
    int i = 0, j = 0;
    const char* p_head = "suncloud://";
    if ((argc != 2) ||
        (strncmp(p_head, argv[1], strlen(p_head)) != 0)) {
        ret.argc = argc;
        ret.argv = argv;
        return &ret;
    }
    
    char* p = argv[1] + strlen(p_head);
    int l = strlen(p);
    if ((l != 0) && (p[l - 1] == '/')) {
        p[l - 1] = '\0';
    }

    p = base64_decode(p, strlen(p));
    const char* commandline = p;
    rdp_exec(p);
    i = 2;
    while ((p = strchr(p, ' ')) != NULL) {
	i += 1;
	p++;
        while (*p == '\0')
            p++;
    }
    ret.argv = malloc(sizeof(char*) * i);
    ret.argc = i;
    ret.argv[0] = strdup(argv[0]);
    p = commandline;

    for (j = 1; j < i; j++) {
	char* k = strchr(p, ' ');
	int len = 0;
	if (k != NULL) {
	    len = k - p + 1;
	}else{
	    len = strlen(p) + 1;
	}

	ret.argv[j] = malloc(len);
	strncpy(ret.argv[j], p, len);
        ret.argv[j][len -1] = '\0';
	if (k != NULL) {
	    p = k + 1;
            while (*p == '\0')
                p++;
        }
    }
    free(commandline);
    return &ret;
}

int
main(int argc, char **argv)
{
    if (argc == 1)
        return 0;
    struct args* cmdline= decode_args(argc, argv);
    argc = cmdline->argc;
    argv = cmdline->argv;

    GOptionContext *context;
    GError *error = NULL;
    int ret = 1;
    int zoom = 100;
    gchar **args = NULL;
    gchar *uri = NULL;
    char *title = NULL;
    char *hotkeys = NULL;
    gboolean verbose = FALSE;
    gboolean debug = FALSE;
    gboolean direct = FALSE;
    RemoteViewer *viewer = NULL;
#ifdef HAVE_SPICE_GTK
    gboolean controller = FALSE;
#endif
    VirtViewerApp *app;
    const GOptionEntry options [] = {
        { "version", 'V', G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK,
          remote_viewer_version, N_("Display version information"), NULL },
        { "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose,
          N_("Display verbose information"), NULL },
        { "title", 't', 0, G_OPTION_ARG_STRING, &title,
          N_("Set window title"), NULL },
        { "direct", 'd', 0, G_OPTION_ARG_NONE, &direct,
          N_("Direct connection with no automatic tunnels"), NULL },
        { "zoom", 'z', 0, G_OPTION_ARG_INT, &zoom,
          N_("Zoom level of window, in percentage"), "ZOOM" },
        { "debug", '\0', 0, G_OPTION_ARG_NONE, &debug,
          N_("Display debugging information"), NULL },
        { "full-screen", 'f', G_OPTION_FLAG_OPTIONAL_ARG, G_OPTION_ARG_CALLBACK, option_fullscreen,
          N_("Open in full screen mode (auto-conf adjusts guest resolution to fit the client's)."), N_("<auto-conf>") },
#ifdef HAVE_SPICE_GTK
        { "spice-controller", '\0', 0, G_OPTION_ARG_NONE, &controller,
          N_("Open connection using Spice controller communication"), NULL },
#endif
        { "hotkeys", 'H', 0, G_OPTION_ARG_STRING, &hotkeys,
          N_("Customise hotkeys"), NULL },
        { G_OPTION_REMAINING, '\0', 0, G_OPTION_ARG_STRING_ARRAY, &args,
          NULL, "-- URI" },
        { NULL, 0, 0, G_OPTION_ARG_NONE, NULL, NULL, NULL }
    };

    virt_viewer_util_init(_("Remote Viewer"));

    /* Setup command line options */
    context = g_option_context_new (_("- Remote viewer client"));
    g_option_context_add_main_entries (context, options, NULL);
    g_option_context_add_group (context, gtk_get_option_group (TRUE));
#ifdef HAVE_GTK_VNC
    g_option_context_add_group (context, vnc_display_get_option_group ());
#endif
#ifdef HAVE_SPICE_GTK
    g_option_context_add_group (context, spice_get_option_group ());
#endif
    g_option_context_parse (context, &argc, &argv, &error);
    if (error) {
        char *base_name;
        base_name = g_path_get_basename(argv[0]);
        g_printerr(_("%s\nRun '%s --help' to see a full list of available command line options\n"),
                   error->message, base_name);
        g_free(base_name);
        g_error_free(error);
        goto cleanup;
    }
    
    g_option_context_free(context);

#ifdef HAVE_SPICE_GTK
    if (controller) {
        if (args) {
            g_printerr(_("Error: extra arguments given while using Spice controller\n"));
            goto cleanup;
        }
    } else
#endif
    if (args) {
        if (g_strv_length(args) > 1) {
            g_printerr(_("Error: can't handle multiple URIs\n"));
            goto cleanup;
        } else if (g_strv_length(args) == 1) {
            uri = g_strdup(args[0]);
        }
    }

    if (zoom < 10 || zoom > 200) {
        g_printerr(_("Zoom level must be within 10-200\n"));
        goto cleanup;
    }

    gtk_window_set_default_icon_name("virt-viewer");

    virt_viewer_app_set_debug(debug);

#ifdef HAVE_SPICE_GTK
    if (controller) {
        viewer = remote_viewer_new_with_controller(verbose);
        g_object_set(viewer, "guest-name", "defined by Spice controller", NULL);
    } else {
#endif
        viewer = remote_viewer_new(uri, title, verbose);
        g_object_set(viewer, "guest-name", uri, NULL);
#ifdef HAVE_SPICE_GTK
    }
#endif
    if (viewer == NULL)
        goto cleanup;

    app = VIRT_VIEWER_APP(viewer);
    g_object_set(app, "fullscreen-auto-conf", fullscreen_auto_conf, NULL);
    g_object_set(app, "fullscreen", fullscreen, NULL);
    virt_viewer_window_set_zoom_level(virt_viewer_app_get_main_window(app), zoom);
    virt_viewer_app_set_direct(app, direct);
    virt_viewer_app_set_hotkeys(app, hotkeys);

    if (!virt_viewer_app_start(app))
        goto cleanup;

    g_signal_connect(virt_viewer_app_get_session(app), "session-connected",
                     G_CALLBACK(connected), app);

    gtk_main();

    ret = 0;

 cleanup:
    g_free(uri);
    if (viewer)
        g_object_unref(viewer);
    g_strfreev(args);

    return ret;
}

/*
 * Local variables:
 *  c-indent-level: 4
 *  c-basic-offset: 4
 *  indent-tabs-mode: nil
 * End:
 */
