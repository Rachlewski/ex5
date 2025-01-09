#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BY_YEAR 1
#define BY_STREAMS_ASC 2
#define BY_STREAMS_DESC 3
#define BY_NAME 4

//TODO: make sure to exit(1) on every failed malloc / calloc
typedef struct Song {
    char *title;
    char *artist;
    int year;
    char *lyrics;
    int streams;
} Song;

//linked list of songs
typedef struct SongsNode {
    Song song;
    struct SongsNode *next;
} SongsNode;

typedef struct Playlist {
    char *name;
    SongsNode *songs;
} Playlist;

//linked list of playlists
typedef struct PlaylistsNode {
    Playlist playlist;
    struct PlaylistsNode *next;
} PlaylistNode;


//some recommendations for functions, you may implement however you want
void showPlaylist(PlaylistNode *chosenPlaylist);

void watchPlaylists(PlaylistNode **playlists);

void freeSongNode(SongsNode *songsNode) {
    if (songsNode == NULL) {
        return;
    }
    if (songsNode->next == NULL) {
        free(songsNode->song.title);
        free(songsNode->song.artist);
        free(songsNode->song.lyrics);
        free(songsNode);
        return;
    }
    freeSongNode(songsNode->next);
    free(songsNode->song.title);
    free(songsNode->song.artist);
    free(songsNode->song.lyrics);
    free(songsNode);
}

void displaySongs(SongsNode **songs) {
    SongsNode *currentSong = *songs;
    int count = 1;
    while (currentSong != NULL) {
        printf("%d. Title: %s\n   Artist: %s\n   Released: %d\n   Streams: %d\n"
               , count, currentSong->song.title, currentSong->song.artist,
               currentSong->song.year, currentSong->song.streams);
        currentSong = currentSong->next;
    }
}

void deleteSong(SongsNode **songs) {
    int songNum;
    displaySongs(songs);
    printf("choose a song to delete, or 0 to quit:\n");
    scanf("%d", &songNum);
    SongsNode *toDelete = *songs;
    SongsNode *previous = NULL;
    for (int i = 1; i < songNum; i++) {
        previous = toDelete;
        toDelete = toDelete->next;
    }
    if (toDelete == NULL) {
        return;
    }
    if (previous == NULL) {
        *songs = toDelete->next;
    } else {
        previous->next = toDelete->next;
        toDelete->next = NULL;
    }
    freeSongNode(toDelete);
}

char* readLine() {
    //clean the buffer
    scanf("%*[\n\t ]");
    //initial array size
    int size = 10;
    char *buffer = (char *) malloc(size * sizeof(char));
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
            char *newBuffer = (char *) realloc(buffer, size * sizeof(char));
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

int compareSongs(SongsNode *current, int sortType) {
    switch (sortType) {
        case BY_YEAR:
            return current->song.year > current->next->song.year;
        case BY_STREAMS_ASC:
            return current->song.streams > current->next->song.streams;
        case BY_STREAMS_DESC:
            return current->song.streams >= current->next->song.streams;
        case BY_NAME:
            return strcmp(current->song.title, current->next->song.title);
        default:
    }
}

void sort(SongsNode **songs, int sortType) {
    if (*songs == NULL || (*songs)->next == NULL) {
        // Handle empty or single-node list
        return;
    }

    SongsNode *current;
    int swapped = 1;
    while (swapped) {
        swapped = 0;
        current = *songs;

        while (current->next != NULL) {
            if (compareSongs(current, sortType)) {
                // Swap nodes
                Song temp = current->song;
                current->song = current->next->song;
                current->next->song = temp;
                swapped = 1;
            }
            current = current->next;
        }
    }
}

void sortPlaylist(SongsNode **songs) {
    int sortType;
    printf("choose:\n"
        "1. sort by year\n"
        "2. sort by streams - ascending order\n"
        "3. sort by streams - descending order\n"
        "4. sort alphabetically\n");
    scanf(" %d", &sortType);
    sort(songs, sortType);
    printf("sorted\n");
}

void printSongsMenu(Playlist *playlist) {
    printf("playlist %s:\n\t"
           "1. Show Playlist\n\t"
           "2. Add Song\n\t"
           "3. Delete Song\n\t"
           "4. Sort\n\t"
           "5. Play\n\t"
           "6. exit\n\t",
           playlist->name);
}


void addPlaylist(PlaylistNode **playlists) {
    PlaylistNode *playlistNode = (PlaylistNode *) malloc(sizeof(PlaylistNode));
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

void freePlaylistNodes(PlaylistNode *playlistNode) {
    if (playlistNode == NULL) {
        return;
    }
    if (playlistNode->next == NULL) {
        freeSongNode(playlistNode->playlist.songs);
        free(playlistNode->playlist.name);
        free(playlistNode);
        return;
    }
    freePlaylistNodes(playlistNode->next);
    freeSongNode(playlistNode->playlist.songs);
    free(playlistNode->playlist.name);
    free(playlistNode);
}

void displayPlaylists(PlaylistNode **playlists) {
    printf("Choose a playlist:\n");
    PlaylistNode *currentPlaylist = *playlists;
    int count = 1;
    while (currentPlaylist != NULL) {
        printf("\t%d. %s\n", count++, currentPlaylist->playlist.name);
        currentPlaylist = currentPlaylist->next;
    }
    printf("\t%d. %s\n", count, "Back to main menu");
}

void removePlaylist(PlaylistNode **playlists) {
    int index;
    displayPlaylists(playlists);
    scanf(" %d", &index);
    PlaylistNode *toDelete = *playlists;
    PlaylistNode *previous = NULL;
    for (int i = 1; i < index; i++) {
        previous = toDelete;
        toDelete = toDelete->next;
    }
    // don't delete
    if (toDelete == NULL) {
        return;
    }
    // delete the first node
    if (previous == NULL) {
        *playlists = toDelete->next;
        freePlaylistNodes(toDelete);
    } else {
        previous->next = toDelete->next;
        toDelete->next = NULL;
        freePlaylistNodes(toDelete);
    }
}


void addSong(SongsNode **songs) {
    SongsNode *songNode = (SongsNode *) malloc(sizeof(SongsNode));
    songNode->next = NULL;
    printf("Enter song's details\nTitle:\n");
    songNode->song.title = readLine();
    printf("Artist:\n");
    songNode->song.artist = readLine();
    printf("Year of release:\n");
    scanf(" %d", &songNode->song.year);
    printf("Lyrics:\n");
    songNode->song.lyrics = readLine();
    songNode->song.streams = 0;
    if (*songs == NULL) {
        *songs = songNode;
    } else {
        SongsNode *currentNode = *songs;
        while (currentNode->next != NULL) {
            currentNode = currentNode->next;
        }
        currentNode->next = songNode;
    }
}

// plays a song of the users choice
void playSong(SongsNode **song, int toPlay) {
    SongsNode *currentNode = *song;
    for (int i = 1; i < toPlay; i++) {
        currentNode = currentNode->next;
    }
    printf("Now playing %s:\n$ %s $\n", (*song)->song.title, (*song)->song.lyrics);
    (*song)->song.streams++;
}

// plays the entire playlist
void play(SongsNode **songs) {
    int count = 1;
    SongsNode *currentSong = *songs;
    while (currentSong != NULL) {
        playSong(&currentSong, count);
        count++;
        currentSong = currentSong->next;
        printf("\n");
    }
}

// prints the menu after choosing a playlist and calls the function the user has chosen
void playlistMenu(PlaylistNode *chosenPlaylist) {
    int choice = 0;
    while (choice != 6) {
        printSongsMenu(&chosenPlaylist->playlist);
        scanf(" %d", &choice);
        switch (choice) {
            case 1:
                showPlaylist(chosenPlaylist);
                break;
            case 2:
                addSong(&chosenPlaylist->playlist.songs);
                break;
            case 3:
                deleteSong(&chosenPlaylist->playlist.songs);
                break;
            case 4:
                sortPlaylist(&chosenPlaylist->playlist.songs);
                break;
            case 5:
                play(&chosenPlaylist->playlist.songs);
                break;
            default:
        }
    }
}

// prints the songs in the playlist and plays a song if chosen
void showPlaylist(PlaylistNode *chosenPlaylist) {
    SongsNode *currentSong = chosenPlaylist->playlist.songs;
    int count = 1;
    // print details for each song
    while (currentSong != NULL) {
        printf("%d. Title: %s\n\tArtist: %s\n\tReleased: %d\n\tStreams: %d\n"
               , count, currentSong->song.title, currentSong->song.artist,
               currentSong->song.year, currentSong->song.streams);
        currentSong = currentSong->next;
    }
    int toPlay;
    printf("choose a song to play, or 0 to quit:\n");
    scanf("%d", &toPlay);
    if (chosenPlaylist->playlist.songs == NULL) {
        return;
    }
    // the user chose a song to play
    while (toPlay) {
        playSong(&chosenPlaylist->playlist.songs, toPlay);
        printf("choose a song to play, or 0 to quit:\n");
        scanf("%d", &toPlay);
    }
}

void watchPlaylists(PlaylistNode **playlists) {
    while (1) {
        displayPlaylists(playlists);
        int playlistNum;
        PlaylistNode *chosenPlaylist = *playlists;
        scanf("%d", &playlistNum);
        for (int i = 1; i < playlistNum; i++) {
            chosenPlaylist = chosenPlaylist->next;
        }
        // there are no playlists
        if (chosenPlaylist == NULL) {
            return;
        }
        playlistMenu(chosenPlaylist);
    }
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
                removePlaylist(&playLists);
            default:
        }
    }

    freePlaylistNodes(playLists);
    printf("Goodbye!\n");
}
