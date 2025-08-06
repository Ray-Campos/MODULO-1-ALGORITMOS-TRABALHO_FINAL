#include <gtk/gtk.h>
#include <stdio.h>

//protótipos de algumas funcções que estão fora de ordem
void show_reserva_user_menu (GtkWidget *widget, gpointer data);
int str_cspn(const char *str1, const char *str2);
void save_voos_dados(void);
void load_voos_dados(void);
void save_voo (const char *num_voo_entry, const char *origem_voo_entry, const char *destino_voo_entry,
               const char *data_voo_entry, const char *hora_pt_voo_entry, const char *hora_cg_voo_entry);

//tipo poltrona
typedef struct{
    int pos_poltrona;
    int dispo;
    char nome_reserv[80];
}poltrona;

//tipo voo_dt
typedef struct{
    char num_voo[30];
    char origem_voo[50];
    char destino_voo[50];
    char data_voo[20];
    char hora_pt[10];
    char hora_cg[10];
    poltrona poltrona_dt[6][4];
}voo_dt;

//tipo usuario(não tem persistência)
typedef struct{
    char nome[80];
    long long num_ident;
}usuario;

//declaração dos arrays que guardarão os dados, e variavéis globais para contar a quantidade de voos e usuários 
int qnt_voo, qnt_user;
voo_dt voos00[50];
usuario def_user[50];

//declaração de varias variavéis do tipo GtkWidget que vão guardar as informações dos widgets usados, e outras para comunicação entre
//  diferentes funções
int index_voo;
GtkWidget *entry_num_voo;
GtkWidget *entry_origem;
GtkWidget *entry_destino;
GtkWidget *entry_data;
GtkWidget *entry_hora_pt;
GtkWidget *entry_hora_cg;

GtkWidget *label_error;
GtkWidget *erro_num;
GtkWidget *erro_origem;
GtkWidget *erro_destino;
GtkWidget *erro_data;
GtkWidget *erro_hora_pt;
GtkWidget *erro_hora_cg;

GtkWidget *label_voos;

GtkWidget *label_verif;
GtkWidget *entry_num_voo_verif;
GtkWidget *button_verif_dispo;
GtkWidget *label_error_verif;
GtkWidget *label_polt;
char temp_verif[300];
char str_polt[2000];
int verif_bool;

GtkWidget *label_reserva;
GtkWidget *label_reserva_polt;
GtkWidget *label_reserva2;
GtkWidget *entry_polt_verif;
GtkWidget *button_verif_polt;
GtkWidget *label_error_reserva;
GtkWidget *button_show_reserva_menu;
GtkWidget *error_show_reserva_menu;

GtkWidget *label_user;
GtkWidget *label_user_polt;
GtkWidget *entry_user_name;
GtkWidget *label_error_name;
GtkWidget *entry_user_id;
GtkWidget *label_error_id;
GtkWidget *label_user_error;
int index_user1;
int index_user2;

GtkWidget *label_conferir;
GtkWidget *label_entry_conferir;
GtkWidget *entry_name_conferir;
GtkWidget *label_error_conferir;
int admin_set;

GtkWidget *label_exib_pesquisar;
GtkWidget *label_origem_pesquisar;
GtkWidget *entry_origem_pesquisar;
GtkWidget *label_error_origem;
GtkWidget *label_destino_pesquisar;
GtkWidget *entry_destino_pesquisar;
GtkWidget *label_error_destino;
GtkWidget *label_data_pesquisar;
GtkWidget *entry_data_pesquisar;
GtkWidget *label_error_data;
GtkWidget *label_error_pesquisar;

//função para salvar dados dos voos em um(1) arquivo txt apenas, provalmente vai dar erro em algum momento
void save_voos_dados(void){
    FILE *dados = fopen("dados_voos.txt", "w");
    if(dados == NULL){
        printf("Erro ao abrir o arquivo!");
        return;
    }
    fprintf(dados, "%i %i\n", qnt_voo, qnt_user);

    for(int i = 0; i < qnt_voo; i++){
        fprintf(dados, "%s|%s|%s|%s|%s|%s\n", voos00[i].num_voo, voos00[i].origem_voo,voos00[i].destino_voo, voos00[i].data_voo,
                voos00[i].hora_pt, voos00[i].hora_cg);
        for(int j = 0; j < 6; j++){
            for(int k = 0; k < 4; k++){
                fprintf(dados, "%i|%i|%s\n",voos00[i].poltrona_dt[j][k].pos_poltrona, voos00[i].poltrona_dt[j][k].dispo, voos00[i].poltrona_dt[j][k].nome_reserv);
            }
        }
    }
    fclose(dados);
}

//função para ler dados dos voos de um(1) arquivo apenas, já deu erro e provalvemente vai dar novamente
void load_voos_dados(void){
    char num_f = '\n';
    FILE *dados = fopen("dados_voos.txt", "r");
    if(dados == NULL){
        printf("Erro ao abrir o arquivo!");
        return;
    }
    fscanf(dados, "%d %d\n", &qnt_voo, &qnt_user);
    for(int i = 0; i < qnt_voo; i++){
        int c = fgetc(dados);
        if (c != '\n') ungetc(c, dados);

        fscanf(dados, "%99[^|]|%99[^|]|%99[^|]|%99[^|]|%99[^|]|%99[^\n]\n", voos00[i].num_voo, voos00[i].origem_voo,voos00[i].destino_voo, voos00[i].data_voo,
               voos00[i].hora_pt, voos00[i].hora_cg);
        /*voos00[i].num_voo[str_cspn(voos00[i].num_voo, "\n")] = '-';*/
        printf("%s", voos00[i].num_voo);
        for(int j = 0; j < 6; j++){
            for(int k = 0; k < 4; k++){
                fscanf(dados, "%i|%i|%[^\n]\n", &voos00[i].poltrona_dt[j][k].pos_poltrona, &voos00[i].poltrona_dt[j][k].dispo, voos00[i].poltrona_dt[j][k].nome_reserv);
                /*if(str_len(voos00[i].poltrona_dt[j][k].nome_reserv) == 0){
                    str_copy(voos00[i].poltrona_dt[j][k].nome_reserv, "-");
                }*/
            }
        }
    }
    fclose(dados);
}

//função para encontrar a ocorrência de um char numa string, qualquer coisa diferente de um char dá erro
int str_cspn(const char *str1, const char *str2){
    int len = 0;
    while(*str1){
        if(*str1 == *str2){
            return len;
        }
        len++;
        str1++;
    }
    return len;
}

//função pra limpar buffer do terminal(detalhe, não se usa terminal!!!!)
void clear_buffer(){
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

//função pra converter um string para inteiro
long long str_int(const char *str1){
    if(*str1 == '\0'){
        return -4;
    }
    long long result = 0;

    while(*str1){
        if(*str1 >= '0' && *str1 <= '9'){
            result = result * 10 + (*str1 - '0');
        }
        else{
            return -4;
        }
        str1++;
    }
    return result;
}

//função que retorna o tamanho de um string
int str_len(const char *string){
    int len = 0;
    while(string[len] != '\0'){
        len++;
    }
    return len;
}

//função que concatena um string dentro de outro
char *str_cat (char *str1, const char *str2, size_t t){
    char *len = str1;

    while(*len != '\0'){
        len++;
    }
    while(*str2 != '\0' && t > 0){
        *len = *str2;
        len++;
        str2++;
        t--;
    }
    *len = '\0';
    return str1;
}

//função que copia o conteúdo de um string para outro
void str_copy(char *str1, const char *str2){
    int i_str2 = str_len(str2);
    int i = 0;
    while(i < i_str2){
        str1[i] = str2[i];
        i++;
    }
    str1[i] = '\0';
}

//função para comparar se dois strings são iguais ou diferentes
int str_comp(const char *str1, const char *str2){
    while(*str1 && *str2){
        if(*str1 != *str2){
            return *str1 - *str2;
        }
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

//função para validar o cadastro de um voo
void cadastrar_voo (GtkWidget *widget, gpointer data){
    int valid = 1;

    const char *num_voo_entry;
    const char *origem_voo_entry;
    const char *destino_voo_entry;
    const char *data_voo_entry;
    const char *hora_pt_voo_entry;
    const char *hora_cg_voo_entry;

    gtk_label_set_text (GTK_LABEL (label_error), "");
    gtk_label_set_text (GTK_LABEL (erro_num), "");
    gtk_label_set_text (GTK_LABEL (erro_origem), "");
    gtk_label_set_text (GTK_LABEL (erro_destino), "");
    gtk_label_set_text (GTK_LABEL (erro_data), "");
    gtk_label_set_text (GTK_LABEL (erro_hora_pt), "");
    gtk_label_set_text (GTK_LABEL (erro_hora_cg), "");

    if(qnt_voo > 0){
        num_voo_entry = gtk_editable_get_text (GTK_EDITABLE (entry_num_voo));
        if(str_len(num_voo_entry) == 0){
            valid = 2;
            gtk_label_set_text (GTK_LABEL (erro_num), "Numero não pode estar vazio!");
        }
        for(int i = 0; i < qnt_voo; i++){
            if(str_comp(voos00[i].num_voo, num_voo_entry) == 0){
                valid = 3;
                gtk_label_set_text (GTK_LABEL (erro_num), "Numero já cadastrado!");
            }
        }
    }
    else{
        num_voo_entry = gtk_editable_get_text (GTK_EDITABLE (entry_num_voo));
        if(str_len(num_voo_entry) == 0){
            valid = 2;
            gtk_label_set_text (GTK_LABEL (erro_num), "Numero não pode estar vazio!");
        }
    }

    origem_voo_entry = gtk_editable_get_text (GTK_EDITABLE (entry_origem));
    if(str_len(origem_voo_entry) == 0){
        valid = 4;
        gtk_label_set_text (GTK_LABEL (erro_origem), "Origem não pode estar vazia!");
    }

    destino_voo_entry = gtk_editable_get_text (GTK_EDITABLE (entry_destino));
    if(str_len(destino_voo_entry) == 0){
        valid = 5;
        gtk_label_set_text (GTK_LABEL (erro_destino), "Destino não pode estar vazio!");
    }

    data_voo_entry = gtk_editable_get_text (GTK_EDITABLE (entry_data));
    if(str_len(data_voo_entry) != 0){
        if(str_len(data_voo_entry) == 10 &&
            data_voo_entry[2] == '/' && data_voo_entry[5] == '/' &&
            data_voo_entry[0] >= '0' && data_voo_entry[0] <= '9' &&
            data_voo_entry[1] >= '0' && data_voo_entry[1] <= '9' &&
            data_voo_entry[3] >= '0' && data_voo_entry[3] <= '9' &&
            data_voo_entry[4] >= '0' && data_voo_entry[4] <= '9' &&
            data_voo_entry[6] >= '0' && data_voo_entry[6] <= '9' &&
            data_voo_entry[7] >= '0' && data_voo_entry[7] <= '9' &&
            data_voo_entry[8] >= '0' && data_voo_entry[8] <= '9' &&
            data_voo_entry[9] >= '0' && data_voo_entry[9] <= '9'){
            int dia = (data_voo_entry[0] - '0') * 10 + (data_voo_entry[1] - '0');
            int mes = (data_voo_entry[3] - '0') * 10 + (data_voo_entry[4] - '0');
            int ano = (data_voo_entry[6] - '0') * 1000 + (data_voo_entry[7] - '0') * 100 +
                      (data_voo_entry[8] - '0') * 10 + (data_voo_entry[9] - '0');
            if(dia >= 1 && dia <= 31 && mes >= 1 && mes <= 12 && ano >= 2025){
                gtk_widget_remove_css_class (erro_data, "error-label");
                gtk_widget_add_css_class (erro_data, "success-label");
                gtk_label_set_text (GTK_LABEL (erro_data), "Data válida!");
            }
            else{
                gtk_widget_remove_css_class (erro_data, "success-label");
                gtk_widget_add_css_class (erro_data, "error-label");
                gtk_label_set_text (GTK_LABEL (erro_data), "Data inválida!");
                valid = 6;
            }
        }
        else{
            gtk_widget_remove_css_class (erro_data, "success-label");
            gtk_widget_add_css_class (erro_data, "error-label");
            gtk_label_set_text (GTK_LABEL (erro_data), "Data inválida!\nFormato aceito: dd/mm/aaaa");
            valid = 6;
        }  
    }
    else{
        gtk_label_set_text (GTK_LABEL (erro_data), "Data não pode estar vazia!");
        valid = 6;
    }

    hora_pt_voo_entry = gtk_editable_get_text (GTK_EDITABLE (entry_hora_pt));
    if(str_len(hora_pt_voo_entry) != 0){
        if(str_len(hora_pt_voo_entry) == 5 &&
            hora_pt_voo_entry[2] == ':' &&
            hora_pt_voo_entry[0] >= '0' && hora_pt_voo_entry[0] <= '2' &&
            hora_pt_voo_entry[1] >= '0' && hora_pt_voo_entry[1] <= '9' &&
            hora_pt_voo_entry[3] >= '0' && hora_pt_voo_entry[3] <= '5' &&
            hora_pt_voo_entry[4] >= '0' && hora_pt_voo_entry[4] <= '9') {

            int hora = (hora_pt_voo_entry[0]-'0')*10 + (hora_pt_voo_entry[1]-'0');
            int minuto = (hora_pt_voo_entry[3]-'0')*10 + (hora_pt_voo_entry[4]-'0');

            if(hora >= 0 && hora <= 23 && minuto >= 0 && minuto <= 59){
                gtk_widget_remove_css_class (erro_hora_pt, "error-label");
                gtk_widget_add_css_class (erro_hora_pt, "success-label");
                gtk_label_set_text (GTK_LABEL (erro_hora_pt), "Hora de partida válida!");
            } 
            else{
                gtk_widget_remove_css_class (erro_hora_pt, "success-label");
                gtk_widget_add_css_class (erro_hora_pt, "error-label");
                gtk_label_set_text (GTK_LABEL (erro_hora_pt), "Hora de partida inválida!");
                valid = 7;
            }
        } 
        else{
            gtk_widget_remove_css_class (erro_hora_pt, "success-label");
            gtk_widget_add_css_class (erro_hora_pt, "error-label");
            gtk_label_set_text (GTK_LABEL (erro_hora_pt), "Hora de partida inválida!\nFormato aceito: hh:mm");
            valid = 7;
        }
    }
    else{
        valid = 7;
        gtk_widget_remove_css_class (erro_hora_pt, "success-label");
        gtk_widget_add_css_class (erro_hora_pt, "error-label");
        gtk_label_set_text (GTK_LABEL (erro_hora_pt), "Hora de partida não pode estar vazia!");
    }
    hora_cg_voo_entry = gtk_editable_get_text (GTK_EDITABLE (entry_hora_cg));
    if(str_len(hora_cg_voo_entry) != 0){
        if(str_len(hora_cg_voo_entry) == 5 &&
            hora_cg_voo_entry[2] == ':' &&
            hora_cg_voo_entry[0] >= '0' && hora_cg_voo_entry[0] <= '2' &&
            hora_cg_voo_entry[1] >= '0' && hora_cg_voo_entry[1] <= '9' &&
            hora_cg_voo_entry[3] >= '0' && hora_cg_voo_entry[3] <= '5' &&
            hora_cg_voo_entry[4] >= '0' && hora_cg_voo_entry[4] <= '9') {

            int hora = (hora_cg_voo_entry[0]-'0')*10 + (hora_cg_voo_entry[1]-'0');
            int minuto = (hora_cg_voo_entry[3]-'0')*10 + (hora_cg_voo_entry[4]-'0');

            if(hora >= 0 && hora <= 23 && minuto >= 0 && minuto <= 59){
                gtk_widget_remove_css_class (erro_hora_cg, "error-label");
                gtk_widget_add_css_class (erro_hora_cg, "success-label");
                gtk_label_set_text (GTK_LABEL (erro_hora_cg), "Hora de partida válida!");
            } 
            else{
                gtk_widget_remove_css_class (erro_hora_cg, "success-label");
                gtk_widget_add_css_class (erro_hora_cg, "error-label");
                gtk_label_set_text (GTK_LABEL (erro_hora_cg), "Hora de partida inválida!");
                valid = 8;
            }
        } 
        else{
            gtk_widget_remove_css_class (erro_hora_cg, "success-label");
            gtk_widget_add_css_class (erro_hora_cg, "error-label");
            gtk_label_set_text (GTK_LABEL (erro_hora_cg), "Hora de partida inválida!\nFormato aceito: hh:mm");
            valid = 8;
        }
    }
    else{
        valid = 8;
        gtk_widget_remove_css_class (erro_hora_cg, "success-label");
        gtk_widget_add_css_class (erro_hora_cg, "error-label");
        gtk_label_set_text (GTK_LABEL (erro_hora_cg), "Hora de partida não pode estar vazia!");
    }

    if(valid == 1){
        save_voo (num_voo_entry, origem_voo_entry, destino_voo_entry,
                  data_voo_entry, hora_pt_voo_entry, hora_cg_voo_entry);
        gtk_widget_remove_css_class (label_error, "error-label");
        gtk_widget_add_css_class (label_error, "success-label");
        gtk_label_set_text (GTK_LABEL (label_error), "Dados salvos com sucesso!");
    }
    else{
        gtk_widget_remove_css_class (label_error, "success-label");
        gtk_widget_add_css_class (label_error, "error-label");
        gtk_label_set_text (GTK_LABEL (label_error), "Preencha os campos corretamente!");
    }
}

//função que guardar os dados de um voo no array voos00 e inicializa todas a poltronas
void save_voo (const char *num_voo_entry, const char *origem_voo_entry, const char *destino_voo_entry,
               const char *data_voo_entry, const char *hora_pt_voo_entry, const char *hora_cg_voo_entry){
    str_copy (voos00[qnt_voo].num_voo, num_voo_entry);
    str_copy (voos00[qnt_voo].origem_voo, origem_voo_entry);
    str_copy (voos00[qnt_voo].destino_voo, destino_voo_entry);
    str_copy (voos00[qnt_voo].data_voo, data_voo_entry);
    str_copy (voos00[qnt_voo].hora_pt, hora_pt_voo_entry);
    str_copy (voos00[qnt_voo].hora_cg, hora_cg_voo_entry);

    int polt_up = 0;
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 4; j++){
            voos00[qnt_voo].poltrona_dt[i][j].dispo = 1;
            voos00[qnt_voo].poltrona_dt[i][j].pos_poltrona = polt_up + 1;
            /*strcpy(voos00[i].poltrona_dt[i][j].nome_reserv, "/");*/
            polt_up++;
        }
    }
    qnt_voo++;
}

//função pra limpar a variavel str_polt, usada para colocar os dados das poltronas na tela
void clear_str_polt (void){
    for(int i = 0; i < 2000; i++){
        str_polt[i] = 0;
    }
}

//função para listar todos os voos disponiveis 
void listar_voos (GtkWidget *widget, gpointer data){
    gtk_label_set_text (GTK_LABEL (label_voos), "");
    char str_voos[3000] = {0};
    char temp[300] = {0};

    if (qnt_voo == 0){
        gtk_widget_remove_css_class (label_voos, "success-label");
        gtk_widget_add_css_class (label_voos, "error-label");
        gtk_label_set_text (GTK_LABEL (label_voos), "Nenhum voo cadastrado!");
    }
    else{
        for(int i = 0; i < qnt_voo; i++){
            gtk_widget_remove_css_class (label_voos, "error-label");
            gtk_widget_add_css_class (label_voos, "success-label");

            snprintf(temp, sizeof(temp),"N° do voo: %s | Origem: %s | Destino: %s | Data: %s | Hora de partida: %s | Hora de chegada: %s\n", 
            voos00[i].num_voo, voos00[i].origem_voo, voos00[i].destino_voo, voos00[i].data_voo, voos00[i].hora_pt, voos00[i].hora_cg);

            str_cat (str_voos, temp, sizeof(str_voos) - str_len(str_voos) - 1);
        }
        gtk_label_set_text (GTK_LABEL (label_voos), str_voos);
    }
}

//função para concatenar um string que descreve a posição e disponibilidade de cada poltrona de um voo,
//dentro da variavel str_polt
void print_polt (){
    char temp[600] = {0};
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 4; j++){
            if(voos00[index_voo].poltrona_dt[i][j].dispo == 1){
                snprintf(temp, sizeof(temp), "|Poltrona %i: <span foreground='green'>Livre</span>     | ", voos00[index_voo].poltrona_dt[i][j].pos_poltrona);
                str_cat(str_polt, temp, sizeof(str_polt) - str_len(str_polt) - 1);
            }
            else{
                snprintf(temp, sizeof(temp), "|Poltrona %i: <span foreground='red'>Reservada</span> | ", voos00[index_voo].poltrona_dt[i][j].pos_poltrona);
                str_cat(str_polt, temp, sizeof(str_polt) - str_len(str_polt) - 1);
            }
        }
        snprintf(temp, sizeof(temp), "\n");
        str_cat(str_polt, temp, sizeof(str_polt) - str_len(str_polt) - 1);
    }
    gtk_label_set_use_markup (GTK_LABEL (label_polt), TRUE);
    gtk_label_set_markup (GTK_LABEL (label_polt), str_polt);
}

//função para encontrar um voo no sistema através de seu número, e mostrar os dados
void verif_voos (GtkWidget *widget, gpointer data){
    gtk_label_set_text (GTK_LABEL (error_show_reserva_menu), "");
    gtk_widget_add_css_class (label_error_verif, "success-label");
    gtk_label_set_text (GTK_LABEL (label_verif),"Digite o numero do voo:");

    clear_str_polt();
    const char *num_voo_verif_entry;
    int num_voo_str_verif = 0;

    num_voo_verif_entry = gtk_editable_get_text (GTK_EDITABLE (entry_num_voo_verif));
    num_voo_str_verif = str_int(num_voo_verif_entry);
    if(str_len(num_voo_verif_entry) == 0 /*|| num_voo_str_verif == -4*/){
        gtk_widget_remove_css_class (label_error_verif, "success-label");
        gtk_widget_add_css_class (label_error_verif, "error-label");
        gtk_label_set_text (GTK_LABEL (label_error_verif), "Digite um número válido!");
    }
    else{
        for(int i = 0; i < qnt_voo; i++){
            if(str_comp(num_voo_verif_entry, voos00[i].num_voo) == 0){
                index_voo = i;
                snprintf(temp_verif, sizeof(temp_verif),"N° do voo: %s | Origem: %s | Destino: %s | Data: %s | Hora de partida: %s | Hora de chegada: %s\n", 
                voos00[i].num_voo, voos00[i].origem_voo, voos00[i].destino_voo, voos00[i].data_voo, voos00[i].hora_pt, voos00[i].hora_cg);

                gtk_label_set_text (GTK_LABEL (label_verif), temp_verif);
                print_polt ();

                gtk_widget_remove_css_class (label_error_verif, "error-label");
                gtk_widget_add_css_class (label_error_verif, "success-label");
                gtk_label_set_text (GTK_LABEL (label_error_verif), "Voo encontrado com sucesso!");
                verif_bool = 1;
            }
        }
    }
    if(verif_bool == 0){
        gtk_label_set_text (GTK_LABEL (label_polt),"");
        gtk_editable_set_text(GTK_EDITABLE(entry_num_voo_verif), "");
        gtk_widget_remove_css_class (label_error_verif, "success-label");
        gtk_widget_add_css_class (label_error_verif, "error-label");
        gtk_label_set_text (GTK_LABEL (label_error_verif), "Voo não encontrado!");
    }
}

//função que atualiza a janela com o menu para o usuário fornecer seus dados e efetuar uma reserva
void show_user_input_menu (GtkWidget *widget, gpointer data){
    show_reserva_user_menu (widget, data);
}

//função para guardar os dados que o usuário forneceu
void save_user_data (long long user_dt, const char *user_name_entry){
    str_copy (voos00[index_voo].poltrona_dt[index_user1][index_user2].nome_reserv, user_name_entry);
    voos00[index_voo].poltrona_dt[index_user1][index_user2].dispo = 0;

    def_user[qnt_user].num_ident = user_dt;
    str_copy (def_user[qnt_user].nome, user_name_entry);
}

//função para validar os dados digitados pelo usuário (nome, CPF) 
void get_user_input (GtkWidget *widget, gpointer data){
    gtk_label_set_text (GTK_LABEL (label_error_name), "");
    gtk_label_set_text (GTK_LABEL (label_error_id), "");
    gtk_label_set_text (GTK_LABEL (label_user_error), "");

    int bool_user_valid = 1;
    const char *user_name_entry;
    const char *user_id_entry;
    long long valid_user_id = 0;

    user_name_entry = gtk_editable_get_text (GTK_EDITABLE (entry_user_name));
    if(str_len(user_name_entry) == 0){
        bool_user_valid = 2;
        gtk_widget_add_css_class (label_error_name, "error-label");
        gtk_label_set_text (GTK_LABEL (label_error_name), "Nome não pode estar vazio!");
    }
    user_id_entry = gtk_editable_get_text (GTK_EDITABLE (entry_user_id));
    if(str_len(user_id_entry) < 11 || str_len(user_id_entry) > 11){
        bool_user_valid = 3;
        gtk_widget_add_css_class (label_error_id, "error-label");
        gtk_label_set_text (GTK_LABEL (label_error_id), "CPF inválido!");
    }
    else{
        valid_user_id = str_int(user_id_entry);
    }

    if(bool_user_valid == 1){
        gtk_widget_remove_css_class (label_user_error, "error-label");
        gtk_widget_add_css_class (label_user_error, "success-label");
        gtk_label_set_text (GTK_LABEL (label_user_error), "Reserva concluída!");
        save_user_data (valid_user_id, user_name_entry);
        qnt_user++;
    }
    else{
        gtk_widget_remove_css_class (label_user_error, "success-label");
        gtk_widget_add_css_class (label_user_error, "error-label");
        gtk_label_set_text (GTK_LABEL (label_user_error), "Preencha os campos corretamente!");
    }
    clear_str_polt ();
    print_polt ();
    gtk_widget_add_css_class (label_user, "success-label");
    gtk_label_set_text (GTK_LABEL (label_user), temp_verif);
    gtk_label_set_use_markup (GTK_LABEL (label_reserva_polt), TRUE);
    gtk_label_set_markup (GTK_LABEL (label_user_polt), str_polt);
}

//função que busca e seleciona uma poltrona pela sua posição
void efetuar_reserva (GtkWidget *widget, gpointer data){
    gtk_label_set_text (GTK_LABEL (label_error_reserva), "");

    const char *polt_verif_entry;
    int temp_pos_polt = 0;
    int bool_reserv = 0;
    int polt_found = 0;
    int polt_dispo = 0;
    int temp_pos_polt_valid = 1;
    int index1 = 0;
    int index2 = 0;

    polt_verif_entry = gtk_editable_get_text (GTK_EDITABLE (entry_polt_verif));
    temp_pos_polt = str_int(polt_verif_entry);
    if(temp_pos_polt == -4){
        gtk_widget_remove_css_class (label_error_reserva, "success-label");
        gtk_widget_add_css_class (label_error_reserva, "error-label");
        gtk_label_set_text (GTK_LABEL (label_error_reserva), "Digite uma posição válida!");
        temp_pos_polt_valid = 0;
    }

    for(index1 = 0; index1 < 6; index1++){
        for(index2 = 0; index2 < 4; index2++){
            if(temp_pos_polt == voos00[index_voo].poltrona_dt[index1][index2].pos_poltrona){
                polt_found = 1;
                if(voos00[index_voo].poltrona_dt[index1][index2].dispo == 1){
                    polt_dispo = 1;
                    break;
                }
                else{
                    polt_dispo = 0;
                }
            }
        }
        if(polt_found == 1 && polt_dispo == 1){
            index_user1 = index1;
            index_user2 = index2;
            break;
        }
    }
    
    if(polt_found == 1){
        gtk_widget_remove_css_class (label_error_reserva, "error-label");
        gtk_widget_add_css_class (label_error_reserva, "success-label");
        gtk_label_set_text (GTK_LABEL (label_error_reserva), "Poltrona encontrada!");
    }
    else if(temp_pos_polt_valid == 1 && polt_found != 1){
        gtk_widget_remove_css_class (label_error_reserva, "success-label");
        gtk_widget_add_css_class (label_error_reserva, "error-label");
        gtk_label_set_text (GTK_LABEL (label_error_reserva), "A poltrona não foi encontrada!");
    }
    if(polt_found == 1 && polt_dispo == 1){
        gtk_widget_remove_css_class (label_error_reserva, "error-label");
        gtk_widget_add_css_class (label_error_reserva, "success-label");
        gtk_label_set_text (GTK_LABEL (label_error_reserva), "Poltrona disponível!");
        show_user_input_menu (widget, data);
    }
    if(polt_found == 1 && polt_dispo != 1){
        gtk_widget_add_css_class (label_error_reserva, "error-label");
        gtk_label_set_text (GTK_LABEL (label_error_reserva), "A poltrona já foi reservada!");
    }
}

//função para buscar um nome de usuario relacionado com uma poltrona, uma reserva já efetuada
void conferir_reserva (GtkWidget *widget, gpointer data){
    const char *name_conferir_entry;
    char *admin_verif = {0};
    int bool_found_reserva = 0;
    char print_reserva[1500] = {0};

    name_conferir_entry = gtk_editable_get_text (GTK_EDITABLE (entry_name_conferir));
    if(str_len(name_conferir_entry) == 0){
        gtk_widget_remove_css_class (label_error_conferir, "success-label");
        gtk_widget_add_css_class (label_error_conferir, "error-label");
        gtk_label_set_text (GTK_LABEL (label_error_conferir), "Preencha o campo corretamente!");
    }
    else{
        for(int i = 0; i < qnt_voo; i++){
            for(int j = 0; j < 6; j++){
                for(int k = 0; k < 4; k++){
                    if(str_comp(name_conferir_entry, voos00[i].poltrona_dt[j][k].nome_reserv) == 0){
                        char temp[400] = {0};
                        snprintf(temp, sizeof(temp), "Numero do voo: %s | Origem: %s | Destino: %s | Data: %s | Poltrona: %i \n", voos00[i].num_voo, voos00[i].origem_voo, voos00[i].destino_voo, voos00[i].data_voo, voos00[i].poltrona_dt[j][k].pos_poltrona);
                        str_cat(print_reserva, temp, sizeof(print_reserva) - str_len(print_reserva) - 1);
                        bool_found_reserva = 1;
                    }
                }
            }
        }
    }
    if(bool_found_reserva == 1 && qnt_voo != 0){
        gtk_widget_remove_css_class (label_error_conferir, "error-label");
        gtk_widget_add_css_class (label_error_conferir, "success-label");
        gtk_label_set_text (GTK_LABEL (label_error_conferir), "Reserva Encontrada!");

        gtk_label_set_text (GTK_LABEL (label_conferir), print_reserva);
    }
    else{
        gtk_widget_remove_css_class (label_error_conferir, "success-label");
        gtk_widget_add_css_class (label_error_conferir, "error-label");
        gtk_label_set_text (GTK_LABEL (label_error_conferir), "Reserva não encontrada!");
    }
}

//função para buscar um voo, baseado na sua origem, destino e data de partida
void pesquisar_voos (GtkWidget *widget, gpointer data){
    gtk_label_set_text (GTK_LABEL (label_exib_pesquisar), "");
    gtk_label_set_text (GTK_LABEL (label_error_pesquisar), "");
    const char *origem_pesquisar_entry;
    const char *destino_pesquisar_entry;
    const char *data_pesquisar_entry;
    char print_pesquisar[2000] = {0};
    int origem_valid = 1, destino_valid = 1, data_valid = 1, pesquisar_found = 0;

    origem_pesquisar_entry = gtk_editable_get_text (GTK_EDITABLE (entry_origem_pesquisar));
    if(str_len(origem_pesquisar_entry) == 0){
        origem_valid = 0;
    }
    destino_pesquisar_entry = gtk_editable_get_text (GTK_EDITABLE (entry_destino_pesquisar));
    if(str_len(destino_pesquisar_entry) == 0){
        destino_valid = 0;
    }
    data_pesquisar_entry = gtk_editable_get_text (GTK_EDITABLE (entry_data_pesquisar));
    if(str_len(data_pesquisar_entry) != 0){
        if(str_len(data_pesquisar_entry) == 10 &&
            data_pesquisar_entry[2] == '/' && data_pesquisar_entry[5] == '/' &&
            data_pesquisar_entry[0] >= '0' && data_pesquisar_entry[0] <= '9' &&
            data_pesquisar_entry[1] >= '0' && data_pesquisar_entry[1] <= '9' &&
            data_pesquisar_entry[3] >= '0' && data_pesquisar_entry[3] <= '9' &&
            data_pesquisar_entry[4] >= '0' && data_pesquisar_entry[4] <= '9' &&
            data_pesquisar_entry[6] >= '0' && data_pesquisar_entry[6] <= '9' &&
            data_pesquisar_entry[7] >= '0' && data_pesquisar_entry[7] <= '9' &&
            data_pesquisar_entry[8] >= '0' && data_pesquisar_entry[8] <= '9' &&
            data_pesquisar_entry[9] >= '0' && data_pesquisar_entry[9] <= '9'){
            int dia = (data_pesquisar_entry[0] - '0') * 10 + (data_pesquisar_entry[1] - '0');
            int mes = (data_pesquisar_entry[3] - '0') * 10 + (data_pesquisar_entry[4] - '0');
            int ano = (data_pesquisar_entry[6] - '0') * 1000 + (data_pesquisar_entry[7] - '0') * 100 +
                      (data_pesquisar_entry[8] - '0') * 10 + (data_pesquisar_entry[9] - '0');
            if(dia >= 1 && dia <= 31 && mes >= 1 && mes <= 12 && ano >= 2025){
                gtk_widget_remove_css_class (label_error_data, "error-label");
                gtk_widget_add_css_class (label_error_data, "success-label");
                gtk_label_set_text (GTK_LABEL (label_error_data), "Data válida!");
                data_valid = 1;
            }
            else{
                gtk_widget_remove_css_class (label_error_data, "success-label");
                gtk_widget_add_css_class (label_error_data, "error-label");
                gtk_label_set_text (GTK_LABEL (label_error_data), "Data inválida!");
                data_valid = 0;
            }
        }
        else{
            gtk_widget_remove_css_class (label_error_data, "success-label");
            gtk_widget_add_css_class (label_error_data, "error-label");
            gtk_label_set_text (GTK_LABEL (label_error_data), "Data inválida!\nFormato aceito: dd/mm/aaaa");
            data_valid = 0;
        }  
    }
    else{
        data_valid = 0;
    }

    if(origem_valid == 1 && destino_valid == 0 && data_valid == 0){
        for(int i = 0; i < qnt_voo; i++){
            if(str_comp(origem_pesquisar_entry, voos00[i].origem_voo) == 0){
                char temp_pesquisar[600] = {0};
                snprintf(temp_pesquisar, sizeof(temp_pesquisar),"N° do voo: %s | Origem: <span foreground='green'>%s</span> | Destino: %s | Data: %s | Hora de partida: %s | Hora de chegada: %s\n", 
                voos00[i].num_voo, voos00[i].origem_voo, voos00[i].destino_voo, voos00[i].data_voo, voos00[i].hora_pt, voos00[i].hora_cg);

                str_cat (print_pesquisar, temp_pesquisar, sizeof(print_pesquisar) - str_len(print_pesquisar) - 1);
                pesquisar_found = 1;
            }
        }    
        if(pesquisar_found == 1){
            gtk_widget_remove_css_class (label_error_pesquisar, "error-label");
            gtk_widget_add_css_class (label_error_pesquisar, "success-label");
            gtk_label_set_text (GTK_LABEL (label_error_pesquisar), "Voo encontrado com sucesso!");

            gtk_label_set_use_markup (GTK_LABEL (label_exib_pesquisar), TRUE);
            gtk_label_set_markup (GTK_LABEL (label_exib_pesquisar), print_pesquisar);
        }
        else{
            gtk_widget_remove_css_class (label_error_pesquisar, "success-label");
            gtk_widget_add_css_class (label_error_pesquisar, "error-label");
            gtk_label_set_text (GTK_LABEL (label_error_pesquisar), "Voo não encontrado!");
        }
    }
    if(destino_valid == 1 && origem_valid == 0 && data_valid == 0){
        for(int i = 0; i < qnt_voo; i++){
            if(str_comp(destino_pesquisar_entry, voos00[i].destino_voo) == 0){
                char temp_pesquisar[400] = {0};
                snprintf(temp_pesquisar, sizeof(temp_pesquisar),"N° do voo: %s | Origem: %s | Destino: <span foreground='green'>%s</span> | Data: %s | Hora de partida: %s | Hora de chegada: %s\n", 
                voos00[i].num_voo, voos00[i].origem_voo, voos00[i].destino_voo, voos00[i].data_voo, voos00[i].hora_pt, voos00[i].hora_cg);

                str_cat (print_pesquisar, temp_pesquisar, sizeof(print_pesquisar) - str_len(print_pesquisar) - 1);
                pesquisar_found = 1;
            }
        }    
        if(pesquisar_found == 1){
            gtk_widget_remove_css_class (label_error_pesquisar, "error-label");
            gtk_widget_add_css_class (label_error_pesquisar, "success-label");
            gtk_label_set_text (GTK_LABEL (label_error_pesquisar), "Voo encontrado com sucesso!");

            gtk_label_set_use_markup (GTK_LABEL (label_exib_pesquisar), TRUE);
            gtk_label_set_markup (GTK_LABEL (label_exib_pesquisar), print_pesquisar);
        }
        else{
            gtk_widget_remove_css_class (label_error_pesquisar, "success-label");
            gtk_widget_add_css_class (label_error_pesquisar, "error-label");
            gtk_label_set_text (GTK_LABEL (label_error_pesquisar), "Voo não encontrado!");
        }
    }
    if(data_valid == 1 && origem_valid == 0 && destino_valid == 0){
        for(int i = 0; i < qnt_voo; i++){
            if(str_comp(data_pesquisar_entry, voos00[i].data_voo) == 0){
                char temp_pesquisar[400] = {0};
                snprintf(temp_pesquisar, sizeof(temp_pesquisar),"N° do voo: %s | Origem: %s | Destino: %s | Data: <span foreground='green'>%s</span> | Hora de partida: %s | Hora de chegada: %s\n", 
                voos00[i].num_voo, voos00[i].origem_voo, voos00[i].destino_voo, voos00[i].data_voo, voos00[i].hora_pt, voos00[i].hora_cg);

                str_cat (print_pesquisar, temp_pesquisar, sizeof(print_pesquisar) - str_len(print_pesquisar) - 1);
                pesquisar_found = 1;
            }
        }
        if(pesquisar_found == 1){
            gtk_widget_remove_css_class (label_error_pesquisar, "error-label");
            gtk_widget_add_css_class (label_error_pesquisar, "success-label");
            gtk_label_set_text (GTK_LABEL (label_error_pesquisar), "Voo encontrado com sucesso!");
            
            gtk_label_set_use_markup (GTK_LABEL (label_exib_pesquisar), TRUE);
            gtk_label_set_markup (GTK_LABEL (label_exib_pesquisar), print_pesquisar);
        }
        else{
            gtk_widget_remove_css_class (label_error_pesquisar, "success-label");
            gtk_widget_add_css_class (label_error_pesquisar, "error-label");
            gtk_label_set_text (GTK_LABEL (label_error_pesquisar), "Voo não encontrado!");
        }
    }
    if(origem_valid == 1 && destino_valid == 1 && data_valid == 0){
        for(int i = 0; i < qnt_voo; i++){
            if(str_comp(origem_pesquisar_entry, voos00[i].origem_voo) == 0 && str_comp(destino_pesquisar_entry, voos00[i].destino_voo) == 0){
                char temp_pesquisar[400] = {0};
                snprintf(temp_pesquisar, sizeof(temp_pesquisar),"N° do voo: %s | Origem: <span foreground='green'>%s</span> | Destino: <span foreground='green'>%s</span> | Data: %s | Hora de partida: %s | Hora de chegada: %s\n", 
                voos00[i].num_voo, voos00[i].origem_voo, voos00[i].destino_voo, voos00[i].data_voo, voos00[i].hora_pt, voos00[i].hora_cg);

                str_cat (print_pesquisar, temp_pesquisar, sizeof(print_pesquisar) - str_len(print_pesquisar) - 1);
                pesquisar_found = 1;
            }
        }    
        if(pesquisar_found == 1){
            gtk_widget_remove_css_class (label_error_pesquisar, "error-label");
            gtk_widget_add_css_class (label_error_pesquisar, "success-label");
            gtk_label_set_text (GTK_LABEL (label_error_pesquisar), "Voo encontrado com sucesso!");

            gtk_label_set_use_markup (GTK_LABEL (label_exib_pesquisar), TRUE);
            gtk_label_set_markup (GTK_LABEL (label_exib_pesquisar), print_pesquisar);
        }
        else{
            gtk_widget_remove_css_class (label_error_pesquisar, "success-label");
            gtk_widget_add_css_class (label_error_pesquisar, "error-label");
            gtk_label_set_text (GTK_LABEL (label_error_pesquisar), "Voo não encontrado!");
        }
    }
    if(origem_valid == 0 && destino_valid == 1 && data_valid == 1){
        for(int i = 0; i < qnt_voo; i++){
            if(str_comp(destino_pesquisar_entry, voos00[i].destino_voo) == 0 && str_comp(data_pesquisar_entry, voos00[i].data_voo) == 0){
                char temp_pesquisar[400] = {0};
                snprintf(temp_pesquisar, sizeof(temp_pesquisar),"N° do voo: %s | Origem: %s | Destino: <span foreground='green'>%s</span> | Data: <span foreground='green'>%s</span> | Hora de partida: %s | Hora de chegada: %s\n", 
                voos00[i].num_voo, voos00[i].origem_voo, voos00[i].destino_voo, voos00[i].data_voo, voos00[i].hora_pt, voos00[i].hora_cg);

                str_cat (print_pesquisar, temp_pesquisar, sizeof(print_pesquisar) - str_len(print_pesquisar) - 1);
                pesquisar_found = 1;
            }
        }    
        if(pesquisar_found == 1){
            gtk_widget_remove_css_class (label_error_pesquisar, "error-label");
            gtk_widget_add_css_class (label_error_pesquisar, "success-label");
            gtk_label_set_text (GTK_LABEL (label_error_pesquisar), "Voo encontrado com sucesso!");

            gtk_label_set_use_markup (GTK_LABEL (label_exib_pesquisar), TRUE);
            gtk_label_set_markup (GTK_LABEL (label_exib_pesquisar), print_pesquisar);
        }
        else{
            gtk_widget_remove_css_class (label_error_pesquisar, "success-label");
            gtk_widget_add_css_class (label_error_pesquisar, "error-label");
            gtk_label_set_text (GTK_LABEL (label_error_pesquisar), "Voo não encontrado!");
        }
    }
    if(origem_valid == 1 && destino_valid == 0 && data_valid == 1){
        for(int i = 0; i < qnt_voo; i++){
            if(str_comp(origem_pesquisar_entry, voos00[i].origem_voo) == 0 && str_comp(data_pesquisar_entry, voos00[i].data_voo) == 0){
                char temp_pesquisar[400] = {0};
                snprintf(temp_pesquisar, sizeof(temp_pesquisar),"N° do voo: %s | Origem: <span foreground='green'>%s</span> | Destino: %s | Data: <span foreground='green'>%s</span> | Hora de partida: %s | Hora de chegada: %s\n", 
                voos00[i].num_voo, voos00[i].origem_voo, voos00[i].destino_voo, voos00[i].data_voo, voos00[i].hora_pt, voos00[i].hora_cg);

                str_cat (print_pesquisar, temp_pesquisar, sizeof(print_pesquisar) - str_len(print_pesquisar) - 1);
                pesquisar_found = 1;
            }
        }    
        if(pesquisar_found == 1){
            gtk_widget_remove_css_class (label_error_pesquisar, "error-label");
            gtk_widget_add_css_class (label_error_pesquisar, "success-label");
            gtk_label_set_text (GTK_LABEL (label_error_pesquisar), "Voo encontrado com sucesso!");

            gtk_label_set_use_markup (GTK_LABEL (label_exib_pesquisar), TRUE);
            gtk_label_set_markup (GTK_LABEL (label_exib_pesquisar), print_pesquisar);
        }
        else{
            gtk_widget_remove_css_class (label_error_pesquisar, "success-label");
            gtk_widget_add_css_class (label_error_pesquisar, "error-label");
            gtk_label_set_text (GTK_LABEL (label_error_pesquisar), "Voo não encontrado!");
        }
    }
    if(origem_valid == 1 && destino_valid == 1 && data_valid == 1){
        for(int i = 0; i < qnt_voo; i++){
            if(str_comp(origem_pesquisar_entry, voos00[i].origem_voo) == 0 && str_comp(destino_pesquisar_entry, voos00[i].destino_voo) == 0 && str_comp(data_pesquisar_entry, voos00[i].data_voo) == 0){
                char temp_pesquisar[400] = {0};
                snprintf(temp_pesquisar, sizeof(temp_pesquisar),"N° do voo: %s | Origem: <span foreground='green'>%s</span> | Destino: <span foreground='green'>%s</span> | Data: <span foreground='green'>%s</span> | Hora de partida: %s | Hora de chegada: %s\n", 
                voos00[i].num_voo, voos00[i].origem_voo, voos00[i].destino_voo, voos00[i].data_voo, voos00[i].hora_pt, voos00[i].hora_cg);

                str_cat (print_pesquisar, temp_pesquisar, sizeof(print_pesquisar) - str_len(print_pesquisar) - 1);
                pesquisar_found = 1;
            }
        }    
        if(pesquisar_found == 1){
            gtk_widget_remove_css_class (label_error_pesquisar, "error-label");
            gtk_widget_add_css_class (label_error_pesquisar, "success-label");
            gtk_label_set_text (GTK_LABEL (label_error_pesquisar), "Voo encontrado com sucesso!");

            gtk_label_set_use_markup (GTK_LABEL (label_exib_pesquisar), TRUE);
            gtk_label_set_markup (GTK_LABEL (label_exib_pesquisar), print_pesquisar);
        }
        else{
            gtk_widget_remove_css_class (label_error_pesquisar, "success-label");
            gtk_widget_add_css_class (label_error_pesquisar, "error-label");
            gtk_label_set_text (GTK_LABEL (label_error_pesquisar), "Voo não encontrado!");
        }
    }
    if(origem_valid == 0 && destino_valid == 0 && data_valid == 0){
        gtk_widget_remove_css_class (label_error_pesquisar, "success-label");
        gtk_widget_add_css_class (label_error_pesquisar, "error-label");
        gtk_label_set_text (GTK_LABEL (label_error_pesquisar), "Preencha ao menos um campo!");
    }
}

//função pra printar hello world no terminal, só usei pra testar os botões
void print_hello (GtkWidget *widget, gpointer data){
    g_print ("Hello World\n");
}

//função pra limpar todos label de erro e entrys do menu de cadastro
void clear_all_cd_voo (){
    gtk_editable_set_text(GTK_EDITABLE(entry_num_voo), "");
    gtk_editable_set_text(GTK_EDITABLE(entry_origem), "");
    gtk_editable_set_text(GTK_EDITABLE(entry_destino), "");
    gtk_editable_set_text(GTK_EDITABLE(entry_data), "");
    gtk_editable_set_text(GTK_EDITABLE(entry_hora_pt), "");
    gtk_editable_set_text(GTK_EDITABLE(entry_hora_cg), "");
    
    gtk_label_set_text (GTK_LABEL (label_error), "");
    gtk_label_set_text (GTK_LABEL (erro_num), "");
    gtk_label_set_text (GTK_LABEL (erro_origem), "");
    gtk_label_set_text (GTK_LABEL (erro_destino), "");
    gtk_label_set_text (GTK_LABEL (erro_data), "");
    gtk_label_set_text (GTK_LABEL (erro_hora_pt), "");
    gtk_label_set_text (GTK_LABEL (erro_hora_cg), "");
}

//função que atualiza a janela com o menu_main
void show_main_menu (GtkWidget *widget, gpointer data){
    GtkStack *stack = GTK_STACK (data);
    gtk_stack_set_visible_child_name (stack, "main");
}

//função que atualiza a janela com o menu_cadastro
void show_cadastro_menu (GtkWidget *widget, gpointer data){
    clear_all_cd_voo ();
    GtkStack *stack = GTK_STACK (data);
    gtk_stack_set_visible_child_name (stack, "Cadastro");
}

//função que atualiza a janela com o menu_voos
void show_voos_menu (GtkWidget *widget, gpointer data){
    gtk_label_set_text (GTK_LABEL (label_voos), ""); 
    GtkStack *stack = GTK_STACK (data);
    gtk_stack_set_visible_child_name (stack, "voos");
}

//função que atualiza a janela com o menu_verif
void show_verif_menu (GtkWidget *widget, gpointer data){
    gtk_editable_set_text (GTK_EDITABLE (entry_num_voo_verif), "");
    gtk_label_set_text (GTK_LABEL (label_error_verif), "");
    gtk_label_set_text (GTK_LABEL (error_show_reserva_menu), "");
    gtk_label_set_text (GTK_LABEL (label_polt), "");
    GtkStack *stack = GTK_STACK (data);
    verif_bool = 0;

    if(qnt_voo == 0){
        gtk_widget_set_visible (button_verif_dispo, FALSE);
        gtk_widget_set_visible (entry_num_voo_verif, FALSE);
        gtk_widget_set_visible (label_error_verif, FALSE);
        gtk_widget_set_visible (button_show_reserva_menu, FALSE);

        gtk_widget_remove_css_class (label_verif, "success-label");
        gtk_widget_add_css_class (label_verif, "error-label");
        gtk_label_set_text (GTK_LABEL (label_verif), "Nenhum voo cadastrado!");
    }
    else{
        gtk_widget_set_visible (button_verif_dispo, TRUE);
        gtk_widget_set_visible (entry_num_voo_verif, TRUE);
        gtk_widget_set_visible (label_error_verif, TRUE);
        gtk_widget_set_visible (button_show_reserva_menu, TRUE);

        gtk_widget_remove_css_class (label_verif, "error-label");
        gtk_widget_add_css_class (label_verif, "success-label");
        gtk_label_set_text (GTK_LABEL (label_verif),"Digite o numero do voo:");
    }
    gtk_stack_set_visible_child_name (stack, "verif");
}

//função que atualiza a janela com o menu_reserva
void show_reserva_menu (GtkWidget *widget, gpointer data){
    clear_str_polt();
    print_polt ();
    gtk_editable_set_text (GTK_EDITABLE (entry_polt_verif), "");
    gtk_label_set_text (GTK_LABEL (label_error_reserva), "");
    GtkStack *stack = GTK_STACK (data);
    gtk_stack_set_visible_child_name (stack, "reserva");

    gtk_widget_add_css_class (label_reserva, "success-label");
    gtk_label_set_text (GTK_LABEL (label_reserva), temp_verif);
    gtk_label_set_use_markup (GTK_LABEL (label_reserva_polt), TRUE);
    gtk_label_set_markup (GTK_LABEL (label_reserva_polt), str_polt);
}

//função que valida se o usuário selecionou um voo utilizando o respectivo número
void validate_reserva_menu (GtkWidget *widget, gpointer data){
    if(verif_bool == 0){
        gtk_label_set_text (GTK_LABEL (error_show_reserva_menu), "Voo não encontrado!");
    }
    else{
        show_reserva_menu(widget, data);
    }
}

//função que atualiza a janela com o menu_user
void show_reserva_user_menu (GtkWidget *widget, gpointer data){
    gtk_editable_set_text (GTK_EDITABLE (entry_user_name), "");
    gtk_editable_set_text (GTK_EDITABLE (entry_user_id), "");
    gtk_label_set_text (GTK_LABEL (label_user_error), "");
    gtk_label_set_text (GTK_LABEL (label_error_id), "");
    gtk_label_set_text (GTK_LABEL (label_error_name), "");
    GtkStack *stack = GTK_STACK (data);
    gtk_stack_set_visible_child_name (stack, "user");

    gtk_label_set_text (GTK_LABEL (label_user), temp_verif);
    gtk_label_set_use_markup (GTK_LABEL (label_user_polt), TRUE);
    gtk_label_set_markup (GTK_LABEL (label_user_polt), str_polt);
}

//função que atualiza a janela com o menu_conferir
void show_conferir_menu (GtkWidget *widget, gpointer data){
    gtk_label_set_text (GTK_LABEL (label_error_conferir), "");
    GtkStack *stack = GTK_STACK (data);
    gtk_stack_set_visible_child_name (stack, "conferir");
}

//função que atualiza a janela com o menu_conferir
void show_pesquisar_menu (GtkWidget *widget, gpointer data){
    gtk_label_set_text (GTK_LABEL (label_error_pesquisar), "");
    gtk_label_set_text (GTK_LABEL (label_exib_pesquisar), "");
    gtk_label_set_text (GTK_LABEL (label_error_origem), "");
    gtk_label_set_text (GTK_LABEL (label_error_destino), "");
    gtk_label_set_text (GTK_LABEL (label_error_data), "");
    gtk_editable_set_text (GTK_EDITABLE (entry_origem_pesquisar), "");
    gtk_editable_set_text (GTK_EDITABLE (entry_destino_pesquisar), "");
    gtk_editable_set_text (GTK_EDITABLE (entry_data_pesquisar), "");

    GtkStack *stack = GTK_STACK (data);
    gtk_stack_set_visible_child_name (stack, "pesquisar");
}

//função activate que define todos os parametros da janela, stack de menus, grids e botões(core da GUI)
void activate (GtkApplication *app, gpointer user_data){
    GtkWidget *window;
    GtkWidget *stack;
    GtkWidget *button;
    GtkWidget *label;
    GtkWidget *menu_main;
    GtkWidget *menu_cadastro;
    GtkWidget *menu_voos;
    GtkWidget *menu_verif;
    GtkWidget *menu_reserva;
    GtkWidget *menu_user;
    GtkWidget *menu_conferir;
    GtkWidget *menu_pesquisar;
    const char *font_css = "* { font-size: 16px; }";

    GtkCssProvider *provider_font = gtk_css_provider_new();
    gtk_css_provider_load_from_string (provider_font, font_css);
    gtk_style_context_add_provider_for_display(gdk_display_get_default (), GTK_STYLE_PROVIDER (provider_font), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    //css styling usados no label de erro, cor vermelha
    GtkCssProvider *provider_red = gtk_css_provider_new ();
    gtk_css_provider_load_from_string (provider_red, ".error-label { color: red; font-weight: bold; }");
    gtk_style_context_add_provider_for_display (gdk_display_get_default (), GTK_STYLE_PROVIDER (provider_red), GTK_STYLE_PROVIDER_PRIORITY_USER);

    //css styling usados no label de sucesso, cor verde
    GtkCssProvider *provider_green = gtk_css_provider_new ();
    gtk_css_provider_load_from_string (provider_green, ".success-label { color: green; font-weight: bold; }");
    gtk_style_context_add_provider_for_display (gdk_display_get_default (), GTK_STYLE_PROVIDER (provider_green), GTK_STYLE_PROVIDER_PRIORITY_USER);

    //declaração da window, tamanho 
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "SISTEMA DE RESERVA DE VOOS");
    gtk_window_set_default_size (GTK_WINDOW(window), 1200, 1000);

    //criação do stack de menus
    stack = gtk_stack_new ();
    gtk_window_set_child (GTK_WINDOW (window), stack);

    //criação do grid menu_main, com tamanho de grade e homogeneidade do menu
    menu_main = gtk_grid_new ();
    gtk_grid_set_column_spacing(GTK_GRID (menu_main), 10);
    gtk_grid_set_column_homogeneous(GTK_GRID(menu_main), TRUE);
    gtk_grid_set_row_spacing(GTK_GRID (menu_main), 10);
    gtk_grid_set_row_homogeneous(GTK_GRID(menu_main), TRUE);
    gtk_stack_add_named (GTK_STACK (stack), menu_main, "main");

    //começo do implementação do menu_cadastro
    //criação do grid menu_cadastro, tamanho da grade
    menu_cadastro = gtk_grid_new ();
    gtk_grid_set_column_spacing (GTK_GRID (menu_cadastro), 10);
    gtk_grid_set_column_homogeneous(GTK_GRID(menu_cadastro), TRUE);
    gtk_grid_set_row_spacing (GTK_GRID (menu_cadastro), 10);
    gtk_grid_set_row_homogeneous(GTK_GRID(menu_cadastro), TRUE);
    gtk_stack_add_named (GTK_STACK (stack), menu_cadastro, "Cadastro");

    //criação do botão de cadastro, localização e alinhamento 
    button = gtk_button_new_with_label ("Cadastrar voo");
    g_signal_connect (button, "clicked", G_CALLBACK (show_cadastro_menu), stack);
    gtk_grid_attach (GTK_GRID (menu_main), button, 2, 1, 1, 1);
    gtk_widget_set_halign (button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (button, GTK_ALIGN_CENTER);

    
    //criação dos botões do menu_cadastro, label de texto e campo para digitar 
    label = gtk_label_new ("Numero do voo:");
    gtk_grid_attach (GTK_GRID (menu_cadastro), label, 1, 0, 1, 1);
    gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label, GTK_ALIGN_CENTER);
    entry_num_voo = gtk_entry_new ();
    gtk_grid_attach (GTK_GRID (menu_cadastro), entry_num_voo, 2, 0, 1, 1);
    gtk_widget_set_halign (entry_num_voo, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (entry_num_voo, GTK_ALIGN_CENTER);

    label = gtk_label_new ("Origem:");
    gtk_grid_attach (GTK_GRID (menu_cadastro), label, 1, 1, 1, 1);
    gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label, GTK_ALIGN_CENTER);
    entry_origem = gtk_entry_new ();
    gtk_grid_attach (GTK_GRID (menu_cadastro), entry_origem, 2, 1, 1, 1);
    gtk_widget_set_halign (entry_origem, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (entry_origem, GTK_ALIGN_CENTER);

    label = gtk_label_new ("Destino:");
    gtk_grid_attach (GTK_GRID (menu_cadastro), label, 1, 2, 1, 1);
    gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label, GTK_ALIGN_CENTER);
    entry_destino = gtk_entry_new ();
    gtk_grid_attach (GTK_GRID (menu_cadastro), entry_destino, 2, 2, 1, 1);
    gtk_widget_set_halign (entry_destino, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (entry_destino, GTK_ALIGN_CENTER);

    label = gtk_label_new ("Data:");
    gtk_grid_attach (GTK_GRID (menu_cadastro), label, 1, 3, 1, 1);
    gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label, GTK_ALIGN_CENTER);
    entry_data = gtk_entry_new ();
    gtk_grid_attach (GTK_GRID (menu_cadastro), entry_data, 2, 3, 1, 1);
    gtk_widget_set_halign (entry_data, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (entry_data, GTK_ALIGN_CENTER);

    label = gtk_label_new ("Hora partida:");
    gtk_grid_attach (GTK_GRID (menu_cadastro), label, 1, 4, 1, 1);
    gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label, GTK_ALIGN_CENTER);
    entry_hora_pt = gtk_entry_new ();
    gtk_grid_attach (GTK_GRID (menu_cadastro), entry_hora_pt, 2, 4, 1, 1);
    gtk_widget_set_halign (entry_hora_pt, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (entry_hora_pt, GTK_ALIGN_CENTER);

    label = gtk_label_new ("Hora chegada:");
    gtk_grid_attach (GTK_GRID (menu_cadastro), label, 1, 5, 1, 1);
    gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label, GTK_ALIGN_CENTER);
    entry_hora_cg = gtk_entry_new ();
    gtk_grid_attach (GTK_GRID (menu_cadastro), entry_hora_cg, 2, 5, 1, 1);
    gtk_widget_set_halign (entry_hora_cg, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (entry_hora_cg, GTK_ALIGN_CENTER);

    button = gtk_button_new_with_label ("Cadastrar");
    g_signal_connect (button, "clicked", G_CALLBACK (cadastrar_voo), NULL);
    gtk_grid_attach (GTK_GRID (menu_cadastro), button, 2, 6, 1, 1);
    gtk_widget_set_halign (button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (button, GTK_ALIGN_CENTER);
    //fim da criação dos botões


    //criação dos labels para exibir codigos de erro do botões do menu_cadastro
    label_error = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_cadastro), label_error, 3, 6, 1, 1);
    gtk_widget_set_halign (label_error, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_error, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class (label_error, "error-label");

    erro_num = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_cadastro), erro_num, 3, 0, 1, 1);
    gtk_widget_set_halign (erro_num, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (erro_num, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class (erro_num, "error-label");

    erro_origem = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_cadastro), erro_origem, 3, 1, 1, 1);
    gtk_widget_set_halign (erro_origem, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (erro_origem, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class (erro_origem, "error-label");

    erro_destino = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_cadastro), erro_destino, 3, 2, 1, 1);
    gtk_widget_set_halign (erro_destino, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (erro_destino, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class (erro_destino, "error-label");

    erro_data = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_cadastro), erro_data, 3, 3, 1, 1);
    gtk_widget_set_halign (erro_data, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (erro_data, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class (erro_data, "error-label");

    erro_hora_pt = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_cadastro), erro_hora_pt, 3, 4, 1, 1);
    gtk_widget_set_halign (erro_hora_pt, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (erro_hora_pt, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class (erro_hora_pt, "error-label");

    erro_hora_cg = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_cadastro), erro_hora_cg, 3, 5, 1, 1);
    gtk_widget_set_halign (erro_hora_cg, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (erro_hora_cg, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class (erro_hora_cg, "error-label");
    //fim da criação dos labels para exibir codigos de erro do botões do menu_cadastro
   
    //criação do botão "Voltar" no menu_cadastro para retroceder ao menu_main
    button = gtk_button_new_with_label ("Voltar");
    g_signal_connect (button, "clicked", G_CALLBACK (show_main_menu), stack);
    gtk_grid_attach (GTK_GRID (menu_cadastro), button, 2, 8, 1, 1);
    gtk_widget_set_halign (button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (button, GTK_ALIGN_CENTER);

    //fim da implementação do menu_cadastro

    //começo da implementação do menu_voos
    //criação do grid menu_voos, tamanho da grade
    menu_voos = gtk_grid_new ();
    gtk_grid_set_column_spacing (GTK_GRID (menu_voos), 10);
    gtk_grid_set_column_homogeneous (GTK_GRID (menu_voos), TRUE);
    gtk_grid_set_row_spacing (GTK_GRID (menu_voos), 10);
    gtk_grid_set_row_homogeneous (GTK_GRID (menu_voos), TRUE);
    gtk_stack_add_named (GTK_STACK (stack), menu_voos, "voos");

    //criação do botão de listar voos, localização e alinhamento
    button = gtk_button_new_with_label ("Listar voos");
    g_signal_connect (button, "clicked", G_CALLBACK (show_voos_menu), stack);
    gtk_grid_attach (GTK_GRID (menu_main), button, 2, 2, 1, 1);
    gtk_widget_set_halign (button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (button, GTK_ALIGN_CENTER);

    label_voos = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_voos), label_voos, 3, 0, 1, 1);
    gtk_widget_set_halign (label_voos, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_voos, GTK_ALIGN_CENTER);

    button = gtk_button_new_with_label ("Exibir Voos");
    g_signal_connect (button, "clicked", G_CALLBACK (listar_voos), stack);
    gtk_grid_attach (GTK_GRID (menu_voos), button, 3 , 2, 1, 1);
    gtk_widget_set_halign (button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (button, GTK_ALIGN_CENTER);

    //criação do botão "Voltar" no menu_voos para retroceder ao menu_main
    button = gtk_button_new_with_label ("Voltar");
    g_signal_connect (button, "clicked", G_CALLBACK (show_main_menu), stack);
    gtk_grid_attach (GTK_GRID (menu_voos), button, 3, 3, 1, 1);
    gtk_widget_set_halign (button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (button, GTK_ALIGN_CENTER);

    //fim da implementação do menu_voos

    //começo da implementação do menu_pesquisar

    menu_pesquisar = gtk_grid_new ();
    gtk_grid_set_column_spacing (GTK_GRID (menu_pesquisar), 10);
    gtk_grid_set_column_homogeneous(GTK_GRID (menu_pesquisar), TRUE);
    gtk_grid_set_row_spacing (GTK_GRID (menu_pesquisar), 10);
    gtk_grid_set_row_homogeneous(GTK_GRID (menu_pesquisar), TRUE);
    gtk_stack_add_named (GTK_STACK (stack), menu_pesquisar, "pesquisar");

    button = gtk_button_new_with_label ("Pesquisar voos");
    g_signal_connect (button, "clicked", G_CALLBACK (show_pesquisar_menu), stack);
    gtk_grid_attach (GTK_GRID (menu_main), button, 2, 3, 1, 1);
    gtk_widget_set_halign (button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (button, GTK_ALIGN_CENTER);

    label_exib_pesquisar = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_pesquisar), label_exib_pesquisar, 0, 0, 3, 2);
    gtk_widget_set_halign (label_exib_pesquisar, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_exib_pesquisar, GTK_ALIGN_CENTER);

    label_origem_pesquisar = gtk_label_new ("Digite a origem desejada:");
    gtk_grid_attach (GTK_GRID (menu_pesquisar), label_origem_pesquisar, 0, 2, 1, 1);
    gtk_widget_set_halign (label_origem_pesquisar, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_origem_pesquisar, GTK_ALIGN_CENTER);

    entry_origem_pesquisar = gtk_entry_new ();
    gtk_grid_attach (GTK_GRID (menu_pesquisar), entry_origem_pesquisar, 1, 2, 1, 1);
    gtk_widget_set_halign (entry_origem_pesquisar, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (entry_origem_pesquisar, GTK_ALIGN_CENTER);

    label_error_origem = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_pesquisar), label_error_origem, 2, 2, 1, 1);
    gtk_widget_set_halign (label_error_origem, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_error_origem, GTK_ALIGN_CENTER);

    label_destino_pesquisar = gtk_label_new ("Digite o destino desejado:");
    gtk_grid_attach (GTK_GRID (menu_pesquisar), label_destino_pesquisar, 0, 3, 1, 1);
    gtk_widget_set_halign (label_destino_pesquisar, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_destino_pesquisar, GTK_ALIGN_CENTER);

    entry_destino_pesquisar = gtk_entry_new ();
    gtk_grid_attach (GTK_GRID (menu_pesquisar), entry_destino_pesquisar, 1, 3, 1, 1);
    gtk_widget_set_halign (entry_destino_pesquisar, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (entry_destino_pesquisar, GTK_ALIGN_CENTER);

    label_error_destino = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_pesquisar), label_error_destino, 2, 3, 1, 1);
    gtk_widget_set_halign (label_error_destino, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_error_destino, GTK_ALIGN_CENTER);

    label_data_pesquisar = gtk_label_new ("Digite a data desejada:");
    gtk_grid_attach (GTK_GRID (menu_pesquisar), label_data_pesquisar, 0, 4, 1, 1);
    gtk_widget_set_halign (label_data_pesquisar, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_data_pesquisar, GTK_ALIGN_CENTER);

    entry_data_pesquisar = gtk_entry_new ();
    gtk_grid_attach (GTK_GRID (menu_pesquisar), entry_data_pesquisar, 1, 4, 1, 1);
    gtk_widget_set_halign (entry_data_pesquisar, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (entry_data_pesquisar, GTK_ALIGN_CENTER);

    label_error_data = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_pesquisar), label_error_data, 2, 4, 1, 1);
    gtk_widget_set_halign (label_error_data, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_error_data, GTK_ALIGN_CENTER);

    button = gtk_button_new_with_label ("Pesquisar");
    g_signal_connect (button, "clicked", G_CALLBACK (pesquisar_voos), stack);
    gtk_grid_attach (GTK_GRID (menu_pesquisar), button, 1, 5, 1, 1);
    gtk_widget_set_halign (button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (button, GTK_ALIGN_CENTER);

    label_error_pesquisar = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_pesquisar), label_error_pesquisar, 2, 5, 1, 1);
    gtk_widget_set_halign (label_error_pesquisar, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_error_pesquisar, GTK_ALIGN_CENTER);

    button = gtk_button_new_with_label ("Voltar");
    g_signal_connect (button, "clicked", G_CALLBACK (show_main_menu), stack);
    gtk_grid_attach (GTK_GRID (menu_pesquisar), button, 1, 6, 1, 1);
    gtk_widget_set_halign (button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (button, GTK_ALIGN_CENTER);

    //fim da implementação do menu_cadastro

    //começo da implementação do menu_verif
    //criação do grid menu_verif, tamanho da grade
    button = gtk_button_new_with_label ("Verificar disponibilidade e Efetuar Reserva");
    g_signal_connect (button, "clicked", G_CALLBACK (show_verif_menu), stack);
    gtk_grid_attach (GTK_GRID (menu_main), button, 2, 4, 1, 1);
    gtk_widget_set_halign (button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (button, GTK_ALIGN_CENTER);

    menu_verif = gtk_grid_new ();
    gtk_grid_set_column_spacing (GTK_GRID (menu_verif), 10);
    gtk_grid_set_column_homogeneous(GTK_GRID (menu_verif), TRUE);
    gtk_grid_set_row_spacing (GTK_GRID (menu_verif), 10);
    gtk_grid_set_row_homogeneous(GTK_GRID (menu_verif), TRUE);
    gtk_stack_add_named (GTK_STACK (stack), menu_verif, "verif");

    label = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_verif), label, 0, 3, 1, 1);
    gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label, GTK_ALIGN_CENTER);

    label_verif = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_verif), label_verif, 0, 0, 3, 1);
    gtk_widget_set_halign (label_verif, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_verif, GTK_ALIGN_CENTER);

    label_polt = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_verif), label_polt, 0, 1, 3, 1);
    gtk_widget_set_halign (label_polt, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_polt, GTK_ALIGN_CENTER);

    entry_num_voo_verif = gtk_entry_new ();
    gtk_grid_attach (GTK_GRID (menu_verif), entry_num_voo_verif, 1, 2, 1, 1);
    gtk_widget_set_halign (entry_num_voo_verif, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (entry_num_voo_verif, GTK_ALIGN_CENTER);

    button_verif_dispo = gtk_button_new_with_label ("Mostrar disponibilidade");
    g_signal_connect (button_verif_dispo, "clicked", G_CALLBACK (verif_voos), NULL);
    gtk_grid_attach (GTK_GRID (menu_verif), button_verif_dispo, 1, 3, 1, 1);
    gtk_widget_set_halign (button_verif_dispo, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (button_verif_dispo, GTK_ALIGN_CENTER);

    label_error_verif = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_verif), label_error_verif, 2, 3, 1, 1);
    gtk_widget_set_halign (label_error_verif, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_error_verif, GTK_ALIGN_CENTER);


    //criação do botão "Voltar" no menu_verif para retroceder ao menu_main
    button = gtk_button_new_with_label ("Voltar");
    g_signal_connect (button, "clicked", G_CALLBACK (show_main_menu), stack);
    gtk_grid_attach (GTK_GRID (menu_verif), button, 1, 5, 1, 1);
    gtk_widget_set_halign (button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (button, GTK_ALIGN_CENTER);

    //começo da implementação da instância reserva do menu_verif

    //criação do grid menu_reserva, tamanho da grade
    button_show_reserva_menu = gtk_button_new_with_label ("Efetuar reserva");
    g_signal_connect (button_show_reserva_menu, "clicked", G_CALLBACK (validate_reserva_menu), stack);
    gtk_grid_attach (GTK_GRID (menu_verif), button_show_reserva_menu, 1, 4, 1, 1);
    gtk_widget_set_halign (button_show_reserva_menu, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (button_show_reserva_menu, GTK_ALIGN_CENTER);

    error_show_reserva_menu = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_verif), error_show_reserva_menu, 2, 4, 1, 1);
    gtk_widget_set_halign (error_show_reserva_menu, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (error_show_reserva_menu, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class (error_show_reserva_menu, "error-label");

    menu_reserva = gtk_grid_new ();
    gtk_grid_set_column_spacing (GTK_GRID (menu_reserva), 10);
    gtk_grid_set_column_homogeneous(GTK_GRID (menu_reserva), TRUE);
    gtk_grid_set_row_spacing (GTK_GRID (menu_reserva), 10);
    gtk_grid_set_row_homogeneous(GTK_GRID (menu_reserva), TRUE);
    gtk_stack_add_named (GTK_STACK (stack), menu_reserva, "reserva");

    label_reserva = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_reserva), label_reserva, 0, 0, 3, 1);
    gtk_widget_set_halign (label_reserva, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_reserva, GTK_ALIGN_CENTER);

    label_reserva_polt = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_reserva), label_reserva_polt, 0, 1, 3, 1);
    gtk_widget_set_halign (label_reserva_polt, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_reserva_polt, GTK_ALIGN_CENTER);

    label_reserva2 = gtk_label_new ("Digite o número da poltrona desejada:");
    gtk_grid_attach (GTK_GRID (menu_reserva), label_reserva2, 0, 2, 1, 1);
    gtk_widget_set_halign (label_reserva2, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_reserva2, GTK_ALIGN_CENTER);

    entry_polt_verif = gtk_entry_new ();
    gtk_grid_attach (GTK_GRID (menu_reserva), entry_polt_verif, 1, 2, 1, 1);
    gtk_widget_set_halign (entry_polt_verif, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (entry_polt_verif, GTK_ALIGN_CENTER);

    button_verif_polt = gtk_button_new_with_label ("Prosseguir com reserva");
    g_signal_connect (button_verif_polt, "clicked", G_CALLBACK (efetuar_reserva), stack);
    gtk_grid_attach (GTK_GRID (menu_reserva), button_verif_polt, 1, 3, 1, 1);
    gtk_widget_set_halign (button_verif_polt, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (button_verif_polt, GTK_ALIGN_CENTER);

    label_error_reserva = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_reserva), label_error_reserva, 2, 3, 1, 1);
    gtk_widget_set_halign (label_error_reserva, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_error_reserva, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class (label_error_reserva, "error-label");

    //criação do botão "Voltar" no menu_reserva para retroceder ao menu_verif
    button = gtk_button_new_with_label ("Voltar");
    g_signal_connect (button, "clicked", G_CALLBACK (show_verif_menu), stack);
    gtk_grid_attach (GTK_GRID (menu_reserva), button, 1, 4, 1, 1);
    gtk_widget_set_halign (button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (button, GTK_ALIGN_CENTER);

    //fim da implementação da instância reserva do menu_verif

    menu_user = gtk_grid_new ();
    gtk_grid_set_column_spacing (GTK_GRID (menu_user), 10);
    gtk_grid_set_column_homogeneous(GTK_GRID (menu_user), TRUE);
    gtk_grid_set_row_spacing (GTK_GRID (menu_user), 10);
    gtk_grid_set_row_homogeneous(GTK_GRID (menu_user), TRUE);
    gtk_stack_add_named (GTK_STACK (stack), menu_user, "user");

    label_user = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_user), label_user, 0, 0, 3, 1);
    gtk_widget_set_halign (label_user, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_user, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class (label_user, "success-label");

    label_user_polt = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_user), label_user_polt, 0, 1, 3, 1);
    gtk_widget_set_halign (label_user_polt, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_user_polt, GTK_ALIGN_CENTER);

    label = gtk_label_new ("Digite o nome do usuário:");
    gtk_grid_attach (GTK_GRID (menu_user), label, 0, 2, 1, 1);
    gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label, GTK_ALIGN_CENTER);

    entry_user_name = gtk_entry_new ();
    gtk_grid_attach (GTK_GRID (menu_user), entry_user_name, 1, 2, 1, 1);
    gtk_widget_set_halign (entry_user_name, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (entry_user_name, GTK_ALIGN_CENTER);

    label_error_name = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_user), label_error_name, 2, 2, 1, 1);
    gtk_widget_set_halign (label_error_name, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_error_name, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class (label_error_name, "error-label");

    label = gtk_label_new ("Digite o CPF do usuário:");
    gtk_grid_attach (GTK_GRID (menu_user), label, 0, 3, 1, 1);
    gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label, GTK_ALIGN_CENTER);

    entry_user_id = gtk_entry_new ();
    gtk_grid_attach (GTK_GRID (menu_user), entry_user_id, 1, 3, 1, 1);
    gtk_widget_set_halign (entry_user_id, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (entry_user_id, GTK_ALIGN_CENTER);

    label_error_id = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_user), label_error_id, 2, 3, 1, 1);
    gtk_widget_set_halign (label_error_id, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_error_id, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class (label_error_id, "error-label");

    button = gtk_button_new_with_label ("Confirmar reserva");
    g_signal_connect (button, "clicked", G_CALLBACK (get_user_input), NULL);
    gtk_grid_attach (GTK_GRID (menu_user), button, 1, 4, 1, 1);
    gtk_widget_set_halign (button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (button, GTK_ALIGN_CENTER);

    label_user_error = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_user), label_user_error, 2, 4, 1, 1);
    gtk_widget_set_halign (label_user_error, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_user_error, GTK_ALIGN_CENTER);

    button = gtk_button_new_with_label ("Voltar");
    g_signal_connect (button, "clicked", G_CALLBACK (show_reserva_menu), stack);
    gtk_grid_attach (GTK_GRID (menu_user), button, 1, 5, 1, 1);
    gtk_widget_set_halign (button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (button, GTK_ALIGN_CENTER);

    label = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_user), label, 0, 4, 1, 1);
    gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label, GTK_ALIGN_CENTER);

    //fim da implementação do menu_verif

    //começo da implementação do menu_conferir
    button = gtk_button_new_with_label ("Conferir reserva");
    g_signal_connect (button, "clicked", G_CALLBACK (show_conferir_menu), stack);
    gtk_grid_attach (GTK_GRID (menu_main), button, 2, 6, 1, 1);
    gtk_widget_set_halign (button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (button, GTK_ALIGN_CENTER);

    menu_conferir = gtk_grid_new ();
    gtk_grid_set_column_spacing (GTK_GRID (menu_conferir), 10);
    gtk_grid_set_column_homogeneous(GTK_GRID (menu_conferir), TRUE);
    gtk_grid_set_row_spacing (GTK_GRID (menu_conferir), 10);
    gtk_grid_set_row_homogeneous(GTK_GRID (menu_conferir), TRUE);
    gtk_stack_add_named (GTK_STACK (stack), menu_conferir, "conferir");

    label_conferir = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_conferir), label_conferir, 0, 0, 3, 1);
    gtk_widget_set_halign (label_conferir, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_conferir, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class (label_conferir, "success-label");

    entry_name_conferir = gtk_entry_new ();
    gtk_grid_attach (GTK_GRID (menu_conferir), entry_name_conferir, 1, 1, 1, 1);
    gtk_widget_set_halign (entry_name_conferir, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (entry_name_conferir, GTK_ALIGN_CENTER);

    label_entry_conferir = gtk_label_new ("Digite o nome presente na reserva:");
    gtk_grid_attach (GTK_GRID (menu_conferir), label_entry_conferir, 0, 1, 1, 1);
    gtk_widget_set_halign (label_entry_conferir, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_entry_conferir, GTK_ALIGN_CENTER);

    button = gtk_button_new_with_label ("Conferir");
    g_signal_connect (button, "clicked", G_CALLBACK (conferir_reserva), NULL);
    gtk_grid_attach (GTK_GRID (menu_conferir), button, 1, 2, 1, 1);
    gtk_widget_set_halign (button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (button, GTK_ALIGN_CENTER);

    label_error_conferir = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (menu_conferir), label_error_conferir, 2, 2, 1, 1);
    gtk_widget_set_halign (label_error_conferir, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label_error_conferir, GTK_ALIGN_CENTER);

    button = gtk_button_new_with_label ("Voltar");
    g_signal_connect (button, "clicked", G_CALLBACK (show_main_menu), stack);
    gtk_grid_attach (GTK_GRID (menu_conferir), button, 1, 3, 1, 1);
    gtk_widget_set_halign (button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (button, GTK_ALIGN_CENTER);

    //fim da implementação do menu_conferir

    //botão para fechar a janela
    button = gtk_button_new_with_label ("Sair");
    g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_window_destroy), window);
    gtk_grid_attach (GTK_GRID (menu_main), button, 2, 7, 1, 1);
    gtk_widget_set_halign (button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (button, GTK_ALIGN_CENTER);

    gtk_stack_set_visible_child_name (GTK_STACK (stack), "main");

    gtk_window_present (GTK_WINDOW (window));
}

//a porra da main
int main(int argc, char **argv){
    GtkApplication *app;
    int status;
    load_voos_dados();

    app = gtk_application_new ("org.gtk.test", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    save_voos_dados();  
    return status;
}