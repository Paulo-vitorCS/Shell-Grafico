#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <cairo.h>
#include <stdlib.h>
#include <unistd.h>

// Comando para compilar a main.c
// gcc -o main main.c `pkg-config --libs --cflags gtk+-2.0`

void abrirEditorTexto(GtkWidget *widget, gpointer nomeWidget);

void abrirExploradorArquivos(GtkWidget *widget, gpointer nomeWidget);

void abrirNavegador(GtkWidget *widget, gpointer nomeWidget);

void exec_shell(GtkWidget *widget, gpointer nomeWidget);

void abrirConfiguracao(GtkWidget *widget, gpointer nomeWidget);

gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer nomeWidget);

gboolean time_handler(GtkWidget *widget);

int mostrarPopup(GtkWidget *widget, GdkEvent *event);

GdkPixbuf *criarIconeShell(const gchar *filename);

int main(int argc, char *argv[]){

    GtkWidget *janela, *botao, *layout, *toolbar, *relogio, *wallpaper;
    GtkToolItem *fechar, *separador, *editorTexto, *exploradorArquivos, *web;
    GtkToolItem *configuracao, *calendario, *shell_exec;
    GtkWidget *label, *toolbar2, *barraSuperior1, *barraSuperior2;
    GtkWidget *menuPopup, *minimizar, *encerrar, *vbox, *sep;

    GtkWidget *menubar;
    GtkWidget *fileMenu;
    GtkWidget *fileMi;
    GtkWidget *newMi;
    GtkWidget *openMi;
    GtkWidget *quitMi;

    GdkColor cor;

    GtkAccelGroup *accel_group = NULL;

    // Início da manipulação dos widgets

    gtk_init (&argc, &argv);

    // Criar a tela do shell gráfico

    janela = gtk_window_new(GTK_WINDOW_TOPLEVEL); // Criar uma nova janela
    gtk_window_fullscreen(GTK_WINDOW(janela)); // Criar tela em fullsreen

    // Definir ícone do shell gráfico

    gtk_window_set_icon(GTK_WINDOW(janela), criarIconeShell("images/operatingSystem.png"));

    // Definir wallpaper do shell gráfico

    layout = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(janela), layout);
    gtk_widget_show(layout);

    wallpaper = gtk_image_new_from_file("images/wallpaper.jpg");
    gtk_layout_put(GTK_LAYOUT(layout), wallpaper, 0, 0);

    // Criar a toolbar

    toolbar = gtk_toolbar_new();
    gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

    separador = gtk_separator_tool_item_new();
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), separador, -1); // Inserir a barra de separação de ícones

    editorTexto = gtk_tool_button_new_from_stock(GTK_STOCK_FILE);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), editorTexto, -1); // Inserir o botão de abrir novo arquivo de texto

    exploradorArquivos = gtk_tool_button_new_from_stock(GTK_STOCK_DIRECTORY);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), exploradorArquivos, -1); // Inserir o botão do explorador de arquivos

    web = gtk_tool_button_new_from_stock(GTK_STOCK_INFO);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), web, -1); // Inserir o botão de abrir navegador

    configuracao = gtk_tool_button_new_from_stock(GTK_STOCK_PROPERTIES);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), configuracao, -1); // Inserir o botão de abrir configurações

    shell_exec = gtk_tool_button_new_from_stock(GTK_STOCK_INDENT);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), shell_exec, -1);

    // Modificação da cor da toolbar

    gdk_color_parse("#D3D3D3", &cor); // Código da cor em hexadecimal
  	gtk_widget_modify_bg(toolbar, GTK_STATE_NORMAL, &cor); // Modificar a cor de fundo do widget

    // Ações para cada botão criado acima

    gtk_layout_put(GTK_LAYOUT(layout), toolbar, 55, 723);
  	gtk_widget_set_size_request(toolbar, 1311, 45);

    g_signal_connect(G_OBJECT(editorTexto), "clicked", G_CALLBACK(abrirEditorTexto), NULL);

    g_signal_connect(G_OBJECT(exploradorArquivos), "clicked", G_CALLBACK(abrirExploradorArquivos), NULL);

    g_signal_connect(G_OBJECT(web), "clicked", G_CALLBACK(abrirNavegador), NULL);

    g_signal_connect(G_OBJECT(configuracao), "clicked", G_CALLBACK(abrirConfiguracao), NULL);

    g_signal_connect(G_OBJECT(shell_exec), "clicked", G_CALLBACK(exec_shell), NULL);

    // Criar o relógio

    relogio = gtk_drawing_area_new();

    gtk_layout_put(GTK_LAYOUT(layout),relogio, 1155, 723);
  	gtk_widget_set_size_request(relogio, 200, 30);

    gdk_color_parse("#D3D3D3", &cor); // Mudar a cor de fundo do relógio
  	gtk_widget_modify_bg(relogio, GTK_STATE_NORMAL, &cor);

    g_signal_connect(relogio, "expose-event", G_CALLBACK(on_expose_event), NULL); // Identifica que a tela foi aberta

    g_timeout_add(1000, (GSourceFunc) time_handler, (gpointer) janela);
	  time_handler(relogio);

    /*
    // Criar uma label em cima de uma toolbar (2)

    toolbar2 = gtk_toolbar_new();

    gdk_color_parse("#D3D3D3", &cor); // Código da cor em hexadecimal
  	gtk_widget_modify_bg(toolbar2, GTK_STATE_NORMAL, &cor); // Modificar a cor de fundo do widget

    gtk_layout_put(GTK_LAYOUT(layout), toolbar2, 508, 0);
  	gtk_widget_set_size_request(toolbar2, 350, 45);

    label = gtk_label_new(NULL);

    gtk_layout_put(GTK_LAYOUT(layout), label, 508, 0);
  	gtk_widget_set_size_request(label, 350, 45);
    gtk_label_set_markup(GTK_LABEL(label), "<span font = \"12\" color = \"black\"> Shell Grafico - Sistemas Operacionais (GBC045) </span>");
    */

    // Criar barra superior 1

    barraSuperior1 = gtk_event_box_new();

    gtk_layout_put(GTK_LAYOUT(layout), barraSuperior1, 0, 0);
  	gtk_widget_set_size_request(barraSuperior1, 1366, 45);

  	menuPopup = gtk_menu_new();

    /*
    minimizar = gtk_menu_item_new_with_label("Minimizar");
    gtk_widget_show(minimizar);
    gtk_menu_shell_append(GTK_MENU_SHELL(menuPopup), minimizar);
    */

	  encerrar = gtk_menu_item_new_with_label("Desligar");
    gtk_widget_show(encerrar);
	  gtk_menu_shell_append(GTK_MENU_SHELL(menuPopup), encerrar);

    // g_signal_connect_swapped(G_OBJECT(minimizar), "activate", G_CALLBACK(gtk_window_iconify), GTK_WINDOW(janela));

    g_signal_connect(G_OBJECT(encerrar), "activate", G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect_swapped(G_OBJECT(barraSuperior1), "button-press-event", G_CALLBACK(mostrarPopup), menuPopup);

    gdk_color_parse("#2F2F2F", &cor); // Modificar a cor da barra superior 1
  	gtk_widget_modify_bg(barraSuperior1, GTK_STATE_NORMAL, &cor); //modifying the background color of the widget

    /*
    // Criar barra superior 2

    barraSuperior2 = gtk_event_box_new();

    gtk_layout_put(GTK_LAYOUT(layout), barraSuperior2, 858, 0);
  	gtk_widget_set_size_request(barraSuperior2, 508, 45);

    g_signal_connect_swapped(G_OBJECT(minimizar), "activate", G_CALLBACK(gtk_window_iconify), GTK_WINDOW(janela));

    g_signal_connect(G_OBJECT(encerrar), "activate", G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(G_OBJECT(janela), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect_swapped(G_OBJECT(barraSuperior2), "button-press-event", G_CALLBACK(mostrarPopup), menuPopup);

    gdk_color_parse("#D3D3D3", &cor); // Modificar a cor da barra superior 1
  	gtk_widget_modify_bg(barraSuperior2, GTK_STATE_NORMAL, &cor); //modifying the background color of the widget
    */

    // Criar menu iniciar

    menubar = gtk_menu_bar_new();
    fileMenu = gtk_menu_new();

    gtk_layout_put(GTK_LAYOUT(layout), menubar, 0, 723);
    gtk_widget_set_size_request(menubar, 55, 45);

    accel_group = gtk_accel_group_new();
    gtk_window_add_accel_group(GTK_WINDOW(janela), accel_group);

    fileMi = gtk_menu_item_new_with_mnemonic("Iniciar");

    newMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_PROPERTIES, NULL);
    sep = gtk_separator_menu_item_new();
    quitMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, accel_group);

    gdk_color_parse("#D3D3D3", &cor); // Modificar a cor do botão Iniciar
    gtk_widget_modify_bg(menubar, GTK_STATE_NORMAL, &cor); //modifying the background color of the widget

    gtk_widget_add_accelerator(quitMi, "activate", accel_group,
        GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), newMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), sep);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);

    g_signal_connect(G_OBJECT(newMi), "activate",
      G_CALLBACK(abrirConfiguracao), NULL);

    g_signal_connect(G_OBJECT(quitMi), "activate",
        G_CALLBACK(gtk_main_quit), NULL);

    //-----------------------------------------------------------------
    // Ícones na área de trabalho

    // Criar a toolbar icone 1
    GtkWidget *desktop1;
    GtkToolItem *editorTexto2;

    desktop1 = gtk_toolbar_new();
    gtk_toolbar_set_style(GTK_TOOLBAR(desktop1), GTK_TOOLBAR_ICONS);

    editorTexto2 = gtk_tool_button_new_from_stock(GTK_STOCK_FILE);
    gtk_toolbar_insert(GTK_TOOLBAR(desktop1), editorTexto2, -1); // Inserir o botão de abrir novo arquivo de texto

    // Modificação da cor da toolbar

    gdk_color_parse("#2F2F2F", &cor); // Código da cor em hexadecimal
    gtk_widget_modify_bg(desktop1, GTK_STATE_NORMAL, &cor); // Modificar a cor de fundo do widget

    // Ações para cada botão criado acima

    gtk_layout_put(GTK_LAYOUT(layout), desktop1, 10, 75);
    gtk_widget_set_size_request(desktop1, 45, 45);

    g_signal_connect(G_OBJECT(editorTexto2), "clicked", G_CALLBACK(abrirEditorTexto), NULL);

    //-----------------------------------------------------------------
    // Ícones na área de trabalho

    // Criar a toolbar icone 2
    GtkWidget *desktop2;
    GtkToolItem *exploradorArquivos2;

    desktop2 = gtk_toolbar_new();
    gtk_toolbar_set_style(GTK_TOOLBAR(desktop2), GTK_TOOLBAR_ICONS);

    exploradorArquivos2 = gtk_tool_button_new_from_stock(GTK_STOCK_DIRECTORY);
    gtk_toolbar_insert(GTK_TOOLBAR(desktop2), exploradorArquivos2, -1); // Inserir o botão do explorador de arquivos

    // Modificação da cor da toolbar

    gdk_color_parse("#2F2F2F", &cor); // Código da cor em hexadecimal
    gtk_widget_modify_bg(desktop2, GTK_STATE_NORMAL, &cor); // Modificar a cor de fundo do widget


    gtk_layout_put(GTK_LAYOUT(layout), desktop2, 65, 75);
    gtk_widget_set_size_request(desktop2, 45, 45);

    g_signal_connect(G_OBJECT(exploradorArquivos2), "clicked", G_CALLBACK(abrirExploradorArquivos), NULL);

    //-----------------------------------------------------------------
    // Ícones na área de trabalho

    // Criar a toolbar icone 3
    GtkWidget *desktop3;
    GtkToolItem *web2;

    desktop3 = gtk_toolbar_new();
    gtk_toolbar_set_style(GTK_TOOLBAR(desktop3), GTK_TOOLBAR_ICONS);

    web2 = gtk_tool_button_new_from_stock(GTK_STOCK_INFO);
    gtk_toolbar_insert(GTK_TOOLBAR(desktop3), web2, -1); // Inserir o botão de abrir navegador

    // Modificação da cor da toolbar

    gdk_color_parse("#2F2F2F", &cor); // Código da cor em hexadecimal
    gtk_widget_modify_bg(desktop3, GTK_STATE_NORMAL, &cor); // Modificar a cor de fundo do widget

    gtk_layout_put(GTK_LAYOUT(layout), desktop3, 10, 120);
    gtk_widget_set_size_request(desktop3, 45, 45);

    g_signal_connect(G_OBJECT(web2), "clicked", G_CALLBACK(abrirNavegador), NULL);

    //-----------------------------------------------------------------
    // Ícones na área de trabalho

    // Criar a toolbar icone 4
    GtkWidget *desktop4;
    GtkToolItem *configuracao2;

    desktop4 = gtk_toolbar_new();
    gtk_toolbar_set_style(GTK_TOOLBAR(desktop4), GTK_TOOLBAR_ICONS);

    configuracao2 = gtk_tool_button_new_from_stock(GTK_STOCK_PROPERTIES);
    gtk_toolbar_insert(GTK_TOOLBAR(desktop4), configuracao2, -1); // Inserir o botão de abrir configurações

    // Modificação da cor da toolbar

    gdk_color_parse("#2F2F2F", &cor); // Código da cor em hexadecimal
    gtk_widget_modify_bg(desktop4, GTK_STATE_NORMAL, &cor); // Modificar a cor de fundo do widget

    gtk_layout_put(GTK_LAYOUT(layout), desktop4, 65, 120);
    gtk_widget_set_size_request(desktop4, 45, 45);

    g_signal_connect(G_OBJECT(configuracao2), "clicked", G_CALLBACK(abrirConfiguracao), NULL);


    //-----------------------------------------------------------------
    // Ícones na área de trabalho

    // Criar a toolbar icone 5
    GtkWidget *desktop5;
    GtkToolItem *shell_exec2;

    desktop5 = gtk_toolbar_new();
    gtk_toolbar_set_style(GTK_TOOLBAR(desktop5), GTK_TOOLBAR_ICONS);

    shell_exec2 = gtk_tool_button_new_from_stock(GTK_STOCK_INDENT);
    gtk_toolbar_insert(GTK_TOOLBAR(desktop5), shell_exec2, -1);

    // Modificação da cor da toolbar

    gdk_color_parse("#2F2F2F", &cor); // Código da cor em hexadecimal
    gtk_widget_modify_bg(desktop5, GTK_STATE_NORMAL, &cor); // Modificar a cor de fundo do widget

    // Ações para cada botão criado acima

    gtk_layout_put(GTK_LAYOUT(layout), desktop5, 10, 175);
    gtk_widget_set_size_request(desktop5, 45, 45);

    g_signal_connect(G_OBJECT(shell_exec2), "clicked", G_CALLBACK(exec_shell), NULL);

    // Fim da manipulação dos widgets

    gtk_widget_show_all(janela); // Exibir a tela criada
    gtk_main(); // Fim

    return 0;

}


void abrirEditorTexto(GtkWidget *widget, gpointer nomeWidget){
    pid_t pid;

    pid = fork();

    if(pid == 0)
        execlp("gedit", NULL);
}

void abrirExploradorArquivos(GtkWidget *widget, gpointer nomeWidget){
    pid_t pid;

    pid = fork();

    if(pid == 0)
        execlp("nautilus", NULL);
}

void abrirNavegador(GtkWidget *widget, gpointer nomeWidget){

    pid_t pid;

    pid = fork();

    if(pid == 0)
        execlp("firefox", NULL);
}


void exec_shell(GtkWidget *widget, gpointer nomeWidget){
    pid_t pid;

    pid = fork();

    if(pid == 0)
        execlp("gnome-terminal", NULL);
}


void abrirConfiguracao(GtkWidget *widget, gpointer nomeWidget){
    pid_t pid;

    pid = fork();

    if(pid == 0)
        execlp("gnome-control-center", NULL);
}

void destroy(GtkWidget* widget, gpointer nomeWidget){

	gtk_main_quit();

}

gchar buf[256];

gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer nomeWidget){

    cairo_t *cr;

    cr = gdk_cairo_create(widget->window);

    cairo_move_to(cr, 30, 30);
    cairo_set_font_size(cr, 14);
    cairo_show_text(cr, buf);

    cairo_destroy(cr);

    return FALSE;

}

gboolean time_handler(GtkWidget *widget){

    if(widget->window == NULL){
      return FALSE;
    }

    GDateTime *now = g_date_time_new_now_local();
    gchar *my_time = g_date_time_format(now, "%d/%m/%Y  |  %H:%M:%S");

    g_sprintf(buf, "%s", my_time);

    g_free(my_time);
    g_date_time_unref(now);

    gtk_widget_queue_draw(widget);

    return TRUE;

}

int mostrarPopup(GtkWidget *widget, GdkEvent *event) {

    const gint RIGHT_CLICK = 3;

    if (event->type == GDK_BUTTON_PRESS) {

        GdkEventButton *bevent = (GdkEventButton *) event;

        if (bevent->button == RIGHT_CLICK) {

            gtk_menu_popup(GTK_MENU(widget), NULL, NULL, NULL, NULL,
            bevent->button, bevent->time);
        }

        return TRUE;
    }

    return FALSE;
}

GdkPixbuf *criarIconeShell(const gchar *filename) {

   GdkPixbuf *pixbuf;
   GError *error = NULL;
   pixbuf = gdk_pixbuf_new_from_file(filename, &error);

   if (!pixbuf) {

      fprintf(stderr, "%s\n", error->message);
      g_error_free(error);
   }

   return pixbuf;
}

void show_question(GtkWidget *widget, gpointer window) {

    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
             GTK_DIALOG_DESTROY_WITH_PARENT,
             GTK_MESSAGE_QUESTION,
             GTK_BUTTONS_YES_NO,
             "Desligar");
    gtk_window_set_title(GTK_WINDOW(dialog), "Encerrar sessão");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
