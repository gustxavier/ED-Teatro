/******************************************************************************
 Name        : Teatro.c
 Author      : Gustavo Xavier | Lucas Roncolado
 Version     : 1.0.0
 Copyright   : Todos os direitos reservados
 Description : Este programa gerencia listas est�ticas encadeadas em C.
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
#define SESS 10

typedef struct{
    int cod, c_lin, c_col;
    char nome[40];
    char email[40];
    char tel[11];
}T_Pessoa;

typedef struct Cx{
    T_Pessoa Item;
    struct Cx *Prox;
}Caixa;

typedef Caixa *Ponteiro;


typedef struct{
    Ponteiro Prim, Ult;
    int Tam;
}Lista_din_enc;

void Menu_Principal();
void Menu_ADM();
void Menu_Sessao(char arquivo[TAM_ARQ]);
void Criar_Lista_Vazia(Lista_din_enc *L);
int Verifica_Lista_Vazia(Lista_din_enc L);
void Insere_Elemento_Lista(Lista_din_enc *L, T_Pessoa X);
void Exibir_Elemento(T_Pessoa);
void Exibir_Elemento_Session(T_Pessoa);
void Remove_Elemento_Lista(Lista_din_enc *L, T_Pessoa *X);
void Exibir_Lista(Lista_din_enc);
void Exibir_Lista_Session(Lista_din_enc);
int Consulta_Elemento(Lista_din_enc , int);
void Dados_Pessoa(T_Pessoa *X);
void Dados_Sessao(T_Pessoa *S);
void Gravar_Arquivo(char arquivo[TAM_ARQ], Lista_din_enc *L);
void Gravar_Arquivo_Sessoes(Lista_din_enc *L);
void Carregar_Arquivo(char arquivo[TAM_ARQ], Lista_din_enc *L, T_Pessoa X);
void Carregar_Sessoes(Lista_din_enc *L, T_Pessoa X);
//Matriz
void Inicializa_Matriz(char matriz[ROW][COL]);
void Mostrar_Painel(char matriz[ROW][COL]);
void Selecionar_Cadeira(char matriz[ROW][COL], T_Pessoa *X);
void Reservar(char matriz[ROW][COL]);
void Legenda();
int Menu_Opcao();
int ColunaY();
int LinhaX();
void Mensagens(int op);
void Limpar_Tela();


int main(){

    Menu_Principal();

    getchar();
    return 0;
}

void Menu_Principal(){
    int opcao;
    int parar = FALSE;
    Lista_din_enc L;
    T_Pessoa X;


    Criar_Lista_Vazia(&L);
    do{
        system("cls");
        printf("\n\n   00- SAIR / 01-ADM\n\n");
        printf("\n\n         ######  T E A T R O  ######");
        printf("\n\n           ######  SESSOES  ######\n\n");
        printf("        -----------------------------");
        printf("\n            COD. |       NOME\n");
        printf("        -----------------------------\n");
        Carregar_Sessoes(&L,X);
        Exibir_Lista_Session(L);
        printf("        -----------------------------");
        printf("\n\n        >>Digite o codigo da sessao: ");

        scanf("%d",&opcao);
        switch(opcao){
            case 00:
                Mensagens(7);
                parar = TRUE;
                break;
            case 01:
                Menu_ADM();
                break;
            default:
                if(Consulta_Elemento(L, opcao)){
                    Menu_Sessao(X.nome);
                }
        }
    }while(parar!=TRUE);
}

void Menu_ADM(){
    int parar = FALSE;
    int op;
    T_Pessoa X;
    Lista_din_enc L;
    FILE *arq;
    Ponteiro P;
    Criar_Lista_Vazia(&L);

    do{
        Limpar_Tela();
        printf("\n\n              ######  T E A T R O  ######");
        printf("\n\n               #####  SESSOES ADM  #####\n\n");
        printf("          *  1- Criar Sessao               *\n");
        printf("          *  2- Remover Sessao             *\n");
        printf("          *  3- Sair                       *\n");
        printf("\n\n         Opcao: ");
        scanf("%d", & op);
        switch(op){
            case 1:
                Limpar_Tela();
                printf("\n\n              ######  T E A T R O  ######");
                printf("\n\n               #####  SESSOES ADM  #####\n\n");
                Dados_Sessao(&X);
                Insere_Elemento_Lista(&L,X);
                Exibir_Elemento_Session(X);
                arq = fopen("sessoes.dat", "wb");
                if(arq!=NULL){
                    P=L.Prim->Prox;
                    while(P!=NULL){
                        fwrite(&(P->Item), sizeof(T_Pessoa),1,arq);   //fechamento
                        P=P->Prox;
                    }
                    fclose(arq);
                }
//                Gravar_Arquivo(X.nome,&L);
                printf("%c",X.nome);
                getch();
                Mensagens(8);
                Mensagens(5);
                break;
            case 2:
                Mensagens(1);
                break;
            case 3:
                parar = TRUE;
                break;
        }
    }while(parar!=TRUE);
}

void Menu_Sessao(char arquivo[TAM_ARQ]){
    char matriz[ROW][COL];
    int opcao;
    int loopContinue=TRUE;
    Lista_din_enc L;
    T_Pessoa X;
    printf("%s",arquivo);
    getch();
    Criar_Lista_Vazia(&L);
    Carregar_Arquivo(arquivo,&L,X);
    Inicializa_Matriz(matriz); //Iniciliza a matriz com valores .(Livre)

    do{
         system("cls");//Limpa a tela
         opcao = Menu_Opcao();
         switch(opcao){
            case 1://Comprar
                Limpar_Tela();
                Mostrar_Painel(matriz);//Mostra o Painel atualizado
                Dados_Pessoa(&X);
                Selecionar_Cadeira(matriz,&X);
                Insere_Elemento_Lista(&L,X); // Realiza a compra de uma cadeira no painel, marcando c/ um X
                Gravar_Arquivo(arquivo, &L);
                Mensagens(8);
                Mensagens(5);
                break;
            case 2://Reservar
                Limpar_Tela();
                Mostrar_Painel(matriz);
                Reservar(matriz); //Realiza a reserva de uma cadeira no painel, marcando c/ um R
                break;
            case 3://Mostrar Painel
                Limpar_Tela();
                Mostrar_Painel(matriz);
                Exibir_Lista(L);
                Mensagens(5);
                break;
            case 4://Gravar arquivo
                printf("nome do arquivo: ");
                scanf("%s", arquivo);
                Gravar_Arquivo(arquivo, &L);
                break;
            case 5://Carregar em arquivo
                printf("nome do arquivo: ");
                scanf("%s", arquivo);
                Carregar_Arquivo(arquivo, &L, X);
                break;
            case 6: //Sair
                loopContinue = FALSE; //condicao para sa�da do programa
                break;
            default:
                Mensagens(6);
                Mensagens(5);
                break;
         }
    }while(loopContinue);
}

//Cria lista din�mica vazia
void Criar_Lista_Vazia(Lista_din_enc *L){
    Ponteiro P;
    P = (Ponteiro) malloc(sizeof (Caixa));
    L->Prim = P;
    L->Ult = L->Prim;
    L->Tam = 0;
}

//Verifica lista vazia
int Verifica_Lista_Vazia(Lista_din_enc L){
    return(L.Prim == L.Ult);
}

//Insere um elemento na lista
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

void Exibir_Elemento(T_Pessoa X){
    printf("  %5d - %s - CADEIRA:%d%d\n",X.cod, X.nome,X.c_lin,X.c_col);
}

void Exibir_Elemento_Session(T_Pessoa S){
    printf("          %5d  -  %s \n",S.cod, S.nome);
}

 void Remove_Elemento_Lista(Lista_din_enc *L, T_Pessoa *X)
  { Ponteiro P, A;
    if(Verifica_Lista_Vazia (*L))
      printf("A Lista esta vazia - remove\n");
     else { P = L->Prim;
            while((P!=L->Ult)&&(X->cod > P->Prox->Item.cod))
                P=P->Prox;
            if ((P==L->Ult)||(X->cod!=P->Prox->Item.cod))
              printf("Elemento NAO EXISTE na lista - Remove\n");
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

 void Exibir_Lista(Lista_din_enc L){
    Ponteiro P;

	if(Verifica_Lista_Vazia(L))
	   printf("Lista Vazia - nada para exibir \n\n");
	  else { P = L.Prim;
		     while(P!=L.Ult)
		       { Exibir_Elemento(P->Prox->Item);
				 P=P->Prox;
			    }
			 }
}

 void Exibir_Lista_Session(Lista_din_enc L){
    Ponteiro P;

	if(Verifica_Lista_Vazia(L))
	   printf("Lista Vazia - nada para exibir \n\n");
	  else { P = L.Prim;
		     while(P!=L.Ult)
		       { Exibir_Elemento_Session(P->Prox->Item);
				 P=P->Prox;
			    }
      }
}

int Consulta_Elemento(Lista_din_enc L, int cod){
    Ponteiro P;
    if(Verifica_Lista_Vazia (L)){
        Mensagens(11);
    }else {
        P = L.Prim->Prox;
        while((P!=NULL)&&(cod > P->Item.cod))
            P=P->Prox;
        if ((P==NULL)||(cod!=P->Item.cod)){
            Mensagens(6);
            Mensagens(5);
        }else{
            return TRUE;
        }
    }
    return FALSE;
}

void Dados_Pessoa(T_Pessoa *X){
    printf("***** Registro de Dados - CLIENTE *****\n");
    printf("\nEntre com o C�digo: ");
    scanf("%d",&(X->cod));
    printf("\nEntre com o Nome : ");
    scanf("%s",X->nome);
    printf("\n* Entre com o email: ");
    scanf("%s",X->email);
    printf("\n* Entre com o Telefone: ");
    scanf("%s", X->tel);
}

void Dados_Sessao(T_Pessoa *X){
    printf("\n  Nome da Sessao: ");
    scanf("%s", X->nome);
    do{
        printf("\n  Codigo da Sessao: ");
        scanf("%d", &X->cod);
        if(X->cod == 00 || X->cod == 01){
            Mensagens(10);
            Mensagens(5);
        }
    }while(X->cod == 00 || X->cod == 01);
}

void Gravar_Arquivo(char arquivo[TAM_ARQ], Lista_din_enc *L){
    FILE *arq;
    Ponteiro P;
    char buffer[] = ".dat";

    strcat(arquivo, buffer);
    arq = fopen(arquivo, "wb");
    if(arq!=NULL){
        P = L->Prim->Prox;
        while(P!=NULL){
            fwrite(&(P->Item), sizeof(T_Pessoa),1,arq);   //fechamento
            P=P->Prox;
        }
        fclose(arq);
    }
}

void Gravar_Arquivo_Sessoes(Lista_din_enc *L){
    FILE *arq;
    Ponteiro P;
    arq = fopen("arquivo.dat", "wb");
    if(arq!=NULL)
      {
        P=L->Prim->Prox;
        while(P!=NULL)
          { fwrite(&(P->Item), sizeof(T_Pessoa),1,arq);   //fechamento
            P=P->Prox;
           }
        fclose(arq);
       }
}

void Carregar_Arquivo(char arquivo[TAM_ARQ], Lista_din_enc *L, T_Pessoa X){
    FILE *arq;
    char buffer[] = ".dat";

    strcat(arquivo, buffer);
    arq = fopen(arquivo, "rb");
    if (arq!=NULL){
        Criar_Lista_Vazia(L);
        X.cod=0;
        while (!feof(arq)){
            if(X.cod!=0)
              Insere_Elemento_Lista(L,X);
            fread(&X, sizeof(T_Pessoa), 1, arq);   //fechamento
           }
       fclose(arq);
      }
}

void Carregar_Sessoes(Lista_din_enc *L, T_Pessoa X){
    FILE *arq;

    arq = fopen("sessoes.dat", "rb");
        if (arq!=NULL){
            Criar_Lista_Vazia(L);
            X.cod=0;
            while (!feof(arq))
              { if(X.cod!=0)
                  Insere_Elemento_Lista(L,X);
                fread(&X, sizeof(T_Pessoa), 1, arq);   //fechamento
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
    printf("\n\n         ####   PAINEL DE OCUPACOES   ####\n\n");
    printf("          ");

    for(i=1;i<=COL;i++)
        printf("%d     ",i);

    for(i=1;i<=ROW;i++){
        printf("\n\n     %d",i);
        for(j=1;j<=COL;j++)
            printf("   (%c)",matriz[i][j]);
    }
    //apresenta o menu na tela
    Legenda();
}

int Menu_Opcao(){
    int opcao;
    system("cls");
    printf("\n\n              ######  T E A T R O  ######");
    printf("\n\n                 ######  MENU  ######\n\n");
    printf("          *  1- Comprar                     *\n");
    printf("          *  2- Reservar                    *\n");
    printf("          *  3- Mostrar Painel              *\n");
    printf("          *  4- Gravar em Arquivo           *\n");
    printf("          *  5- Carregar Lista de Arquivo   *\n");
    printf("          *  6- Exit                        *\n\n");
    printf("                 opcao: ");

    scanf("%d",&opcao);

    return opcao;
}
//apresenta  a legenda na tela
void Legenda(){
    printf("\n\n\n        (.)-Livre  (X)-Ocupado  (R)-Reservado\n\n");
}
    //Efetiva a compra da cadeira
void Selecionar_Cadeira(char matriz[ROW][COL],T_Pessoa *X){
    int row, col;
    printf("\n       <<< COMPRA DE CADEIRAS >>>\n\n");
    row = LinhaX();
    col = ColunaY();
    X->c_lin = row;
    X->c_col = col;

    if(matriz[row][col]==32){//SPACE
        matriz[row][col] = 88;//X
        system("cls");
        printf("\n\n      COMPRA EFETIVADA COM SUCESSO !!");
    }else if (matriz[row][col]==88){//X
        printf("\n\n      CADEIRA OCUPADA!! ESCOLHA OUTRA !!\n\n");
    }else if(matriz[row][col]==82){//R
        printf("\n\n    CADEIRA RESERVADA!! ESCOLHA OUTRA !!\n\n");
    }
    Mensagens(5);
}
//Reserva a cadeira no teatro
void Reservar(char matriz[ROW][COL]){
   int row, col;
    printf("\n\n         RESERVA DE CADEIRAS\n\n");
   row = LinhaX();
   col = ColunaY();

   if(matriz[row][col]=='.'){
       matriz[row][col] = 82;//R
       printf("\n\n      RESERVA EFETIVADA COM SUCESSO !!");
   }
   else if (matriz[row][col]==88)//X
             printf("\n\n      CADEIRA OCUPADA!! ESCOLHA OUTRA !!\n\n");
        else if(matriz[row][col]==82)//R
               printf("\n\n      CADEIRA RESERVADA !! ESCOLHA OUTRA !!\n\n");

    Mensagens(5);
}
//ler a linha oferecida pelo usuario
int LinhaX(){
    int row;
    do{
        printf("          Linha: ");
        scanf("%d",&row);
        if(row>ROW){
            printf("\t\t\n Esta cadeira nao existe. Escolha uma opcao valida!!!\n\n");
        }
    }while(row>ROW);

    return row;
}

//L� a coluna oferecida pelo usuario
int ColunaY(){
   int col;
   do{
        printf("          Coluna: ");
        scanf("%d",&col);
        if(col>COL){
            printf("\t\t\n Esta cadeira nao existe. Escolha uma opcao valida!!!\n\n");
        }
   }while(col>COL);

   return col;
}

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
            printf("\nObrigado por usar nosso software!\n");
            break;
        case 8:
            printf("\n\n      OPERACAO REALIZADA COM SUCESSO!");
            break;
        case 9:
            printf("\n\n      FALHA NA OPERACAO!");
            break;
        case 10:
            printf("ESTE CODIGO NAO ESTA DISPONIVEL!");
            break;
        case 11:
            printf("LISTA VAZIA !!!");
            break;
    }
}

void Limpar_Tela(){
    system("cls");
}
