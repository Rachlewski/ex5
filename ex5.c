#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Song {
    char *title;
    char *artist;
    int year;
    char *lyrics;
    int streams;
} Song;

typedef struct Playlist {
    char *name;
    Song **songs;
    int songsNum;
} Playlist;

//linked list of playlists
typedef struct PlaylistsNode {
    Playlist playlist;
    struct PlaylistsNode *next;
} PlaylistNode;

//some recommendations for functions, you may implement however you want

void deleteSong() {
    //todo
    printf("Song deleted successfully.\n");
}


void playSong(Playlist *plqaylist, int songIndex) {
    //todo
}


void freeSong(Song *song) {
    free(song->title);
    free(song->artist);
    free(song->lyrics);
    free(song);
}

void freePlaylist(Playlist *playlist, int songsSize) {
    free(playlist->name);
    for (int i = 0; i < songsSize; i++) {
        freeSong(playlist->songs[i]);
    }
    free(playlist->songs);
    free(playlist);
}

char* readLine() {
    //clean the buffer
    scanf("%*[^\n]");
    scanf("%*c");
    //initial array size
    int size = 10;
    char *buffer = (char *)malloc(size * sizeof(char));
    if (!buffer) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    int i = 0;
    char c;
    while (scanf("%c", &c) == 1 && c != '\n') {
        // Check if buffer needs to be resized
        if (i + 1 >= size) {
            size += 10;
            char *newBuffer = (char *)realloc(buffer, size * sizeof(char));
            if (!newBuffer) {
                printf("Memory reallocation failed.\n");
                exit(1);
            }
            buffer = newBuffer;
        }

        buffer[i++] = c;
    }
    // end the string
    buffer[i] = '\0';

    return buffer;
}

void printPlaylistsMenu() {
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
}


void sortPlaylist() {
    //todo
    printf("sorted\n");
}


void watchPlaylists(PlaylistNode ** playlists) {
    printf("Choose a playlist;\n");
    PlaylistNode *currentPlaylist = *playlists;
    int count = 1;
    while (currentPlaylist != NULL) {
        printf("\t%d. %s\n", count++, currentPlaylist->playlist.name);
        currentPlaylist = currentPlaylist->next;
    }
    printf("\t%d. %s\n", count, "Back to main menu");
}

void addPlaylist(PlaylistNode ** playlists){
    PlaylistNode *playlistNode = (PlaylistNode *)malloc(sizeof(PlaylistNode));
    playlistNode->next = NULL;
    printf("Enter playlist's name:\n");
    playlistNode->playlist.name = readLine();
    if (*playlists == NULL) {
        *playlists = playlistNode;
    } else {
        PlaylistNode *currentNode = *playlists;
        while (currentNode->next != NULL) {
            currentNode = currentNode->next;
        }
        currentNode->next = playlistNode;
    }
}

void removePlaylist(void) {
    //todo
}


int main() {
    int choice = 0;
    PlaylistNode *playLists = NULL;
    while (choice != 4) {
        printPlaylistsMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                watchPlaylists(&playLists);
                break;
            case 2:
                addPlaylist(&playLists);
                break;
            case 3:
                removePlaylist();
        }
    }
    printf("Goodbye!\n");
}
