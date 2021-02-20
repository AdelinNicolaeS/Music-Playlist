// Copyright 311CA STANCA ADELIN-NICOLAE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./playlist.h"

#define NMAX 1000

int main(int argc, char *argv[]) {
  int q, i;
  FILE *fin = fopen(argv[argc - 2], "rb");
  FILE *fout = fopen(argv[argc - 1], "wb");
  char command[NMAX], buffer[NMAX], *song, *saveptr;
  struct DoubleLinkedList *list;
  list = malloc(sizeof(struct DoubleLinkedList));
  init_list(list);
  fscanf(fin, "%d\n", &q);
  for (i = 0; i < q; i++) {
    fgets(buffer, NMAX, fin);
    buffer[strlen(buffer) - 1] = '\0';
    song = strtok_r(buffer, " ", &saveptr);
    snprintf(command, sizeof(command), "%s", song);
    song = strtok_r(NULL, "\n", &saveptr);
    if (strcmp(command, "ADD_FIRST") == 0) {
      add_first(list, song, fout);
    }
    if (strcmp(command, "ADD_LAST") == 0) {
      add_last(list, song, fout);
    }
    if (strcmp(command, "ADD_AFTER") == 0) {
      add_after(list, song, fout);
    }
    if (strcmp(command, "DEL_FIRST") == 0) {
      del_first(list, fout);
    }
    if (strcmp(command, "DEL_LAST") == 0) {
      del_last(list, fout);
    }
    if (strcmp(command, "DEL_CURR") == 0) {
      del_curr(list, fout);
    }
    if (strcmp(command, "DEL_SONG") == 0) {
      del_song(list, song, fout);
    }
    if (strcmp(command, "MOVE_NEXT") == 0) {
      move_next(list, fout);
    }
    if (strcmp(command, "MOVE_PREV") == 0) {
      move_prev(list, fout);
    }
    if (strcmp(command, "SHOW_FIRST") == 0) {
      show_first(list, fout);
    }
    if (strcmp(command, "SHOW_LAST") == 0) {
      show_last(list, fout);
    }
    if (strcmp(command, "SHOW_CURR") == 0) {
      show_curr(list, fout);
    }
    if (strcmp(command, "SHOW_PLAYLIST") == 0) {
      show_playlist(list, fout);
    }
  }
  fclose(fin);
  fclose(fout);
  free_list(&list);
  return 0;
}
