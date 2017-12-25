#include "saver.h"

#include <iostream>
#include <fstream>
#include <sstream>

void saveLevelBinary(std::vector<Chunk>& v)
{
	std::string fileName;
	std::cout << "Enter file name:\n";
	std::cin >> fileName;
	fileName = "levels/" + fileName + ".lvl";

	FILE * file;
	file = fopen(fileName.c_str(), "wb");

	int n = v.size(),
		s = CHUNK_SIDE;

	fwrite(&n, sizeof(int), 1, file);
	fwrite(&s, sizeof(int), 1, file);

	for (size_t i = 0; i < v.size(); i++)
		fwrite(v[i].tiles, sizeof(uint8_t), CHUNK_SIZE, file);

	fclose(file);
}

void loadLevelBinary(std::vector<Chunk>& v)
{
	std::string fileName;
	std::cout << "Enter file name:\n";
	std::cin >> fileName;
	fileName = "levels/" + fileName + ".lvl";

	FILE * file;
	file = fopen(fileName.c_str(), "rb");

	int n, s;

	fread(&n, sizeof(int), 1, file);
	fread(&s, sizeof(int), 1, file);

	std::cout << "n = " << n << "; s = " << s << ";\n";

	for(int i = 0; i < n; i++)
	{
		v.push_back(Chunk());
		fread(v.back().tiles, sizeof(uint8_t), s*s, file);
	}

	fclose(file);
}

void saveLevelJson(std::vector<Chunk>& v)
{
	std::string fileName;
	std::cout << "Enter file name:\n";
	std::cin >> fileName;
	fileName += ".json";

	std::ofstream file;
	file.open("levels/" + fileName);
	file << JsonChunks(v);
	file.close();
	std::cout << "Saved level under name " << fileName << '\n';
}

std::string makeJsonArray(uint8_t * arr, int count)
{
	std::string s;
	s += "[";
	int i = 0;
	while(true)
	{
		s += std::to_string(arr[i++]);
		if (i == count)	break;
		s += ",";
	};
	s += "]";

	return s;
}

std::string JsonChunk(Chunk chunk)
{
	std::stringstream ss;
	ss << "[\n";

	int i = 0;
	while(true)
	{
		ss << makeJsonArray(chunk.tiles[i++], CHUNK_SIDE);
		if (i == CHUNK_SIDE) break;
		ss << ",\n";
	};

	ss << " ]";

	return ss.str();
}

std::string JsonChunks(std::vector<Chunk>& v)
{
	if(v.size() == 0) return "";

	std::stringstream ss;

	ss << "{\n\"chunks\":[\n";

	int i = 0;
	while(true)
	{
		ss << JsonChunk(v[i++]);
		if (i == v.size()) break;
		ss << ",\n";
	}

	ss << "\n]\n}";

	return ss.str();
}
