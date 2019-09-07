#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define altura 15
#define largura 15

void gotoxy(int x, int y) {
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int inicia(int *corpo) {
    int alturaReal = altura + 2;
    int larguraReal = largura + 2;
    int i, j;
    int tamanho = 1;

    corpo[0] = ((altura / 2) * largura) + (largura / 2);

    for(i = 0; i < alturaReal; i++) {
        for(j = 0; j < larguraReal; j++) {
            if(i != 0 && i != (alturaReal - 1) && j != 0 && j != (larguraReal - 1))
                printf("  ");
            else
                printf("%c ", 157);
        }
        printf("\n");
    }

    gotoxy((largura + 3) * 2 + 1, 1);
    printf("PONTUACAO");

    gotoxy((largura + 3) * 2, 3);
    for(i = 0; i < 5; i++) {
        printf("%c%c", 223, 223);
    }

    gotoxy((largura + 3) * 2, 5);
    for(i = 0; i < 5; i++) {
        printf("%c%c", 220, 220);
    }

    for(i = 0; i < 3; i++) {
        gotoxy((largura + 3) * 2, (3 + i));
        printf("%c", 219);
        gotoxy((largura + 8) * 2, (3 + i));
        printf("%c", 219);
    }

    for(i = 0; i < tamanho; i++) {
        gotoxy((corpo[i] % largura + 1) * 2, corpo[i] / largura + 1);
        printf("%c ", 254);
    }

    return tamanho;
}

int geraFruta(int tamanho, int *corpo) {
    int fruta;
    int i, gerando = 1;

    while(gerando) {
        fruta = rand() % (altura * largura);
        gerando = 0;

        for(i = 0; i < tamanho; i++) {
            if(corpo[i] == fruta) {
                gerando = 1;
            }
        }
    }

    gotoxy((fruta % largura + 1) * 2, fruta / largura + 1);
    printf("X ");

    return fruta;
}

void move(int tamanho, int *corpo, int direcao) {
    int i;
    int novaPosicao = corpo[0] + ((direcao % 2) * (((direcao / 2) * (-2)) + 1)) + (((direcao + 1) % 2) * (((direcao / 2) * (largura * 2)) -largura));

    for(i = tamanho; i > 0; i--)
        corpo[i] = corpo[i - 1];

    corpo[0] = novaPosicao;
}

void desenha(int tamanho, int *corpo, int pontos) {
    gotoxy(((largura + 4) * 2 + 1), 4);
    printf("%5d", pontos);

    gotoxy((corpo[tamanho] % largura + 1) * 2, corpo[tamanho] / largura + 1);
    printf("  ");
    gotoxy((corpo[0] % largura + 1) * 2, corpo[0] / largura + 1);
    printf("%c ", 254);
    gotoxy((corpo[0] % largura + 1) * 2, corpo[0] / largura + 1);
}

int checaColisao(int tamanho, int *corpo, int direcao, int fruta) {
    int i;

    if(direcao == 1 && corpo[0] % largura == 0)
        return 2;
    if(direcao == 3 && corpo[0] % largura == (largura - 1))
        return 2;
    if(direcao == 0 && corpo[0] < 0)
        return 2;
    if(direcao == 2 && corpo[0] >= altura * largura)
        return 2;

    for(i = tamanho - 1; i > 0; i--)
        if(corpo[0] == corpo[i])
            return 1;

    if(corpo[0] == fruta)
        return 3;

    return 0;
}

int main() {
    srand(time(NULL));

    int i, j;
    int fruta, tamanho, corpo[altura * largura];
    int direcao = 1, perdeu = 0, pontos = 0, status = 0; // 0 - Nao colidiu. 1 - Colidiu corpo. 2 - Colidiu Parede. 3 - Colidiu fruta.

    tamanho = inicia(corpo);
    fruta = geraFruta(tamanho, corpo);

    while(1) {
        move(tamanho, corpo, direcao);
        status = checaColisao(tamanho, corpo, direcao, fruta);

        if(status == 3) {
            fruta = geraFruta(tamanho, corpo);
            pontos += 20;
            tamanho++;
        }
        else if(status == 1 || status == 2)
            break;

        desenha(tamanho, corpo, pontos);

        pontos++;
        Sleep(200);


        if(kbhit()) {
            switch(getch()) {
            case 'w':
                if(direcao != 2)
                    direcao = 0;
                break;
            case 'a':
                if(direcao != 1)
                    direcao = 3;
                break;
            case 's':
                if(direcao != 0)
                    direcao = 2;
                break;
            case 'd':
                if(direcao != 3)
                    direcao = 1;
                break;
            }

            while(kbhit())
                getch();
        }
    }

    gotoxy(((largura + 4) * 2 + 1), altura / 2);
    printf("PERDEU");
    gotoxy(0, altura + 4);

    return 0;
}
