#include "boardio.h"

#include <stdio.h>

// Read the whole file info a buffer located on the arena
// This function was written by Kaiden Kaine in december of 2023
// I have gotten permission to use it.
U8 *LoadFileDataArena(Arena *arena, const char *filepath, U32 *bytes_read) {
  FILE *fp = fopen(filepath, "rb");
  
  if (fp) {
    fseek(fp, 0, SEEK_END);
    *bytes_read = ftell(fp);
    rewind(fp);

    U8 *buffer = ArenaPushNoZero(arena, *bytes_read);
    fread(buffer, *bytes_read, 1, fp);

    fclose(fp);

    return buffer;

  } else fprintf(stderr, "couldn't open file \"%s\"\n", filepath);

  return NULL;
}



// get bitboard of input file. make sure index
// is only either 0 (black pieces) or 1 (white pieces). 
BitBoard BitBoardFromFile(Arena *tempArena, const char* fileName) {
  BitBoard board = { 0 };

  // since this data is only temporaly needed
  // We can use the temp arena, after we are done
  // we can use TempArenaDeinit() to reset the 
  // arena to its original state.
  TempArena temp = TempArenaInit(tempArena);

  U32 bytesRead = 0;
  U8 *buffer = LoadFileDataArena(tempArena, fileName, &bytesRead);

  U32 row = 0;
  U32 col = 0;
  for (U32 index = 0; index < bytesRead; index++) {
    if (buffer[index] == '\n') {
      col = 0;
      row += 1;
      continue;
    } 

    if (buffer[index] != 'O') board.rows[row] |= 1llu<<col;

    
    col += 1;
  }
  

  // After we are done allocations 
  // we can "free" all the memory 
  // that we used during our calculations
  TempArenaDeinit(temp);

  return board;
}

void BitBoardFilePrint(FILE *fp, BitBoard board) {
  for (U8 i = 0; i < 8; i++) {
    for (U8 j = 0; j < 8; j++) {
      U8 color = (i + j) % 2 ;
      char c = (board.rows[i] & (1<<j)) > 0;
      if (c) c = (color)? 'W': 'B';
      else c = 'O';
      putc(c, fp);
    }
    putc('\n', fp);
  }
}

void CoordOutputMove(Coord coord) {
  putchar('A' + coord.x);
  putchar('\n');
}

Coord CoordFromEnemyInput(void) {
  I8 x = getchar();
  I8 y = getchar();
  getchar();
  Coord coord;
  coord.x = x - 'A';
  coord.y = 8 - (y - '1');
  return coord;
}