#include <stdio.h>
typedef struct{
    int pos_poltrona;
    int dispo;
    char nome_reserv[80];
}poltrona;
typedef struct{
    int num_voo;
    char origem_voo[50];
    char destino_voo[50];
    char data_voo[20];
    char hora_pt[10];
    char hora_cg[10];
    poltrona poltrona_dt[5][2];
}voo_dt;
typedef struct{
    char nome[80];
    int num_ident;
}usuario;
void clear_buffer(){
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}
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

int get_menu_opt(const char *input, int min, int max){
    int opt, input_stts;

    while(1){
        printf("%s", input);
        input_stts = scanf("%i", &opt);
        clear_buffer();
        if(input_stts == 1){
            if(opt >= min && opt <= max){
                return opt;
            }
            else{
                printf("ERRO! Escolha uma opção entre %i e %i!\n", min, max);
            }
        }
        else{
            printf("ERRO! Digite um número válido!\n");
        }
    }
}
void print_menu(void){
    printf("+---------------------------------------+\n");
    printf("| Sistema de Reservas de Voo            |\n");
    printf("+---------------------------------------+\n");
    printf("| 1 - Cadastrar novo voo                |\n");
    printf("| 2 - Exibir voos disponiveis           |\n");
    printf("| 3 - Verificar disponibilidade do voo  |\n");
    printf("| 4 - Efetuar reserva                   |\n");
    printf("| 5 - Conferir reserva                  |\n");
    printf("| 6 - Sair                              |\n");
    printf("+---------------------------------------+\n");
}
int get_int(const char *input){
    int num, input_stts;

    while(1){
        printf("%s", input);
        input_stts = scanf("%i", &num);
        clear_buffer();
        if(input_stts == 1){
            return num;
        }
        else{
            printf("ERRO! Digite um número válido!\n");
        }
    }
}
int main(void){
    int operacao, qnt_voo = 0, qnt_user = 0, temp_num_voo;
    voo_dt voos00[10];
    usuario def_user[10];

    do{
        print_menu();
        operacao = get_menu_opt("Escolha uma função: ", 1, 6);

        switch(operacao){
            case 1:{
                    if(qnt_voo > 0){
                        int num_pass;
                        do{
                            num_pass = 0;
                            voos00[qnt_voo].num_voo = get_int("Número do voo: ");
                            for(int i = 0; i < qnt_voo; i++){
                                if(voos00[i].num_voo == voos00[qnt_voo].num_voo){
                                    printf("Numero de voo já cadastrado!\n");
                                    num_pass++;
                                }
                            }
                        }while(num_pass == 1);
                    }
                    else{
                        voos00[qnt_voo].num_voo = get_int("Número do voo: ");
                    }
                    printf("Origem: ");
                    fgets(voos00[qnt_voo].origem_voo, 50, stdin);
                    printf("Destino: ");
                    fgets(voos00[qnt_voo].destino_voo, 50, stdin);
                    printf("Data: ");
                    fgets(voos00[qnt_voo].data_voo, 20, stdin);
                    printf("Hora de partida: ");
                    fgets(voos00[qnt_voo].hora_pt, 10, stdin);
                    printf("Hora de chegada: ");
                    fgets(voos00[qnt_voo].hora_cg, 10, stdin);
                    int polt_up = 0;
                    for(int i = 0; i < 5; i++){
                        for(int j = 0; j < 2; j++){
                            voos00[qnt_voo].poltrona_dt[i][j].dispo = 0;
                            voos00[qnt_voo].poltrona_dt[i][j].pos_poltrona = polt_up + 1;
                            polt_up++;
                        }
                    }
                    qnt_voo++;
                    printf("Voo cadastrado com sucesso!\n");
                    break;
            }
            case 2:{
                if(qnt_voo == 0){
                    printf("Nenhum voo cadastrado!\n");
                    break;
                }
                printf("------------+LISTA DE VOOS CADASTRADOS+------------\n");
                for(int i = 0; i < qnt_voo; i++){
                    printf("N° do voo: %i |\n Origem: %s |\n Destino: %s |\n Data: %s |\n Hora de partida: %s |\n Hora de chegada: %s |\n", voos00[i].num_voo, voos00[i].origem_voo, voos00[i].destino_voo, voos00[i].data_voo, voos00[i].hora_pt, voos00[i].hora_cg);
                    printf("+-----------------------+\n");
                }
                break;
            }
            case 3:{
                int verif_voo, bool_voo = 0;
                if(qnt_voo == 0){
                    printf("Nenhum voo cadastrado!\n");
                    break;
                }
                verif_voo = get_int("Digite o número do voo: ");
                for(int i = 0; i < qnt_voo; i++){
                    if(verif_voo == voos00[i].num_voo){
                        bool_voo ++;
                        temp_num_voo = i;
                        printf("Voo encontrado!\n");
                        printf("N° do voo: %i |\n Origem: %s |\n Destino: %s |\n Data: %s |\n Hora de partida: %s |\n Hora de chegada: %s |\n", voos00[i].num_voo, voos00[i].origem_voo, voos00[i].destino_voo, voos00[i].data_voo, voos00[i].hora_pt, voos00[i].hora_cg);
                        printf("+-----------------------+\n");
                        break;
                    }
                }
                if(bool_voo == 1){
                    printf("------------+Lista de poltronas+------------\n");
                    for(int i = 0; i < 5; i++){
                        for(int j = 0; j < 2; j++){
                            if(voos00[temp_num_voo].poltrona_dt[i][j].dispo == 0){
                                printf("| Poltrona %i: Livre     | ", voos00[temp_num_voo].poltrona_dt[i][j].pos_poltrona);
                            }
                            else{
                                printf("| Poltrona %i: Reservada | ", voos00[temp_num_voo].poltrona_dt[i][j].pos_poltrona);
                            }
                        }
                        printf("\n");
                    }
                    break;
                }
                else{
                    printf("Nenhum voo encontrado!\n");
                    break;
                }
            }
            case 4:{
                if(qnt_voo == 0){
                    printf("Nenhum voo cadastrado!\n");
                    break;
                }
                int verif_voo, bool_voo = 0;
                verif_voo = get_int("Digite o número do voo: ");
                for(int i = 0; i < qnt_voo; i++){
                    if(verif_voo == voos00[i].num_voo){
                        bool_voo++;
                        temp_num_voo = i;
                        break;
                    }
                }
                if(bool_voo == 0){
                    printf("Voo não encontrado!\n");
                    break;
                }
                int temp_pos, bool_reserv = 0;
                temp_pos = get_int("Digite o número da poltrona desejada: ");
                for(int i = 0; i < 5; i++){
                    for(int j = 0; j < 2; j++){
                        if(voos00[temp_num_voo].poltrona_dt[i][j].pos_poltrona == temp_pos){
                            if(voos00[temp_num_voo].poltrona_dt[i][j].dispo == 0){
                                bool_reserv = get_int("Poltrona disponivel para reserva!\nDeseja solicitar uma reserva?\nDigite 0 para 'não' e 1 para 'sim': ");
                                if(bool_reserv == 1){
                                    printf("Digite o nome do Usuario: ");
                                    fgets(def_user[qnt_user].nome, 80, stdin);
                                    def_user[qnt_user].num_ident = get_int("Digite o numero de identificação: ");
                                    for(int k = 0; k < sizeof(def_user[qnt_user].nome); k++){
                                        voos00[temp_num_voo].poltrona_dt[i][j].nome_reserv[k] = def_user[qnt_user].nome[k];
                                    }
                                    voos00[temp_num_voo].poltrona_dt[i][j].dispo = 1;
                                    printf("%s",voos00[temp_num_voo].poltrona_dt[i][j].nome_reserv);
                                    printf("Reserva da poltrona %i concluida!\n", temp_pos);
                                    qnt_user++;
                                    break;
                                }
                                else{
                                    printf("Retornando...");
                                    break;
                                }
                            }
                            else{
                                printf("A poltrona solicitada já foi reservada!");
                                break;
                            }
                        }
                    }
                }
                break;
            }
            case 5:{
                if(qnt_voo == 0){
                    printf("Nenhum voo cadastrado!\n");
                    break;
                }
                int bool_found = 0;
                char temp_nome[80];
                printf("Digite o nome presente na reserva: ");
                fgets(temp_nome, 80, stdin);
                for(int i = 0; i < qnt_voo; i++){
                    for(int j = 0; j < 5; j++){
                        for(int k = 0; k < 2; k++){
                            if(str_comp(voos00[i].poltrona_dt[j][k].nome_reserv, temp_nome) == 0){
                                printf("Reserva encontrada!\nPoltrona :%i\n", voos00[i].poltrona_dt[j][k].pos_poltrona);
                                printf("N° do voo: %i |\n Origem: %s |\n Destino: %s |\n Data: %s |\n Hora de partida: %s |\n Hora de chegada: %s |\n", voos00[i].num_voo, voos00[i].origem_voo, voos00[i].destino_voo, voos00[i].data_voo, voos00[i].hora_pt, voos00[i].hora_cg);
                                printf("+-----------------------+\n");
                                bool_found++;
                                break;
                            }
                        }
                    }
                }
                if(bool_found == 0){
                    printf("Reserva não encontrada!\n");
                    break;
                }
                break;
            }
            case 6:{
                printf("Fechando sistema...\n");
                break;
            }
            default:{
                printf("ERRO INESPERADO!!!\n");
                return 0;
            }
        }
    }while(operacao >= 1 && operacao <= 5);
    return 0;
}