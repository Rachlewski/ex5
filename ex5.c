#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct Song {
    char* title;
    char* artist;
    int year;
    char* lyrics;
    int streams;
} Song;

typedef struct Playlist {
    char* name;
    Song** songs;
    int songsNum;
} Playlist;

//some recommendations for functions, you may implement however you want

void deleteSong() {
//todo
    printf("Song deleted successfully.\n");
}



void playSong(Playlist* plqaylist, int songIndex) {
  //todo
}



void freeSong(Song* song) {
    free (song->title);
    free (song->artist);
    free (song->lyrics);
    free (song);
}

void freePlaylist(Playlist* playlist, int songsSize){
    free(playlist->name);
    for (int i =0; i < songsSize; i++){
      freeSong(playlist->songs[i]);
    }
    free(playlist->songs);
    free(playlist);
}

void printPlaylistsMenu() {
    printf("Please Choose:\n"); 
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");   
}



void sortPlaylist() {
    //todo
    printf("sorted\n");
}


void watchPlaylists(void) {
    //TODO
}

void addPlaylist(void) {
    //todo
}

void removePlaylist(void) {
    //todo
}

int main() {
  int choice =0;
  while(choice != 4){
      printPlaylistsMenu();
      scanf("%d", &choice);
      switch (choice) {
        case 1:
          watchPlaylists();
          break;
        case 2:
            addPlaylist();
            break;
        case 3:
            removePlaylist();
      }
    }
    printf("Goodbye!\n");
}
