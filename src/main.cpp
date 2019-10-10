/* OpenCV includes */
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#define PATHDIR "/Users/viniciuscoelho/github/carplate-ocr/dataChar/"

using namespace std;

typedef pair<CvRect, double> CvRectD;

vector<pair<int, CvRectD>> r;

bool comp(pair<int, CvRectD> a, pair<int, CvRectD> b);
char retornaLetra(IplImage* imagem);
char retornaNumero(IplImage* imagem);
double verificaLados(int* a, int* b, int tamanho);
double verificaBases(int* a, int* b, int tamanho);
char retornaCaracterLetra(IplImage* imagem);
char retornaCaracterNumero(IplImage* imagem);
void pegaDireita(IplImage* imagem, int* ladoDireito, double it);
void pegaEsquerda(IplImage* imagem, int* ladoEsquerdo, double it);
void pegaTopo(IplImage* imagem, int* vetorTopo, double it);
void pegaBase(IplImage* imagem, int* vetorBase, double it);
int pegaMenorDistancia(int* vetor, int tamanho, int x);
char logica(char car[], char charMedia);
int segunda(char* letras, char* numeros);
int principal();
void apagaArquivos();

int main()
{
    char letras[4], numeros[5];
    bool flag = true;
    while (flag) {
        apagaArquivos();
        //system("clear");
        principal();
        if (!segunda(letras, numeros)) {
            //system("clear");
            cout << "Erro ao identificar imagem.\n\nTente reajustar o ponto de corte.\n";
            //system("pause");
        } else {
            //system("clear");
            cout << "Numero da placa: " << letras << " - " << numeros << "\n";
            flag = false;
        }
    }
    apagaArquivos();
    //system("pause");
    return 0;
}

void apagaArquivos()
{
    int i = 0;
    char num[200], ex[10] = ".jpg", ger[200] = "";
    sprintf(num, "%s%d", ger, i);
    char path[200];
    strcpy(path, strcat(num, ex));
    while (cvLoadImage(path, 0)) {
        char comando[100];
        strcpy(comando, "rm -rf ");
        strcat(comando, path);
        system(comando);
        path[0] = '\0';
        i++;
        sprintf(num, "%s%d", ger, i);
        strcat(path, strcat(num, ex));
    }
}

int segunda(char* letras, char* numeros)
{
    char retorno[100];
    memset(retorno, '*', 100);
    char cl;
    IplImage* img;
    int i = 0, flag = 0;
    char num[20], ex[10] = ".jpg", ger[20] = "";
    sprintf(num, "%s%d", ger, i);
    cl = 0;

    /* Matching letters */

    while ((img = cvLoadImage(strcat(num, ex), 0)) != 0) {
        if (img->width < img->height) {
            cl = retornaCaracterLetra(img);
            if (cl >= 65 && cl <= 90)
                retorno[i] = cl;
        }
        sprintf(num, "%s%d", ger, i);
        i++;
    }
    int contL;
    for (int j = 0; j < 97; j++) //tres letras seguidas
    {
        if (retorno[j] != '*' && retorno[j + 1] != '*' && retorno[j + 2] != '*') {
            letras[0] = retorno[j];
            letras[1] = retorno[j + 1];
            letras[2] = retorno[j + 2];
            letras[3] = '\0';
            contL = j;
            flag = 1;
            break;
        }
        flag = 0;
    }
    if (!flag) //duas seguidas e uma falha
    {
        for (int j = 0; j < 96; j++) {
            if (retorno[j] != '*' && retorno[j + 1] != '*' && retorno[j + 3] != '*') {
                letras[0] = retorno[j];
                letras[1] = retorno[j + 1];
                letras[2] = retorno[j + 3];
                letras[3] = '\0';
                contL = j + 1;
                flag = 1;
                break;
            }
            flag = 0;
        }
    }

    if (!flag) //uma seguida de falha e duas seguidas
    {
        for (int j = 0; j < 96; j++) {
            if (retorno[j] != '*' && retorno[j + 2] != '*' && retorno[j + 3] != '*') {
                letras[0] = retorno[j];
                letras[1] = retorno[j + 2];
                letras[2] = retorno[j + 3];
                letras[3] = '\0';
                contL = j + 1;
                flag = 1;
                break;
            }
            flag = 0;
        }
    }

    if (!flag) //intercaladas
    {
        for (int j = 0; j < 95; j++) {
            if (retorno[j] != '*' && retorno[j + 2] != '*' && retorno[j + 4] != '*') {
                letras[0] = retorno[j];
                letras[1] = retorno[j + 2];
                letras[2] = retorno[j + 3];
                letras[3] = '\0';
                contL = j + 2;
                flag = 1;
                break;
            }
            flag = 0;
        }
    }

    if (!flag)
        return 0;

    /* Matching numbers */

    i = contL + 3 - 1;
    memset(retorno, '*', 100);
    sprintf(num, "%s%d", ger, i);
    while ((img = cvLoadImage(strcat(num, ex), 0)) != 0) {
        if (img->width < img->height) {
            cl = retornaCaracterNumero(img);
            if (cl >= 48 && cl <= 57)
                retorno[i - contL - 3] = cl;
        }
        sprintf(num, "%s%d", ger, i);
        i++;
    }

    int contN;
    for (int j = 0; j < 96; j++) //quatro seguidas
    {
        if (retorno[j] != '*' && retorno[j + 1] != '*' && retorno[j + 2] != '*' && retorno[j + 3] != '*') {
            numeros[0] = retorno[j];
            numeros[1] = retorno[j + 1];
            numeros[2] = retorno[j + 2];
            numeros[3] = retorno[j + 3];
            numeros[4] = '\0';
            contN = j;
            flag = 1;
            break;
        }
        flag = 0;
    }
    if (!flag) //tres seguidas uma falha
    {
        for (int j = 0; j < 95; j++) {
            if (retorno[j] != '*' && retorno[j + 1] != '*' && retorno[j + 2] != '*' && retorno[j + 4] != '*') {
                numeros[0] = retorno[j];
                numeros[1] = retorno[j + 1];
                numeros[2] = retorno[j + 2];
                numeros[3] = retorno[j + 4];
                numeros[4] = '\0';
                contN = j;
                flag = 1;
                break;
            }
            flag = 0;
        }
    }
    if (!flag) //duas seguidas uma falha
    {
        for (int j = 0; j < 95; j++) {
            if (retorno[j] != '*' && retorno[j + 1] != '*' && retorno[j + 3] != '*' && retorno[j + 4] != '*') {
                numeros[0] = retorno[j];
                numeros[1] = retorno[j + 1];
                numeros[2] = retorno[j + 3];
                numeros[3] = retorno[j + 4];
                numeros[4] = '\0';
                contN = j;
                flag = 1;
                break;
            }
            flag = 0;
        }
    }
    if (!flag) //uma seguida de falha
    {
        for (int j = 0; j < 95; j++) {
            if (retorno[j] != '*' && retorno[j + 2] != '*' && retorno[j + 3] != '*' && retorno[j + 4] != '*') {
                numeros[0] = retorno[j];
                numeros[1] = retorno[j + 2];
                numeros[2] = retorno[j + 3];
                numeros[3] = retorno[j + 4];
                numeros[4] = '\0';
                contN = j;
                flag = 1;
                break;
            }
            flag = 0;
        }
    }

    if (!flag) //intercaladas
    {
        for (int j = 0; j < 94; j++) {
            if (retorno[j] != '*' && retorno[j + 2] != '*' && retorno[j + 4] != '*' && retorno[j + 6] != '*') {
                numeros[0] = retorno[j];
                numeros[1] = retorno[j + 2];
                numeros[2] = retorno[j + 4];
                numeros[3] = retorno[j + 6];
                numeros[4] = '\0';
                contN = j;
                flag = 1;
                break;
            }
            flag = 0;
        }
    }

    if (!flag)
        return 0;
    return 1;
}

int principal()
{
    char imagem[100];
    IplImage *src = NULL, *dst = NULL;
    while (src == NULL) {
        cout << "Digite o nome da imagem: ";
        cin >> imagem;
        src = cvLoadImage(imagem, 0);
        if (src == NULL)
            cout << "Imagem nao encontrada!\n";
    }
    IplImage* img = cvCreateImage(cvSize(src->width / 2, src->height / 2), src->depth, src->nChannels);
    cvPyrDown(src, img, CV_GAUSSIAN_5x5);
    dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
    int corte = 0;
    char ger[20] = "", num[20], ex[10] = ".jpg", opc = 'n';
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq *contour = 0, *contorno = 0, *aux = 0;
    cvSmooth(img, img, CV_BLUR, 5, 5);

    while (opc == 'n' && opc != 's') {
        if (opc == 'n') {
            src = cvLoadImage(imagem, 0);
            cvPyrDown(src, img, CV_GAUSSIAN_5x5);
            cout << "Digite um ponto de corte para o threshold: ";
            cin >> corte;
            cvThreshold(img, img, corte, 255, CV_THRESH_BINARY);
            cvShowImage("Threshold", img);
            cvWaitKey(-1);
            cin.ignore();
            cout << "A imagem esta adequada? s/n ";
            cin >> opc;
        }
    }

    cvDestroyWindow("Threshold");
    cvSaveImage("threshold.jpg", img, 0);
    cvFindContours(img, storage, &contorno, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
    contour = contorno;
    aux = contour;
    int count = 0;
    for (int i = 0; i < dst->width; ++i)
        for (int j = 0; j < dst->height; ++j)
            cvLine(dst, cvPoint(i, j), cvPoint(i, j), CV_RGB(255, 255, 255), 1, 8, 0);

    for (; aux != 0; aux = aux->h_next) {
        CvRect rect = cvBoundingRect(aux, 1);
        double area = cvContourArea(aux, CV_WHOLE_SEQ);
        int x = rect.x;
        r.push_back(make_pair(x, make_pair(rect, area)));
    }
    sort(r.begin(), r.end(), comp);

    for (int i = 0; i < r.size(); ++i) {
        double area = r[i].second.second;
        CvRect rect = r[i].second.first;
        if (area > 85.0 && area < 600.0 && rect.height < 35 && rect.width < 25) {
            IplImage* d = cvLoadImage("threshold.jpg", 0);
            sprintf(num, "%s%d", ger, count);
            cvSetImageROI(d, rect); //recorte dos contornos
            cvSaveImage(strcat(num, ex), d, 0);
            count++;
        }
    }
    r.clear();
    cvWaitKey(1000);
    cvReleaseImage(&img);
    cvDestroyWindow("Janela");

    return 1;
}

bool comp(pair<int, CvRectD> a, pair<int, CvRectD> b)
{
    return a.first < b.first;
}

char retornaCaracterNumero(IplImage* imagem)
{
    char pathChar[200];
    IplImage* imagemBase;
    int largura = imagem->width;
    int altura = imagem->height;

    int vetorBase[largura];
    int vetorBase1[largura];
    int vetorTopo[largura];
    int vetorTopo1[largura];
    int vetorEsquerda[altura];
    int vetorEsquerda1[altura];
    int vetorDireita[altura];
    int vetorDireita1[altura];
    pegaBase(imagem, vetorBase, 1.0);
    pegaEsquerda(imagem, vetorEsquerda, 1.0);
    pegaDireita(imagem, vetorDireita, 1.0);
    pegaTopo(imagem, vetorTopo, 1.0);

    char cct[] = { ' ', ' ', ' ', ' ' }, charMedia;
    double comp[4], maiorMedia = 0.0;
    double maiorComp[] = { 0, 0, 0, 0 };

    char car[] = "0/";
    for (int j = 0; j < 10; j++) {
        strcpy(pathChar, PATHDIR);
        strcat(pathChar, car);
        int i = 1;
        char num[20], ex[10] = ".jpg", ger[20] = "";
        sprintf(num, "%s%d", ger, i);
        strcat(pathChar, strcat(num, ex));
        while (imagemBase = cvLoadImage(pathChar, 0)) {
            pegaBase(imagemBase, vetorBase1, (double)imagemBase->width / (double)largura);
            pegaEsquerda(imagemBase, vetorEsquerda1, (double)imagemBase->height / (double)altura);
            pegaDireita(imagemBase, vetorDireita1, (double)imagemBase->height / (double)altura);
            pegaTopo(imagemBase, vetorTopo1, (double)imagemBase->width / (double)largura);
            comp[0] = verificaLados(vetorEsquerda, vetorEsquerda1, altura);
            comp[1] = verificaBases(vetorBase, vetorBase1, largura);
            comp[2] = verificaLados(vetorDireita, vetorDireita1, altura);
            comp[3] = verificaBases(vetorTopo, vetorTopo1, largura);
            if ((comp[0] + comp[1] + comp[2] + comp[3]) / 4.0 > maiorMedia) {
                maiorMedia = (comp[0] + comp[1] + comp[2] + comp[3]) / 4.0;
                charMedia = car[0];
            }
            for (int z = 0; z < 4; z++) {
                if (comp[z] > 0.5)
                    if (comp[z] > maiorComp[z]) {
                        maiorComp[z] = comp[z];
                        cct[z] = car[0];
                    }
            }
            i++;
            strcpy(pathChar, PATHDIR);
            strcat(pathChar, car);
            sprintf(num, "%s%d", ger, i);
            strcat(pathChar, strcat(num, ex));
        }
        car[0]++;
    }
    return logica(cct, charMedia);
}

char retornaCaracterLetra(IplImage* imagem)
{
    char pathChar[200];
    IplImage* imagemBase;
    int largura = imagem->width;
    int altura = imagem->height;

    int vetorBase[largura];
    int vetorBase1[largura];
    int vetorTopo[largura];
    int vetorTopo1[largura];
    int vetorEsquerda[altura];
    int vetorEsquerda1[altura];
    int vetorDireita[altura];
    int vetorDireita1[altura];
    pegaBase(imagem, vetorBase, 1.0);
    pegaEsquerda(imagem, vetorEsquerda, 1.0);
    pegaDireita(imagem, vetorDireita, 1.0);
    pegaTopo(imagem, vetorTopo, 1.0);

    char cct[] = { ' ', ' ', ' ', ' ' }, charMedia;
    double comp[4], maiorMedia = 0.0;
    double maiorComp[] = { 0, 0, 0, 0 };

    char car[] = "A/";
    for (int j = 0; j < 26; j++) {
        strcpy(pathChar, PATHDIR);
        strcat(pathChar, car);
        int i = 1;
        char num[20], ex[10] = ".jpg", ger[20] = "";
        sprintf(num, "%s%d", ger, i);
        strcat(pathChar, strcat(num, ex));

        while (imagemBase = cvLoadImage(pathChar, 0)) {
            pegaBase(imagemBase, vetorBase1, (double)imagemBase->width / (double)largura);
            pegaEsquerda(imagemBase, vetorEsquerda1, (double)imagemBase->height / (double)altura);
            pegaDireita(imagemBase, vetorDireita1, (double)imagemBase->height / (double)altura);
            pegaTopo(imagemBase, vetorTopo1, (double)imagemBase->width / (double)largura);
            comp[0] = verificaLados(vetorEsquerda, vetorEsquerda1, altura);
            comp[1] = verificaBases(vetorBase, vetorBase1, largura);
            comp[2] = verificaLados(vetorDireita, vetorDireita1, altura);
            comp[3] = verificaBases(vetorTopo, vetorTopo1, largura);

            if ((comp[0] + comp[1] + comp[2] + comp[3]) / 4.0 > maiorMedia) {
                maiorMedia = (comp[0] + comp[1] + comp[2] + comp[3]) / 4.0;
                charMedia = car[0];
            }
            for (int z = 0; z < 4; z++) {
                if (comp[z] > 0.5)
                    if (comp[z] > maiorComp[z]) {
                        maiorComp[z] = comp[z];
                        cct[z] = car[0];
                    }
            }
            i++;
            strcpy(pathChar, PATHDIR);
            strcat(pathChar, car);
            sprintf(num, "%s%d", ger, i);
            strcat(pathChar, strcat(num, ex));
        }
        car[0]++;
    }
    return logica(cct, charMedia);
}

char logica(char car[], char charMedia)
{
    char retorno = '*';
    if (car[0] == car[1] && car[0] == car[2] && car[0] == car[3])
        return car[0];
    else if ((car[0] == car[1] && car[0] == car[2]) || (car[0] == car[2] && car[0] == car[3]))
        return car[0];
    else if ((car[1] == car[2] && car[1] == car[3]) || (car[1] == car[0] && car[1] == car[3]))
        return car[1];
    else if (car[0] == car[1] && car[2] != car[3])
        return car[0];
    else if (car[0] == car[2] && car[1] != car[3])
        return car[0];
    else if (car[0] == car[3] && car[1] != car[2])
        return car[0];
    else if (car[1] == car[2] && car[0] != car[3])
        return car[1];
    else if (car[1] == car[3] && car[0] != car[2])
        return car[1];
    else if (car[2] == car[3] && car[0] != car[1])
        return car[2];
    else if (car[0] == car[1] && car[2] == car[3]) {
        if (car[0] == charMedia)
            return car[0];
        else
            return car[2];
    } else if (car[0] == car[1] && car[2] == car[3]) {
        if (car[0] == charMedia)
            return car[0];
        else
            return car[2];
    } else
        return retorno;
}

void pegaDireita(IplImage* imagem, int* ladoDireito, double it)
{
    int k = 0;
    for (double i = 0.0; i < imagem->height; i += it) {
        int j = imagem->width - 1;
        while (((uchar*)(imagem->imageData + imagem->widthStep * (int)i))[j] > 10 && j > imagem->width / 2)
            j--;
        ladoDireito[k++] = j;
    }
}

void pegaEsquerda(IplImage* imagem, int* ladoEsquerdo, double it)
{
    int k = 0;
    for (double i = 0.0; i < imagem->height; i += it) {
        int j = 0;
        while (((uchar*)(imagem->imageData + imagem->widthStep * (int)i))[j] > 10 && j < imagem->width / 2)
            j++;
        ladoEsquerdo[k++] = j;
    }
}

void pegaTopo(IplImage* imagem, int* vetorTopo, double it)
{
    int k = 0;
    for (double i = 0.0; i < imagem->width; i += it) {
        int j = 0;
        while (((uchar*)(imagem->imageData + imagem->widthStep * j))[(int)i] > 10 && j < imagem->height / 2)
            j++;
        vetorTopo[k++] = j;
    }
}

void pegaBase(IplImage* imagem, int* vetorBase, double it)
{
    int k = 0;
    for (double i = 0.0; i < imagem->width; i += it) {
        int j = imagem->height - 1;
        while (((uchar*)(imagem->imageData + imagem->widthStep * j))[(int)i] > 10 && j > imagem->height / 2)
            j--;
        vetorBase[k++] = j;
    }
}

double verificaLados(int a[], int b[], int tamanho)
{
    int da, db;
    da = pegaMenorDistancia(a, tamanho, 0);
    db = pegaMenorDistancia(b, tamanho, 0);

    int i, cont = 0;
    for (i = 0; i < tamanho; i++)
        if ((a[i] - da) <= b[i] - db && b[i] - db <= (a[i] - da))
            cont++;
    return (double)cont / (double)i;
}

double verificaBases(int a[], int b[], int tamanho)
{
    int da, db;
    da = pegaMenorDistancia(a, tamanho, tamanho);
    db = pegaMenorDistancia(b, tamanho, tamanho);

    int i, cont = 0;
    for (i = 0; i < tamanho; i++)
        if ((a[i] - da - 2) <= b[i] - db && b[i] - db <= (a[i] - da + 2))
            cont++;
    return (double)cont / (double)i;
}

int pegaMenorDistancia(int* vetor, int tamanho, int x)
{
    int n, m = vetor[0] - x;
    if (m < 0)
        m *= -1;
    for (int i = 1; i < tamanho; i++) {
        if (vetor[i] - x < 0)
            n = x - vetor[i];
        else
            n = vetor[i] - x;

        m = min(m, n);
    }
    return m;
}
