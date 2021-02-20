// Copyright 311CA STANCA ADELIN-NICOLAE
#ifndef PLAYLIST_H__
#define PLAYLIST_H__

struct data {
  char title[100];
  char artist[100];
  char album[100];
  char year[100];
};

struct Node {
  struct Node *prev;
  struct Node *next;
  struct data info;
};

struct DoubleLinkedList {
  struct Node *head;
  struct Node *tail;
  struct Node *cursor;
  int size;
};

void init_list(struct DoubleLinkedList *list);

void move_next(struct DoubleLinkedList *list, FILE *fout);

void move_prev(struct DoubleLinkedList *list, FILE *fout);

void del_first(struct DoubleLinkedList *list, FILE *fout);

void del_last(struct DoubleLinkedList *list, FILE *fout);

void del_curr(struct DoubleLinkedList *list, FILE *fout);

void del_song(struct DoubleLinkedList *list, char *song, FILE *fout);

void show_first(struct DoubleLinkedList *list, FILE *fout);

void show_last(struct DoubleLinkedList *list, FILE *fout);

void show_curr(struct DoubleLinkedList *list, FILE *fout);

void show_playlist(struct DoubleLinkedList *list, FILE *fout);

void create_node(struct Node **p, char* song);

void add_first(struct DoubleLinkedList *list, char *song, FILE *fout);

void add_last(struct DoubleLinkedList *list, char *song, FILE *fout);

void add_after(struct DoubleLinkedList *list, char *song, FILE *fout);

void free_list(struct DoubleLinkedList **plist);

#endif  //  PLAYLIST_H__
