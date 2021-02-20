// Copyright 311CA STANCA ADELIN-NICOLAE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./playlist.h"

// Initializam cele 3 noduri importante ale listei, precum si dimensiunea
void init_list(struct DoubleLinkedList *list) {
  list->head = NULL;
  list->tail = NULL;
  list->cursor = NULL;
  list->size = 0;
}
void move_next(struct DoubleLinkedList *list, FILE *fout) {
  if (list->cursor != NULL && list->cursor->next != NULL) {
    list->cursor = list->cursor->next;
  } else if (list->cursor == NULL) {
    fprintf(fout, "Error: no track playing\n");
  }
}
void move_prev(struct DoubleLinkedList *list, FILE *fout) {
  if (list->cursor != NULL && list->cursor->prev != NULL) {
    list->cursor = list->cursor->prev;
  } else if (list->cursor == NULL) {
    fprintf(fout, "Error: no track playing\n");
  }
}
void del_first(struct DoubleLinkedList *list, FILE *fout) {
  if (list->size == 0) {
    fprintf(fout, "Error: delete from empty playlist\n");
  } else if (list->size == 1) {
    list->size--;
    struct Node *q = list->head;
    list->head = NULL;
    list->tail = NULL;
    list->cursor = NULL;
    free(q);
  } else {
    if (list->cursor == list->head) {
      move_next(list, fout);
    }
    struct Node *q = list->head;
    list->head = list->head->next;
    list->head->prev = NULL;
    list->size--;
    free(q);
  }
}
void del_last(struct DoubleLinkedList *list, FILE *fout) {
  if (list->size == 0) {
    fprintf(fout, "Error: delete from empty playlist\n");
  } else if (list->size == 1) {
    struct Node *q = list->tail;
    list->size--;
    list->head = NULL;
    list->tail = NULL;
    list->cursor = NULL;
    free(q);
  } else {
    if (list->cursor == list->tail) {
      move_prev(list, fout);
    }
    struct Node *q = list->tail;
    list->tail = list->tail->prev;
    list->tail->next = NULL;
    list->size--;
    free(q);
  }
}
void del_curr(struct DoubleLinkedList *list, FILE *fout) {
  if (list->cursor == NULL || list->size == 0) {
    fprintf(fout, "Error: no track playing\n");
  } else if (list->cursor == list->head) {
    del_first(list, fout);
  } else if (list->cursor == list->tail) {
    del_last(list, fout);
  } else {
    struct Node *q = list->cursor;
    list->cursor->prev->next = list->cursor->next;
    list->cursor->next->prev = list->cursor->prev;
    list->cursor = list->cursor->next;
    list->size--;
    free(q);
  }
}
void del_song(struct DoubleLinkedList *list, char *song, FILE *fout) {
  if (list == NULL || list->size == 0) {
    fprintf(fout, "Error: no song found to delete\n");
  } else {
    struct Node *p = list->head;
    int ok = 0;
    char *artist, *name, *saveptr;
    artist = strtok_r(song, "-", &saveptr);
    name = strtok_r(NULL, "\n", &saveptr);
    name++;
    name[strlen(name) - 4] = '\0';
    artist[strlen(artist) - 1] = '\0';
    // verificam daca melodia se afla in playlist
    struct Node *parc = list->head;
    while (ok == 0 && parc != NULL) {
      if (strcmp(parc->info.title, name) == 0) {
        ok = 1;
        p = parc;
      }
      parc = parc->next;
    }
    if (ok == 0) {
      fprintf(fout, "Error: no song found to delete\n");
    } else {
      if (p == list->cursor) {
        del_curr(list, fout);
      } else if (p == list->head) {
        del_first(list, fout);
      } else if (p == list->tail) {
        del_last(list, fout);
      } else {
        p->next->prev = p->prev;
        p->prev->next = p->next;
        free(p);
      }
    }
  }
}
void show_first(struct DoubleLinkedList *list, FILE *fout) {
  if (list == NULL || list->size == 0) {
    fprintf(fout, "Error: show empty playlist\n");
  } else {
    fprintf(fout, "Title: %s\n", list->head->info.title);
    fprintf(fout, "Artist: %s\n", list->head->info.artist);
    fprintf(fout, "Album: %s\n", list->head->info.album);
    fprintf(fout, "Year: %s\n", list->head->info.year);
  }
}
void show_last(struct DoubleLinkedList *list, FILE *fout) {
  if (list == NULL || list->size == 0) {
    fprintf(fout, "Error: show empty playlist\n");
  } else {
    fprintf(fout, "Title: %s\n", list->tail->info.title);
    fprintf(fout, "Artist: %s\n", list->tail->info.artist);
    fprintf(fout, "Album: %s\n", list->tail->info.album);
    fprintf(fout, "Year: %s\n", list->tail->info.year);
  }
}
void show_curr(struct DoubleLinkedList *list, FILE *fout) {
  if (list == NULL || list->size == 0) {
    fprintf(fout, "Error: show empty playlist\n");
  } else {
    fprintf(fout, "Title: %s\n", list->cursor->info.title);
    fprintf(fout, "Artist: %s\n", list->cursor->info.artist);
    fprintf(fout, "Album: %s\n", list->cursor->info.album);
    fprintf(fout, "Year: %s\n", list->cursor->info.year);
  }
}
void show_playlist(struct DoubleLinkedList *list, FILE *fout) {
  fprintf(fout, "[");
  if (list->size == 1) {
    fprintf(fout, "%s", list->head->info.title);
  } else if (list->size > 1) {
    struct Node *it = list->head;
    for (it = list->head; it != list->tail; it = it->next) {
      fprintf(fout, "%s; ", it->info.title);
    }
    fprintf(fout, "%s", list->tail->info.title);
  }
  fprintf(fout, "]\n");
}
// Functia aloca memorie si initializeaza nodul ce urmeaza sa fie adaugat
void create_node(struct Node **p, char* song) {
  char fisier[1000];
  snprintf(fisier, sizeof(fisier), "./songs/");
  int i = strlen(fisier);
  unsigned int j;
  for (j = 0; j < strlen(song); j++, i++) {
    fisier[i] = song[j];
  }
  fisier[i] = '\0';
  FILE *fin = fopen(fisier, "rb");
  *p = malloc(sizeof(struct Node));
  memset(*p, 0, sizeof(struct Node));
  (*p)->next = NULL;
  (*p)->prev = NULL;
  fseek(fin, -94, SEEK_END);
  fread((*p)->info.title, 30, 1, fin);
  fseek(fin, -64, SEEK_END);
  fread((*p)->info.artist, 30, 1, fin);
  fseek(fin, -34, SEEK_END);
  fread((*p)->info.album, 30, 1, fin);
  fseek(fin, -4, SEEK_END);
  fread((*p)->info.year, 4, 1, fin);
  fclose(fin);
}
void add_first(struct DoubleLinkedList *list, char *song, FILE *fout) {
  struct Node *p = NULL;
  create_node(&p, song);
  if (list->size == 0) {
    list->head = p;
    list->tail = p;
    list->cursor = p;
    list->size++;
  } else {
    int ok = 0;
    struct Node *parc = list->head, *it;
    while (ok == 0 && parc != NULL) {
      if (strcmp(parc->info.title, p->info.title) == 0) {
        ok = 1;
        it = parc;
      }
      parc = parc->next;
    }
    // daca melodia se afla deja in playlist, o stergem
    if (ok == 1) {
      if (list->size == 1) {
        struct Node *q = list->head;
        free(q);
        list->head = NULL;
        list->cursor = NULL;
        list->tail = NULL;
        list->size--;
      } else {
        if (it == list->head) {
          del_first(list, fout);
        } else if (it == list->tail) {
          del_last(list, fout);
        } else {
          if (it == list->cursor) {
            move_next(list, fout);
          }
          it->prev->next = it->next;
          it->next->prev = it->prev;
          free(it);
          list->size--;
        }
      }
    }
    // adaugam melodia pe pozitia corespunzatoare
    p->next = list->head;
    list->head->prev = p;
    list->size++;
    list->head = p;
  }
}
void add_last(struct DoubleLinkedList *list, char *song, FILE *fout) {
  struct Node *p = NULL;
  create_node(&p, song);
  if (list->size == 0) {
    list->head = p;
    list->tail = p;
    list->cursor = p;
    list->size++;
  } else {
    int ok = 0;
    struct Node *parc = list->head, *it;
    while (ok == 0 && parc != NULL) {
      if (strcmp(parc->info.title, p->info.title) == 0) {
        ok = 1;
        it = parc;
      }
      parc = parc->next;
    }
    // daca melodia era deja in MP3, o stergem
    if (ok == 1) {
      if (list->size == 1) {
        struct Node *q = list->head;
        free(q);
        list->head = NULL;
        list->cursor = NULL;
        list->tail = NULL;
        list->size--;
      } else {
        if (it == list->tail) {
          del_last(list, fout);
        } else if (it == list->head) {
          del_first(list, fout);
        } else {
          if (it == list->cursor) {
            move_next(list, fout);
          }
          it->prev->next = it->next;
          it->next->prev = it->prev;
          free(it);
          list->size--;
        }
      }
    }
    // adaugare efectiva in lista
    list->tail->next = p;
    p->prev = list->tail;
    list->tail = p;
    list->size++;
  }
}
void add_after(struct DoubleLinkedList *list, char *song, FILE *fout) {
  struct Node *p = NULL;
  create_node(&p, song);
  if (list->cursor != NULL &&
      strcmp(list->cursor->info.title, p->info.title) != 0) {
    int ok = 0;
    struct Node *parc = list->head, *it;
    while (ok == 0 && parc != NULL) {
      if (strcmp(parc->info.title, p->info.title) == 0) {
        ok = 1;
        it = parc;
      }
      parc = parc->next;
    }
    if (ok == 1) {
      if (list->size == 1) {
        struct Node *q = list->head;
        free(q);
        list->head = NULL;
        list->cursor = NULL;
        list->tail = NULL;
        list->size--;
      } else {
        if (it == list->head) {
          del_first(list, fout);
        } else if (it == list->tail) {
          del_last(list, fout);
        } else {
          it->next->prev = it->prev;
          it->prev->next = it->next;
          free(it);
          list->size--;
        }
      }
    }
    if (list->cursor == list->tail) {
      p->prev = list->tail;
      list->tail->next = p;
      p->next = NULL;
      list->tail = p;
    } else {
      list->cursor->next->prev = p;
      p->next = list->cursor->next;
      list->cursor->next = p;
      p->prev = list->cursor;
    }
    list->size++;
  } else {
    free(p);
  }
}
// Functia elibereaza memoria pentru fiecare nod si memoria aferenta listei
void free_list(struct DoubleLinkedList **plist) {
  struct DoubleLinkedList *list = *plist;
  struct Node *it = list->head;
  struct Node *tmp;
  while (it != NULL) {
    tmp = it;
    it = it->next;
    free(tmp);
  }
  free(list);
}
