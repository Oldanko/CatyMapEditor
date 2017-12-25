#ifndef SAVER_H
#define SAVER_H

#include <vector>
#include <string>
#include "level.h"

void saveLevelJson(std::vector<Chunk>& v);

void saveLevelBinary(std::vector<Chunk>& v);
void loadLevelBinary(std::vector<Chunk>& v);

std::string makeJsonArray(uint8_t * arr, int count);
std::string JsonChunk(Chunk chunk);
std::string JsonChunks(std::vector<Chunk>& v);

#endif
