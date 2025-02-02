#include <stdio.h>
#include "pico/stdlib.h"


#include "definicoes.h"


int animacaoBasica(){
  
     npClear();
    //exemplo de uso da função npSetLED para acender um LED de cada vez
    for (uint i = 0; i < LED_COUNT; i++){
        npSetLED(i, 60, 0, 0);
        sleep_ms(200);
        npWrite();
        }

}




// Estrutura para cores GRB
typedef struct {
    int green;
    int red;
    int blue;
} LED;

// Matriz de LEDs 5x5
#define ROWS 5
#define COLS 5
LED ledMatrix[ROWS][COLS];

// Define o FPS padrão para animações (quadros por segundo)
#define FPS 10 // Pode ser alterado pela equipe de desenvolvimento

// ** Funções Prototipadas **
int getIndexMatriz(int x, int y);
void initializeMatrix(LED matrix[ROWS][COLS], int green, int red, int blue);
void applyMatrixToNeoPixel(LED matrix[ROWS][COLS]);
void animateMatrix(LED frames[][ROWS][COLS], int numFrames, int fps);

// ** Função para Converter Posição da Matriz para Vetor **
int getIndexMatriz(int x, int y) {
    // Se a linha for par (0, 2, 4), percorremos da esquerda para a direita.
    // Se a linha for ímpar (1, 3), percorremos da direita para a esquerda.
    if (y % 2 == 0) {
        return 24 - (y * 5 + x); // Linha par (esquerda para direita).
    } else {
        return 24 - (y * 5 + (4 - x)); // Linha ímpar (direita para esquerda).
    }
}

// ** Inicializa a Matriz de LEDs com uma Cor Padrão **
void initializeMatrix(LED matrix[ROWS][COLS], int green, int red, int blue) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            matrix[i][j].green = green;
            matrix[i][j].red = red;
            matrix[i][j].blue = blue;
        }
    }
}

// ** Aplica os Valores da Matriz no Controlador NeoPixel **
void applyMatrixToNeoPixel(LED matrix[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int index = getIndexMatriz(j, i);
            npSetLED(index, matrix[i][j].green, matrix[i][j].red, matrix[i][j].blue);
        }
    }
    npWrite(); // Atualiza os LEDs
}

// ** Executa uma Animação com Frames **
void animateMatrix(LED frames[][ROWS][COLS], int numFrames, int fps) {
    int frameDelay = 1000 / fps; // Calcula o delay entre os frames (em ms)

    for (int frame = 0; frame < numFrames; frame++) {
        // Aplica o frame atual na matriz de LEDs
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                ledMatrix[i][j].green = frames[frame][i][j].green;
                ledMatrix[i][j].red = frames[frame][i][j].red;
                ledMatrix[i][j].blue = frames[frame][i][j].blue;
            }
        }
        applyMatrixToNeoPixel(ledMatrix); // Atualiza os LEDs no hardware
        sleep_ms(frameDelay);            // Aguarda o tempo necessário para o próximo frame
    }
}

// ** Animação Tecla 5**
void animacao5() {
    // Define os frames da animação (mínimo 5 frames)
    LED frames[5][ROWS][COLS];

    // Frame 1: Todos os LEDs vermelhos
    initializeMatrix(frames[0], 0, 30, 0); // Cor Vermelha

    // Frame 2: Todos os LEDs verdes
    initializeMatrix(frames[1], 30, 0, 0); // Cor Verde

    // Frame 3: Todos os LEDs azuis
    initializeMatrix(frames[2], 0, 0, 30); // Cor Azul

    // Frame 4: Todos os LEDs brancos
    initializeMatrix(frames[3], 30, 30, 30); // Cor Branca

    // Frame 5: Todos os LEDs apagados
    initializeMatrix(frames[4], 0, 0, 0); // Apagado

    // Executa a animação com os 5 frames no FPS definido
    animateMatrix(frames, 5, FPS);

}

