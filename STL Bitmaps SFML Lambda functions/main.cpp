/******************************
 * Name: Amir Tadros          *
 * Assignment # 6             *
 * Compiler: CodeBlocks 20.03 *
 * OS: Windows 10             *
 *****************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <bitset>

 // SFML Specific
#include"SFML\System.hpp"
#include"SFML\Window.hpp"
#include"SFML\Graphics.hpp"

const std::string DATA_PATH = "ass6data.bin";

auto countItems() // step 1
{
	std::fstream fin(DATA_PATH, std::ios::in | std::ios::binary | std::ios::ate);

	auto ret = fin.tellg() / sizeof(std::pair<unsigned, unsigned>);

	fin.close();

	return ret;
}

std::string hexFormat(std::size_t hexNum)
{
	std::stringstream ss;
	ss << std::hex << "0x" << hexNum;
	return ss.str();
}

void frontAndbackFile()	// step 2
{
	std::fstream fin(DATA_PATH, std::ios::in | std::ios::binary);

	auto output = [](std::fstream& fin) {
		for (int i = 0; i < 5; i++) {
			std::pair<unsigned, unsigned> temp;
			fin.read(reinterpret_cast<char*>(&temp), sizeof(temp));
			std::cout << std::setw(12) << std::right << hexFormat(temp.first);
			std::cout << std::setw(12) << std::right << hexFormat(temp.second) << '\n';
		}
	};

	output(fin);
	fin.seekg(std::streamoff(-5LL * static_cast<long long>(sizeof(std::pair<unsigned, unsigned>))), fin.end); // go to 5 pairs before the end
	output(fin);

	fin.close();
}

auto dataRead(std::map<unsigned, unsigned>& dataMap) // step 3
{
	std::fstream fin(DATA_PATH, std::ios::in | std::ios::binary);

	if (!fin)
	{
		std::cout << "Failed to open data file";
		exit(EXIT_FAILURE);
	}

	std::map<unsigned, unsigned> ret;
	std::pair<unsigned, unsigned> temp;
	while (fin.read(reinterpret_cast<char*>(&temp), sizeof(temp)))
		dataMap[temp.second] = temp.first;

	fin.close();
}

void frontAndbackData(const std::map<unsigned, unsigned> dataMap) // step 4
{
	auto output = [](auto it) {
		std::cout << std::setw(12) << std::right << hexFormat(it.first);
		std::cout << std::setw(12) << std::right << hexFormat(it.second) << '\n';
	};

	auto it = dataMap.cbegin();
	std::advance(it, 5); // 5 elements from the first elememnt
	std::for_each(dataMap.cbegin(), it, output);
	std::advance(it, dataMap.size() - 10); // 5 elements before the last element
	std::for_each(it, --dataMap.cbegin(), output);
}

void map2set(const std::map<unsigned, unsigned> dataMap, std::vector<std::bitset<32>>& dataSet) // step 5
{
	std::for_each(dataMap.cbegin(), dataMap.cend(), [&](auto element) { dataSet.push_back(element.second); });
}

void setRange(const std::vector<std::bitset<32>>& dataSet) // step 6 / 7.2
{
	auto itBegin = dataSet.cbegin();
	std::advance(itBegin, 5005);
	auto itEnd = itBegin;
	std::advance(itEnd, 5);
	std::for_each(itBegin, itEnd, [](auto element) { std::cout << element << '\n'; });
}

void flipBits(std::vector<std::bitset<32>>& dataSet) // step 7.1
{
	std::for_each(dataSet.begin(), dataSet.end(), [](auto& elemenet) { elemenet.flip(); });
}

void setToInt(std::vector<std::bitset<32>>& dataSet) // step 8
{
	auto itBegin = dataSet.cbegin();
	std::advance(itBegin, 5005);
	auto itEnd = itBegin;
	std::advance(itEnd, 5);
	std::for_each(itBegin, itEnd, [](auto element) { std::cout << std::hex << "0x" << element.to_ulong() << '\n'; });
}

void initializeImage(sf::Image& image, std::vector<std::bitset<32>> dataSet) // step 9 / 10
{
	image.create(100, 100); // step 9
	for (auto i = 0; i < 100; i++)
		for (auto j = 0; j < 100; j++)
		{
			auto pos = j + i * 100;
			image.setPixel(j, i, sf::Color(dataSet[pos].to_ulong())); // step 10
		}
}

void initializeTexture(sf::Texture& texture, sf::Image image) // step 11
{
	texture.loadFromImage(image);
}

void initializeSprite(sf::Sprite& sprite, sf::Texture& texture) // step 12
{
	sprite.setTexture(texture);
	sprite.setScale(3, 3);
}

void updateSFMLEvents(sf::RenderWindow& renderWindow)
{
	sf::Event event;
	while (renderWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case(sf::Event::Closed):
			renderWindow.close();
		default:
			break;
		}
	}
}

void render(sf::RenderWindow& renderWindow, sf::Sprite sprite) // step 13
{
	renderWindow.clear();
	renderWindow.draw(sprite);
	renderWindow.display();
}

int main()
{
	std::cout << "Number of records = " << countItems() << "\n"; // step 1
	std::cout << "\nOriginal input data\n";
	frontAndbackFile(); // step 2
	std::map<unsigned, unsigned> dataMap;
	dataRead(dataMap); // step 3
	std::cout << "\nMap Data\n";
	frontAndbackData(dataMap); // step 4
	std::vector<std::bitset<32>> dataSet;
	map2set(dataMap, dataSet); // step 5
	std::cout << "\nVector data (records 5005 to 5010)\n";
	setRange(dataSet); // step 6
	flipBits(dataSet); // step 7.1
	std::cout << "\nVector data after bit flip\n";
	setRange(dataSet); // step 7.2
	std::cout << "\nVector data as unsigned\n";
	setToInt(dataSet); // step 8

	sf::RenderWindow renderWindow(sf::VideoMode(300, 300), "Assignment 6");
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;

	initializeImage(image, dataSet); // step 9 / 10
	initializeTexture(texture, image); // step 11
	initializeSprite(sprite, texture); // step 12

	while (renderWindow.isOpen())
	{
		updateSFMLEvents(renderWindow);
		render(renderWindow, sprite); // step 13
	}

	return(EXIT_SUCCESS);
}