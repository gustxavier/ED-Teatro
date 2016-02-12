/******************************************************************************
 Name        : Teatro.c
 Author      : Gustavo Xavier | Lucas Roncolado
 Version     : 1.0.0
 Copyright   : Todos os direitos reservados
 Description : Este programa gerencia listas estáticas encadeadas em C.
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#define COL 6
#define ROW 4
#define FALSE 0
#define TRUE 1
#define TAM_ARQ 40

typedef struct{
    int cod, c_lin, c_col;
    char nome[40];
    char email[40];
    char tel[11];
    char event_date[11];
}T_Pessoa;

typedef struct{
    int cod;
    char nome[40];
    char event_date[11];
}T_Sessao;

typedef struct Cx{
    T_Pessoa Item;
    T_Sessao S_Item;
    struct Cx *Prox;
}Caixa;

typedef Caixa *Ponteiro;

typedef struct{
    Ponteiro Prim, Ult;
    int Tam;
}Lista_din_enc;

/* Definição das funções de manipulação das LISTAS */
void Menu_Principal();
void Menu_ADM();
void Menu_Sessao(char arquivo[TAM_ARQ]);
void Criar_Lista_Vazia(Lista_din_enc *L);
int Verifica_Lista_Vazia(Lista_din_enc L);
void Insere_Elemento_Lista_Sessao(Lista_din_enc *L, T_Sessao S);
void Insere_Elemento_Lista(Lista_din_enc *L, T_Pessoa X);
void Exibir_Elemento(T_Pessoa);
void Exibir_Elemento_Session(T_Sessao);
void Remove_Elemento_Lista(Lista_din_enc *L, T_Pessoa *X);
void Remove_Elemento_Lista_Sessoes(Lista_din_enc *L, T_Sessao *S);
int Remove_Arquivo_Sessao(Lista_din_enc L, int cod);
void Exibir_Lista(Lista_din_enc L, int tipo);
void Consulta_Elemento(Lista_din_enc , int);
void Consulta_Elemento_Sessao(Lista_din_enc L, int cod);
void Dados_Pessoa(T_Pessoa *X);
void Dados_Sessao(T_Sessao *S);
void Gravar_Arquivo(char arquivo[TAM_ARQ], Lista_din_enc *L,int tipo);
void Gravar_Arquivo_Sessoes(Lista_din_enc *L);
void Carregar_Arquivo(char arquivo[TAM_ARQ], Lista_din_enc *L, T_Pessoa X);
void Carregar_Sessoes(Lista_din_enc *L, T_Sessao S);
/* Definição das funções de manipulação da MATRIZ */
void Inicializa_Matriz(char matriz[ROW][COL]);
void Mostrar_Painel(char matriz[ROW][COL]);
void Comprar_Cadeira(char matriz[ROW][COL], T_Pessoa *X);
void Reservar_Cadeira(char matriz[ROW][COL], T_Pessoa *X);
void Legenda();
int Menu_Opcao();
int ColunaY();
int LinhaX();
/* Definição das funções de manipulação das TELAS */
void Mensagens(int op);
void Cabecalhos(int opcao);
void Limpar_Tela();

int main(){

    Menu_Principal();
    getchar();

    return 0;
}

/********************* L I S T A S *********************/

/* Menu principal da aplicação */
void Menu_Principal(){
    int opcao;
    int parar = FALSE;
    Lista_din_enc L;
    T_Sessao S;

    Criar_Lista_Vazia(&L);

    do{
        Cabecalhos(1);
        Carregar_Sessoes(&L,S);
        Exibir_Lista(L, 0);
        printf("        ------------------------------------------");
        printf("\n\n        >>Digite o codigo da sessao: ");

        scanf("%d",&opcao);
        switch(opcao){
            case 0:
                Mensagens(7);
                parar = TRUE;
                break;
            case 1:
                Menu_ADM();
                break;
            default:
                Consulta_Elemento_Sessao(L, opcao);
                break;
        }
    }while(parar!=TRUE);
}

/* Menu de administração das Sessões */
void Menu_ADM(){
    T_Sessao S;
    Lista_din_enc L;
    Lista_din_enc L_NEW;
    int parar = FALSE;
    int op;
    char arquivo[TAM_ARQ];
    FILE *arq;
    Ponteiro P;

    Criar_Lista_Vazia(&L);
    Carregar_Sessoes(&L,S);

    do{
        Cabecalhos(2);
        scanf("%d", &op);
        switch(op){
            case 0:
                parar = TRUE;
                break;
            case 1://Criar Evento
                Limpar_Tela();
                Dados_Sessao(&S);
                Insere_Elemento_Lista_Sessao(&L,S);
                arq = fopen("sessoes.dat", "wb");
                if(arq!=NULL){
                    P=L.Prim->Prox;
                    while(P!=NULL){
                        fwrite(&(P->S_Item), sizeof(T_Sessao),1,arq);   //fechamento
                        P=P->Prox;
                    }
                    fclose(arq);
                }
                Criar_Lista_Vazia(&L_NEW);
                strcpy(arquivo,S.nome);
                Gravar_Arquivo(arquivo,&L_NEW, 1);
                Mensagens(8);
                Mensagens(5);
                break;
            case 2://Remover Sessão
                Cabecalhos(4);
                Exibir_Lista(L, 0);
                printf("\n\n      Digite o codigo do elemento a ser removido:");
                scanf("%5d", &S.cod);
                if(Remove_Arquivo_Sessao(L,S.cod)){
                    Remove_Elemento_Lista_Sessoes(&L,&S);
                    Gravar_Arquivo_Sessoes(&L);
                    Mensagens(8);
                    Mensagens(5);
                }
                break;
            case 3://Exibir Sessões
                Cabecalhos(3);
                Carregar_Sessoes(&L,S);
                Exibir_Lista(L, 0);
                Mensagens(5);
                break;
            default:
                Mensagens(6);
                Mensagens(5);
                break;
        }
    }while(parar!=TRUE);
}

/* Menu de administração das CADEIRAS de uma sessão */
void Menu_Sessao(char arquivo[TAM_ARQ]){
    char matriz[ROW][COL];
    int opcao;
    int loopContinue=TRUE;
    Lista_din_enc L;
    T_Pessoa X;
    char* p = strtok(arquivo,".");
    strcpy(arquivo,p);

    Criar_Lista_Vazia(&L);
    Carregar_Arquivo(arquivo,&L,X);
    Inicializa_Matriz(matriz); //Iniciliza a matriz com valores .(Livre)

    do{
         system("cls");//Limpa a tela
         opcao = Menu_Opcao();
         switch(opcao){
            case 0: //Sair
                loopContinue = FALSE; //condicao para saída do programa
                break;
            case 1://Comprar
                Dados_Pessoa(&X);
                Mostrar_Painel(matriz);//Mostra o Painel atualizado
                Comprar_Cadeira(matriz,&X);
                Insere_Elemento_Lista(&L,X); // Realiza a compra de uma cadeira no painel, marcando c/ um X
                Gravar_Arquivo(arquivo,&L,0);
                Limpar_Tela();
                Mensagens(8);
                Mensagens(5);
                break;
            case 2://Reservar
                Dados_Pessoa(&X);
                Mostrar_Painel(matriz);//Mostra o Painel atualizado
                Reservar_Cadeira(matriz,&X);
                Insere_Elemento_Lista(&L,X); // Realiza a compra de uma cadeira no painel, marcando c/ um X
                Gravar_Arquivo(arquivo,&L, 0);
                Limpar_Tela();
                Mensagens(8);
                Mensagens(5);
                break;
            case 3://Mostrar Painel
                Carregar_Arquivo(arquivo,&L,X);
                Mostrar_Painel(matriz);
                Exibir_Lista(L,1);
                Mensagens(5);
                break;
            default:
                Mensagens(6);
                Mensagens(5);
                break;
         }
    }while(loopContinue);
}

/* Cria lista dinâmica vazia */
void Criar_Lista_Vazia(Lista_din_enc *L){
    Ponteiro P;
    P = (Ponteiro) malloc(sizeof (Caixa));
    L->Prim = P;
    L->Ult = L->Prim;
    L->Tam = 0;
}

/* Verifica lista vazia */
int Verifica_Lista_Vazia(Lista_din_enc L){
    return(L.Prim == L.Ult);
}

/* Insere um elemento na lista */
void Insere_Elemento_Lista(Lista_din_enc *L, T_Pessoa X){
    Ponteiro P, A;
    P = (Ponteiro) malloc(sizeof (Caixa));
    P->Item = X;
    A = L->Prim;
    while((A != L->Ult)&&((X.cod) > (A->Prox->Item.cod))){
        A = A->Prox;
    }
    P->Prox = A->Prox;
    A->Prox = P;
    if(A==L->Ult){
        L->Ult = P;
    }
    L->Tam++;
}

/* Insere um elemento na lista de sessões */
void Insere_Elemento_Lista_Sessao(Lista_din_enc *L, T_Sessao S){
    Ponteiro P, A;
    P = (Ponteiro) malloc(sizeof (Caixa));
    P->S_Item = S;
    A = L->Prim;
    while((A != L->Ult)&&((S.cod) > (A->Prox->S_Item.cod))){
        A = A->Prox;
    }
    P->Prox = A->Prox;
    A->Prox = P;
    if(A==L->Ult){
        L->Ult = P;
    }
    L->Tam++;
}

/* Exibe um elemento do tipo PESSOA */
void Exibir_Elemento(T_Pessoa X){
    printf("\n************* CODIGO: %d *************\n",X.cod);
    printf("*  Nome: %s \n",X.nome);
    printf("*  E_Mail: %s \n",X.email);
    printf("*  Telefone: %s\n",X.tel);
    printf("*  Cadeira: %d%d\n",X.c_lin,X.c_col);
    printf("****************************************\n");
}

/* Exibe um elemento do tipo SESSÂO */
void Exibir_Elemento_Session(T_Sessao S){
    printf("          %5d  -  %s  -  %s \n",S.cod, S.nome, S.event_date);
}

/* Remove um elemento da lista de tipo Pessoa */
void Remove_Elemento_Lista(Lista_din_enc *L, T_Pessoa *X){
    Ponteiro P, A;
    if(Verifica_Lista_Vazia (*L))
      Mensagens(11);
     else { P = L->Prim;
            while((P!=L->Ult)&&(X->cod > P->Prox->Item.cod))
                P=P->Prox;
            if ((P==L->Ult)||(X->cod!=P->Prox->Item.cod))
              Mensagens(14);
             else { *X = P->Prox->Item;
                    printf("\n Elemento removido : \n");
                    Exibir_Elemento(*X);
                    A = P->Prox;
                    if (A==L->Ult)
                      L->Ult = P;
                    P->Prox=A->Prox;
                    free(A);
                    L->Tam--;
                 }
          }
}

/* Remove um elemento da lista com tipo Sessao */
void Remove_Elemento_Lista_Sessoes(Lista_din_enc *L, T_Sessao *S){
    Ponteiro P, A;
    if(Verifica_Lista_Vazia (*L))
      Mensagens(11);
     else { P = L->Prim;
            while((P!=L->Ult)&&(S->cod > P->Prox->S_Item.cod))
                P=P->Prox;
            if ((P==L->Ult)||(S->cod!=P->Prox->S_Item.cod))
              Mensagens(14);
            else { *S = P->Prox->S_Item;
                    printf("\n     Elemento removido : \n");
                    Exibir_Elemento_Session(*S);
                    A = P->Prox;
                    if (A==L->Ult)
                      L->Ult = P;
                    P->Prox=A->Prox;
                    free(A);
                    L->Tam--;
                 }
          }
}

/* Remove um arquivo de uma Sessão */
int Remove_Arquivo_Sessao(Lista_din_enc L, int cod){
    Ponteiro P;
    char arquivo[TAM_ARQ];

    if(Verifica_Lista_Vazia (L)){
        Mensagens(11);
        return 0;
    }else {
        P = L.Prim->Prox;
        while((P!=NULL)&&(cod > P->S_Item.cod)){
            P=P->Prox;
        }
        if (cod != P->S_Item.cod){
            Mensagens(6);
            Mensagens(5);
            return 0;
        }else{
            strcpy(arquivo, P->S_Item.nome);
            strcat(arquivo,".dat");
            remove(arquivo);
            return 1;
        }
    }
}

/* Exibe listas dos tipo PESSOA ou SESSÃO */
void Exibir_Lista(Lista_din_enc L, int tipo){
    Ponteiro P;

	if(Verifica_Lista_Vazia(L))
        Mensagens(11);
    else {
        P = L.Prim;
		while(P!=L.Ult){
            if(tipo == 0){//tipo 0 é o tipo SESSAO
                Exibir_Elemento_Session(P->Prox->S_Item);
            }else{
                Exibir_Elemento(P->Prox->Item);
            }
             P=P->Prox;
        }
    }
}

/* Busca um elemento do tipo PESSOA */
void Consulta_Elemento(Lista_din_enc L, int cod){
    Ponteiro P;
    if(Verifica_Lista_Vazia (L)){
        Mensagens(11);
    }else {
        P = L.Prim->Prox;
        while((P!=NULL)&&(cod > P->Item.cod)){
            P=P->Prox;
        }
        if (cod != P->Item.cod){
            Mensagens(6);
            Mensagens(5);
        }else{
            Menu_Sessao(P->Item.nome);
        }
    }
}

/* Busca um elemento do tipo SESSÃO */
void Consulta_Elemento_Sessao(Lista_din_enc L, int cod){
    Ponteiro P;
    if(Verifica_Lista_Vazia (L)){
        Mensagens(11);
    }else {
        P = L.Prim->Prox;
        while((P!=NULL)&&(cod > P->S_Item.cod)){
            P=P->Prox;
        }
        if (cod != P->S_Item.cod){
            Mensagens(6);
            Mensagens(5);
        }else{
            Menu_Sessao(P->S_Item.nome);
        }
    }
}

/* Captura os dados do Cliente */
void Dados_Pessoa(T_Pessoa *X){
    Limpar_Tela();
    printf("      -----------------------------");
    printf("\n            DADOS DO CLIENTE\n");
    printf("      -----------------------------\n");
    printf("\n      Entre com um Codigo: ");
    scanf("%d",&(X->cod));
    printf("\n      Entre com o Nome : ");
    scanf("%s",X->nome);
    printf("\n      Entre com o E-mail: ");
    scanf("%s",X->email);
    printf("\n      Entre com o Telefone: ");
    scanf("%s", X->tel);
}

/* Captura os dados da SESSÃO */
void Dados_Sessao(T_Sessao *S){
    do{
        Limpar_Tela();
        printf("\n        -----------------------------");
        printf("\n               DADOS DA SESSAO\n");
        printf("        -----------------------------\n");
        printf("\n        Nome da Sessao: ");
        scanf("%s", S->nome);
        printf("\n        Codigo da Sessao: ");
        scanf("%d", &S->cod);
        if(S->cod == 0 || S->cod == 1){
            Mensagens(10);
            Mensagens(5);
        }
    }while((S->cod == 00) || (S->cod == 01));
    printf("\n        Data da Sessao: ");
    scanf("%s", S->event_date);
}

/* Grava arquivos a partir de um nome tipo 0 = Pessoa / 1 = Sessao*/
void Gravar_Arquivo(char arquivo[TAM_ARQ], Lista_din_enc *L, int tipo){
    FILE *arq;
    Ponteiro P;
    char arq_set[TAM_ARQ];
    int k = 0;
    char buffer[] = ".dat";

    while(arquivo[k]!= ' '){//Remove .dat duplicados nos nomes dos arquivos
        arq_set[k] = arquivo[k];
        k++;
    }
    strcat(arq_set, buffer);

    if(tipo == 0){
        if((arq = fopen(arq_set, "wb")) == NULL){ /* Abre arquivo binário para escrita */
            printf("Erro na abertura do arquivo");
            Mensagens(5);
            exit(1);
        }
        if(arq!=NULL){
            P = L->Prim->Prox;
            while(P!=NULL){
                fwrite(&(P->Item), sizeof(T_Pessoa),1,arq);   //Fecha arquivo
                P=P->Prox;
            }
            fclose(arq);
        }
    }else if(tipo == 1){
        if((arq = fopen(arq_set, "wb")) == NULL){ /* Abre arquivo binário para escrita */
            printf("Erro na abertura do arquivo");
            Mensagens(5);
            exit(1);
        }
        if(arq!=NULL){
            P = L->Prim->Prox;
            while(P!=NULL){
                fwrite(&(P->S_Item), sizeof(T_Sessao),1,arq);   //Fecha arquivo
                P=P->Prox;
            }
            fclose(arq);
        }
    }

}

/* Grava um arquivo de nome sessoes.dat */
void Gravar_Arquivo_Sessoes(Lista_din_enc *L){
    FILE *arq;
    Ponteiro P;
    arq = fopen("sessoes.dat", "wb");
    if(arq!=NULL)
      {
        P=L->Prim->Prox;
        while(P!=NULL)
          { fwrite(&(P->S_Item), sizeof(T_Sessao),1,arq);   //fechamento
            P=P->Prox;
           }
        fclose(arq);
       }
}

/* Carregar arquivos a partir de um nome */
void Carregar_Arquivo(char arquivo[TAM_ARQ], Lista_din_enc *L, T_Pessoa X){
    FILE *arq;
    char arq_set[TAM_ARQ];
    char buffer[] = ".dat";
    int k= 0;

    while(arquivo[k]!= ' '){//Remove .dat duplicados nos nomes dos arquivos
        arq_set[k] = arquivo[k];
        k++;
    }

    strcat(arq_set, buffer);

    if((arq = fopen(arq_set, "rb")) == NULL){ /* Abre o arquivo novamente para leitura */
       Mensagens(16);
       Mensagens(5);
       exit(1);
    }
    if (arq!=NULL){
        Criar_Lista_Vazia(L);
        X.cod=0;
        while (!feof(arq)){
            if(X.cod!=0)
              Insere_Elemento_Lista(L,X);
            fread(&X, sizeof(T_Pessoa), 1, arq);   //Fecha arquivo
           }
       fclose(arq);
      }
}

/* Carrega o arquivo sessoes.dat */
void Carregar_Sessoes(Lista_din_enc *L, T_Sessao S){
    FILE *arq;

    arq = fopen("sessoes.dat", "rb");
        if (arq!=NULL){
            Criar_Lista_Vazia(L);
            S.cod=0;
            while (!feof(arq))
              { if(S.cod!=0)
                  Insere_Elemento_Lista_Sessao(L,S);
                fread(&S, sizeof(T_Sessao), 1, arq);   //fechamento
               }
            fclose(arq);
        }
}

/********************* M A T R I Z *********************/

void Inicializa_Matriz(char matriz[ROW][COL]){
    int i,j;
    for(i=1;i<=ROW;i++){
        for(j=1;j<=COL;j++){
            matriz[i][j] = 32;
        }
    }
}

void Mostrar_Painel(char matriz[ROW][COL]){
    int i,j;
    Limpar_Tela();
    printf("\n\n         ####   PAINEL DE OCUPACOES   ####\n\n");
    printf("          ");

    for(i=1;i<=COL;i++)
        printf("%d     ",i);

    for(i=1;i<=ROW;i++){
        printf("\n\n     %d",i);
        for(j=1;j<=COL;j++)
            printf("   (%c)",matriz[i][j]);
    }
    Mensagens(12);
}

int Menu_Opcao(){
    int opcao;
    Cabecalhos(5);
    printf("                 opcao: ");
    scanf("%d",&opcao);

    return opcao;
}

/* Efetiva a compra da cadeira */
void Comprar_Cadeira(char matriz[ROW][COL],T_Pessoa *X){
    int row, col, parar = FALSE;
    printf("        -----------------------------");
    printf("\n             COMPRAR CADEIRA\n");
    printf("        -----------------------------\n");

    while(parar==FALSE){
        row = LinhaX();
        col = ColunaY();
        X->c_lin = row;
        X->c_col = col;
        if(matriz[row][col]==32){//SPACE
            matriz[row][col] = 88;//X
            Limpar_Tela();
            Mensagens(1);
            parar = TRUE;
        }else if (matriz[row][col]==88){//X
            Mensagens(3);
        }else if(matriz[row][col]==82){//R
            Mensagens(4);
        }
    }
    Mensagens(5);
}

/* Reserva a cadeira no teatro */
void Reservar_Cadeira(char matriz[ROW][COL],T_Pessoa *X){
    int row, col, parar = FALSE;
    printf("        -----------------------------");
    printf("\n             RESERVAR CADEIRA\n");
    printf("        -----------------------------\n");

    while(parar==FALSE){
        row = LinhaX();
        col = ColunaY();
        X->c_lin = row;
        X->c_col = col;
        if(matriz[row][col]==32){
            matriz[row][col] = 82;//R
            Limpar_Tela();
            Mensagens(2);
            parar = TRUE;
        }else if (matriz[row][col]==88){//X
            Mensagens(3);
        }else if(matriz[row][col]==82){//R
            Mensagens(4);
        }
    }

    Mensagens(5);
}

/* Ler a linha oferecida pelo usuario */
int LinhaX(){
    int row;
    do{
        printf("          Linha: ");
        scanf("%d",&row);
        if(row>ROW){
            Mensagens(15);
        }
    }while(row>ROW);

    return row;
}

/* Lê a coluna oferecida pelo usuario */
int ColunaY(){
   int col;
   do{
        printf("          Coluna: ");
        scanf("%d",&col);
        if(col>COL){
            Mensagens(15);
        }
   }while(col>COL);

   return col;
}

/* Mensagens para impressão */
void Mensagens(int op){
    switch(op){
        case 1:
            printf("\n\n      COMPRA EFETIVADA COM SUCESSO !!");
            break;
        case 2:
            printf("\n\n      RESERVA EFETIVADA COM SUCESSO !!");
            break;
        case 3:
            printf("\n\n      CADEIRA OCUPADA!! ESCOLHA OUTRA !!\n\n");
            break;
        case 4:
            printf("\n\n      CADEIRA RESERVADA!! ESCOLHA OUTRA !!\n\n");
            break;
        case 5:
            printf("\n\n      PRECIONE QUALQUER TECLA PARA CONTINUAR!");
            getch();
            break;
        case 6:
            printf("\n\n\n    DIGITE APENAS VALORES CORESPONDENTES AO MENU !!");
            break;
        case 7:
            printf("\n  Obrigado por usar nosso software!\n");
            break;
        case 8:
            printf("\n\n      OPERACAO REALIZADA COM SUCESSO!");
            break;
        case 9:
            printf("\n\n      FALHA NA OPERACAO!");
            break;
        case 10:
            printf("\n       ESTE CODIGO NAO ESTA DISPONIVEL!");
            break;
        case 11:
            printf("\n            LISTA VAZIA !!!\n");
            break;
        case 12:
            printf("\n\n\n       (.)-Livre  (X)-Ocupado  (R)-Reservado\n\n");
            break;
        case 13:
            printf("\n\n      NOVO EVENTO CADASTRADO COM SUCESSO!");
            break;
        case 14:
            printf("\n\n      ESTE CODIGO NAO EXISTE NA LISTA!");
            break;
        case 15:
            printf("\t\t\n (!)  Esta cadeira nao existe. Escolha uma opcao valida!!!  (!)\n\n");
            break;
        case 16:
            printf("\n\n  (!)  Ocorreu um erro inesperado  (!)");
            break;

    }
}

/* Cabeçalhos dos Menus */
void Cabecalhos(int opcao){
    switch(opcao){
    case 1:
        Limpar_Tela();
        printf("\n\n   0- SAIR / 1-ADM\n\n");
        printf("\n\n               ######  T E A T R O  ######");
        printf("\n\n                 ######  SESSOES  ######\n\n");
        printf("        ------------------------------------------");
        printf("\n            COD. |        NOME        |   DATA\n");
        printf("        ------------------------------------------\n");
        break;
    case 2:
        Limpar_Tela();
        printf("\n\n              ######  T E A T R O  ######");
        printf("\n\n               #####  SESSOES ADM  #####\n\n");
        printf("          *  1- Criar Sessao               *\n");
        printf("          *  2- Remover Sessao             *\n");
        printf("          *  3- Mostrar Sessoes            *\n");
        printf("          *  0- Sair                       *\n");
        printf("\n\n         Opcao: ");
        break;
    case 3:
        Limpar_Tela();
        printf("\n        ------------------------------------------");
        printf("\n                   SESSOES CADASTRADAS\n");
        printf("        ------------------------------------------");
        printf("\n            COD. |        NOME        |   DATA\n");
        printf("        ------------------------------------------\n");
        break;
    case 4:
        Limpar_Tela();
        printf("\n      ------------------------------------------");
        printf("\n                     REMOVER SESSAO \n");
        printf("      ------------------------------------------\n\n");
        break;
    case 5:
        Limpar_Tela();
        printf("\n\n              ######  T E A T R O  ######");
        printf("\n\n                 ######  MENU  ######\n\n");
        printf("          *  1- Comprar                     *\n");
        printf("          *  2- Reservar                    *\n");
        printf("          *  3- Mostrar Painel              *\n");
        printf("          *  0- Exit                        *\n\n");
        break;
    }
}

/* Lima a tela do prompt(Windows - cls) / (Linux - clear) */
void Limpar_Tela(){
    system("cls");
}
