//
// Created by Gustavo on 10/11/2018.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "LDDE_PRIV.h"

/*************** CRIA ***************/
int cria(LDDE **pp, int tamInfo)
{
    int ret = FRACASSO;
    LDDE *desc = (LDDE*) malloc(sizeof(LDDE));

    if( !desc ) {
        ret = FRACASSO;
    }
    else {
        desc->refMovel = NULL;
        desc->tamInfo = tamInfo;
        desc->PosLogAtual = 0;
        desc->tamLista = 0;
        ret = SUCESSO;
    }

    (*pp) = desc;

    return ret;
}

/*************** INSERE NO INICIO ***************/
int insereNoInicio(LDDE *p, void *novo)
{
    NoLDDE *temp;
    int ret = FRACASSO;

    if( (temp = (NoLDDE*) malloc(sizeof(NoLDDE))) != NULL ) {
        if( (temp->dados = (void*) malloc(p->tamInfo)) != NULL ) {
            memcpy(temp->dados, novo, p->tamInfo);
            temp->ant=NULL;
            if(p->refMovel==NULL){ //lista vazia
                p->refMovel=temp;
                temp->prox=NULL;
                p->tamLista++;
                p->PosLogAtual++;
            }
            else{
                while(p->refMovel->ant!=NULL){ //enquanto nao esta na primeira posição
                    p->refMovel=p->refMovel->ant;
                    p->PosLogAtual--;
                }
                temp->prox = p->refMovel; //esta na primeira posicao
                p->refMovel->ant = temp;
                p->refMovel = temp;
                p->tamLista++;
            }
            ret = SUCESSO;
        }
        else {
            free(temp);
        }
    }

    return ret;
}

/*************** INSERE NO FINAL ***************/
int insereNoFim(LDDE *p, void *novo)
{
    NoLDDE *temp, *aux;
    int ret = FRACASSO;

    if( (temp = (NoLDDE*) malloc(sizeof(NoLDDE))) != NULL ) {
        if((temp->dados = (void*) malloc(p->tamInfo)) != NULL ) {
            memcpy(temp->dados, novo, p->tamInfo);
            temp->prox = NULL;
            if(p->refMovel == NULL) { //lista vazia
                p->refMovel = temp;
                temp->ant = NULL;
                p->tamLista++;
                p->PosLogAtual++;
            }
            else {
                while(p->refMovel->prox != NULL){ //chegar na ultima posição
                    p->refMovel = p->refMovel->prox;
                    p->PosLogAtual++;
                }
                p->refMovel->prox = temp; //esta na ultima posicao
                temp->ant = p->refMovel;
                p->refMovel=temp;
                p->PosLogAtual++;
                p->tamLista++;
            }
            ret = SUCESSO;
        }
        else {
            free(temp);
        }
    }

    return ret;
}

/*************** INSERE NA POSI��O L�GICA ***************/
int insereNaPosLog (LDDE *p, void *novo, unsigned int posLog)
{
    NoLDDE *temp,*aux;
    unsigned int ret=FRACASSO;

    if( p->refMovel != NULL && posLog > 0) {
        if( (temp = (NoLDDE*) malloc(sizeof(NoLDDE))) != NULL ) {
            if((temp->dados = (void *) malloc(p->tamInfo)) != NULL) {
                memcpy(temp->dados, novo, p->tamInfo);
                if(posLog == 1){
                    return insereNoInicio(p,novo);
                }
                else if(posLog == p->tamLista+1){
                    return insereNoFim(p,novo);
                }
                else if(posLog > p->tamLista+1){
                    return FRACASSO;
                }
                else{
                    if(posLog != p->PosLogAtual){
                        while(posLog!=p->PosLogAtual){ //enquanto nao estiver na posicao
                            if(posLog<(p->PosLogAtual)){ //esta antes do refmovel
                                p->refMovel = p->refMovel->ant;
                                p->PosLogAtual--;
                            }
                            else if(posLog> (p->PosLogAtual)){ //esta depois do refmovel
                                p->refMovel = p->refMovel->prox;
                                p->PosLogAtual++;
                            }
                            else
                                break;
                        }
                    }
                    //esta na posicao
                    aux = p->refMovel->ant;
                    temp->prox = p->refMovel;
                    temp->ant = aux;
                    p->refMovel->ant = temp;
                    aux->prox = temp;
                    p->refMovel = temp;
                    p->tamLista++;
                    ret = SUCESSO;
                }
            }
            else {
                free(temp);
            }
        }
    }

    return ret;
}

/*************** REMOVE DO INICIO ***************/
int removeDoInicio(LDDE *p, void *reg)
{
    int ret = FRACASSO;

    if(p->refMovel != NULL) {
        while(p->refMovel->ant!=NULL){
            p->refMovel = p->refMovel->ant;
            p->PosLogAtual--;
        } //esta na posicao 1
        memcpy(reg, p->refMovel->dados, p->tamInfo);
        free(p->refMovel->dados);
        if(p->refMovel->prox == NULL) { //1 elemento so
            free(p->refMovel);
            p->refMovel = NULL;
        }
        else { //pelo menos 2 elementos
            p->refMovel = p->refMovel->prox;
            free(p->refMovel->ant->dados);
            free(p->refMovel->ant);
            p->refMovel->ant = NULL;
        }
        p->tamLista--;
        ret = SUCESSO;
    }

    return ret;
}


/*************** REMOVE DO FINAL ***************/
int removeDoFim(LDDE *p, void *reg)
{
    NoLDDE *aux;
    int ret = FRACASSO;

    if(p->refMovel != NULL) {
        while(p->refMovel->prox != NULL) {
            p->refMovel = p->refMovel->prox;
            p->PosLogAtual++;
        } //esta na ultima posicao
        memcpy(reg, p->refMovel->dados, p->tamInfo);
        free(p->refMovel->dados);
        if(p->refMovel->ant != NULL) {
            aux=p->refMovel->ant;
            aux->prox=NULL;
            p->PosLogAtual--;
            p->refMovel=aux;
        }
        else {
            free(p->refMovel);
            p->refMovel = NULL;
            p->PosLogAtual--;
        }
        p->tamLista--;
        ret = SUCESSO;
    }

    return ret;

}

/*************** REMOVE DA POSI��O L�GICA ***************/
int removeDaPosLog(LDDE *p, void *reg, unsigned int posLog)
{
    NoLDDE *pos,*aux;
    unsigned int ret = FRACASSO;

    if( p->refMovel != NULL && posLog > 0) {
        if(posLog==1){
            return removeDoInicio(p,reg);
        }
        else if(posLog==p->tamLista){
            return removeDoFim(p,reg);
        }
        else if(posLog>p->tamLista)
            return FRACASSO;
        else {                              //nao esta na posicao
            while(posLog!=p->PosLogAtual){
                if(posLog<p->PosLogAtual){ //esta antes da posicao atual
                    p->refMovel=p->refMovel->ant;
                    p->PosLogAtual--;
                }
                else if(posLog>p->PosLogAtual){ //esta depois da posicao atual
                    p->refMovel=p->refMovel->prox;
                    p->PosLogAtual++;
                }
                else
                    break;
            }
            //esta na posicao atual
            pos = p->refMovel;
            memcpy(reg,pos->dados,p->tamInfo);
            free(p->refMovel->dados);
            pos->ant->prox = pos->prox;
            pos->prox->ant = pos->ant;
            pos=pos->prox;
            free(p->refMovel);
            p->refMovel=pos;
            ret = SUCESSO;
        }
        p->tamLista--;
    }

    return ret;
}

/*************** BUSCA NO INICIO ***************/
int buscaNoInicio(LDDE *p, void *reg)
{
    int ret = FRACASSO;

    if(p->refMovel != NULL) {
        while(p->refMovel->ant!=NULL){
            p->refMovel=p->refMovel->ant;
            p->PosLogAtual--;
        }
        memcpy(reg, p->refMovel->dados, p->tamInfo);
        ret = SUCESSO;
    }

    return ret;
}

/*************** BUSCA NO FINAL ***************/
int buscaNoFim(LDDE *p, void *reg)
{
    NoLDDE *aux;
    int ret = FRACASSO;
    if(p->refMovel != NULL) {
        while(p->refMovel->prox!=NULL) {
            p->refMovel = p->refMovel->prox;
            p->PosLogAtual++;
        }
        memcpy(reg, p->refMovel->dados, p->tamInfo);
        ret = SUCESSO;
    }

    return ret;
}

/*************** BUSCA NA POSI��O L�GICA ***************/
int buscaNaPosLog(LDDE *p, void *reg, unsigned int posLog)
{
    int ret = FRACASSO;

    if(p->refMovel != NULL && posLog > 0) {
        if(posLog==1){
            return buscaNoInicio(p,reg);
        }
        else if(posLog==p->tamLista){
            return buscaNoFim(p,reg);
        }
        else if(posLog==p->PosLogAtual){
            memcpy(reg,p->refMovel->dados,p->tamInfo);
            ret = SUCESSO;
        }
        else{
            while(posLog!=p->PosLogAtual) {
                if(posLog<p->PosLogAtual){ //esta antes da posiçao atual
                    p->refMovel = p->refMovel->ant;
                    p->PosLogAtual--;
                }
                else if(posLog>p->PosLogAtual){ //esta depois da posiçao atual
                    p->refMovel = p->refMovel->prox;
                    p->PosLogAtual++;
                }
                else
                    break;
            }
            //esta na posicao
            memcpy(reg,p->refMovel->dados,p->tamInfo);
            ret = SUCESSO;
        }
    }

    return ret;
}

/*************** TAMANHO ***************/
int tamanho(LDDE *p)
{
    return p->tamLista;
}

/*************** PURGA ***************/
void reinicia(LDDE *p)
{
    if(p->refMovel != NULL) {
        while(p->refMovel->ant!=NULL){
            p->refMovel=p->refMovel->ant;
        }
        while(p->refMovel->prox != NULL) {
            p->refMovel = p->refMovel->prox;
            free(p->refMovel->ant->dados);
            free(p->refMovel->ant);
        }
        free(p->refMovel->dados);
        free(p->refMovel);
        p->refMovel = NULL;
        p->PosLogAtual=0;
        p->tamLista=0;
    }
}

/*************** DESTROI ***************/
void destroi(LDDE **pp)
{
    reinicia(*pp);
    free(*pp);
    (*pp) = NULL;
}

/*
int enderecosFisicos(LDDE *p)
{
    NoLDDE *aux;

    if(p->refMovel != NULL) {
        printf("tamanho de cada no: %u bytes\n", p->tamInfo);
        aux = p->refMovel;
        while(aux != NULL) {
            printf("endereco %X \n", (unsigned int)aux);
            aux = aux->prox;
        }
        if(p->refMovel->prox) {
            printf("distancia entre nos: %X \n", p->refMovel - p->refMovel->prox);
        }

        return SUCESSO;
    }

    return FRACASSO;
}
*/


